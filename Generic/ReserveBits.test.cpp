
#include "ReserveBits.h"

void testSimpleCLZ()
{
    {
        ReserveBits::SingleLayerSimple<ReserveBits::Mode::_CLZ> rbs;
        assert(rbs.bits == 0xFFFFFFFF);
    }
    ReserveBits::SingleLayerSimple<ReserveBits::Mode::_CLZ> rbs {32};
    assert(rbs.bits == 0xFFFFFFFF);
    assert(rbs.reserve() == 31);    assert(rbs.isReserved(31));
    assert(rbs.bits == 0x7FFFFFFF);
    assert(rbs.reserve() == 30);    assert(rbs.isReserved(30));
    assert(rbs.reserve() == 29);    assert(rbs.isReserved(29));
    assert(rbs.reserve() == 28);    assert(rbs.isReserved(28));
    assert(rbs.bits == 0x0FFFFFFF);
    rbs.free(29);                   assert(rbs.isFreed(29));
    assert(rbs.bits == 0x2FFFFFFF);
    assert(rbs.reserve() == 29);    assert(rbs.isReserved(29));
    assert(rbs.bits == 0x0FFFFFFF);
    rbs.free(27);                   assert(rbs.isFreed(27));
    assert(rbs.bits == 0x0FFFFFFF);
}
void testSimple14CLZ()
{
    ReserveBits::SingleLayerSimple<ReserveBits::Mode::_CLZ> rbs {14};
    assert(rbs.bits == 0x00003FFF);
    assert(rbs.reserve() == 13);    assert(rbs.isReserved(13));
    assert(rbs.bits == 0x00001FFF);
    assert(rbs.reserve() == 12);    assert(rbs.isReserved(12));
    assert(rbs.reserve() == 11);    assert(rbs.isReserved(11));
    assert(rbs.reserve() == 10);    assert(rbs.isReserved(10));
    assert(rbs.bits == 0x000003FF);
    rbs.free(11);                   assert(rbs.isFreed(11));
    assert(rbs.bits == 0x00000BFF);
    assert(rbs.reserve() == 11);    assert(rbs.isReserved(11));
    assert(rbs.bits == 0x000003FF);
    rbs.free(9);                    assert(rbs.isFreed(9));
    assert(rbs.bits == 0x000003FF);
    assert(rbs.reserve() == 9 );    assert(rbs.isReserved(9));
    assert(rbs.reserve() == 8 );    assert(rbs.isReserved(8));
    assert(rbs.reserve() == 7 );    assert(rbs.isReserved(7));
    assert(rbs.reserve() == 6 );    assert(rbs.isReserved(6));
    assert(rbs.reserve() == 5 );    assert(rbs.isReserved(5));
    assert(rbs.reserve() == 4 );    assert(rbs.isReserved(4));
    assert(rbs.reserve() == 3 );    assert(rbs.isReserved(3));
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.reserve() == 1 );    assert(rbs.isReserved(1));
    assert(rbs.reserve() == 0 );    assert(rbs.isReserved(0));
    assert(rbs.bits == 0x00000000);
    assert(rbs.reserve() == (u8)~0);
    rbs.free(5);                    assert(rbs.isFreed(5));
    assert(rbs.reserve() == 5 );    assert(rbs.isReserved(5));
    assert(rbs.reserve() == (u8)~0);
}
void testSimpleCTZ()
{
    {
        ReserveBits::SingleLayerSimple<ReserveBits::Mode::_CTZ> rbs {32};
        assert(rbs.bits == 0xFFFFFFFF);
    }
    ReserveBits::SingleLayerSimple<ReserveBits::Mode::_CTZ> rbs;
    assert(rbs.bits == 0xFFFFFFFF);
    assert(rbs.reserve() == 0 );    assert(rbs.isReserved(0));
    assert(rbs.bits == 0xFFFFFFFE);
    assert(rbs.reserve() == 1 );    assert(rbs.isReserved(1));
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.reserve() == 3 );    assert(rbs.isReserved(3));
    assert(rbs.bits == 0xFFFFFFF0);
    rbs.free(2);                    assert(rbs.isFreed(2));
    assert(rbs.bits == 0xFFFFFFF4);
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.bits == 0xFFFFFFF0);
    rbs.free(4);                    assert(rbs.isFreed(4));
    assert(rbs.bits == 0xFFFFFFF0);
}
void testSimple14CTZ()
{
    ReserveBits::SingleLayerSimple<ReserveBits::Mode::_CTZ> rbs {14};
    assert(rbs.bits == 0x00003FFF);
    assert(rbs.reserve() == 0 );    assert(rbs.isReserved(0));
    assert(rbs.bits == 0x00003FFE);
    assert(rbs.reserve() == 1 );    assert(rbs.isReserved(1));
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.reserve() == 3 );    assert(rbs.isReserved(3));
    assert(rbs.bits == 0x00003FF0);
    rbs.free(2);                    assert(rbs.isFreed(2));
    assert(rbs.bits == 0x00003FF4);
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.bits == 0x00003FF0);
    rbs.free(4);                    assert(rbs.isFreed(4));
    assert(rbs.bits == 0x00003FF0);
    assert(rbs.reserve() == 4 );    assert(rbs.isReserved(4));
    assert(rbs.reserve() == 5 );    assert(rbs.isReserved(5));
    assert(rbs.reserve() == 6 );    assert(rbs.isReserved(6));
    assert(rbs.reserve() == 7 );    assert(rbs.isReserved(7));
    assert(rbs.reserve() == 8 );    assert(rbs.isReserved(8));
    assert(rbs.reserve() == 9 );    assert(rbs.isReserved(9));
    assert(rbs.reserve() == 10);    assert(rbs.isReserved(10));
    assert(rbs.reserve() == 11);    assert(rbs.isReserved(11));
    assert(rbs.reserve() == 12);    assert(rbs.isReserved(12));
    assert(rbs.reserve() == 13);    assert(rbs.isReserved(13));
    assert(rbs.bits == 0x00000000);
    assert(rbs.reserve() == (u8)~0);
    rbs.free(5);                    assert(rbs.isFreed(5));
    assert(rbs.reserve() == 5 );    assert(rbs.isReserved(5));
    assert(rbs.reserve() == (u8)~0);
}

