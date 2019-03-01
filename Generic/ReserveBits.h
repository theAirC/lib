#pragma once

#include <lib/Naming.h>
#include <lib/Instructions.h>

namespace ReserveBits {

struct SingleLayer
{
    u32 bits;

    SingleLayer()         : bits(~0UL)                {}
    SingleLayer(u8 count) : bits((1ULL << count) - 1) {}

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
};

enum struct Mode : bool {
    _CTZ = false,
    _CLZ = true,
};

// Used when there is hardware support for CLZ/CTZ.
template<Mode mode>
struct SingleLayerSimple : SingleLayer
{
    SingleLayerSimple()         : SingleLayer()      {}
    SingleLayerSimple(u8 count) : SingleLayer(count) {}

    u8 reserve()
    {
        u8 offset;
        switch (mode) {
            case Mode::_CTZ: offset = CTZ(bits); break;
            case Mode::_CLZ: offset = CLZ(bits); break;
        }
        if (offset >= 32) return ~0;

        // Reserve the currently found index
        u8 index;
        switch (mode) {
            case Mode::_CTZ: index = offset;      break;
            case Mode::_CLZ: index = 31 - offset; break;
        }
        bits = CBI(bits, index);

        return index;
    }
};

// Used when there is no hardware support for CLZ/CTZ,
// or when 'wear leveling' is needed.
// Performs a 'round robin' search by rolling the <bits> before CLZ/CTZ.
template<Mode mode>
struct SingleLayerRolled : SingleLayer
{ 
    u8 rollTimes;

    SingleLayerRolled()         : SingleLayer(),      rollTimes(0) {}
    SingleLayerRolled(u8 count) : SingleLayer(count), rollTimes(0) {}

    void roll(u8 times)
    {
        switch (mode) {
            case Mode::_CTZ: rollTimes = (u8)(rollTimes + times) % 32; break;
            case Mode::_CLZ: rollTimes = (u8)(rollTimes - times) % 32; break; // See getRolledBits() to see why we decrement
        }
    }
    u32 getRolledBits() const
    {
        // We rotate right by rollTimes.
        // If rollTimes is decremented by roll(),
        // the net effect is a rotate left.
        return ROR(bits, rollTimes);
    }

    u8 reserve()
    {
        // Start the CLZ/CTZ just after the last reserved index,
        // since that one will be the least likely to be reserved.
        u8 offset;
        switch (mode) {
            case Mode::_CTZ: offset = CTZ(getRolledBits()); break;
            case Mode::_CLZ: offset = CLZ(getRolledBits()); break;
        }
        if (offset >= 32) return ~0;

        switch (mode) {
            case Mode::_CTZ: roll(offset);     break; // Bring first the current reservation bit
            case Mode::_CLZ: roll(offset + 1); break; // Bring first the bit right next to the current reservation
        }

        u8 index = rollTimes;

        if (mode == Mode::_CTZ) rollTimes += 1;

        bits = CBI(bits, index); // Reserve the currently found index
        return index;
    }
};

}
