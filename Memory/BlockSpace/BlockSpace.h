#pragma once

#include <lib/Naming.h>
#include <lib/Generic/ReserveBits.h>

namespace Memory::BlockSpace {

    typedef u8 BlockID;
    static constexpr BlockID BlockID_None = 0xFF;

    // Max <_BlockCount> is 32
    template<u32 _BlockCount, u32 _BlockSize>
    struct RB
    {
        static constexpr u32 BlockCount = _BlockCount;
        static constexpr u32 BlockSize = _BlockSize;

        ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CTZ> reserveBits {BlockCount};
        BlockID blocksNext[BlockCount];
        byte blocks[BlockCount][BlockSize];

        BlockID newBlock()
        {
            u8 index = reserveBits.reserve();
            if (index >= 32) return BlockID_None;

            blocksNext[index] = BlockID_None;
            return index;
        }
        void deleteBlock(BlockID blockID)
        {
            assert(blockID < BlockCount);
            reserveBits.free(blockID);
        }
        void deleteBlockList(BlockID blockID)
        {
            do {
                assert(blockID < BlockCount);
                if (reserveBits.isFreed(blockID)) break;
                reserveBits.free(blockID);
                blockID = blocksNext[blockID];
            } while (blockID != BlockID_None);
        }
    };

}
