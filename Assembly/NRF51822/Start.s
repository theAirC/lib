
#ifndef APP_ENTRY_POINT
#define APP_ENTRY_POINT main
#endif

  .global reset_handler
  .global Reset_Handler
  .global Memory_set
  .global Memory_copy

  .syntax unified
  .section .init, "ax"
  .align 2
  .code 16

  .equ NRF_POWER_RAMON_ADDRESS,            0x40000524
  .equ NRF_POWER_RAMONB_ADDRESS,           0x40000554
  .equ NRF_POWER_RAMONx_RAMxON_ONMODE_Msk, 0x3

// Reset Handler
  .thumb_func
  reset_handler:
  Reset_Handler:
    // Power on RAM blocks
      MOVS R1, #NRF_POWER_RAMONx_RAMxON_ONMODE_Msk
      LDR  R0, =NRF_POWER_RAMON_ADDRESS
      LDR  R2, [R0]
      ORRS R2, R1
      STR  R2, [R0]
      LDR  R0, =NRF_POWER_RAMONB_ADDRESS
      LDR  R2, [R0]
      ORRS R2, R1
      STR  R2, [R0]

    // Set up main stack
      LDR  R0, =__stack_end__
      //#ifdef __ARM_EABI__
      //MOVS R1, #0x7
      //BICS R0, R1
      //#endif
      MOV  SP, R0
      #ifdef INITIALIZE_STACK
      LDR  R0, =__stack_start__
      LDR  R1, =__stack_size__
      MOVS R2, #0
      BL   Memory_set
      #endif

    // Set up process stack
      LDR  R0, =__stack_process_end__
      //#ifdef __ARM_EABI__
      //MOVS R2, #0x7
      //BICS R0, R2
      //#endifX
      MSR  PSP, R0
      MOVS R1, #2
      MSR  CONTROL, R1
      #ifdef INITIALIZE_STACK
      LDR  R0, =__stack_process_start__
      LDR  R1, =__stack_process_size__
      MOVS R2, #0
      BL   Memory_set
      #endif

    // Copy initialized memory sections into RAM (if destination is flash, this has no effect)
      LDR  R0, =__data_load_start__
      LDR  R1, =__data_start__
      LDR  R2, =__data_size__
      BL   Memory_copy
      LDR  R0, =__text_load_start__
      LDR  R1, =__text_start__
      LDR  R2, =__text_size__
      BL   Memory_copy
      LDR  R0, =__fast_load_start__
      LDR  R1, =__fast_start__
      LDR  R2, =__fast_size__
      BL   Memory_copy
      LDR  R0, =__ctors_load_start__
      LDR  R1, =__ctors_start__
      LDR  R2, =__ctors_size__
      BL   Memory_copy
      LDR  R0, =__dtors_load_start__
      LDR  R1, =__dtors_start__
      LDR  R2, =__dtors_size__
      BL   Memory_copy
      LDR  R0, =__rodata_load_start__
      LDR  R1, =__rodata_start__
      LDR  R2, =__rodata_size__
      BL   Memory_copy
      LDR  R0, =__tdata_load_start__
      LDR  R1, =__tdata_start__
      LDR  R2, =__tdata_size__
      BL   Memory_copy
      #ifdef INITIALIZE_SECONDARY_SECTIONS
      LDR  R0, =__data2_load_start__
      LDR  R1, =__data2_start__
      LDR  R2, =__data2_size__
      BL   Memory_copy
      LDR  R0, =__text2_load_start__
      LDR  R1, =__text2_start__
      LDR  R2, =__text2_size__
      BL   Memory_copy
      LDR  R0, =__rodata2_load_start__
      LDR  R1, =__rodata2_start__
      LDR  R2, =__rodata2_size__
      BL   Memory_copy
      #endif
      #ifdef INITIALIZE_TCM_SECTIONS
      LDR  R0, =__data_tcm_load_start__
      LDR  R1, =__data_tcm_start__
      LDR  R2, =__data_tcm_size__
      BL   Memory_copy
      LDR  R0, =__text_tcm_load_start__
      LDR  R1, =__text_tcm_start__
      LDR  R2, =__text_tcm_size__
      BL   Memory_copy
      LDR  R0, =__rodata_tcm_load_start__
      LDR  R1, =__rodata_tcm_start__
      LDR  R2, =__rodata_tcm_size__
      BL   Memory_copy
      #endif

    // Zero the bss
      LDR  R0, =__bss_start__
      LDR  R1, =__bss_size__
      MOVS R2, #0
      BL   Memory_set
      LDR  R0, =__tbss_start__
      LDR  R1, =__tbss_size__
      MOVS R2, #0
      BL   Memory_set
      #ifdef INITIALIZE_SECONDARY_SECTIONS
      LDR  R0, =__bss2_start__
      LDR  R1, =__bss2_size__
      MOVS R2, #0
      BL   Memory_set
      #endif
      #ifdef INITIALIZE_TCM_SECTIONS
      LDR  R0, =__bss_tcm_start__
      LDR  R1, =__bss_tcm_size__
      MOVS R2, #0
      BL   Memory_set
      #endif

    // Initialize the heap
      // We only need to zero out the first 8 bytes (if size >= 8),
      // but zeroing out everything needs fewer instructions.
      LDR  R0, =__heap_start__
      LDR  R1, =__heap_size__
      MOVS R2, #0
      BL   Memory_set

    // Initialize user sections
      #ifdef INITIALIZE_USER_SECTIONS
      LDR  R2, =InitializeUserMemorySections
      BLX  R2
      #endif

    // Call constructors
      LDR  R0, =__ctors_end__
      LDR  R1, =__ctors_size__
      RSBS R1, #0
    1:
      BPL  2f
      LDR  R2, [R0, R1]
      PUSH {R0-R1}
      BLX  R2
      POP  {R0-R1}
      ADDS R1, #4
      B    1b
    2:

    // Jump to application entry point
      MOVS R0, #0
      MOVS R1, #0
      LDR  R2, =APP_ENTRY_POINT
      BX   R2

// Sets memory [R0, R0 + R1) to (byte)R2
  .thumb_func
  Memory_set:
      SUBS R1, #1
      BMI  1f // or BLO to allow R1 values > 0x80000000
      STRB R2, [R0, R1]
      B    Memory_set
    1:
      BX   LR

/*
// Sets memory [R0 - '~R1', R0) to (byte)R2
  .thumb_func
  Memory_set:
      ADDS R1, #1
      BPL  1f // or BEQ to allow '~R1' values > 0x80000000
      STRB R2, [R0, R1]
      B    Memory_set
    1:
      BX   LR
*/

// Copies memory [R0, R0 + R2) to [R1, R1 + R2)
  .thumb_func
  Memory_copy:
      SUBS R2, #1
      BMI  1f // or BLO to allow R2 values > 0x80000000
      LDRB R3, [R0, R2]
      STRB R3, [R1, R2]
      B    Memory_copy
    1:
      BX   LR

// Setup attibutes of stack and heap sections so they don't take up room in the elf file
.section .stack, "wa", %nobits
