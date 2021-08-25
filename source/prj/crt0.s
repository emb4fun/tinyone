// **********************************************************************
// *                    SEGGER Microcontroller GmbH                     *
// *                        The Embedded Experts                        *
// **********************************************************************
// *                                                                    *
// *            (c) 2014 - 2018 SEGGER Microcontroller GmbH             *
// *            (c) 2001 - 2018 Rowley Associates Limited               *
// *                                                                    *
// *           www.segger.com     Support: support@segger.com           *
// *                                                                    *
// **********************************************************************
// *                                                                    *
// * All rights reserved.                                               *
// *                                                                    *
// * Redistribution and use in source and binary forms, with or         *
// * without modification, are permitted provided that the following    *
// * conditions are met:                                                *
// *                                                                    *
// * - Redistributions of source code must retain the above copyright   *
// *   notice, this list of conditions and the following disclaimer.    *
// *                                                                    *
// * - Neither the name of SEGGER Microcontroller GmbH                  *
// *   nor the names of its contributors may be used to endorse or      *
// *   promote products derived from this software without specific     *
// *   prior written permission.                                        *
// *                                                                    *
// * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
// * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
// * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
// * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
// * DISCLAIMED.                                                        *
// * IN NO EVENT SHALL SEGGER Microcontroller GmbH BE LIABLE FOR        *
// * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
// * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
// * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
// * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
// * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
// * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
// * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
// * DAMAGE.                                                            *
// *                                                                    *
// **********************************************************************
//
//
//                           Preprocessor Definitions
//                           ------------------------
// APP_ENTRY_POINT
//
//   Defines the application entry point function, if undefined this setting
//   defaults to "main".
//
// INITIALIZE_STACKS
//
//   If defined, the contents of the stacks will be initialized to a the 
//   value 0xCC.
//
// INITIALIZE_SECONDARY_SECTIONS
//
//   If defined, the .text2, .data2 and .bss2 sections will be initialized.
//
// INITIALIZE_USER_SECTIONS
//
//   If defined, the function InitializeUserMemorySections will be called prior
//   to entering main in order to allow the user to initialize any user defined
//   memory sections.
//
// SUPERVISOR_START
//
//   If defined, the application will start up in supervisor mode. If 
//   undefined the application will start up in system mode.
//
// FULL_LIBRARY
//
//  If defined then 
//    - argc, argv are setup by the debug_getargs.
//    - the exit symbol is defined and executes on return from main.
//    - the exit symbol calls destructors, atexit functions and then debug_exit.
//  
//  If not defined then
//    - argc and argv are zero.
//    - the exit symbol is defined, executes on return from main and loops
//

  .section .init, "ax"
  .code 32
  .balign 4

#ifndef APP_ENTRY_POINT
#define APP_ENTRY_POINT main
#endif

#ifndef ARGSSPACE
#define ARGSSPACE 128
#endif
                
  .weak _start
  .global __start  
  .extern APP_ENTRY_POINT
  .global exit
  .weak exit

#ifdef INITIALIZE_USER_SECTIONS
  .extern InitializeUserMemorySections
#endif

/*****************************************************************************
 * Function    : _start                                                      *
 * Description : Main entry point and startup code for C system.             *
 *****************************************************************************/
_start:
__start:                        
  mrs r0, cpsr
  bic r0, r0, #0x1F

  /* Setup stacks */ 
  orr r1, r0, #0x1B /* Undefined mode */
  msr cpsr_cxsf, r1
  ldr sp, =__stack_und_end__
#ifdef __ARM_EABI__
  bic sp, sp, #0x7
#endif
  
  orr r1, r0, #0x17 /* Abort mode */
  msr cpsr_cxsf, r1
  ldr sp, =__stack_abt_end__
#ifdef __ARM_EABI__
  bic sp, sp, #0x7
#endif

  orr r1, r0, #0x12 /* IRQ mode */
  msr cpsr_cxsf, r1
  ldr sp, =__stack_irq_end__
#ifdef __ARM_EABI__
  bic sp, sp, #0x7
#endif

  orr r1, r0, #0x11 /* FIQ mode */
  msr cpsr_cxsf, r1
  ldr sp, =__stack_fiq_end__
#ifdef __ARM_EABI__
  bic sp, sp, #0x7
