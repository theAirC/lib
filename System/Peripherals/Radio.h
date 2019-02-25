#pragma once

#include <modules/nrfx/hal/nrf_radio.h>

#include <lib/Naming.h>
#include <lib/Instructions.h>

namespace System::Peripherals {

/// Initial configuration

    struct Radio_Config
    {
        // RF transmit power
        enum struct TX_Power
        {
            #ifdef RADIO_TXPOWER_TXPOWER_Pos8dBm
            P8dBm  = RADIO_TXPOWER_TXPOWER_Pos8dBm,  // + 8 dBm
            #endif
            #ifdef RADIO_TXPOWER_TXPOWER_Pos7dBm
            P7dBm  = RADIO_TXPOWER_TXPOWER_Pos7dBm,  // + 7 dBm
            #endif
            #ifdef RADIO_TXPOWER_TXPOWER_Pos6dBm
            P6dBm  = RADIO_TXPOWER_TXPOWER_Pos6dBm,  // + 6 dBm
            #endif
            #ifdef RADIO_TXPOWER_TXPOWER_Pos5dBm
            P5dBm  = RADIO_TXPOWER_TXPOWER_Pos5dBm,  // + 5 dBm
            #endif
            P4dBm  = RADIO_TXPOWER_TXPOWER_Pos4dBm,  // + 4 dBm
            #ifdef RADIO_TXPOWER_TXPOWER_Pos3dBm
            P3dBm  = RADIO_TXPOWER_TXPOWER_Pos3dBm,  // + 3 dBm
            #endif
            #ifdef RADIO_TXPOWER_TXPOWER_Pos2dBm
            P2dBm  = RADIO_TXPOWER_TXPOWER_Pos2dBm,  // + 2 dBm
            #endif
            P0dBm  = RADIO_TXPOWER_TXPOWER_0dBm,     // + 0 dBm
            N4dBm  = RADIO_TXPOWER_TXPOWER_Neg4dBm,  // - 4 dBm
            N8dBm  = RADIO_TXPOWER_TXPOWER_Neg8dBm,  // - 8 dBm
            N12dBm = RADIO_TXPOWER_TXPOWER_Neg12dBm, // -12 dBm
            N16dBm = RADIO_TXPOWER_TXPOWER_Neg16dBm, // -16 dBm
            N20dBm = RADIO_TXPOWER_TXPOWER_Neg20dBm, // -20 dBm
            N30dBm = RADIO_TXPOWER_TXPOWER_Neg30dBm, // -30 dBm
            #ifdef RADIO_TXPOWER_TXPOWER_Neg40dBm
            N40dBm = RADIO_TXPOWER_TXPOWER_Neg40dBm, // -40 dBm
            #endif
        };

        // Radio mode (protocol and bitrate)
        enum struct Mode
        {
            #ifdef RADIO_MODE_MODE_Nrf_250Kbit
            NRF_250Kbps         = RADIO_MODE_MODE_Nrf_250Kbit,        // 250 Kbps NRF proprietary
            #endif
            NRF_1Mbps           = RADIO_MODE_MODE_Nrf_1Mbit,          //   1 Mbps NRF proprietary
            NRF_2Mbps           = RADIO_MODE_MODE_Nrf_2Mbit,          //   2 Mbps NRF proprietary
            BLE_1Mbps           = RADIO_MODE_MODE_Ble_1Mbit,          //   1 Mbps Bluetooth Low Energy
            #ifdef RADIO_MODE_MODE_Ble_2Mbit
            BLE_2Mbps           = RADIO_MODE_MODE_Ble_2Mbit,          //   2 Mbps Bluetooth Low Energy
            #endif
            #ifdef RADIO_MODE_MODE_Ble_LR125Kbit
            BLE_LR_125Kbps      = RADIO_MODE_MODE_Ble_LR125Kbit,      // 125 Kbps Bluetooth Low Energy Long Range
            #endif
            #ifdef RADIO_MODE_MODE_Ble_LR500Kbit
            BLE_LR_500Kbps      = RADIO_MODE_MODE_Ble_LR500Kbit,      // 500 Kbps Bluetooth Low Energy Long Range
            #endif
            #ifdef RADIO_MODE_MODE_Ieee802154_250Kbit
            IEEE_802154_250Kbps = RADIO_MODE_MODE_Ieee802154_250Kbit, // 250 Kbps IEEE 802.15.4-2006
            #endif
        };

