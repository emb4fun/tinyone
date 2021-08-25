/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2017 by Michael Fischer (www.emb4fun.de).
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
*
***************************************************************************
*  History:
*
*  23.12.2017  mifi  First Version for the BeagleBone.  
**************************************************************************/
#define __TALCPU_CAN_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "tal.h"

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "dcan.h"
#include "interrupt.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

#define CAN_ID_EXT_MASK    0x1FFFFFFF
#define CAN_ID_STD_MASK    0x7FF

/*
 * Some CAN timing calculators:
 * http://www.bittiming.can-wiki.info
 */ 

/*
 * BTR defines for 24MHz clock, Sample Point 87.5%
 */
#define BTR_1000K_24MHZ       0x00000502  
#define BTR_500K_24MHZ        0x00001C02
#define BTR_250K_24MHZ        0x00001C05
#define BTR_125K_24MHZ        0x00001C0B
#define BTR_100K_24MHZ        0x00001C0E
#define BTR_50K_24MHZ         0x00001C1D
#define BTR_20K_24MHZ         0x00011C0A
#define BTR_10K_24MHZ         0x00021C15
#define BTR_5K_24MHZ          0x00041C2B


#define DCAN_NO_INT_PENDING   0x0000
#define DCAN_ERROR_OCCURED    0x8000

#define TX_MSG_INDEX          1
#define RX_MSG_START          2
#define RX_MSG_END            64

            
/*
 * Filter defines
 */
#define FILTER_ID_MASK           0x1FFFFFFF
#define FILTER_IN_USE_FLAG       0x80000000
#define FILTER_IS_EXT_FLAG       0x40000000
#define FILTER_IS_ALL_FLAG       0x20000000

#define FILTER_ID(_x)            (_x & FILTER_ID_MASK)
#define IS_FILTER_IN_USE(_x)     (_x & FILTER_IN_USE_FLAG)
#define IS_FILTER_EXT(_x)        (_x & FILTER_IS_EXT_FLAG)
#define IS_FILTER_ALL(_x)        (_x & FILTER_IS_ALL_FLAG)

#define CAN_STD_ENTRY_ID_MASK    (0x7FF)
#define CAN_EXT_ENTRY_ID_MASK    (0x1FFFFFFF)
            
/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static TAL_CAN_DCB *DCBArray[TAL_CAN_PORT_MAX];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  DCANMsgRAMInit                           (platform\evmAM335x\dcan.c) */
/*                                                                       */
/*  This function initializes the DCAN message RAM.                      */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void DCANMsgRAMInit (TAL_CAN_DCB *pDCB)
{
   uint32_t dValue;
   
   /* See 23.3.8.1.3 DCAN RAM Hardware Initialization */
   
   switch (pDCB->ePort)
   { 
      case TAL_CAN_PORT_1:
      {
         /* Start DCAN0 RAM initialization sequence */
         HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= 
               CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_START;
         
         /* Wait for the RAMINIT_DONE bit to be set to ensure successful RAM initialization. */
         dValue = HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT);
         while ((dValue & CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_DONE) != 
                 CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_DONE)
         {
            dValue = HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT);
         } 

         /* Clear the DCAN0 RAM Initialization complete bit */         
         HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= 
               CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_DONE;
         break;
      } /* TAL_CAN_PORT_1 */
    
      case TAL_CAN_PORT_2:
      {
         /* Start DCAN1 RAM initialization sequence */
         HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= 
               CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_START;
         
         /* Wait for the RAMINIT_DONE bit to be set to ensure successful RAM initialization. */
         dValue = HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT);
         while ((dValue & CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_DONE) != 
                 CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_DONE)
         {
            dValue = HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT);
         } 

         /* Clear the DCAN1 RAM Initialization complete bit */         
         HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= 
               CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_DONE;
         break;
      } /* TAL_CAN_PORT_1 */
      
      default:
      {
         /* Do nothing */
         break;
      }
   }
   
} /* DCANMsgRAMInit */

