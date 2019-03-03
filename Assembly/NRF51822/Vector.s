//! Vector table, exception & intrrupt handlers

    .syntax unified
    .code 16

    .section .init, "ax"
    .align 0

/// Default Handlers, comment out manually the ones you have defined externally

    .thumb_func
    .weak Uncaught_Handler
  Uncaught_Handler:
    B . // while (1);

  .weakref NMI_Handler            , Uncaught_Handler
  .weakref HardFault_Handler      , Uncaught_Handler
  .weakref SVC_Handler            , Uncaught_Handler
  .weakref PendSV_Handler         , Uncaught_Handler
  .weakref SysTick_Handler        , Uncaught_Handler

  .weakref POWER_CLOCK_IRQHandler , Uncaught_Handler
  .weakref RADIO_IRQHandler       , Uncaught_Handler
  .weakref UART0_IRQHandler       , Uncaught_Handler
  .weakref SPI0_TWI0_IRQHandler   , Uncaught_Handler
  .weakref SPI1_TWI1_IRQHandler   , Uncaught_Handler
  .weakref GPIOTE_IRQHandler      , Uncaught_Handler
  //.weakref ADC_IRQHandler         , Uncaught_Handler
  .weakref TIMER0_IRQHandler      , Uncaught_Handler
  .weakref TIMER1_IRQHandler      , Uncaught_Handler
  .weakref TIMER2_IRQHandler      , Uncaught_Handler
  .weakref RTC0_IRQHandler        , Uncaught_Handler
  .weakref TEMP_IRQHandler        , Uncaught_Handler
  .weakref RNG_IRQHandler         , Uncaught_Handler
  .weakref ECB_IRQHandler         , Uncaught_Handler
  .weakref CCM_AAR_IRQHandler     , Uncaught_Handler
  .weakref WDT_IRQHandler         , Uncaught_Handler
  .weakref RTC1_IRQHandler        , Uncaught_Handler
  .weakref QDEC_IRQHandler        , Uncaught_Handler
  .weakref LPCOMP_IRQHandler      , Uncaught_Handler
  .weakref SWI0_IRQHandler        , Uncaught_Handler
  .weakref SWI1_IRQHandler        , Uncaught_Handler
  .weakref SWI2_IRQHandler        , Uncaught_Handler
  .weakref SWI3_IRQHandler        , Uncaught_Handler
  .weakref SWI4_IRQHandler        , Uncaught_Handler
  .weakref SWI5_IRQHandler        , Uncaught_Handler

/// Vector Table

    .section .vectors, "ax"
    .align 0
    .global _vectors
    .extern __stack_end__

  _vectors:
    .word __stack_end__
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word SVC_Handler
    .word 0 // Reserved
    .word 0 // Reserved
    .word PendSV_Handler
    .word SysTick_Handler

  // External Interrupts
    .word POWER_CLOCK_IRQHandler
    .word RADIO_IRQHandler
    .word UART0_IRQHandler
    .word SPI0_TWI0_IRQHandler
    .word SPI1_TWI1_IRQHandler
    .word 0 // Reserved
    .word GPIOTE_IRQHandler
    .word ADC_IRQHandler
    .word TIMER0_IRQHandler
    .word TIMER1_IRQHandler
    .word TIMER2_IRQHandler
    .word RTC0_IRQHandler
    .word TEMP_IRQHandler
    .word RNG_IRQHandler
    .word ECB_IRQHandler
    .word CCM_AAR_IRQHandler
    .word WDT_IRQHandler
    .word RTC1_IRQHandler
    .word QDEC_IRQHandler
    .word LPCOMP_IRQHandler
    .word SWI0_IRQHandler
    .word SWI1_IRQHandler
    .word SWI2_IRQHandler
    .word SWI3_IRQHandler
    .word SWI4_IRQHandler
    .word SWI5_IRQHandler
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
    .word 0 // Reserved
  _vectors_end:

  #ifdef VECTORS_IN_RAM
    .section .vectors_ram, "ax"
    .align 0
    .global _vectors_ram

  _vectors_ram:
    .space _vectors_end - _vectors, 0
  #endif