        #ifdef NRF52
        // Always include packet's S1 field in RAM representation for DMA
        enum struct Field_S1_Include
        {
            Auto   = RADIO_PCNF0_S1INCL_Automatic, // Only include if its BitSize is > 0
            Always = RADIO_PCNF0_S1INCL_Include    // Always include
        };
        #endif

        #ifdef NRF52
        // Packet's preamble length
        enum struct Preamble_Length
        {
            Bits8  = RADIO_PCNF0_PLEN_8bit, //  8 bit
            Bits16 = RADIO_PCNF0_PLEN_16bit // 16 bit
        };
        #endif

        // On-Air bit-endianness
        enum struct Endianness
        {
            Little = RADIO_PCNF1_ENDIAN_Little, // LSb first
            Big    = RADIO_PCNF1_ENDIAN_Big     // MSb first
        };

        // Packet whitening
        enum struct Whitening
        {
            Disabled = RADIO_PCNF1_WHITEEN_Disabled,
            Enabled  = RADIO_PCNF1_WHITEEN_Enabled
        };

        // Packet's CRC length
        enum struct CRC_Length
        {
            OFF    = RADIO_CRCCNF_LEN_Disabled, // None
            Bits8  = RADIO_CRCCNF_LEN_One,      //  8 bit
            Bits16 = RADIO_CRCCNF_LEN_Two,      // 16 bit
            Bits24 = RADIO_CRCCNF_LEN_Three     // 24 bit
        };

        // Include or exclude packet's address field in CRC calculation
        enum struct CRC_Address
        {
            Include = RADIO_CRCCNF_SKIPADDR_Include,
            Exclude = RADIO_CRCCNF_SKIPADDR_Skip
        };

        #ifdef NRF52
        // Radio ramp-up time
        enum struct RampUp
        {
            Default = RADIO_MODECNF0_RU_Default, // t_{RXEN}      == 140 us
            Fast    = RADIO_MODECNF0_RU_Fast     // t_{RXEN,FAST} ==  40 us
        };
        #endif

        #ifdef NRF52
        // Radio TX value to transmit when not started
        enum struct TX_DefaultValue
        {
            Bit1   = RADIO_MODECNF0_DTX_B1,    // Transmit frequency shift for logic 1
            Bit0   = RADIO_MODECNF0_DTX_B0,    // Transmit frequency shift for logic 0
            Center = RADIO_MODECNF0_DTX_Center // Transmit center frequency (zero shift)
        };
        #endif

        TX_Power         TX_power;
        Mode             mode;
        byte             Field_Length_bitSize :4; // [0..15]
        byte             Field_S0_byteSize    :1; // [0..1]
        byte             Field_S1_bitSize     :4; // [0..15]
        #ifdef NRF52
        Field_S1_Include Field_S1_include;
        Preamble_Length  Preamble_length;
        #endif
        byte             Payload_Size_max     :8; // [0..254] S0 + Length + S1 + Payload <= 254
        byte             Payload_Size_static  :8; // [0..254]
        byte             Address_Base_length  :3; // [2..4]
        Endianness       endianness;
        Whitening        whitening;
        CRC_Length       CRC_length;
        CRC_Address      CRC_address;
        #ifdef NRF52
        RampUp           rampUp;
        TX_DefaultValue  TX_defaultValue;
        #endif
    };

    const Radio_Config Radio_Config_Default = {
        .TX_power             = Radio_Config::TX_Power::P4dBm,
        .mode                 = Radio_Config::Mode::NRF_1Mbps,
        .Field_Length_bitSize = 0,
        .Field_S0_byteSize    = 0,
        .Field_S1_bitSize     = 0,
        #ifdef NRF52
        .Field_S1_include     = Radio_Config::Field_S1_Include::Auto,
        .Preamble_length      = Radio_Config::Preamble_Length::Bits8,
        #endif
        .Payload_Size_max     = 8,
        .Payload_Size_static  = 8,
        .Address_Base_length  = 2,
        .endianness           = Radio_Config::Endianness::Little,
        .whitening            = Radio_Config::Whitening::Disabled,
        .CRC_length           = Radio_Config::CRC_Length::Bits8,
        .CRC_address          = Radio_Config::CRC_Address::Include,
        #ifdef NRF52
        .rampUp               = Radio_Config::RampUp::Default,
        .TX_defaultValue      = Radio_Config::TX_DefaultValue::Bit1
        #endif
    };

/// Definitions

