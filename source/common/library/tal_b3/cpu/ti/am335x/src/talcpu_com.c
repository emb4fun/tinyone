/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2016-2023 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.
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
**************************************************************************/
#define __TALCPU_COM_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include "tal.h"

#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_uart_irda_cir.h"
#include "uart_irda_cir.h"
#include "interrupt.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static TAL_COM_DCB *DCBArray[TAL_COM_PORT_MAX];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  IRQHandler                                                           */
/*                                                                       */
/*  This is the generic IRQ handler.                                     */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void IRQHandler (TAL_COM_DCB *pDCB)
{
   TAL_RESULT   Error;
   uint32_t    dBaseAddress = pDCB->HW.dBaseAddress;
   uint32_t    dIntID;
   uint8_t     bData;
   
   /* Checking the source of UART interrupt. */
   dIntID = UARTIntIdentityGet(dBaseAddress);
   
   switch(dIntID)
   {
      /*
       * RX Interrupt
       */
      case UART_INTID_RX_THRES_REACH:
      {
         /* Read data */
         bData = (uint8_t)HWREG(dBaseAddress + UART_RHR);

         /* If we have no overflow... */
         if (TAL_FALSE == pDCB->bRxOverflow)
         {
            /* ... put it into the ring buffer */
            Error = tal_MISCRingAdd(&pDCB->RxRing, &bData);
            if (TAL_OK == Error)
            {
               /* Signal counting semaphore */
               OS_SemaSignalFromInt(&pDCB->RxRdySema);
            }
            else
            {
               /* Ups, overflow */
               pDCB->bRxOverflow = TAL_OK;
            }
         }  
         break;
      } /* UART_INTID_RX_THRES_REACH */

      /*
       * TX Interrupt
       */
      case UART_INTID_TX_THRES_REACH:
      {
         /* 
          * Check for TX interrupt, but only if it is anabled 
          */
         if (HWREG(dBaseAddress + UART_IER) & UART_INT_THR)
         {
            /* Read Data from the ring buffer */
            Error = tal_MISCRingGet(&pDCB->TxRing, &bData);
            if (Error != TAL_OK)
            {
               /* Ups, no data available, disable interrupt */
               UARTIntDisable(dBaseAddress, UART_INT_THR);
            }
            else
            {
               /* Send data */
               HWREG(dBaseAddress + UART_THR) = bData;
            }   
         } /* end "TX interrupt */
         break;
      } /* UART_INTID_TX_THRES_REACH */
       
      default:
      {
         /* Do nothinh */
         break;
      }
   } /* end switch(dIntID) */
   
} /* IRQHandler */

/*************************************************************************/
/*  UARTx_IRQHandler                                                     */
/*                                                                       */
/*  This is the UARTx IRQ handler.                                       */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void UART1_IRQHandler (void)
{
   TAL_CPU_IRQ_ENTER();
   IRQHandler(DCBArray[TAL_COM_PORT_1]);
   TAL_CPU_IRQ_EXIT();
} /* UART1_IRQHandler */

static void UART2_IRQHandler (void)
{
   TAL_CPU_IRQ_ENTER();
   IRQHandler(DCBArray[TAL_COM_PORT_2]);
   TAL_CPU_IRQ_EXIT();
} /* UART2_IRQHandler */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  cpu_COMInit                                                          */
/*                                                                       */
/*  Prepare the hardware for use by the Open function later. Set the HW  */
/*  information depending of ePort and "enable" the COM port.            */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_COMInit (TAL_COM_DCB *pDCB)
{
   TAL_RESULT    Error = TAL_ERR_COM_PORT_RANGE;
   TAL_COM_HW  *pHW    = &pDCB->HW;
   
   switch (pDCB->ePort)
   {
      case TAL_COM_PORT_1:
      {
         Error = tal_BoardEnableCOM1();
         if (TAL_OK == Error)
         {
            DCBArray[TAL_COM_PORT_1] = pDCB;
         
            pHW->dBaseAddress = SOC_UART_0_REGS;
            pHW->nIrqNumber   = CPU_UART0_INT;
            pHW->nIrqPriority = CPU_UART0_PRIO;
            
            /* Reset USART */
            UARTModuleReset(pHW->dBaseAddress);
            
            /* Set irq, priority and function */
            tal_CPUIrqSetPriority(pHW->nIrqNumber, pHW->nIrqPriority);
            tal_CPUIrqSetFunction(pHW->nIrqNumber, UART1_IRQHandler);
         }
         break;
      } /* TAL_COM_PORT_1 */
      
      case TAL_COM_PORT_2:
      {
         Error = tal_BoardEnableCOM2();
         if (TAL_OK == Error)
         {
            DCBArray[TAL_COM_PORT_2] = pDCB;
         
            pHW->dBaseAddress = SOC_UART_1_REGS;
            pHW->nIrqNumber   = CPU_UART1_INT;
            pHW->nIrqPriority = CPU_UART1_PRIO;
            
            /* Reset USART */
            UARTModuleReset(pHW->dBaseAddress);
            
            /* Set irq, priority and function */
            tal_CPUIrqSetPriority(pHW->nIrqNumber, pHW->nIrqPriority);
            tal_CPUIrqSetFunction(pHW->nIrqNumber, UART2_IRQHandler);
         }
         break;
      } /* TAL_COM_PORT_2 */
      
      default:
      {
         /* Do nothing */
         break;
      }
   } /* end switch (pDCB->ePort) */
   
   return(Error);
} /* cpu_COMInit */

