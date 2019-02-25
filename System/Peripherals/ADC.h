#pragma once

#include <modules/nrfx/hal/nrf_adc.h>

#include <lib/Naming.h>
#include <lib/Instructions.h>

namespace System::Peripherals {

/// Initial configuration

    struct ADC_Config
    {
        enum Resolution {
            Resolution_Bits8  = NRF_ADC_CONFIG_RES_8BIT,
            Resolution_Bits9  = NRF_ADC_CONFIG_RES_9BIT,
            Resolution_Bits10 = NRF_ADC_CONFIG_RES_10BIT
        };
        enum Reference {
            Reference_BangGap         = (NRF_ADC_CONFIG_EXTREFSEL_NONE  << 2) | NRF_ADC_CONFIG_REF_VBG,
            Reference_Supply_OneHalf  = (NRF_ADC_CONFIG_EXTREFSEL_NONE  << 2) | NRF_ADC_CONFIG_REF_SUPPLY_ONE_HALF,
            Reference_Supply_OneThird = (NRF_ADC_CONFIG_EXTREFSEL_NONE  << 2) | NRF_ADC_CONFIG_REF_SUPPLY_ONE_THIRD,
            Reference_External_AREF0  = (NRF_ADC_CONFIG_EXTREFSEL_AREF0 << 2) | NRF_ADC_CONFIG_REF_EXT,
            Reference_External_AREF1  = (NRF_ADC_CONFIG_EXTREFSEL_AREF1 << 2) | NRF_ADC_CONFIG_REF_EXT
        };
        enum InputType {
            InputType_Pin_Direct       = NRF_ADC_CONFIG_SCALING_INPUT_FULL_SCALE,
            InputType_Pin_TwoThirds    = NRF_ADC_CONFIG_SCALING_INPUT_TWO_THIRDS,
            InputType_Pin_OneThird     = NRF_ADC_CONFIG_SCALING_INPUT_ONE_THIRD,
            InputType_Supply_TwoThirds = NRF_ADC_CONFIG_SCALING_SUPPLY_TWO_THIRDS,
            InputType_Supply_OneThird  = NRF_ADC_CONFIG_SCALING_SUPPLY_ONE_THIRD
        };
        enum Channel {
            Channel_None = NRF_ADC_CONFIG_INPUT_DISABLED,
            Channel_A0   = NRF_ADC_CONFIG_INPUT_0,
            Channel_A1   = NRF_ADC_CONFIG_INPUT_1,
            Channel_A2   = NRF_ADC_CONFIG_INPUT_2,
            Channel_A3   = NRF_ADC_CONFIG_INPUT_3,
            Channel_A4   = NRF_ADC_CONFIG_INPUT_4,
            Channel_A5   = NRF_ADC_CONFIG_INPUT_5,
            Channel_A6   = NRF_ADC_CONFIG_INPUT_6,
            Channel_A7   = NRF_ADC_CONFIG_INPUT_7
        };

        Resolution resolution;
        Reference  reference;
        InputType  inputType;
        Channel    channel;
    };

    const ADC_Config ADC_Config_Default = {
        .resolution = ADC_Config::Resolution_Bits10,
        .reference  = ADC_Config::Reference_BangGap,
        .inputType  = ADC_Config::InputType_Supply_OneThird,
        .channel    = ADC_Config::Channel_None
    };

/// Definitions

    struct ADC
    {
        enum Task {
            Task_Start = NRF_ADC_TASK_START,
            Task_Stop = NRF_ADC_TASK_STOP
        };

        enum Event {
            Event_End = NRF_ADC_EVENT_END
        };

        enum Interrupt {
            Interrupt_End = NRF_ADC_INT_END_MASK
        };

        void init(ADC_Config config);
        void shutdown();

        void enable();
        void disable();
        bool isEnabled();

        void triggerTask(Task task);
        void clearEvent(Event event);
        bool checkEvent(Event event);
        bool checkAndClearEvent(Event event);
        u32 getTaskAddress(Task task);
        u32 getEventAddress(Event event);

