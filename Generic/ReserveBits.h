#pragma once

#include <lib/Naming.h>
#include <lib/Instructions.h>

struct ReserveBits
{
    u32 bits;

    ReserveBits() : bits(~0UL) {}
    ReserveBits(u8 count) : bits(((1UL << count) - 1) << (32 - count)) {}

    virtual u8 reserve() = 0;

    virtual void free(u8 index)
    {
        bits = SBI(bits, index);
    }

    virtual bool isFreed(u8 index)
    {
        return VAL(bits, index) != 0;
    }
    virtual bool isReserved(u8 index)
    {
        return VAL(bits, index) == 0;
    }
}

// Used when there is hardware support for CLZ
struct ReserveBits_Simple : ReserveBits
{
    ReserveBits_Simple() : ReserveBits() {}
    ReserveBits_Simple(u8 count) : ReserveBits(count) {}

    u8 reserve()
    {
        u8 offset = CLZ(bits);
        if (offset >= 32) return ~0UL;
        // Reserve the currently found index (which happens to be == 31 - <offset>)
        u8 index = 31 - offset;
        bits = CBI(bits, index);
        return index;
    }
};

// Used when there is no hardware support for CLZ or any equivalents,
// or when 'wear leveling' is needed
// Performs a 'round robin' search by rolling the <bits> before CLZ
struct ReserveBits_Rolled : ReserveBits
{ 
    u8 rollTimes;

    ReserveBits_Rolled() : ReserveBits() {}
    ReserveBits_Rolled(u8 count) : ReserveBits(count) {}

    void roll(u8 times)
    {
        // See getRolledBits() to see why we decrement
        rollTimes = (rollTimes - times) % 32;
    }
    u8 getRolledBits() const
    {
        // We rotate right by rollTimes,
        // but rollTimes is decremented by roll(),
        // so the net effect is a rotate left.
        return ROR(bits, rollTimes);
    }

    u8 reserve()
    {
        // Start the CLZ just after the last reserved index
        // since that one will be the least likely to be reserved.
        u8 offset = CLZ(getRolledBits(bits));
        if (offset >= 32) return ~0UL;
        // Bring the bit right next (less significant) to the current reservation first (MSb)
        roll(offset + 1);
        // Reserve the currently found index (which happens to be == <rollTimes>)
        bits = CBI(bits, rollTimes);
        return rollTimes;
    }
};
