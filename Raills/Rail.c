
#include "Rail.h"

/// Functions that need to be moved out to math lib.

    byte upperlog2(uint number)
    {
        if (number < 1) return 0;
        byte i = 0;
        number--;
        while (number) {
            number >>= 1;
            i++;
        }
        return i;
    }

    reg bitFieldInsert(reg number, reg bitField, byte offset, byte count)
    {
        reg mask = (1 << count) - 1;
        bitField &= mask;
        mask <<= offset;
        bitField <<= offset;
        return (number & ~mask) | bitField;
    }
    reg bitFieldExtract(reg number, reg offset, reg count)
    {
        reg mask = (1 << count) - 1;
        return (number >> offset) & mask;
    }

    reg reverseBits(reg number, byte count)
    {
        reg result = 0;
        for (byte i = 0; i < count; i++) {
            result <<= 1;
            result |= number & 0x1;
            number >>= 1;
        }
        return result;
    }

/// Memory

    const byte Rail_Version = 0;
    const byte Rail_AddressBitCount = upperlog2(Rail_RIF_Count);
    const reg Rail_AddressMask = (1 << Rail_AddressBitCount) - 1;

    Rail_Service Rail_Services[16];
    Rail_RIF Rail_RIFs[Rail_RIF_Count];

/// Helper Functions

    void Rail_reversePath(byte *packet, byte bitCount)
    {
        if (bitCount == 0) return;
        byte byteCount = upperdiv(bitCount, 8);

        // Shift to align last
        byte remaining = (bitCount - 1) % 8 + 1;
        byte emptyBits = 8 - remaining;
        byte mask = ((1 << emptyBits) - 1) << remaining; // or ~((1 << remaining) - 1)
        byte tempA = 0;
        byte tempB = 0;
        for (byte i = 0; i < byteCount; i++) {
            tempA = packet[i] & mask;
            packet[i] <<= emptyBits;
            packet[i] |= tempB;
            tempB = tempA >> remaining;
        }

        // Reverse the bits of each byte
        for (byte i = 0; i < byteCount; i++) {
            packet[i] = reverseBits(packet[i], 8);
        }

        // Reverse the whole array
        for (byte i = 0; i < byteCount / 2; i++) {
            byte j = byteCount - i - 1;
            tempA = packet[i];
            packet[i] = packet[j];
            packet[j] = tempA;
        }
    }

/// Main functions

    enum Rail_retCode Rail_nullRIF_sendPacket(reg RIF_ID, byte *packet, size_t packetLength)
    {
        return Rail_retCode_RIF_Down;
    }

    void Rail_Init()
    {
        for (reg i = 0; i < Rail_RIF_Count; i++) {
            Rail_RIFs[i].ID = i;
            Rail_RIFs[i].ID_reverse = reverseBits(i, Rail_AddressBitCount);
            Rail_RIFs[i].sendPacket = Rail_nullRIF_sendPacket;
        }
    }

    enum Rail_retCode Rail_acceptPacket(byte *packet, size_t packetLength, reg s_rifID)
    {
        byte pVersion = packet[0] >> 4;
        if (pVersion > Rail_Version) return Rail_retCode_IncompatibleVersion;

        byte accessType = packet[0] & 0x0F;
        byte refNo = packet[1];
        byte d_count = packet[2];
        byte ds_count = packet[3];

        if (d_count == 0) {
            Rail_Service service = Rail_Services[accessType];
            if (service.acceptPayload) {
                byte offset = 4 + upperdiv(ds_count, 8);
                size_t replyLength = service.acceptPayload(&packet[offset], packetLength);
                if (replyLength) {
                    packetLength = offset + replyLength;
                    packet[0] = (pVersion << 4) | 0x0;
                    // packet[1] unchanged
                    packet[2] = ds_count;
                    // packet[3] unchanged
                    Rail_reversePath(&packet[4], ds_count);
                    return Rail_RIFs[s_rifID].sendPacket(s_rifID, packet, packetLength);
                }
                else return Rail_retCode_OKnoReply;
            }
            else return Rail_retCode_ServiceNotSupported;
        }
        else if (d_count >= Rail_AddressBitCount) {
            d_count -= Rail_AddressBitCount;
            byte index = 4 + (d_count / 8);
            byte bitIndex = (d_count % 8);

            u16 addrBytes = (packet[index] << 0) | (packet[index + 1] << 8);

            // Read the destination RIF_ID and replace it with the source RIF's ID_reverse
            reg d_rifID = bitFieldExtract(addrBytes, bitIndex, Rail_AddressBitCount);
            addrBytes = bitFieldInsert(addrBytes, Rail_RIFs[s_rifID].ID_reverse, bitIndex, Rail_AddressBitCount);

            packet[2] = d_count;
            packet[index] = (addrBytes >> 0) & 0xFF;
            packet[index + 1] = (addrBytes >> 8) & 0xFF;

            if (d_rifID < asize(Rail_RIFs)) return Rail_RIFs[d_rifID].sendPacket(d_rifID, packet, packetLength);
            else return Rail_retCode_UnknownRIFID;
        }
        else return Rail_retCode_BadDestinationCount;
    }
