//! Class for devices using an unmanaged SPI interface.

#pragma once

#include <lib/System/GPIO.h>
#include <lib/System/Peripherals/SPI.h>

namespace System::Devices {

    struct Device_SPI {
        using SPI = Peripherals::SPI;

        SPI &mySPI;
        GPIO::Output &chipSelect;

        Device_SPI(Peripherals::SPI &mySPI, GPIO::Output &chipSelect)
            : mySPI(mySPI), chipSelect(chipSelect)
        {
            chipSelect.high();
        }

        byte transfer(byte data)
        {
            chipSelect.low();

            mySPI.write(data);
            while (!mySPI.checkAndClearEvent(SPI::Event::Ready));
            byte result = mySPI.read();

            chipSelect.high();

            return result;
        }
    };

}