        void enableInterrupt(Interrupt interrupt);
        void disableInterrupt(Interrupt interrupt);
        bool isInterruptEnabled(Interrupt interrupt);

        void applyConfig(ADC_Config config);

        void setResolution(ADC_Config::Resolution resolution);
        void setReference(ADC_Config::Reference reference);
        void setInputType(ADC_Config::InputType inputType);
        void setChannel(ADC_Config::Channel channel);

        void start();
        bool isBusy();
        bool hasFinished();
        u32 getResult();
        u32 getOneSample();
    };

    ADC adc;

/// Implementation

    void ADC::init(ADC_Config config)
    {
        clearEvent(Event_End);
        applyConfig(config);
    }
    void ADC::shutdown()
    {
        disable();
    }

    void ADC::enable()
    {
        nrf_adc_enable();
    }
    void ADC::disable()
    {
        nrf_adc_disable();
    }
    bool ADC::isEnabled()
    {
        return nrf_adc_enable_check();
    }

    void ADC::applyConfig(ADC_Config config)
    {
        setResolution(config.resolution);
        setReference(config.reference);
        setInputType(config.inputType);
        setChannel(config.channel);
    }

    void ADC::setResolution(ADC_Config::Resolution resolution)
    {
        NRF_ADC->CONFIG = BFI(NRF_ADC->CONFIG, ADC_CONFIG_RES_Pos, ADC_CONFIG_RES_Msk, resolution);
    }
    void ADC::setReference(ADC_Config::Reference reference)
    {
        NRF_ADC->CONFIG = BFI(NRF_ADC->CONFIG, ADC_CONFIG_REFSEL_Pos, ADC_CONFIG_REFSEL_Msk, (u32)reference >> 0);
        NRF_ADC->CONFIG = BFI(NRF_ADC->CONFIG, ADC_CONFIG_EXTREFSEL_Pos, ADC_CONFIG_EXTREFSEL_Msk, (u32)reference >> 2);
    }
    void ADC::setInputType(ADC_Config::InputType inputType)
    {
        NRF_ADC->CONFIG = BFI(NRF_ADC->CONFIG, ADC_CONFIG_INPSEL_Pos, ADC_CONFIG_INPSEL_Msk, inputType);
    }
    void ADC::setChannel(ADC_Config::Channel channel)
    {
        NRF_ADC->CONFIG = BFI(NRF_ADC->CONFIG, ADC_CONFIG_PSEL_Pos, ADC_CONFIG_PSEL_Msk, channel);
    }

    void ADC::triggerTask(Task task)
    {
        nrf_adc_task_trigger((nrf_adc_task_t)task);
    }
    void ADC::clearEvent(Event event)
    {
        nrf_adc_event_clear((nrf_adc_event_t)event);
    }
    bool ADC::checkEvent(Event event)
    {
        return nrf_adc_event_check((nrf_adc_event_t)event);
    }
    bool ADC::checkAndClearEvent(Event event)
    {
        bool result = checkEvent(event);
        if (result) clearEvent(event);
        return result;
    }
    u32 ADC::getTaskAddress(Task task)
    {
        return nrf_adc_task_address_get((nrf_adc_task_t)task);
    }
    u32 ADC::getEventAddress(Event event)
    {
        return nrf_adc_event_address_get((nrf_adc_event_t)event);
    }

    void ADC::enableInterrupt(Interrupt interrupt)
    {
        nrf_adc_int_enable(interrupt);
    }
    void ADC::disableInterrupt(Interrupt interrupt)
    {
        nrf_adc_int_disable(interrupt);
    }
    bool ADC::isInterruptEnabled(Interrupt interrupt)
    {
        return nrf_adc_int_enable_check(interrupt);
    }

    void ADC::start()
    {
        triggerTask(Task_Start);
    }
    bool ADC::isBusy()
    {
        return nrf_adc_busy_check();
    }
    bool ADC::hasFinished()
    {
        return checkAndClearEvent(Event_End);
    }
    u32 ADC::getResult()
    {
        return nrf_adc_result_get();
    }
    u32 ADC::getOneSample()
    {
        start();
        while (!hasFinished());
        return getResult();
    }

}
