/*****************************************************************************

  Copyright (c) 2012-2014 Rowley Associates Limited.

  This file may be distributed under the terms of the License Agreement
  provided with this software.

  THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *****************************************************************************/

/*****************************************************************************
 *                           Preprocessor Definitions
 *                           ------------------------
 *
 * CTL_TASKING
 *
 *   If defined CTL task rescheduling is done on exit from the irq_handler.
 *
 * PRESERVE_VFP_REGISTERS
 *
 *   If defined the VFP registers are saved/restored on entry/exit of the irq_handler.
 *
 * NO_CACHE_ENABLE
 * 
 *   When specified the MMU is not enabled.
 *
 * NO_ICACHE_ENABLE
 *
 *   If not defined (and NO_CACHE_ENABLE not defined), the I cache is enabled.
 *
 * __NO_FPU
 *
 *   If defined then the VFP coprocessor is NOT enabled.
 *
 * __NO_RUNFAST_MODE
 *
 *   If defined do NOT turn on flush-to-zero and default NaN modes.
 *
 *
 *****************************************************************************/

#ifndef TARGET_AM335x
#include "OMAP35x.h"
#define MASK_ACTIVE_IRQ 0x3f
#else
#include "AM335x.h"
#define MASK_ACTIVE_IRQ 0x7f
#endif

  .section .vectors, "ax"
  .code 32
  .align 0
  .global _vectors
  .global reset_handler
  
/*****************************************************************************
 *                                                                           *
 * Exception Vectors                                                         *
 *                                                                           *
 *****************************************************************************/
