/**************************************************************************
*  Copyright (c) 2022 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.
*
*  With parts of antoinealb.net and ported to uC/OS-III:
*  https://antoinealb.net/programming/2013/12/06/lwip-ucos2.html
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
***************************************************************************
*  History:
*
*  15.10.2022  mifi  First Version.
**************************************************************************/
#if defined(RTOS_UCOS3)
#define __SYS_ARCH_UCOS3_C__

/*
 * The operating system emulation layer is located in two files, cc.h
 * and sys_arch.c. It provides a common interface between the lwIP code
 * and the underlying operating system kernel. The general idea is that
 * porting lwIP to new architectures requires only small changes to a few
 * header files and a new sys_arch implementation. It is also possible to
 * do a sys_arch implementation that does not rely on any underlying
 * operating system. 
 *
 * Source lwIP Wiki:
 *    http://lwip.wikia.com/wiki/Porting_for_an_OS_1.4.0#sys_arch.c
 */    
 

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "project.h"
#include "tal.h"
#include "osal.h"

#include "lwip\sys.h"
#include "lwip\api.h"
#include "lwip\tcpip.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define SEMA_MAGIC   0x616D6573
#define MBOX_MAGIC   0x786F626D

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static OS_STACK (tcpip_stack, TCPIP_THREAD_STACKSIZE);
static OS_STACK (iprx_stack, TASK_IP_RX_STK_SIZE);

static OS_TCB tcb_tcpip;
static OS_TCB tcb_iprx;

#if defined(TASK_IP_RX2_STK_SIZE)
static OS_STACK (iprx_2_stack, TASK_IP_RX2_STK_SIZE);
static OS_TCB tcb_iprx_2;
#endif

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*
 * System
 */

/*
 * lwIP system initialization. This function is called before any other
 * sys_arch-function is called and is meant to be used to initialize
 * anything that has to be up and running for the rest of the functions to
 * work. for example to set up a pool of semaphores.
 */
void sys_init (void) 
{
}

/*
 * Returns the current time in milliseconds
 */
u32_t sys_now (void)
{
   return(OS_TimeGet());
}



/*
 * Semaphores
 */
 
/* 
 * Creates a new semaphore returns it through the sem pointer provided
 * as argument to the function, in addition the function returns ERR_MEM
 * if a new semaphore could not be created and ERR_OK if the semaphore
 * was created. The count argument specifies the initial state of the
 * semaphore.
 */   
err_t sys_sem_new (sys_sem_t *sem, u8_t count)
{
   OS_ERR err;
   
   sem->dValid = SEMA_MAGIC;
   
   OSSemCreate(&sem->Sema, NULL, count, &err);
   (void)err;

   return(ERR_OK);
}

/*
 * Frees a semaphore created by sys_sem_new. Since these two functions
 * provide the entry and exit point for all semaphores used by lwIP,
 * you have great flexibility in how these are allocated and deallocated
 * (for example, from the heap, a memory pool, a semaphore pool, etc).
 */
void sys_sem_free (sys_sem_t *sem)
{
   OS_ERR err;
   
   OSSemDel(&sem->Sema, OS_OPT_DEL_NO_PEND, &err);
   (void)err;
}

/*
 * Signals (or releases) a semaphore referenced by * sem.
 */
void sys_sem_signal (sys_sem_t *sem)
{
   OS_ERR err;
   
   OSSemPost(&sem->Sema, OS_OPT_POST_ALL, &err);
   (void)err;
}

/*
 * Blocks the thread while waiting for the semaphore to be signaled.
 * The timeout parameter specifies how many milliseconds the function should
 * block before returning; if the function times out, it should return
 * SYS_ARCH_TIMEOUT. If timeout=0, then the function should block indefinitely.
 * If the function acquires the semaphore, it should return how many milliseconds
 * expired while waiting for the semaphore. The function may return 0 if the
 * semaphore was immediately available.
 */
u32_t sys_arch_sem_wait (sys_sem_t *sem, u32_t timeout)
{
   OS_ERR   err;
   uint32_t StartTime;
   uint32_t EndTime;
   
   if (timeout != 0)
   {
      timeout = OS_MS_2_TICKS(timeout);
   }      

   StartTime = OS_TimeGet();

   OSSemPend(&sem->Sema, timeout, OS_OPT_PEND_BLOCKING, NULL, &err);
   if (OS_ERR_TIMEOUT == err)
   {
      timeout = SYS_ARCH_TIMEOUT;
   }
   else
   {
      EndTime = OS_TimeGet();
      timeout = OS_TICKS_2_MS(EndTime - StartTime);
   }
   
   return(timeout);
}

/*
 * Check if a sempahore is valid/allocated:
 * return 1 for valid, 0 for invalid 
 */