    struct Radio
    {
        enum struct Task {
            TX_Enable        = NRF_RADIO_TASK_TXEN,      // Enable in TX mode
            RX_Enable        = NRF_RADIO_TASK_RXEN,      // Enable in RX mode
            Start            = NRF_RADIO_TASK_START,     // Start
            Stop             = NRF_RADIO_TASK_STOP,      // Stop
            Disable          = NRF_RADIO_TASK_DISABLE,   // Disable
            RSSI_Start       = NRF_RADIO_TASK_RSSISTART, // Start the RSSI and take one sample
            RSSI_Stop        = NRF_RADIO_TASK_RSSISTOP,  // Stop the RSSI measurement
            BitCounter_Start = NRF_RADIO_TASK_BCSTART,   // Start the bit counter
            BitCounter_Stop  = NRF_RADIO_TASK_BCSTOP,    // Stop the bit counter
        };

        enum struct Event {
            Ready            = NRF_RADIO_EVENT_READY,    // Radio has ramped up and is ready to be started
            Address          = NRF_RADIO_EVENT_ADDRESS,  // Address sent or received
            Payload          = NRF_RADIO_EVENT_PAYLOAD,  // Packet payload sent or received
            End              = NRF_RADIO_EVENT_END,      // Packet transmitted or received
            Disabled         = NRF_RADIO_EVENT_DISABLED, // Radio has been disabled
            Device_Match     = NRF_RADIO_EVENT_DEVMATCH, // A device address match occurred on the last received packet
            Device_Miss      = NRF_RADIO_EVENT_DEVMISS,  // No device address match occurred on the last received packet
            RSSI_End         = NRF_RADIO_EVENT_RSSIEND,  // Sampling of receive signal strength complete
            BitCounter_Match = NRF_RADIO_EVENT_BCMATCH,  // Bit counter reached bit count value
        };

        enum struct Interrupt {
            Ready            = NRF_RADIO_INT_READY_MASK,
            Address          = NRF_RADIO_INT_ADDRESS_MASK,
            Payload          = NRF_RADIO_INT_PAYLOAD_MASK,
            End              = NRF_RADIO_INT_END_MASK,
            Disabled         = NRF_RADIO_INT_DISABLED_MASK,
            Device_Match     = NRF_RADIO_INT_DEVMATCH_MASK,
            Device_Miss      = NRF_RADIO_INT_DEVMISS_MASK,
            RSSI_End         = NRF_RADIO_INT_RSSIEND_MASK,
            BitCounter_Match = NRF_RADIO_INT_BCMATCH_MASK,
        };

        enum struct Shortcut {
            Ready_TO_Start              = NRF_RADIO_SHORT_READY_START_MASK,
            End_TO_Disable              = NRF_RADIO_SHORT_END_DISABLE_MASK,
            Disabled_TO_TX_Enable       = NRF_RADIO_SHORT_DISABLED_TXEN_MASK,
            Disabled_TO_RX_Enable       = NRF_RADIO_SHORT_DISABLED_RXEN_MASK,
            Address_TO_RSSI_Start       = NRF_RADIO_SHORT_ADDRESS_RSSISTART_MASK,
            End_TO_Start                = NRF_RADIO_SHORT_END_START_MASK,
            Address_TO_BitCounter_Start = NRF_RADIO_SHORT_ADDRESS_BCSTART_MASK,
            Disabled_TO_RSSI_Stop       = NRF_RADIO_SHORT_DISABLED_RSSISTOP_MASK,
        };

        enum struct State {
            Disabled   = NRF_RADIO_STATE_DISABLED,  // No operations and power consumption is at a minimum
            RX_RampUp  = NRF_RADIO_STATE_RXRU,      // Ramping up and preparing for reception
            RX_Idle    = NRF_RADIO_STATE_RXIDLE,    // Ready to start reception
            RX         = NRF_RADIO_STATE_RX,        // Waiting to receive a frame
            RX_Disable = NRF_RADIO_STATE_RXDISABLE, // Disabling the receiver
            TX_RampUp  = NRF_RADIO_STATE_TXRU,      // Ramping up and preparing for transmission
            TX_Idle    = NRF_RADIO_STATE_TXIDLE,    // Ready to start transmission
            TX         = NRF_RADIO_STATE_TX,        // Transmitting a frame
            TX_Disable = NRF_RADIO_STATE_TXDISABLE, // Disabling the transmitter
        };

