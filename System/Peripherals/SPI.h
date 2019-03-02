#pragma once

#include <lib/Naming.h>
#include <lib/Instructions.h>
#include <lib/System/PinNumber.h>
#include <lib/System/Peripherals/Helpers.h>

namespace System::Peripherals {

/// Definitions

    struct SPI
    {
        NRF_SPI_Type *const instance;

        struct Config
        {
            enum struct Frequency : u32 {
                Kbps125 = SPI_FREQUENCY_FREQUENCY_K125, // 125 Kbps
                Kbps250 = SPI_FREQUENCY_FREQUENCY_K250, // 250 Kbps
                Kbps500 = SPI_FREQUENCY_FREQUENCY_K500, // 500 Kbps
                Mbps1   = SPI_FREQUENCY_FREQUENCY_M1,   //   1 Mbps
                Mbps2   = SPI_FREQUENCY_FREQUENCY_M2,   //   2 Mbps
                Mbps4   = SPI_FREQUENCY_FREQUENCY_M4,   //   4 Mbps
                Mbps8   = SPI_FREQUENCY_FREQUENCY_M8,   //   8 Mbps
            };
            enum struct Polarity : u32 {
                Active_High = SPI_CONFIG_CPOL_ActiveHigh, // Serial clock (SCK) will be high when active, low when idle
                Active_Low  = SPI_CONFIG_CPOL_ActiveLow,  // Serial clock (SCK) will be low when active, high when idle
            };
            enum struct Phase : u32 {
                Leading  = SPI_CONFIG_CPHA_Leading,  // Sample on leading edge of the clock, change on trailing edge
                Trailing = SPI_CONFIG_CPHA_Trailing, // Sample on trailing edge of the clock, change on leading edge
            };
            // // Alternative interpretation of Polarity and Phase
            // enum struct Mode : u32 {
            //     Mode0 = Polarity::Active_High | Phase::Leading,
            //     Mode1 = Polarity::Active_High | Phase::Trailing,
            //     Mode2 = Polarity::Active_Low  | Phase::Leading,
            //     Mode3 = Polarity::Active_Low  | Phase::Trailing,
            // }
            enum struct BitOrder : u32 {
                MSb_First = SPI_CONFIG_ORDER_MsbFirst, // Most significant bit shifted out first
                LSb_First = SPI_CONFIG_ORDER_LsbFirst, // Least significant bit shifted out first
            };

            Frequency         frequency;
            Polarity          polarity;
            Phase             phase;
            BitOrder          bitOrder;
            System::PinNumber pinSCK;
            System::PinNumber pinMOSI;
            System::PinNumber pinMISO;
        };
        static constexpr Config defaultConfig = {
            .frequency = Config::Frequency::Kbps125,
            .polarity  = Config::Polarity::Active_High,
            .phase     = Config::Phase::Leading,
            .bitOrder  = Config::BitOrder::MSb_First,
            .pinSCK    = System::PinNumber::None,
            .pinMOSI   = System::PinNumber::None,
            .pinMISO   = System::PinNumber::None,
        };

        enum struct Task : u32 {};
        enum struct Event : u32 {
            Ready = offsetof(NRF_SPI_Type, EVENTS_READY), // TXD byte sent and RXD byte received
        };
        enum struct Interrupt : u32 {
            None  = 0UL,
            Ready = SPI_INTENSET_READY_Msk, // Interrupt on READY event
            All   = ~0UL,
        };
        enum struct Shortcut : u32 {};

        void init(Config config = defaultConfig);
        void enable();
        void disable();
        bool isEnabled();
        void shutdown();

        void applyConfig  (Config config);
        void setFrequency (Config::Frequency frequency);
        void setPolarity  (Config::Polarity  polarity);
        void setPhase     (Config::Phase     phase);
        void setBitOrder  (Config::BitOrder  bitOrder);
        void setPinSCK    (System::PinNumber pinSCK);
        void setPinMOSI   (System::PinNumber pinMOSI);
        void setPinMISO   (System::PinNumber pinMISO);

        void write(byte data);
        byte read();

        // void       triggerTask             (Task      task);
        // u32            getTaskAddress      (Task      task);
        void         clearEvent            (Event     event);
        bool         checkEvent            (Event     event);
        bool checkAndClearEvent            (Event     event);
        u32            getEventAddress     (Event     event);
        u32            getInterrupts       ();
        void        enableInterrupts       (Interrupt interrupts);
        void       disableInterrupts       (Interrupt interrupts);
        bool            isInterruptEnabled (Interrupt interrupt);
        // void           setShortcuts        (Shortcut  shortcuts);
        // u32            getShortcuts        ();
        // void        enableShortcuts        (Shortcut  shortcuts);
        // void       disableShortcuts        (Shortcut  shortcuts);
        // bool            isShortcutEnabled  (Shortcut  shortcut);
    };

    SPI SPI0 {NRF_SPI0}; // Shared with I2C0
    SPI SPI1 {NRF_SPI1}; // Shared with SPIS1, I2C1