_vectors:
#ifdef STARTUP_FROM_RESET
  ldr pc, [pc, #reset_handler_address - . - 8]  /* reset */
#else
  b .                                           /* reset - infinite loop */
#endif
  ldr pc, [pc, #undef_handler_address - . - 8]  /* undefined instruction */
  ldr pc, [pc, #swi_handler_address - . - 8]    /* swi handler */
  ldr pc, [pc, #pabort_handler_address - . - 8] /* abort prefetch */
  ldr pc, [pc, #dabort_handler_address - . - 8] /* abort data */
  nop
  ldr pc, [pc, #irq_handler_address - . - 8]    /* irq */
  ldr pc, [pc, #fiq_handler_address - . - 8]    /* fiq */

reset_handler_address:
  .word reset_handler
undef_handler_address:
  .word undef_handler
swi_handler_address:
  .word swi_handler
pabort_handler_address:
  .word pabort_handler
dabort_handler_address:
  .word dabort_handler
irq_handler_address:
  .word irq_handler
fiq_handler_address:
  .word fiq_handler

#ifndef NO_CACHE_ENABLE
  .section .mmu, "ax"
  .balign 0x4000, 0xff
mmu_page_table:
  .space 0x4000
#endif

  .section .init, "ax"
  .extern CPU_IRQHandler
  .code 32
  .align 0

/******************************************************************************
 *                                                                            *
 * Default exception handlers                                                 *
 *                                                                            *
 ******************************************************************************/
reset_handler:
#ifndef TARGET_AM335x
  // Stop WDT2
  ldr r0, =WDT2_BASE
  ldr r1, =0xAAAA
  str r1, [r0, #0x48]
1:
  ldr r1, [r0, #0x34]
  cmp r1, #(1<<4)
  beq 1b
  ldr r1, =0x5555
  str r1, [r0, #0x48]
#endif

  /* Set the vector base address */
  ldr r0, =_vectors
  mcr p15, 0, r0, c12, c0, 0

#ifndef NO_CACHE_ENABLE
  /* Set the translation table base address */
  ldr r0, =mmu_page_table
  mcr p15, 0, r0, c2, c0, 0          /* Write to TTB register */

  /* Setup the domain access control so accesses are not checked */
  ldr r0, =0xFFFFFFFF
  mcr p15, 0, r0, c3, c0, 0          /* Write to domain access control register */

  /* Create translation table */
  ldr r0, =mmu_page_table
  bl libarm_mmu_flat_initialise_level_1_table

  /* Make the SDRAM cacheable */
  ldr r0, =mmu_page_table
  ldr r1, =__SDRAM_segment_start__
  ldr r2, =__SDRAM_segment_end__
  sub r2, r2, r1
  bl libarm_mmu_flat_set_level_1_cacheable_write_back_region

  /* Enable the MMU and caches */
  mrc p15, 0, r0, c1, c0, 0          /* Read MMU control register */
  orr r0, r0, #0x00001000            /* Enable ICache */
  orr r0, r0, #0x00000005            /* Enable DCache and MMU */
  mcr p15, 0, r0, c1, c0, 0          /* Write MMU control register */
  nop
  nop
#elif !defined(NO_ICACHE_ENABLE)
  mrc p15, 0, r0, c1, c0, 0          /* Read MMU control register */
  orr r0, r0, #0x00001000            /* Enable ICache */ 
  mcr p15, 0, r0, c1, c0, 0          /* Write MMU control register */
  nop
  nop
#endif

#if !defined(__NO_FPU) && !defined(__SOFTFP__)
  // enable cp11 and cp10
  mrc p15, #0x00, r0, c1, c0, #0x02
  orr r0, r0, #0x00F00000
  mcr p15, #0x00, r0, c1, c0, #0x02
  // enable VFP
  mov r0, #0x40000000
  fmxr fpexc, r0
#ifndef __NO_RUNFAST_MODE
  nop
  nop
  nop  
  vmrs r0, fpscr
  orrs r0, r0, #(0x3 << 24) // FZ and DN
  vmsr fpscr, r0
#endif
#endif

  b _start

/******************************************************************************
 *                                                                            *
 * Default exception handlers                                                 *
 * These are declared weak symbols so they can be redefined in user code.     * 
 *                                                                            *
 ******************************************************************************/

undef_handler:
  b .  /* Endless loop */

swi_handler:
  b .  /* Endless loop */

pabort_handler:
  b .  /* Endless loop */

dabort_handler:
  b .  /* Endless loop */

irq_handler:


#define MODE_SYS 0x1F
#define MODE_IRQ 0x12 
#define I_BIT 0x80

  // Store the APCS registers
  sub lr, lr, #4
  push {r0-r3, r12, lr}
  
#if !defined(__NO_FPU) && !defined(__SOFTFP__)
  vpush {d0-d7}
  vpush {d16-d31}
  vmrs r0, fpscr
  push {r0}  
#endif

  // Switch to system mode with interrupts disabled
  mrs  r12, spsr
  push {r12}

  mrs r14, cpsr
  orr r14, r14, #MODE_SYS
  msr cpsr, r14       
          
  // Now running in system mode - need to ensure that sp is 8 byte aligned to call the ISR
  mov r1, sp
  and r1, r1, #4
  sub sp, sp, r1
  // Save the system mode lr and the sp adjustment
  push {r1, lr}

  // Call the IRQHandler 
  blx CPU_IRQHandler

  // Restore the system mode lr and remove the sp adjustment
  pop {r1, lr}
  add sp, sp, r1

  // Switch back to irq mode
  cpsid i, #MODE_IRQ
  pop {r3}
  msr spsr, r3 

#if !defined(__NO_FPU) && !defined(__SOFTFP__)
  pop {r0}
  vmsr fpscr, r0
  vpop {d16-d31}
  vpop {d0-d7}  
#endif

  // Return from interrupt
  pop {r0-r3, r12, lr}
  movs pc, lr

fiq_handler:
  b .  /* Endless loop */

  .weak undef_handler, swi_handler, pabort_handler, dabort_handler, irq_handler, fiq_handler

libarm_mmu_flat_set_level_1_cacheable_write_back_region:
  add r0, r0, r1, lsr #18
  mov r1, r2, lsr #20
1:
  ldr r2, [r0]
  orr r2, r2, #0x0000000C /* set cacheable bit and write back bit */
  str r2, [r0], #4
  subs r1, r1, #1
  bne 1b
  bx lr

libarm_mmu_flat_initialise_level_1_table:
  mov r1, #0x1000
  mov r2, #18
1:
  str r2, [r0], #4
  add r2, r2, #0x100000
  subs r1, r1, #1
  bne 1b
  bx lr