        void init(Radio_Config config) const;
        void shutdown() const;

        void triggerTask(Task task) const;
        void clearEvent(Event event) const;
        bool checkEvent(Event event) const;
        bool checkAndClearEvent(Event event) const;
        u32 getTaskAddress(Task task) const;
        u32 getEventAddress(Event event) const;

        void enableInterrupt(Interrupt interrupt) const;
        void disableInterrupt(Interrupt interrupt) const;
        bool isInterruptEnabled(Interrupt interrupt) const;

        void enableShortcut(Shortcut shortcut) const;
        void disableShortcut(Shortcut shortcut) const;

        void applyConfig(Radio_Config config) const;

        /// Frequency

            // Sets radio's frequency to 2400 + <frequency> MHz
            // Valid range for <frequency>:
            //   for NRF51: [  0..100] -> [2400..2500]
            //   for NRF52: [-40..100] -> [2360..2500]
            void setFrequency(s8 frequency) const;

        /// Addresses
            // The radio module remembers 8 on-air addresses each of which is assigned an ID called logical address
            // The on-air addresses are built from fields BASE (LSB) and PREFIX (MSB), according to the table below:
            // Logical address    Prefix       Base
            // 0                  prefix[0]    base[0]
            // 1                  prefix[1]    base[1]
            // 2                  prefix[2]    base[1]
            // 3                  prefix[3]    base[1]
            // 4                  prefix[4]    base[1]
            // 5                  prefix[5]    base[1]
            // 6                  prefix[6]    base[1]
            // 7                  prefix[7]    base[1]

            // Base field of the radio's addresses
            // When Address_Base_Length is smaller than 4, the MSBs of <addressBases[*]> are used
            void Address_setBases(const u32 addressBases[2]) const;

            // Prefix field of the radio's addresses
            void Address_setPrefixes(const byte addressPrefixes[8]) const;

            // Selects the address to use for transmitting,
            // according to the logical address <logicalID>
            void Address_selectTX(byte logicalID) const;

            // Enables the reception of packets which match the address
            // that corresponds to the logical address <logicalID>
            void Address_enableRX(byte logicalID) const;

            // Disables the reception of packets which match the address
            // that corresponds to the logical address <logicalID>
            void Address_disableRX(byte logicalID) const;

            // Enables the reception of packets which match
            // any of the configured addresses
            void Address_enableRX_All() const;

            // Disables the reception of packets which match
            // any of the configured addresses
            void Address_disableRX_All() const;

        /// TX and RX

            void TX_enable() const;
            void RX_enable() const;
            void start(const byte *packet) const;
            void disable() const;

            State getCurrentState() const;

            enum struct CRC_Status {
                Error = RADIO_CRCSTATUS_CRCSTATUS_CRCError,
                OK    = RADIO_CRCSTATUS_CRCSTATUS_CRCOk
            };
            CRC_Status CRC_readStatus() const;

            // Returns the last RSSI sample in negative dBm,
            // e.g. a value of 73 corresponds to a reading of -73 dBm
            byte RSSI_readSample() const;
    };

    const Radio radio;

/// Implementation

    void Radio::init(Radio_Config config) const
    {
        nrf_radio_power_set(true);
        applyConfig(config);
    }
    void Radio::shutdown() const
    {
        disable();
        nrf_radio_power_set(false);
    }

    void Radio::triggerTask(Task task) const 
    {
        nrf_radio_task_trigger((nrf_radio_task_t)task);
    }
    void Radio::clearEvent(Event event) const 
    {
        nrf_radio_event_clear((nrf_radio_event_t)event);
    }
    bool Radio::checkEvent(Event event) const 
    {
        return nrf_radio_event_check((nrf_radio_event_t)event);
    }
    bool Radio::checkAndClearEvent(Event event) const 
    {
        bool result = checkEvent(event);
        if (result) clearEvent(event);
        return result;
    }
    u32 Radio::getTaskAddress(Task task) const 
    {
        return nrf_radio_task_address_get((nrf_radio_task_t)task);
    }
    u32 Radio::getEventAddress(Event event) const 
    {
        return nrf_radio_event_address_get((nrf_radio_event_t)event);
    }