#endif

  orr r1, r0, #0x13 /* Supervisor mode */
  msr cpsr_cxsf, r1
  ldr sp, =__stack_svc_end__
#ifdef __ARM_EABI__
  bic sp, sp, #0x7
#endif

#ifdef SUPERVISOR_START
  /* Start application in supervisor mode */
  ldr r1, =__stack_end__ /* Setup user/system mode stack */ 
#ifdef __ARM_EABI__
  bic r1, r1, #0x7
#endif
  mov r2, sp
  stmfd r2!, {r1}
  ldmfd r2, {sp}^
#else
  /* Start application in system mode */
  orr r1, r0, #0x1F /* System mode */
  msr cpsr_cxsf, r1
  ldr sp, =__stack_end__
#ifdef __ARM_EABI__
  bic sp, sp, #0x7
#endif
#endif

#ifdef INITIALIZE_STACKS  
  mov r2, #0xCC
  ldr r0, =__stack_und_start__
  ldr r1, =__stack_und_end__
  bl memory_set
  ldr r0, =__stack_abt_start__
  ldr r1, =__stack_abt_end__
  bl memory_set
  ldr r0, =__stack_irq_start__
  ldr r1, =__stack_irq_end__
  bl memory_set
  ldr r0, =__stack_fiq_start__
  ldr r1, =__stack_fiq_end__
  bl memory_set
  ldr r0, =__stack_svc_start__
  ldr r1, =__stack_svc_end__
  bl memory_set  
  ldr r0, =__stack_start__
  ldr r1, =__stack_end__
  bl memory_set  
#endif

  /* Copy initialized memory sections into RAM (if necessary). */
  ldr r0, =__data_load_start__
  ldr r1, =__data_start__
  ldr r2, =__data_end__
  bl memory_copy
  ldr r0, =__text_load_start__
  ldr r1, =__text_start__
  ldr r2, =__text_end__
  bl memory_copy
  ldr r0, =__fast_load_start__
  ldr r1, =__fast_start__
  ldr r2, =__fast_end__
  bl memory_copy
  ldr r0, =__ctors_load_start__
  ldr r1, =__ctors_start__
  ldr r2, =__ctors_end__
  bl memory_copy
  ldr r0, =__dtors_load_start__
  ldr r1, =__dtors_start__
  ldr r2, =__dtors_end__
  bl memory_copy
  ldr r0, =__rodata_load_start__
  ldr r1, =__rodata_start__
  ldr r2, =__rodata_end__
  bl memory_copy
  ldr r0, =__tdata_load_start__
  ldr r1, =__tdata_start__
  ldr r2, =__tdata_end__
  bl memory_copy
#ifdef INITIALIZE_SECONDARY_SECTIONS
  ldr r0, =__data2_load_start__
  ldr r1, =__data2_start__
  ldr r2, =__data2_end__
  bl memory_copy
  ldr r0, =__text2_load_start__
  ldr r1, =__text2_start__
  ldr r2, =__text2_end__
  bl memory_copy
  ldr r0, =__rodata2_load_start__
  ldr r1, =__rodata2_start__
  ldr r2, =__rodata2_end__
  bl memory_copy
#endif /* #ifdef INITIALIZE_SECONDARY_SECTIONS */
  
  /* Zero the bss. */
  ldr r0, =__bss_start__
  ldr r1, =__bss_end__
  mov r2, #0
  bl memory_set
  ldr r0, =__tbss_start__
  ldr r1, =__tbss_end__
  mov r2, #0
  bl memory_set
#ifdef INITIALIZE_SECONDARY_SECTIONS
  ldr r0, =__bss2_start__
  ldr r1, =__bss2_end__
  mov r2, #0
  bl memory_set
#endif /* #ifdef INITIALIZE_SECONDARY_SECTIONS */

  /* Initialize the heap */
  ldr r0, = __heap_start__
  ldr r1, = __heap_end__
  sub r1, r1, r0
  cmp r1, #8
  movge r2, #0
  strge r2, [r0], #+4
  strge r1, [r0]

#ifdef INITIALIZE_USER_SECTIONS
  ldr r2, =InitializeUserMemorySections
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else
  bx r2
#endif
#endif

  /* Call constructors */
  ldr r0, =__ctors_start__
  ldr r1, =__ctors_end__