/*************************************************************************/
/*  SendObject                                                           */
/*                                                                       */
/*  Send the given Object.                                               */
/*                                                                       */
/*  In    : pDCB, pCANObject                                             */
/*  Out   : none                                                         */
/*  Return: TAL_OK                                                       */
/*************************************************************************/
static TAL_RESULT SendObject (TAL_CAN_DCB *pDCB, TAL_CAN_OBJECT *pCANObject)
{
   uint32_t dBaseAddress = pDCB->HW.dBaseAddress;
   uint8_t *pData;
   uint32_t dARB;
   uint32_t dDLC;
   uint32_t dDataA;
   uint32_t dDataB;

   /* Set the message valid and direction bit */
   dARB = DCAN_IFARB_MSGVAL | (DCAN_IFARB_DIR_TRANSMIT << DCAN_IFARB_DIR_SHIFT);
   
   /* Set identifier and XTD bit if needed */   
   if(pCANObject->bFlags & TAL_CAN_OBJECT_FLAGS_EXT_ID)
   {
      dARB |= DCAN_IFARB_XTD;
      dARB |= ((pCANObject->dIdentifier & CAN_ID_EXT_MASK) << DCAN_ID_MSK_29_BIT);
   }
   else
   {
      dARB |= ((pCANObject->dIdentifier & CAN_ID_STD_MASK) << DCAN_ID_MSK_11_BIT);
   }
   
   /* Setup Message Control Register */
   dDLC = (pCANObject->bDLC & DCAN_IFMCTL_DATALENGTHCODE) | DCAN_IFMCTL_EOB;
   
   /* Setup Data A Register */
   pData    = (uint8_t*)&dDataA;
   *pData++ = pCANObject->Data[0]; 
   *pData++ = pCANObject->Data[1]; 
   *pData++ = pCANObject->Data[2]; 
   *pData   = pCANObject->Data[3]; 
   
   /* Setup Data B Register */
   pData    = (uint8_t*)&dDataB;
   *pData++ = pCANObject->Data[4]; 
   *pData++ = pCANObject->Data[5]; 
   *pData++ = pCANObject->Data[6]; 
   *pData   = pCANObject->Data[7]; 

   /*lint -save -e778 -e788 -e845 */   
   
   /* Wait in loop until busy bit is cleared */
   while(DCANIFBusyStatusGet(dBaseAddress, DCAN_IF1_REG)); /*lint !e722*/
   
   /* Write the Arbitration Register */
   HWREG(dBaseAddress + DCAN_IFARB(DCAN_IF1_REG)) = dARB;

   /* Write the data bytes to IFDATA / IFDATB register */
   HWREG(dBaseAddress + DCAN_IFDATA(DCAN_IF1_REG)) = dDataA;
   HWREG(dBaseAddress + DCAN_IFDATB(DCAN_IF1_REG)) = dDataB;
     
   /* Write the Message Control register */
   HWREG(dBaseAddress + DCAN_IFMCTL(DCAN_IF1_REG)) = dDLC;
   
   /* Enable transmit interrupt */
   DCANMsgObjIntEnable(dBaseAddress, DCAN_TRANSMIT_INT, DCAN_IF1_REG);
   
   /*lint -restore */
   
   pDCB->HW.dTxIE = 1;  /* Set TX interrupt enabled indicator */

   /* Configure the command register which will send the object */
   DCANCommandRegSet(dBaseAddress, (DCAN_DAT_A_ACCESS | DCAN_MSG_WRITE | DCAN_TXRQST_ACCESS | 
                                    DCAN_DAT_B_ACCESS | DCAN_ACCESS_CTL_BITS | 
                                    DCAN_ACCESS_ARB_BITS), TX_MSG_INDEX, DCAN_IF1_REG);    
   
   return(TAL_OK);   
} /* SendObject */

/*************************************************************************/
/*  TX_IRQHandler                                                        */
/*                                                                       */
/*  This is the generic TX IRQHandler.                                   */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void TX_IRQHandler (TAL_CAN_DCB *pDCB)
{
   TAL_RESULT     Error;
   TAL_CAN_OBJECT CANObject;
   uint32_t      dBaseAddress = pDCB->HW.dBaseAddress;

   /* Set the ClrIntPnd bit of DCAN_IFCMD register */
   DCANCommandRegSet(dBaseAddress, DCAN_CLR_INTPND, TX_MSG_INDEX, DCAN_IF1_REG);
   
   /* Read a new Object from the ring buffer */
   Error = tal_MISCRingGet(&pDCB->TxRing, (uint8_t*)&CANObject);
   if (Error != TAL_OK)
   {
      /* Ups, no data available, disable interrupt */
      pDCB->HW.dTxIE = 0;
   }
   else
   {
      /* Send Object */
      SendObject(pDCB, &CANObject); 
   }   

} /* TX_IRQHandler */

