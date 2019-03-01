
#include "BlockSpace.h"

using namespace Memory;

BlockSpace::RB<32, 64> bs;
BlockSpace::BlockID bid0, bid1, bid2;

void reserve3Blocks()
{
    bid0 = bs.newBlock();
    assert(bs.reserveBits.isReserved(bid0));
    assert(bs.blocksNext[bid0] == BlockSpace::BlockID_None);

    bid1 = bs.newBlock();
    assert(bs.reserveBits.isReserved(bid1));
    assert(bs.blocksNext[bid1] == BlockSpace::BlockID_None);

    bid2 = bs.newBlock();
    assert(bs.reserveBits.isReserved(bid2));
    assert(bs.blocksNext[bid2] == BlockSpace::BlockID_None);
}

int main()
{
    {
        reserve3Blocks();

        bs.blocksNext[bid1] = bid2;

        bs.deleteBlockList(bid1);
        assert(bs.reserveBits.isReserved(bid0));
        assert(bs.reserveBits.isFreed(bid1));
        assert(bs.reserveBits.isFreed(bid2));

        bs.deleteBlockList(bid0);
        assert(bs.reserveBits.isFreed(bid0));
        assert(bs.reserveBits.isFreed(bid1));
        assert(bs.reserveBits.isFreed(bid2));
    }

    {
        reserve3Blocks();

        bs.blocksNext[bid0] = bid1;
        bs.blocksNext[bid1] = bid2;
        bs.blocksNext[bid2] = bid0;

        bs.deleteBlockList(bid0);
        assert(bs.reserveBits.isFreed(bid0));
        assert(bs.reserveBits.isFreed(bid1));
        assert(bs.reserveBits.isFreed(bid2));
    }

    return 0;
}