    void Radio::enableInterrupt(Interrupt interrupt) const 
    {
        nrf_radio_int_enable((nrf_radio_int_mask_t)interrupt);
    }
    void Radio::disableInterrupt(Interrupt interrupt) const 
    {
        nrf_radio_int_disable((nrf_radio_int_mask_t)interrupt);
    }
    bool Radio::isInterruptEnabled(Interrupt interrupt) const 
    {
        return nrf_radio_int_enable_check((nrf_radio_int_mask_t)interrupt);
    }

    void Radio::enableShortcut(Shortcut shortcut) const
    {
        nrf_radio_shorts_enable((u32)shortcut);
    }
    void Radio::disableShortcut(Shortcut shortcut) const
    {
        nrf_radio_shorts_disable((u32)shortcut);
    }

    void Radio::applyConfig(Radio_Config config) const
    {
        NRF_RADIO->TXPOWER = (u32)config.TX_power << RADIO_TXPOWER_TXPOWER_Pos;
        NRF_RADIO->MODE    = (u32)config.mode     << RADIO_MODE_MODE_Pos;

        #ifdef NRF51
        NRF_RADIO->PCNF0 = config.Field_Length_bitSize  << RADIO_PCNF0_LFLEN_Pos
                         | config.Field_S0_byteSize     << RADIO_PCNF0_S0LEN_Pos
                         | config.Field_S1_bitSize      << RADIO_PCNF0_S1LEN_Pos;
        #endif
        #ifdef NRF52
        NRF_RADIO->PCNF0 = config.Field_Length_bitSize  << RADIO_PCNF0_LFLEN_Pos
                         | config.Field_S0_byteSize     << RADIO_PCNF0_S0LEN_Pos
                         | config.Field_S1_bitSize      << RADIO_PCNF0_S1LEN_Pos
                         | (u32)config.Field_S1_include << RADIO_PCNF0_S1INCL_Pos
                         | (u32)config.Preamble_length  << RADIO_PCNF0_PLEN_Pos;
        #endif
        NRF_RADIO->PCNF1 = config.Payload_Size_max      << RADIO_PCNF1_MAXLEN_Pos
                         | config.Payload_Size_static   << RADIO_PCNF1_STATLEN_Pos
                         | config.Address_Base_length   << RADIO_PCNF1_BALEN_Pos
                         | (u32)config.endianness       << RADIO_PCNF1_ENDIAN_Pos
                         | (u32)config.whitening        << RADIO_PCNF1_WHITEEN_Pos;

        switch (config.whitening) {
            case Radio_Config::Whitening::Enabled:
                NRF_RADIO->DATAWHITEIV = 0xFF; // Initial value
                break;
            default:
                break;
        }

        NRF_RADIO->CRCCNF = (u32)config.CRC_length  << RADIO_CRCCNF_LEN_Pos
                          | (u32)config.CRC_address << RADIO_CRCCNF_SKIPADDR_Pos;

        switch (config.CRC_length) {
            case Radio_Config::CRC_Length::Bits8:
                NRF_RADIO->CRCPOLY = 0x000107; // CRC polynomial: x^8+x^2^x^1+1
                NRF_RADIO->CRCINIT = 0x0000FF; // Initial value
                break;
            case Radio_Config::CRC_Length::Bits16:
                NRF_RADIO->CRCPOLY = 0x011021; // CRC polynomial: x^16+x^12^x^5+1
                NRF_RADIO->CRCINIT = 0x00FFFF; // Initial value
                break;
            case Radio_Config::CRC_Length::Bits24:
                NRF_RADIO->CRCPOLY = 0x000000; // CRC polynomial: [TODO]
                NRF_RADIO->CRCINIT = 0xFFFFFF; // Initial value
                break;
            default:
                break;
        }

        #ifdef NRF52
        NRF_RADIO->MODECNF0 = (u32)config.rampUp          << RADIO_MODECNF0_RU_Pos
                            | (u32)config.TX_defaultValue << RADIO_MODECNF0_DTX_Pos;
        #endif
    }