/*************************************************************************/
/*  cpu_COMOpen                                                          */
/*                                                                       */
/*  Open the COM port.                                                   */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_COMOpen (TAL_COM_DCB *pDCB)
{
   TAL_RESULT    Error = TAL_ERROR;
   TAL_COM_HW  *pHW    = &pDCB->HW;
   uint32_t     dBaseAddress;
   uint32_t     dLenStbFlag;
   uint32_t     dParityFlag;
   uint32_t     dBaudRate;
   uint32_t     dDivisorValue;
   uint32_t     dUartClock;

   dBaseAddress = pDCB->HW.dBaseAddress;
   dLenStbFlag  = 0;
   dParityFlag  = 0;
   
   /* 
    * Check parameter first 
    */

   /* Check word length */
   switch (pDCB->Settings.eLength)
   {
      case TAL_COM_LENGTH_8:
      {
         dLenStbFlag |= UART_FRAME_WORD_LENGTH_8;
         break;
      }
      
      default:
      {
         Error = TAL_ERR_COM_LENGTH;
         goto COMOpenEnd;  /*lint !e801*/
         break;   /*lint !e527*/
      }
   } /* switch (pDCB->Settings.eLength) */
   
   /* Check parity settings */
   switch (pDCB->Settings.eParity)
   {
      case TAL_COM_PARITY_NONE:
      {
         dParityFlag |= UART_PARITY_NONE;    /*lint !e845*/
         break;
      } 
      
      case TAL_COM_PARITY_EVEN:
      {
         dParityFlag |= UART_EVEN_PARITY;
         break;
      }
      
      case TAL_COM_PARITY_ODD:
      {
         dParityFlag |= UART_ODD_PARITY;
         break;
      }
      
      default:
      {
         Error = TAL_ERR_COM_PARITY;
         goto COMOpenEnd;  /*lint !e801*/
         break;   /*lint !e527*/
      }
   } /* switch (pDCB->Settings.eParity) */
    
   /* Check stop bit settings */
   switch (pDCB->Settings.eStop)
   {
      case TAL_COM_STOP_1_0:
      {
         dLenStbFlag |= UART_FRAME_NUM_STB_1;   /*lint !e845*/
         break;
      }
      
      default:
      {
         Error = TAL_ERR_COM_STOP;
         goto COMOpenEnd;  /*lint !e801*/
         break;   /*lint !e527*/
      }
   } /* switch (pDCB->Settings.eStop) */

   /* Check baud rate */
   if (pDCB->Settings.dBaudrate != 0)
   {
      dBaudRate = pDCB->Settings.dBaudrate;
   }
   else
   {
      Error = TAL_ERR_COM_BAUDRATE;
      goto COMOpenEnd;  /*lint !e801*/
   }
   
   /*
    * Initialize the UARTx
    * (examples\beaglebone\uart\uartEcho.c)
    */

   /* FCLK Functionla clock: PER_CLKOUTM2 / 4 */
   dUartClock = tal_CPUGetFrequencyPeri() / 4;
    
   /* 
    * Performing a module reset. 
    */
   UARTModuleReset(dBaseAddress);
   
   /* 
    * Performing FIFO configurations. 
    */
   
   /* Disable FIFO */ 
   UARTFIFORegisterWrite(dBaseAddress, 0);
   
   /*
    * Performing Baud Rate settings.
    */
   
   /* Computing the Divisor Value. */
   dDivisorValue = UARTDivisorValCompute(dUartClock,
                                         dBaudRate,
                                         UART16x_OPER_MODE,
                                         UART_MIR_OVERSAMPLING_RATE_42);

   /* Programming the Divisor Latches. */
   UARTDivisorLatchWrite(dBaseAddress, dDivisorValue);
     
   /* 
    * Switching to Configuration Mode B. 
    */
   UARTRegConfigModeEnable(dBaseAddress, UART_REG_CONFIG_MODE_B);

   /* 
    * Programming the Line Characteristics. 
    */
   UARTLineCharacConfig(dBaseAddress, dLenStbFlag, dParityFlag);

   /* 
    * Disabling write access to Divisor Latches. 
    */
   UARTDivisorLatchDisable(dBaseAddress);

   /* 
    * Disabling Break Control. 
    */
   UARTBreakCtl(dBaseAddress, UART_BREAK_COND_DISABLE);

   /* 
    * Switching to UART16x operating mode. 
    */
   UARTOperatingModeSelect(dBaseAddress, UART16x_OPER_MODE);
     
   /*
    * Performing Interrupt configuration.
    */  
     
   /* Enabling the specified UART interrupts. */
   UARTIntEnable(dBaseAddress, UART_INT_RHR_CTI);
   
   /* Enable the system interrupt in AINTC. */
   IntSystemEnable((unsigned int)pHW->nIrqNumber);
   
   Error = TAL_OK;

COMOpenEnd:   
   return(Error);
} /* cpu_COMOpen */