void testRolledCLZ()
{
    {
        ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CLZ> rbs {32};
        assert(rbs.bits == 0xFFFFFFFF);
    }
    ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CLZ> rbs;
    assert(rbs.bits == 0xFFFFFFFF);
    assert(rbs.reserve() == 31);    assert(rbs.isReserved(31));
    assert(rbs.bits == 0x7FFFFFFF);
    assert(rbs.reserve() == 30);    assert(rbs.isReserved(30));
    assert(rbs.reserve() == 29);    assert(rbs.isReserved(29));
    assert(rbs.reserve() == 28);    assert(rbs.isReserved(28));
    assert(rbs.bits == 0x0FFFFFFF);
    rbs.free(29);                   assert(rbs.isFreed(29));
    assert(rbs.bits == 0x2FFFFFFF);
    assert(rbs.reserve() == 27);    assert(rbs.isReserved(27));
    assert(rbs.bits == 0x27FFFFFF);
    rbs.free(26);                   assert(rbs.isFreed(26));
    assert(rbs.bits == 0x27FFFFFF);
}
void testRolled14CLZ()
{
    ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CLZ> rbs(14);
    assert(rbs.bits == 0x00003FFF);
    assert(rbs.reserve() == 13);    assert(rbs.isReserved(13));
    assert(rbs.bits == 0x00001FFF);
    assert(rbs.reserve() == 12);    assert(rbs.isReserved(12));
    assert(rbs.reserve() == 11);    assert(rbs.isReserved(11));
    assert(rbs.reserve() == 10);    assert(rbs.isReserved(10));
    assert(rbs.bits == 0x000003FF);
    rbs.free(11);                   assert(rbs.isFreed(11));
    assert(rbs.bits == 0x00000BFF);
    assert(rbs.reserve() == 9 );    assert(rbs.isReserved(9));
    assert(rbs.bits == 0x000009FF);
    rbs.free(8);                    assert(rbs.isFreed(8));
    assert(rbs.bits == 0x000009FF);
    assert(rbs.reserve() == 8 );    assert(rbs.isReserved(8));
    assert(rbs.reserve() == 7 );    assert(rbs.isReserved(7));
    assert(rbs.reserve() == 6 );    assert(rbs.isReserved(6));
    assert(rbs.reserve() == 5 );    assert(rbs.isReserved(5));
    assert(rbs.reserve() == 4 );    assert(rbs.isReserved(4));
    assert(rbs.reserve() == 3 );    assert(rbs.isReserved(3));
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.reserve() == 1 );    assert(rbs.isReserved(1));
    assert(rbs.reserve() == 0 );    assert(rbs.isReserved(0));
    assert(rbs.bits == 0x00000800);
    assert(rbs.reserve() == 11);    assert(rbs.isReserved(11));
    assert(rbs.bits == 0x00000000);
    assert(rbs.reserve() == (u8)~0);
}
void testRolledCTZ()
{
    {
        ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CTZ> rbs {32};
        assert(rbs.bits == 0xFFFFFFFF);
    }
    ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CTZ> rbs;
    assert(rbs.bits == 0xFFFFFFFF);
    assert(rbs.reserve() == 0 );    assert(rbs.isReserved(0));
    assert(rbs.bits == 0xFFFFFFFE);
    assert(rbs.reserve() == 1 );    assert(rbs.isReserved(1));
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.reserve() == 3 );    assert(rbs.isReserved(3));
    assert(rbs.bits == 0xFFFFFFF0);
    rbs.free(2);                    assert(rbs.isFreed(2));
    assert(rbs.bits == 0xFFFFFFF4);
    assert(rbs.reserve() == 4 );    assert(rbs.isReserved(4));
    assert(rbs.bits == 0xFFFFFFE4);
    rbs.free(5);                    assert(rbs.isFreed(5));
    assert(rbs.bits == 0xFFFFFFE4);
}
void testRolled14CTZ()
{
    ReserveBits::SingleLayerRolled<ReserveBits::Mode::_CTZ> rbs(14);
    assert(rbs.bits == 0x00003FFF);
    assert(rbs.reserve() == 0 );    assert(rbs.isReserved(0));
    assert(rbs.bits == 0x00003FFE);
    assert(rbs.reserve() == 1 );    assert(rbs.isReserved(1));
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.reserve() == 3 );    assert(rbs.isReserved(3));
    assert(rbs.bits == 0x00003FF0);
    rbs.free(2);                    assert(rbs.isFreed(2));
    assert(rbs.bits == 0x00003FF4);
    assert(rbs.reserve() == 4 );    assert(rbs.isReserved(4));
    assert(rbs.bits == 0x00003FE4);
    rbs.free(5);                    assert(rbs.isFreed(5));
    assert(rbs.bits == 0x00003FE4);
    assert(rbs.reserve() == 5 );    assert(rbs.isReserved(5));
    assert(rbs.reserve() == 6 );    assert(rbs.isReserved(6));
    assert(rbs.reserve() == 7 );    assert(rbs.isReserved(7));
    assert(rbs.reserve() == 8 );    assert(rbs.isReserved(8));
    assert(rbs.reserve() == 9 );    assert(rbs.isReserved(9));
    assert(rbs.reserve() == 10);    assert(rbs.isReserved(10));
    assert(rbs.reserve() == 11);    assert(rbs.isReserved(11));
    assert(rbs.reserve() == 12);    assert(rbs.isReserved(12));
    assert(rbs.reserve() == 13);    assert(rbs.isReserved(13));
    assert(rbs.bits == 0x00000004);
    assert(rbs.reserve() == 2 );    assert(rbs.isReserved(2));
    assert(rbs.bits == 0x00000000);
    assert(rbs.reserve() == (u8)~0);
}

int main()
{
    testSimpleCLZ();
    testSimple14CLZ();
    testSimpleCTZ();
    testSimple14CTZ();
    testRolledCLZ();
    testRolled14CLZ();
    testRolledCTZ();
    testRolled14CTZ();

    return 0;
}