int sys_sem_valid (sys_sem_t *sem) 
{
   if (SEMA_MAGIC == sem->dValid)
   {
      return(1);
   }
   
   return(0);
}

/*
 * Set a semaphore invalid so that sys_sem_valid returns 0 
 */
void sys_sem_set_invalid(sys_sem_t *sem)
{
   sem->dValid = 0;
}


/*
 * Mailboxes
 */
 
/*
 * Trys to create a new mailbox and return it via the mbox pointer provided
 * as argument to the function. Returns ERR_OK if a mailbox was created and
 * ERR_MEM if the mailbox on error. 
 */
err_t sys_mbox_new (sys_mbox_t *mbox, int size)
{
   OS_ERR err;
   
   if (size != MBOX_SIZE_MAX)
   {
      /* Error */
      TAL_FAILED();
      return(ERR_MEM);
   }

   mbox->dValid = MBOX_MAGIC;
   
   OSQCreate(&mbox->Q, NULL, size, &err);
   OSSemCreate(&mbox->Full, NULL, size, &err);
   (void)err;
   
   return(ERR_OK);
}

/*
 * Deallocates a mailbox. If there are messages still present in the mailbox
 * when the mailbox is deallocated, it is an indication of a programming error
 * in lwIP and the developer should be notified.
 */
void sys_mbox_free (sys_mbox_t *mbox)
{
   OS_ERR err;

   OSQFlush(&mbox->Q, &err);
   OSQDel(&mbox->Q, OS_OPT_DEL_NO_PEND, &err);
   OSSemDel(&mbox->Full, OS_OPT_DEL_NO_PEND, &err);
   (void)err;
}   


/*
 * Posts the "msg" to the mailbox. 
 */