ctor_loop:
  cmp r0, r1
  beq ctor_end
  ldr r2, [r0], #+4
  stmfd sp!, {r0-r1}
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else    
  bx r2
#endif
  ldmfd sp!, {r0-r1}
  b ctor_loop
ctor_end:

  .type start, function
start:
  /* Jump to application entry point */
#ifdef FULL_LIBRARY
  mov r0, #ARGSSPACE
  ldr r1, =args
  ldr r2, =debug_getargs
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else    
  bx r2
#endif 
  ldr r1, =args
#else
  mov r0, #0
  mov r1, #0
#endif
  ldr r2, =APP_ENTRY_POINT
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else    
  bx r2
#endif

exit:
#ifdef FULL_LIBRARY  
  mov r5, r0 // save the exit parameter/return result
  
  /* Call destructors */
  ldr r0, =__dtors_start__
  ldr r1, =__dtors_end__
dtor_loop:
  cmp r0, r1
  beq dtor_end
  ldr r2, [r0], #+4
  stmfd sp!, {r0-r1}
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else    
  bx r2
#endif
  ldmfd sp!, {r0-r1}
  b dtor_loop
dtor_end:

  /* Call atexit functions */
  ldr r2, =_execute_at_exit_fns
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else    
  bx r2
#endif 

  /* Call debug_exit with return result/exit parameter */
  mov r0, r5
  ldr r2, =debug_exit
  mov lr, pc
#ifdef __ARM_ARCH_3__
  mov pc, r2
#else    
  bx r2
#endif 

#endif

  /* Returned from application entry point/debug_exit, loop forever. */
exit_loop:
  b exit_loop

memory_copy:
  cmp r0, r1
  moveq pc, lr  
  subs r2, r2, r1
  moveq pc, lr  

  /* if either pointer or length is not word aligned then byte copy */
  orr r3, r0, r1
  orr r3, r3, r2
  tst r3, #0x3
  bne 2f
  /* word copy */
1:
  ldr r3, [r0], #+4
  str r3, [r1], #+4
  subs r2, r2, #4
  bne 1b
  mov pc, lr
  /* byte copy */
2:
  ldrb r3, [r0], #+1
  strb r3, [r1], #+1
  subs r2, r2, #1
  bne 2b
  mov pc, lr

memory_set:
  cmp r0, r1
  moveq pc, lr
  strb r2, [r0], #1
  b memory_set

  // default C/C++ library helpers

.macro HELPER helper_name
  .section .text.\helper_name, "ax", %progbits
  .balign 4
  .type \helper_name, function
  .global \helper_name
  .weak \helper_name  
\helper_name:  
.endm

HELPER __aeabi_read_tp
  ldr r0, =__tbss_start__-8
  bx lr
HELPER abort
  b .
HELPER __assert
  b .
HELPER __aeabi_assert
  b .
HELPER __sync_synchronize
  bx lr
HELPER __getchar
  b debug_getchar
HELPER __putchar
  b debug_putchar
HELPER __open
  b debug_fopen
HELPER __close
  b debug_fclose
HELPER __write   
  mov r3, r0
  mov r0, r1
  mov r1, #1  
  b debug_fwrite
HELPER __read  
  mov r3, r0
  mov r0, r1
  mov r1, #1 
  b debug_fread
HELPER __seek
  push {r4, lr}
  mov r4, r0
  bl debug_fseek
  cmp r0, #0
  bne 1f
  mov r0, r4
  bl debug_ftell
  b 2f
1:
  ldr r0, =-1
2:
  pop {r4, lr}
  bx lr
  // char __user_locale_name_buffer[];
  .section .bss.__user_locale_name_buffer, "aw", %nobits
  .global __user_locale_name_buffer
  .weak __user_locale_name_buffer
  __user_locale_name_buffer:
  .word 0x0

#ifdef FULL_LIBRARY
  .bss
args:
  .space ARGSSPACE
#endif

  /* Setup attibutes of stack and heap sections so they don't take up unnecessary room in the elf file */
  .section .stack, "wa", %nobits
  .section .stack_abt, "wa", %nobits
  .section .stack_irq, "wa", %nobits
  .section .stack_fiq, "wa", %nobits
  .section .stack_svc, "wa", %nobits
  .section .stack_und, "wa", %nobits
  .section .heap, "wa", %nobits

