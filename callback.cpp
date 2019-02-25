
typedef void (*vfv)(void); 

struct callback24u8
{
    u32 address :24;
    u8  data    : 8;

    callback24u8(const vfv address, const u8 data)
    : address((u32)address >> 1),
      data(data)
    {}

    u32 getCorrectAddress() const
    {
        return (address << 1) | 0x1;
    }

    template<typename T>
    T call() const
    {
        typedef T (*fuctionType)(u8);
        fuctionType func = (fuctionType)getCorrectAddress();
        return func(data);
    }
};