    u32 operator|(SPI::Interrupt a, SPI::Interrupt b) { return (u32)a | (u32)b; }
    u32 operator|(SPI::Shortcut a, SPI::Shortcut b) { return (u32)a | (u32)b; }

/// Implementation

    void SPI::init(Config config)
    {
        applyConfig(config);
    }
    void SPI::enable()
    {
        //instance->ENABLE = BFI(instance->ENABLE, SPI_ENABLE_ENABLE_Pos, SPI_ENABLE_ENABLE_Mask, SPI_ENABLE_ENABLE_Enabled);
        instance->ENABLE = SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos;
    }
    void SPI::disable()
    {
        //instance->ENABLE = BFI(instance->ENABLE, SPI_ENABLE_ENABLE_Pos, SPI_ENABLE_ENABLE_Mask, SPI_ENABLE_ENABLE_Disabled);
        instance->ENABLE = SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos;
    }
    bool SPI::isEnabled()
    {
        //return BFE(instance->ENABLE, SPI_ENABLE_ENABLE_Pos, SPI_ENABLE_ENABLE_Mask) == SPI_ENABLE_ENABLE_Enabled;
        return instance->ENABLE == SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos;
    }
    void SPI::shutdown()
    {
        disable();
    }

    void SPI::applyConfig(Config config)
    {
        setFrequency (config.frequency);
        setPolarity  (config.polarity);
        setPhase     (config.phase);
        setBitOrder  (config.bitOrder);
        setPinSCK    (config.pinSCK);
        setPinMOSI   (config.pinMOSI);
        setPinMISO   (config.pinMISO);
    }
    void SPI::setFrequency(Config::Frequency frequency)
    {
        instance->FREQUENCY = (u32)frequency;
    }
    void SPI::setPolarity(Config::Polarity polarity)
    {
        instance->CONFIG = BFI(instance->CONFIG, SPI_CONFIG_CPOL_Pos, SPI_CONFIG_CPOL_Msk, (u32)polarity);
    }
    void SPI::setPhase(Config::Phase phase)
    {
        instance->CONFIG = BFI(instance->CONFIG, SPI_CONFIG_CPHA_Pos, SPI_CONFIG_CPHA_Msk, (u32)phase);
    }
    void SPI::setBitOrder(Config::BitOrder bitOrder)
    {
        instance->CONFIG = BFI(instance->CONFIG, SPI_CONFIG_ORDER_Pos, SPI_CONFIG_ORDER_Msk, (u32)bitOrder);
    }
    void SPI::setPinSCK(System::PinNumber pinSCK)
    {
        instance->PSELSCK = (u32)pinSCK;
    }
    void SPI::setPinMOSI(System::PinNumber pinMOSI)
    {
        instance->PSELMOSI = (u32)pinMOSI;
    }
    void SPI::setPinMISO(System::PinNumber pinMISO)
    {
        instance->PSELMISO = (u32)pinMISO;
    }

    void SPI::write(byte data)
    {
        instance->TXD = data;
    }
    byte SPI::read()
    {
        return instance->RXD;
    }

    // void SPI::triggerTask        (Task task)            {        Helpers::triggerTask        (instance, (u32)task);  }
    // u32  SPI::getTaskAddress     (Task task)            { return Helpers::getTaskAddress     (instance, (u32)task);  }
    void SPI::clearEvent         (Event event)          {        Helpers::clearEvent         (instance, (u32)event); }
    bool SPI::checkEvent         (Event event)          { return Helpers::checkEvent         (instance, (u32)event); }
    bool SPI::checkAndClearEvent (Event event)          { return Helpers::checkAndClearEvent (instance, (u32)event); }
    u32  SPI::getEventAddress    (Event event)          { return Helpers::getEventAddress    (instance, (u32)event); }
    u32  SPI::getInterrupts      ()                     { return instance->INTENSET;                                 }
    void SPI::enableInterrupts   (Interrupt interrupts) {        instance->INTENSET = (u32)interrupts;               }
    void SPI::disableInterrupts  (Interrupt interrupts) {        instance->INTENCLR = (u32)interrupts;               }
    bool SPI::isInterruptEnabled (Interrupt interrupt)  { return instance->INTENSET & (u32)interrupt;                }
    // void SPI::setShortcuts       (Shortcut shortcuts)   {        instance->SHORTS   = (u32)shortcuts;                }
    // u32  SPI::getShortcuts       ()                     { return instance->SHORTS;                                   }
    // void SPI::enableShortcuts    (Shortcut shortcuts)   {        instance->SHORTS  |= (u32)shortcuts                 }
    // void SPI::disableShortcuts   (Shortcut shortcuts)   {        instance->SHORTS  &= (u32)shortcuts                 }
    // bool SPI::isShortcutEnabled  (Shortcut shortcut)    { return instance->SHORTS   & (u32)shortcuts                 }

}