/*************************************************************************/
/*  cpu_COMClose                                                         */
/*                                                                       */
/*  Close the COM port.                                                  */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_COMClose (TAL_COM_DCB *pDCB)
{
   TAL_RESULT    Error = TAL_OK;
   TAL_COM_HW  *pHW    = &pDCB->HW;
   uint32_t     dBaseAddress;
   
   dBaseAddress = pDCB->HW.dBaseAddress;

   /* Disable the IRQ */   
   IntSystemEnable((unsigned int)pHW->nIrqNumber);
   
   /* Reset the UART */
   UARTModuleReset(dBaseAddress);
   
   return(Error);
} /* cpu_COMClose */

/*************************************************************************/
/*  cpu_COMStartTx                                                       */
/*                                                                       */
/*  Send the data from the ring buffer if the TX interrupt is disabled.  */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_COMStartTx (TAL_COM_DCB *pDCB)
{
   TAL_RESULT  Error       = TAL_OK;
   uint32_t   dBaseAddress = pDCB->HW.dBaseAddress;
   uint8_t    bData;

   TAL_CPU_DISABLE_ALL_INTS();
   if (HWREG(dBaseAddress + UART_IER) & UART_INT_THR)
   {
      /* TX interrupt is enabled, do nothing */
   }
   else
   {
      /* Get data from the ring buffer */
      Error = tal_MISCRingGet(&pDCB->TxRing, &bData);
      if (TAL_OK == Error)
      {
         /* Send data */
         HWREG(dBaseAddress + UART_THR) = bData;
      
         /* Enable TX interrupt */
         UARTIntEnable(dBaseAddress, UART_INT_THR);
      }
   }
   TAL_CPU_ENABLE_ALL_INTS();
   
   return(Error);
} /* cpu_COMStartTx */

/*************************************************************************/
/*  cpu_COMTxIsRunning                                                   */
/*                                                                       */
/*  Check if TX is still running.                                        */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / TAL_ERROR                                           */
/*************************************************************************/
TAL_RESULT cpu_COMTxIsRunning (TAL_COM_DCB *pDCB)
{
   TAL_RESULT  Error;
   uint32_t   dBaseAddress = pDCB->HW.dBaseAddress;

   TAL_CPU_DISABLE_ALL_INTS();
   if (HWREG(dBaseAddress + UART_IER) & UART_INT_THR)
   {
      /* TX is still running */
      Error = TAL_OK;
   }
   else
   {
      /* TX is not running */
      Error = TAL_ERROR;
   }
   TAL_CPU_ENABLE_ALL_INTS();
   
   return(Error);
} /* cpu_COMTxIsRunning */

/*** EOF ***/
