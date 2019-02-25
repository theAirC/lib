#pragma once

#include <lib/Naming.h>
#include <lib/Generic/ReserveBits.h>

namespace Memory {

// Max <BlockCount> is 32
template<u32 BlockCount, u32 BlockSize>
struct BlockSpace
{
    static constexpr u32 BlockCount = BlockCount;
    static constexpr u32 BlockSize = BlockSize;

    typedef u8 BlockID;
    static constexpr BlockID BlockID_NULL = 0xFF;

    ReserveBits_Rolled reserveBits (BlockCount);
    BlockID blocksNext[BlockCount];
    byte blocks[BlockCount][BlockSize];

    BlockID newBlock()
    {
        u8 index = reserveBits.reserve();
        if (index >= 32) return BlockID_NULL;

        blocksNext[index] = BlockID_NULL;
        return index;
    }
    void deleteBlock(BlockID blockID)
    {
        assert(blockID < BlockCount);
        reserveBits.free(blockID);
    }
    void deleteBlockList(BlockID blockID)
    {
        assert(blockID < BlockCount);
        do {
            reserveBits.free(blockID);
            blockID = blocksNext[blockID];
        } while (blockID != BlockID_NULL);
    }
}

}
