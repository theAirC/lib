//! Library for MAX9939 SPI PGA

#pragma once

#include <lib/System/Devices/Device_SPI.h>

namespace System::Devices {

    struct MAX9939 : Device_SPI
    {
        enum struct Register : byte {
            Trim = 0,
            Gain = 1,
        };
        enum struct Gain : byte {
            X1   = 0,
            X10  = 1,
            X20  = 2,
            X30  = 3,
            X40  = 4,
            X60  = 5,
            X80  = 6,
            X120 = 7,
            X157 = 8,
            X025 = 9,
        };
        static constexpr byte Register_Trim_Vos_offset  = 1;
        static constexpr byte Register_Trim_Vos_mask    = 0x3E;
        static constexpr byte Register_Gain_Gain_offset = 1;
        static constexpr byte Register_Gain_Gain_mask   = 0x1E;
        static constexpr byte Register_ANY_Meas_offset  = 6;
        static constexpr byte Register_ANY_Meas_mask    = 0x40;
        static constexpr byte Register_ANY_Shdn_offset  = 7;
        static constexpr byte Register_ANY_Shdn_mask    = 0x80;

        MAX9939(Peripherals::SPI &mySPI, GPIO::Output &chipSelect);

        void write(Register reg, byte value);

        void init();
        void shutdown();

        // Sets the Input offset voltage (Vos) trim to the specified <level> [-15..+15]
        // The actual voltage does not scale linearly with the <level>:
        // Level  Vos(mV)
        //    0        0
        //    1      1.3
        //    2      2.5
        //    3      3.8
        //    4      4.9
        //    5      6.1
        //    6      7.3
        //    7      8.4
        //    8     10.6
        //    9     11.7
        //   10     12.7
        //   11     13.7
        //   12     14.7
        //   13     15.7
        //   14     16.7
        //   15     17.6
        // <measurementMode> (de)activates measurement mode:
        // input INA- is disconnected and shorted to INA+
        // Note: changing the gain will deactivate measurement mode
        // Used to measure and compensate for Vos
        void setVos(s8 level, bool measurementMode = false);

        // Sets the gain
        void setGain(Gain gain);
    };

    MAX9939::MAX9939(Peripherals::SPI &mySPI, GPIO::Output &chipSelect)
        : Device_SPI(mySPI, chipSelect)
    { }

    void MAX9939::write(Register reg, byte value)
    {
        transfer(value | (byte)reg);
    }

    void MAX9939::init()
    {
        write(Register::Trim, 0);
        write(Register::Gain, 0);
    }
    void MAX9939::shutdown()
    {
        write(Register::Trim, (1 << Register_ANY_Shdn_offset) & Register_ANY_Shdn_mask);
    }

    void MAX9939::setVos(s8 level, bool measurementMode)
    {
        byte vos = level >= 0 ? level : (-level | (1 << 4));
        byte meas = measurementMode ? 1 : 0;
        byte reg = ((vos  << Register_Trim_Vos_offset) & Register_Trim_Vos_mask)
                 | ((meas << Register_ANY_Meas_offset) & Register_ANY_Meas_mask);
        write(Register::Trim, reg);
    }

    void MAX9939::setGain(Gain gain)
    {
        write(Register::Gain, ((byte)gain << Register_Gain_Gain_offset) & Register_Gain_Gain_mask);
    }

}