/*************************************************************************/
/*  RX_IRQHandler                                                        */
/*                                                                       */
/*  This is the generic RX IRQHandler.                                   */
/*                                                                       */
/*  In    : pDCB, dMsgNum                                                */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void RX_IRQHandler (TAL_CAN_DCB *pDCB, uint32_t dMsgNum)
{
   TAL_RESULT     Error;
   TAL_CAN_OBJECT CANObject;
   uint32_t      dBaseAddress = pDCB->HW.dBaseAddress;
   uint8_t      *pData;
   uint32_t      dARB;
   uint32_t      dDLC;
   uint32_t      dDataA;
   uint32_t      dDataB;
   uint32_t      dIdentifier;
   uint8_t       bFlags;
   uint8_t       bDLC;
   

   /*
    * Get the data from the IF2 register
    */

   /* Transfer the message object addressed by message number to the IF2 register */
   DCANCommandRegSet(dBaseAddress, (DCAN_DAT_A_ACCESS | DCAN_DAT_B_ACCESS | 
                                    DCAN_TXRQST_ACCESS | DCAN_CLR_INTPND | 
                                    DCAN_ACCESS_CTL_BITS | DCAN_ACCESS_ARB_BITS | 
                                    DCAN_ACCESS_MSK_BITS | DCAN_MSG_READ), 
                                    dMsgNum, DCAN_IF2_REG);

   /* Clear the NewDat bit of DCAN_IFMCTL register */
   DCANNewDataControl(dBaseAddress, DCAN_NEW_DAT_CLR, DCAN_IF2_REG);
   
   dARB   = HWREG(dBaseAddress + DCAN_IFARB(DCAN_IF2_REG));
   dDLC   = HWREG(dBaseAddress + DCAN_IFMCTL(DCAN_IF2_REG)) & DCAN_IFMCTL_DATALENGTHCODE;
   dDataA = HWREG(dBaseAddress + DCAN_IFDATA(DCAN_IF2_REG));
   dDataB = HWREG(dBaseAddress + DCAN_IFDATB(DCAN_IF2_REG));
   
   /* Clear the IntPnd bit of DCAN_IFMCTL register */
   DCANClrIntPnd(dBaseAddress, DCAN_IF2_REG);

   /* Set the ClrIntPnd bit of DCAN_IFCMD register */
   DCANCommandRegSet(dBaseAddress, DCAN_CLR_INTPND, dMsgNum, DCAN_IF2_REG);
   
   
   /*
    * Copy the data to the CANObject
    */
   
   /* Get Identifier */
   if (dARB & DCAN_IFARB_XTD)
   {
      bFlags      = TAL_CAN_OBJECT_FLAGS_EXT_ID;
      dIdentifier = (dARB >> DCAN_ID_MSK_29_BIT) & CAN_ID_EXT_MASK;
   }
   else
   {
      bFlags      = TAL_CAN_OBJECT_FLAGS_STD_ID;
      dIdentifier = (dARB >> DCAN_ID_MSK_11_BIT) & CAN_ID_STD_MASK;
   }
   
   /* Get DLC */
   bDLC = (dDLC > 8) ? 8 : (uint8_t)dDLC;
   
   CANObject.dIdentifier = dIdentifier;
   CANObject.bFlags      = bFlags;
   CANObject.bDLC        = bDLC;

   /* Get data */
   pData = (uint8_t*)&dDataA;
   CANObject.Data[0] = *pData++;
   CANObject.Data[1] = *pData++;
   CANObject.Data[2] = *pData++;
   CANObject.Data[3] = *pData;

   pData = (uint8_t*)&dDataB;
   CANObject.Data[4] = *pData++;
   CANObject.Data[5] = *pData++;
   CANObject.Data[6] = *pData++;
   CANObject.Data[7] = *pData;
   
   /* If we have no overflow... */
   if (TAL_FALSE == pDCB->bRxOverflow)
   {
      /* ... put it into the ring buffer */
      Error = tal_MISCRingAdd(&pDCB->RxRing, (uint8_t*)&CANObject);
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

} /* RX_IRQHandler */

/*************************************************************************/
/*  IRQHandler                                                           */
/*                                                                       */
/*  This is the generic IRQ handler.                                     */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void IRQHandler (TAL_CAN_DCB *pDCB)
{
   uint32_t dBaseAddress = pDCB->HW.dBaseAddress;
   uint32_t dIntStatus;
   uint32_t dErrVal;
   uint32_t dMsgNum;

   
   /* Get the status of Interrupt register DCAN_INT */
   dIntStatus = (uint32_t)DCANIntRegStatusGet(dBaseAddress, DCAN_INT_LINE0_STAT);
   while (dIntStatus != DCAN_NO_INT_PENDING)
   {
      /*
       * Check for ERROR interrupt
       */
      if (dIntStatus == DCAN_ERROR_OCCURED)
      { 
         /* Get the status of Error and status DCAN_ES register */
         dErrVal = DCANErrAndStatusRegInfoGet(dBaseAddress);       

         /* Check for ACK error */         
         if (DCAN_ES_LEC_ACKERROR == dErrVal)
         {
            /* 
             * Ack error. The message this CAN core transmitted was
             * not acknowledged by another node. To clear this error,
             * switch to Loopback which will clear the ACK problem.
             */
             
            /* Enable test mode */ 
            DCANTestModeControl(dBaseAddress, DCAN_CTL_TEST);
            
            /* Set loopback */
            DCANTestModesEnable(dBaseAddress, DCAN_TST_LPBCK_MD);

            /* Wait for TX OK */
            dErrVal = DCANErrAndStatusRegInfoGet(dBaseAddress);
            while (0 == (dErrVal & DCAN_TXD_MSG_SUCCESSFULLY))
            {
               dErrVal = DCANErrAndStatusRegInfoGet(dBaseAddress);
            }   
         
            /* Switch back, remove loopback */
            DCANTestModeControl(dBaseAddress, 0);
         }            
      }
      else
      {
         /*
          * Message object interrupt
          */
         dMsgNum = dIntStatus;
         
         /* Check for TX interrupt */
         if (TX_MSG_INDEX == dMsgNum)
         {
            TX_IRQHandler(pDCB);
         } 
         else
         {
            RX_IRQHandler(pDCB, dMsgNum);
         }
         
      } /* end of valid interrupt */   

      /* Get the status of Interrupt register DCAN_INT */
      dIntStatus = DCANIntRegStatusGet(dBaseAddress, DCAN_INT_LINE0_STAT);
   } /* end while (dIntStatus) */ 

} /* IRQHandler */

/*************************************************************************/
/*  CANx_IRQHandler                                                      */
/*                                                                       */
/*  This is the CANx IRQ handler.                                        */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void CAN0_IRQHandler (void)
{
   TAL_CPU_IRQ_ENTER();
   IRQHandler(DCBArray[TAL_CAN_PORT_1]);
   TAL_CPU_IRQ_EXIT();
} /* CAN0_IRQHandler */

static void CAN1_IRQHandler (void)
{
   TAL_CPU_IRQ_ENTER();
   IRQHandler(DCBArray[TAL_CAN_PORT_2]);
   TAL_CPU_IRQ_EXIT();
} /* CAN1_IRQHandler */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  cpu_CANInit                                                          */
/*                                                                       */
/*  Prepare the hardware for use by the Open function later. Set the HW  */
/*  information depending of ePort and "enable" the CAN port.            */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANInit (TAL_CAN_DCB *pDCB)
{
   TAL_RESULT      Error     = TAL_ERR_CAN_PORT_RANGE;
   TAL_CAN_HW    *pHW        = &pDCB->HW;
      
   switch (pDCB->ePort)
   {
      case TAL_CAN_PORT_1:
      {
         Error = tal_BoardEnableCAN1();
         if (TAL_OK == Error)
         {
            DCBArray[TAL_CAN_PORT_1] = pDCB;

            /* Clear HW info first */
            memset(pHW, 0x00, sizeof(TAL_CAN_HW));

            pHW->dBaseAddress = SOC_DCAN_0_REGS;
            pHW->nIrqNumber   = CPU_DCAN0_INT0;
            pHW->nIrqPriority = CPU_CAN0_PRIO;
            
            /* Reset DCAN */
            DCANReset(pHW->dBaseAddress);
            
            /* Set irq, priority and function */
            tal_CPUIrqSetPriority(pHW->nIrqNumber, pHW->nIrqPriority);
            tal_CPUIrqSetFunction(pHW->nIrqNumber, CAN0_IRQHandler);
         }
         break;
      } /* TAL_CAN_PORT_1 */
   
      case TAL_CAN_PORT_2:
      {
         Error = tal_BoardEnableCAN2();
         if (TAL_OK == Error)
         {
            DCBArray[TAL_CAN_PORT_2] = pDCB;

            /* Clear HW info first */
            memset(pHW, 0x00, sizeof(TAL_CAN_HW));

            pHW->dBaseAddress = SOC_DCAN_1_REGS;
            pHW->nIrqNumber   = CPU_DCAN1_INT0;
            pHW->nIrqPriority = CPU_CAN1_PRIO;

            /* Reset DCAN */
            DCANReset(pHW->dBaseAddress);
            
            /* Set irq, priority and function */
            tal_CPUIrqSetPriority(pHW->nIrqNumber, pHW->nIrqPriority);
            tal_CPUIrqSetFunction(pHW->nIrqNumber, CAN1_IRQHandler);
         }
         break;
      } /* TAL_CAN_PORT_2 */
      
      default:
      {
         /* Do nothing */
         Error = TAL_ERR_CAN_PORT_RANGE;
         break;
      }
   } /* end switch (pDCB->ePort) */
   
   return(Error);
} /* cpu_CANInit */

/*************************************************************************/
/*  cpu_CANOpen                                                          */
/*                                                                       */
/*  Open the CAN port.                                                   */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANOpen (TAL_CAN_DCB *pDCB)
{
   TAL_RESULT   Error = TAL_ERROR;
   uint32_t    dBaseAddress = pDCB->HW.dBaseAddress;
   uint32_t    dBTR = 0;
   uint32_t    dIndex;

   /* 
    * Check baud rate 
    */
   switch (pDCB->Settings.dBitRate)
   {
      case TAL_CAN_BR_1000K: dBTR = BTR_1000K_24MHZ; break;
      case TAL_CAN_BR_500K:  dBTR = BTR_500K_24MHZ;  break;
      case TAL_CAN_BR_250K:  dBTR = BTR_250K_24MHZ;  break;
      case TAL_CAN_BR_125K:  dBTR = BTR_125K_24MHZ;  break;
      case TAL_CAN_BR_100K:  dBTR = BTR_100K_24MHZ;  break;
      case TAL_CAN_BR_50K:   dBTR = BTR_50K_24MHZ;   break;
      case TAL_CAN_BR_20K:   dBTR = BTR_20K_24MHZ;   break;
      case TAL_CAN_BR_10K:   dBTR = BTR_10K_24MHZ;   break;
      case TAL_CAN_BR_5K:    dBTR = BTR_5K_24MHZ;    break;
   
      default:
      {
         Error = TAL_ERR_CAN_BAUDRATE;
         goto CANOpenEnd;  /*lint !e801*/
         break;   /*lint !e527*/
      }
   } /* end switch (pDCB->Settings.dBaudrate) */
   
   
   /* Initialize the DCAN message RAM */
   DCANMsgRAMInit(pDCB);

   /* Reset the DCAN module */
   DCANReset(dBaseAddress);

   /* Enable the DCAN peripheral in Initialization mode */
   DCANInitModeSet(dBaseAddress);
   
   /* Enable the CPU write access to the configuration registers of DCAN peripheral */
   DCANConfigRegWriteAccessControl(dBaseAddress, DCAN_CONF_REG_WR_ACCESS_ENABLE);
   
   /* Write the CAN Bit-Timing values to the DCAN_BTR register */
   DCANBitTimingConfig(dBaseAddress, dBTR);

   /*  Disable the CPU write access to the configuration registers of DCAN peripheral */
   DCANConfigRegWriteAccessControl(dBaseAddress, DCAN_CONF_REG_WR_ACCESS_DISABLE);

   /* Invalidate all message objects in the message RAM */   
   for(dIndex=0; dIndex < 64; dIndex++)
   {
      /*
       * Invalidate the message object and transfer the control bits from the
       * IF1 register set to the message object addressed by message number.
       */
   
      /* Invalidate a message object */
      DCANMsgObjInvalidate(dBaseAddress, DCAN_IF1_REG);

      /* Transfer */
      DCANCommandRegSet(dBaseAddress, (DCAN_ACCESS_ARB_BITS | DCAN_MSG_WRITE), dIndex, DCAN_IF1_REG);
   }
   
   /* Enable the DCAN peripheral in Normal mode of operation */
   DCANNormalModeSet(dBaseAddress);   
   
   /* Enable the Auto-bus-on feature of DCAN peripheral */
   DCANAutoBusOnControl(dBaseAddress, DCAN_AUTO_BUS_ON_ENABLE);

   /* Enable Interrupt #0 line of DCAN peripheral */
   DCANIntLineEnable(dBaseAddress, DCAN_CTL_IE0);

   /* Enable the Interrupts SIS and EIE of DCAN peripheral */
   //DCANIntEnable(dBaseAddress, (DCAN_CTL_SIE | DCAN_CTL_EIE));
   
   /* Enables the system interrupt in AINTC */
   IntSystemEnable((unsigned int)pDCB->HW.nIrqNumber);

   Error = TAL_OK;

CANOpenEnd:   
   return(Error);
} /* cpu_CANOpen */

/*************************************************************************/
/*  cpu_CANClose                                                         */
/*                                                                       */
/*  Close the CAN port.                                                  */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANClose (TAL_CAN_DCB *pDCB)
{
   uint32_t dBaseAddress = pDCB->HW.dBaseAddress;

   /* Disables the system interrupt in AINTC */
   IntSystemDisable((unsigned int)pDCB->HW.nIrqNumber);    
   
   /* Disable all the Interrupts of DCAN peripheral */
   DCANIntDisable(dBaseAddress, 0xFF);
   
   /* Reset the DCAN module */
   DCANReset(dBaseAddress);
   
   return(TAL_OK);
} /* cpu_CANClose */

/*************************************************************************/
/*  cpu_CANStartTx                                                       */
/*                                                                       */
/*  Send the data from the ring buffer if the TX interrupt is disabled.  */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANStartTx (TAL_CAN_DCB *pDCB)
{
   TAL_RESULT       Error = TAL_ERROR;
   TAL_CAN_OBJECT   CANObject;
   
   TAL_CPU_DISABLE_ALL_INTS();
   if (pDCB->HW.dTxIE)
   {
      /* TX interrupt is enabled, do nothing */
   }
   else
   {
      Error = tal_MISCRingGet(&pDCB->TxRing, (uint8_t*)&CANObject);
      if (TAL_OK == Error)
      {
         Error = SendObject(pDCB, &CANObject);
         if (TAL_OK == Error)
         {
            /* Enable TX interrupt, this is only an indicator here */
            pDCB->HW.dTxIE = 1;
         }
      }
   }
   TAL_CPU_ENABLE_ALL_INTS();
   
   return(Error);
} /* cpu_CANStartTx */

/*************************************************************************/
/*  cpu_CANTxIsRunning                                                   */
/*                                                                       */
/*  Check if TX is still running.                                        */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / TAL_ERROR                                           */
/*************************************************************************/
TAL_RESULT cpu_CANTxIsRunning (TAL_CAN_DCB *pDCB)
{
   TAL_RESULT  Error = TAL_OK;
   
   TAL_CPU_DISABLE_ALL_INTS();
   if (pDCB->HW.dTxIE)
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
} /* cpu_CANTxIsRunning */

/*************************************************************************/
/*  cpu_CANEnableSilentMode                                              */
/*                                                                       */
/*  Enable "Silent" mode.                                                */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANEnableSilentMode (TAL_CAN_DCB *pDCB)
{
   (void)pDCB;   
   
   /* Not supported */
   
   return(TAL_ERROR);
} /* cpu_CANEnableSilentMode */

/*************************************************************************/
/*  cpu_CANDisableSilentMode                                             */
/*                                                                       */
/*  Disable "Silent" mode.                                               */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANDisableSilentMode (TAL_CAN_DCB *pDCB)
{
   (void)pDCB;   
   
   /* Not supported */
   
   return(TAL_ERROR);
} /* cpu_CANDisableSilentMode */

/*************************************************************************/
/*  cpu_CANIdentRegister                                                 */
/*                                                                       */
/*  Register a CAN identifier.                                           */
/*                                                                       */
/*  In    : pDCB, dIdentifier, Type                                      */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANIdentRegister (TAL_CAN_DCB  *pDCB,
                                uint32_t       dIdentifier,
                                TAL_CAN_TYPE_ID Type)
{
   TAL_RESULT Error = TAL_ERROR;
   uint16_t   wIndex;
   uint32_t   dBaseAddress = pDCB->HW.dBaseAddress;
   uint32_t   *FilterArray = pDCB->HW.FilterArray;
   uint8_t    bRegsiterOK = TAL_FALSE;
   uint32_t   dIDLen;
   uint16_t   wMsgIndex;
   
   /* 
    * Handle special case TAL_CAN_ID_RX_ALL, all identifier
    * from type STD or EXT should be received.
    */
   if (TAL_CAN_ID_RX_ALL == dIdentifier)
   {
      /* Test if STD or EXT ID */
      if (TAL_CAN_TYPE_STD_ID == Type)
      {
         dIdentifier = CAN_ID_STD_MASK;
      }
      else
      {
         dIdentifier = CAN_ID_EXT_MASK;
      } 
      
      dIdentifier |= FILTER_IS_ALL_FLAG; 
   }

   /*
    * Check for STD or EXT identifier
    */   
   if (TAL_CAN_TYPE_STD_ID == Type)
   {
      /* Check valid range */
      if (FILTER_ID(dIdentifier) > CAN_STD_ENTRY_ID_MASK)
      {
         Error = TAL_ERR_CAN_ID_RANGE;
         goto IdentRegisterEnd;  /*lint !e801*/
      } 
   }
   else
   {
      /* Check valid range */
      if (FILTER_ID(dIdentifier) > CAN_EXT_ENTRY_ID_MASK)
      {
         Error = TAL_ERR_CAN_ID_RANGE;
         goto IdentRegisterEnd;  /*lint !e801*/
      }

      dIdentifier |= FILTER_IS_EXT_FLAG; 
   }

   dIdentifier |= FILTER_IN_USE_FLAG;

   /*
    * Check if this identifier is registered
    */
   for (wIndex = 0; wIndex < CAN_MAX_FILTER_COUNT; wIndex++)
   {
      if (FilterArray[wIndex] == dIdentifier)
      {
         Error = TAL_ERR_CAN_ID_USED;
         goto IdentRegisterEnd;  /*lint !e801*/
      }
   }

   /*
    * At this point the identifier is not registered.
    * Find a free entry which can be used.
    */
   for (wIndex = 0; wIndex < CAN_MAX_FILTER_COUNT; wIndex++)
   {
      if (0 == IS_FILTER_IN_USE(FilterArray[wIndex]))
      {
         bRegsiterOK = TAL_TRUE;
         break;
      }
   }      
   if (TAL_FALSE == bRegsiterOK)
   {
      Error = TAL_ERR_CAN_ID_NO_ENTRY;
      goto IdentRegisterEnd;  /*lint !e801*/
   }

   /*
    * At this point an update of the Acceptance Filter is needed
    */
   wMsgIndex = wIndex + RX_MSG_START;
   
   /* Check the status from DCAN_MSGVAL(n) register */
   if (DCANMsgValidStatusGet(dBaseAddress, wMsgIndex) != 0)
   {
      Error = TAL_ERROR;
      goto IdentRegisterEnd;  /*lint !e801*/
   }

   /* Update filter entry */
   FilterArray[wIndex] = dIdentifier;
   
   /*
    * Configure a message object in DCAN message RAM as a receive message
    * object. See examples\evmAM335x\dcanLp8k\dcan_frame.c CANRxObjectConfig
    */

   dIDLen = (TAL_CAN_TYPE_STD_ID == Type) ? DCAN_11_BIT_ID : DCAN_29_BIT_ID;

   /* Control the Acceptance mask feature, Acceptance mask used. */
   DCANUseAcceptanceMaskControl(dBaseAddress, DCAN_MASK_USED, DCAN_IF1_REG);

   /* Configure the mask settings for a message object */
   if (TAL_CAN_TYPE_STD_ID == Type)
   {
      if (0 == IS_FILTER_ALL(dIdentifier))
      {
         DCANMsgObjectMskConfig(dBaseAddress, 
                                DCAN_IDENTIFIER_MSK(0x7ff, DCAN_ID_MSK_11_BIT), 
                                DCAN_MSK_MSGDIR_DISABLE,
                                DCAN_MSK_EXT_ID_DISABLE, DCAN_IF1_REG);
      }
      else
      {
         DCANMsgObjectMskConfig(dBaseAddress, 
                                DCAN_IDENTIFIER_MSK(0, DCAN_ID_MSK_11_BIT), 
                                DCAN_MSK_MSGDIR_DISABLE,
                                DCAN_MSK_EXT_ID_ENABLE, DCAN_IF1_REG);
      }                                
   }
   else
   {
      if (0 == IS_FILTER_ALL(dIdentifier))
      {
         DCANMsgObjectMskConfig(dBaseAddress, 
                                DCAN_IDENTIFIER_MSK(0x1FFFFFFF, DCAN_ID_MSK_29_BIT), 
                                DCAN_MSK_MSGDIR_DISABLE,
                                DCAN_MSK_EXT_ID_ENABLE, DCAN_IF1_REG);
      }
      else
      {
         DCANMsgObjectMskConfig(dBaseAddress, 
                                DCAN_IDENTIFIER_MSK(0, DCAN_ID_MSK_29_BIT), 
                                DCAN_MSK_MSGDIR_DISABLE,
                                DCAN_MSK_EXT_ID_ENABLE, DCAN_IF1_REG);
      }                                
   }  

   /* Validate the message object */
   DCANMsgObjValidate(dBaseAddress, DCAN_IF1_REG);    

   /* Set the message identifier length and number */
   DCANMsgIdSet(dBaseAddress, FILTER_ID(dIdentifier), dIDLen, DCAN_IF1_REG);

   /* Set the direction for the message object */
   DCANMsgDirectionSet(dBaseAddress, DCAN_RX_DIR, DCAN_IF1_REG);

   /* Enable the Message object interrupts of the DCAN */
   DCANMsgObjIntEnable(dBaseAddress, DCAN_RECEIVE_INT, DCAN_IF1_REG);

   /* Configure the end of block settings for the DCAN */
   DCANFIFOEndOfBlockControl(dBaseAddress, DCAN_END_OF_BLOCK_ENABLE, DCAN_IF1_REG);

   /* Set the fields of DCAN_IFCMD register */
   DCANCommandRegSet(dBaseAddress, (DCAN_ACCESS_CTL_BITS | DCAN_MSG_WRITE |
                     DCAN_ACCESS_MSK_BITS | DCAN_ACCESS_ARB_BITS), 
                     wMsgIndex, DCAN_IF1_REG);

   Error = TAL_OK;

IdentRegisterEnd:   
   return(Error);
} /* cpu_CANIdentRegister */   

/*************************************************************************/
/*  cpu_CANIdentDeRegister                                               */
/*                                                                       */
/*  DeRegister a CAN identifier.                                         */
/*                                                                       */
/*  In    : pDCB, dIdentifier, Type                                      */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT cpu_CANIdentDeRegister (TAL_CAN_DCB  *pDCB,
                                  uint32_t       dIdentifier,
                                  TAL_CAN_TYPE_ID Type)
{
   TAL_RESULT Error = TAL_ERROR;
   uint16_t   wIndex;
   uint32_t   dBaseAddress = pDCB->HW.dBaseAddress;
   uint32_t  *FilterArray   = pDCB->HW.FilterArray;
   uint8_t    bDeRegsiterOK = TAL_FALSE;
   uint16_t   wMsgIndex;
   
   /* 
    * Handle special case TAL_CAN_ID_RX_ALL, all identifier
    * from type STD or EXT should be received.
    */
   if (TAL_CAN_ID_RX_ALL == dIdentifier)
   {
      /* Test if STD or EXT ID */
      if (TAL_CAN_TYPE_STD_ID == Type)
      {
         dIdentifier = CAN_ID_STD_MASK;
      }
      else
      {
         dIdentifier = CAN_ID_EXT_MASK;
         dIdentifier |= FILTER_IS_EXT_FLAG; 
      } 
      
      dIdentifier |= FILTER_IS_ALL_FLAG; 
      dIdentifier |= FILTER_IN_USE_FLAG;
      goto IdentDeRegisteCheck;  /*lint !e801*/
   }

   /*
    * Check for STD or EXT identifier
    */   
   if (TAL_CAN_TYPE_STD_ID == Type)
   {
      /* Check valid range */
      if (FILTER_ID(dIdentifier) > CAN_STD_ENTRY_ID_MASK)
      {
         Error = TAL_ERR_CAN_ID_RANGE;
         goto IdentDeRegisterEnd;   /*lint !e801*/
      } 
   }
   else
   {
      /* Check valid range */
      if (FILTER_ID(dIdentifier) > CAN_EXT_ENTRY_ID_MASK)
      {
         Error = TAL_ERR_CAN_ID_RANGE;
         goto IdentDeRegisterEnd;   /*lint !e801*/
      }

      dIdentifier |= FILTER_IS_EXT_FLAG; 
   }

   dIdentifier |= FILTER_IN_USE_FLAG;
   
   /*
    * Check if this identifier is registered,
    * and remove it from the list if available.
    */
IdentDeRegisteCheck:   
   for (wIndex = 0; wIndex < CAN_MAX_FILTER_COUNT; wIndex++)
   {
      if (FilterArray[wIndex] == dIdentifier)
      {
         FilterArray[wIndex] = 0;
         bDeRegsiterOK       = TAL_TRUE;
         break;
      }
   }
   if (TAL_FALSE == bDeRegsiterOK)
   {
      Error = TAL_ERR_CAN_ID_NOT_USED;
      goto IdentDeRegisterEnd;   /*lint !e801*/
   }

   /*
    * At this point an update of the Acceptance Filter is needed
    */
   wMsgIndex = wIndex + RX_MSG_START;

   /*
    * See examples\evmAM335x\dcanLp8k\dcan_frame.c 
    * CANRxIntDisable and CANInValidateMsgObject
    */

   /* Disable the Message object interrupts of the DCAN */
   DCANMsgObjIntDisable(dBaseAddress, DCAN_RECEIVE_INT, DCAN_IF1_REG);
   
   /* Invalidate a message object */
   DCANMsgObjInvalidate(dBaseAddress, DCAN_IF1_REG);

   /* Set the fields of DCAN_IFCMD register */
   DCANCommandRegSet(dBaseAddress, (DCAN_ACCESS_ARB_BITS | DCAN_MSG_WRITE), 
                     wMsgIndex, DCAN_IF1_REG);      

   Error = TAL_OK;
   
IdentDeRegisterEnd:   
   return(Error);
} /* cpu_CANIdentDeRegister */   

/*** EOF ***/