void sys_mbox_post (sys_mbox_t *mbox, void *msg)
{
   OS_ERR err;

   OSSemPend(&mbox->Full, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
   OSQPost(&mbox->Q, msg, sizeof(void*), OS_OPT_POST_FIFO, &err);
   (void)err;
}

/*
 * Tries to post a message to mbox by polling (no timeout). The function
 * returns ERR_OK on success and ERR_MEM if it can't post at the moment.
 */
err_t sys_mbox_trypost (sys_mbox_t *mbox, void *msg)
{
   OS_ERR err;
   
   OSSemPend(&mbox->Full, 0, OS_OPT_PEND_NON_BLOCKING, NULL, &err);
   if (OS_ERR_NONE == err)
   {
      OSQPost(&mbox->Q, msg, sizeof(void*), OS_OPT_POST_FIFO, &err);
   }
   else
   {
      return(ERR_MEM);
   }   
   
   return(ERR_OK);
}

/*
 * Blocks the thread until a message arrives in the mailbox, but does not
 * block the thread longer than timeout milliseconds
 * (similar to the sys_arch_sem_wait() function). If timeout=0, then the
 * function should block indefinitely. The msg argument is a pointer to the
 * message in the mailbox and may be NULL to indicate that the message
 * should be dropped. This should return either SYS_ARCH_TIMEOUT or the
 * number of milliseconds elapsed waiting for a message.
 */
u32_t sys_arch_mbox_fetch (sys_mbox_t *mbox, void **msg, u32_t timeout)
{
   OS_ERR      err;
   OS_MSG_SIZE size;
   uint32_t    StartTime;
   uint32_t    EndTime;
   void       *tmp;
   
   if (timeout != 0)
   {
      timeout = OS_MS_2_TICKS(timeout);
   }      

   StartTime = OS_TimeGet();
   
   tmp = OSQPend(&mbox->Q, timeout, OS_OPT_PEND_BLOCKING, &size, NULL, &err);
   if (OS_ERR_TIMEOUT == err)
   {
      timeout = SYS_ARCH_TIMEOUT;
   }
   else
   {
      EndTime = OS_TimeGet();
      timeout = OS_TICKS_2_MS(EndTime - StartTime);
   
      OSSemPost(&mbox->Full, OS_OPT_POST_ALL, &err);

      *msg = tmp;   
   }
   
   return(timeout);
}

/*
 * This is similar to sys_arch_mbox_fetch, however if a message is not
 * present in the mailbox, it immediately returns with the code SYS_MBOX_EMPTY.
 * On success 0 is returned with msg pointing to the message retrieved from
 * the mailbox.
 */
u32_t sys_arch_mbox_tryfetch (sys_mbox_t *mbox, void **msg)
{
   OS_ERR      err;
   OS_MSG_SIZE size;
   void       *tmp;
   
   tmp = OSQPend(&mbox->Q, 0, OS_OPT_PEND_NON_BLOCKING, &size, NULL, &err);
   if (OS_ERR_NONE == err)
   {
      OSSemPost(&mbox->Full, OS_OPT_POST_ALL, &err);

      *msg = tmp;   
      
      return(0);
   }
   else
   {
      return(SYS_MBOX_EMPTY);
   }
}


/*
 * Check if an mbox is valid/allocated:
 * return 1 for valid, 0 for invalid
 */
int sys_mbox_valid (sys_mbox_t *mbox)
{
   if (MBOX_MAGIC == mbox->dValid)
   {
      return(1);
   }
   
   return(0);
}

/* 
 * Set an mbox invalid so that sys_mbox_valid returns 0 
 */
void sys_mbox_set_invalid (sys_mbox_t *mbox)
{
   mbox->dValid = 0;
}


/*
 * Threads
 */

/*
 * name is the thread name. thread(arg) is the call made as the thread's
 * entry point. stacksize is the recommanded stack size for this thread.
 * prio is the priority that lwIP asks for. Stack size(s) and priority(ies)
 * have to be are defined in lwipopts.h, and so are completely customizable
 * for your system.
 */
sys_thread_t sys_thread_new (const char *name, lwip_thread_fn thread, 
                             void *arg, int stacksize, int prio)                             
{
   sys_thread_t rc = 0;
   
   if ((TCPIP_THREAD_PRIO == prio) && (TCPIP_THREAD_STACKSIZE == stacksize))
   {
      OS_TaskCreate(&tcb_tcpip, thread, arg, prio,
                    tcpip_stack, sizeof(tcpip_stack), 
                    (char*)name);
      rc = 1;                   
   }   

   if ((TASK_IP_RX_PRIORITY == prio) && (TASK_IP_RX_STK_SIZE == stacksize))
   {
      OS_TaskCreate(&tcb_iprx, thread, arg, prio,
                    iprx_stack, sizeof(iprx_stack), 
                    (char*)name);

      rc = 1;                   
   }   
   
#if defined(TASK_IP_RX2_STK_SIZE)

   if ((TASK_IP_RX2_PRIORITY == prio) && (TASK_IP_RX2_STK_SIZE == stacksize))
   {
      OS_TaskCreate(&tcb_iprx_2, thread, arg, prio,
                    iprx_2_stack, sizeof(iprx_2_stack), 
                    (char*)name);

      rc = 1;                   
   }  

#endif
   
   return(rc);
}                              


/*
 * Misc
 */
 
#if defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__)
/* Cortex-M3/M4 */

/*
 * sys_arch_protect / sys_arch_unprotect
 *
 * These functions are used to disable interrupts around crucial blocks of
 * code that should not be interrupted -- usually done in order to preserve
 * data integrity. In order to allow your code to nest protection blocks
 * (for example a block of protection inside another block of protection,
 * possibly in different functions), a parameter (shown as "x" here) is
 * used to remember the prior protection state so that when a block ends
 * the previous state is restored.
 */ 

uint32_t sys_arch_protect (void)
{
   uint32_t mask = 0;
   
   __asm__ volatile ("MRS %0, primask" : "=r" (mask) );
   __asm__ volatile ("cpsid i" : : : "memory");
   
   return(mask);
}

void sys_arch_unprotect (uint32_t mask)
{
   __set_PRIMASK(mask);
}
#endif /* defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7M__) */


#if (SYS_LIGHTWEIGHT_PROT) && defined(beaglebone)

/* Sitara header files required for this interface driver. */
#include "interrupt.h"

/**
 * This function is used to lock access to critical sections when lwipopt.h
 * defines SYS_LIGHTWEIGHT_PROT. It disables interrupts and returns a value
 * indicating the interrupt enable state when the function entered. This
 * value must be passed back on the matching call to sys_arch_unprotect().
 *
 * @return the interrupt level when the function was entered.
 */
sys_prot_t sys_arch_protect (void)
{
   sys_prot_t status;
   status = (IntMasterStatusGet() & 0xFF);
   
   IntMasterIRQDisable();
   return status;
}

/**
 * This function is used to unlock access to critical sections when lwipopt.h
 * defines SYS_LIGHTWEIGHT_PROT. It enables interrupts if the value of the lev
 * parameter indicates that they were enabled when the matching call to
 * sys_arch_protect() was made.
 *
 * @param lev is the interrupt level when the matching protect function was
 * called
 */
void sys_arch_unprotect (sys_prot_t lev)
{
   /* Only turn interrupts back on if they were originally on when the matching
      sys_arch_protect() call was made. */
   if((lev & 0x80) == 0) 
   {
      IntMasterIRQEnable();
   }
}

#endif /* (SYS_LIGHTWEIGHT_PROT) && defined(beaglebone) */

#endif /* RTOS_UCOS3 */

/*** EOF ***/