    void Radio::setFrequency(s8 frequency) const
    {
        #ifdef NRF51
        byte frequencyField = (byte)(frequency + 0);
        NRF_RADIO->FREQUENCY = (frequencyField << RADIO_FREQUENCY_FREQUENCY_Pos) & RADIO_FREQUENCY_FREQUENCY_Msk;
        #endif
        #ifdef NRF52
        if (frequency >= 0) {
            byte frequencyField = (byte)(frequency + 0);
            NRF_RADIO->FREQUENCY = RADIO_FREQUENCY_MAP_Default << RADIO_FREQUENCY_MAP_Pos
                                 | (frequencyField << RADIO_FREQUENCY_FREQUENCY_Pos) & RADIO_FREQUENCY_FREQUENCY_Msk;
        }
        else {
            byte frequencyField = (byte)(frequency + 40);
            NRF_RADIO->FREQUENCY = RADIO_FREQUENCY_MAP_Low << RADIO_FREQUENCY_MAP_Pos
                                 | (frequencyField << RADIO_FREQUENCY_FREQUENCY_Pos) & RADIO_FREQUENCY_FREQUENCY_Msk;
        }
        #endif
    }

    void Radio::Address_setBases(const u32 addressBases[2]) const
    {
        NRF_RADIO->BASE0 = addressBases[0];
        NRF_RADIO->BASE1 = addressBases[1];
    }
    void Radio::Address_setPrefixes(const byte addressPrefixes[8]) const
    {
        NRF_RADIO->PREFIX0 = ((addressPrefixes[0] << RADIO_PREFIX0_AP0_Pos) & RADIO_PREFIX0_AP0_Msk)
                           | ((addressPrefixes[1] << RADIO_PREFIX0_AP1_Pos) & RADIO_PREFIX0_AP1_Msk)
                           | ((addressPrefixes[2] << RADIO_PREFIX0_AP2_Pos) & RADIO_PREFIX0_AP2_Msk)
                           | ((addressPrefixes[3] << RADIO_PREFIX0_AP3_Pos) & RADIO_PREFIX0_AP3_Msk);
        NRF_RADIO->PREFIX1 = ((addressPrefixes[4] << RADIO_PREFIX1_AP4_Pos) & RADIO_PREFIX1_AP4_Msk)
                           | ((addressPrefixes[5] << RADIO_PREFIX1_AP5_Pos) & RADIO_PREFIX1_AP5_Msk)
                           | ((addressPrefixes[6] << RADIO_PREFIX1_AP6_Pos) & RADIO_PREFIX1_AP6_Msk)
                           | ((addressPrefixes[7] << RADIO_PREFIX1_AP7_Pos) & RADIO_PREFIX1_AP7_Msk);
    }
    void Radio::Address_selectTX(byte logicalID) const
    {
        NRF_RADIO->TXADDRESS = (logicalID << RADIO_TXADDRESS_TXADDRESS_Pos) & RADIO_TXADDRESS_TXADDRESS_Msk;
    }
    void Radio::Address_enableRX(byte logicalID) const
    {
        NRF_RADIO->RXADDRESSES |= (1 << logicalID) & 0x000000FF;
    }
    void Radio::Address_disableRX(byte logicalID) const
    {
        NRF_RADIO->RXADDRESSES &= ~((1 << logicalID) & 0x000000FF);
    }
    void Radio::Address_enableRX_All() const
    {
        NRF_RADIO->RXADDRESSES = 0xFF;
    }
    void Radio::Address_disableRX_All() const
    {
        NRF_RADIO->RXADDRESSES = 0x00;
    }

    void Radio::TX_enable() const
    {
        NRF_RADIO->SHORTS = 0;
        triggerTask(Task::TX_Enable);
    }
    void Radio::RX_enable() const
    {
        NRF_RADIO->SHORTS = RADIO_SHORTS_ADDRESS_RSSISTART_Msk | RADIO_SHORTS_DISABLED_RSSISTOP_Msk;
        triggerTask(Task::RX_Enable);
    }
    void Radio::start(const byte *packet) const
    {
        NRF_RADIO->PACKETPTR = (uint32_t)packet;
        triggerTask(Task::Start);
    }
    void Radio::disable() const
    {
        triggerTask(Task::Disable);
    }

    Radio::State Radio::getCurrentState() const
    {
        return (Radio::State)nrf_radio_state_get();
    }

    Radio::CRC_Status Radio::CRC_readStatus() const
    {
        return (Radio::CRC_Status)NRF_RADIO->CRCSTATUS;
    }

    byte Radio::RSSI_readSample() const
    {
        return NRF_RADIO->RSSISAMPLE;
    }

}
