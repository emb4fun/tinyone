/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2015-2023 by Michael Fischer (www.emb4fun.de).
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

/*=======================================================================*/
/*  Includes                                                             */
/*=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "tal.h"
#include "tcts.h"
#include "ff.h"
#include "diskio.h"

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "hw_gpio_v2.h"
#include "hs_mmcsd.h"
#include "cache.h"

#include "beaglebone.h"
#include "edma_event.h"
#include "interrupt.h"
#include "mmcsd_proto.h"
#include "hs_mmcsdlib.h"
#include "edma.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Control register
 */
#define CONTROL_CONF_MMODE_SHIFT       0
#define CONTROL_CONF_PUDEN_SHIFT       3
#define CONTROL_CONF_PUTYPESEL_SHIFT   4
#define CONTROL_CONF_RXACTIVE_SHIFT    5

/*
 * Will the Filesystem used with a real time os?
 * 0:No RTOS or 1:RTOS available
 */
#define FFS_USE_REAL_TIME_OS  1

#define BLOCK_SIZE   512

#define SD_LED_ON()           tal_LEDSet(TAL_LED_CHANNEL_4)
#define SD_LED_OFF()          tal_LEDClear(TAL_LED_CHANNEL_4)


/* Frequency */
#define HSMMCSD_IN_FREQ                96000000 /* 96MHz */
#define HSMMCSD_INIT_FREQ              400000   /* 400kHz */

/* EDMA3 Event queue number. */
#define EVT_QUEUE_NUM                  0
 
/* MMCSD instance related macros. */
#define MMCSD_INST_BASE                (SOC_MMCHS_0_REGS)
#define MMCSD_INT_NUM                  (SYS_INT_MMCSD0INT)

/* EDMA instance related macros. */
#define EDMA_INST_BASE                 (SOC_EDMA30CC_0_REGS)
#define EDMA_COMPLTN_INT_NUM           (SYS_INT_EDMACOMPINT)
#define EDMA_ERROR_INT_NUM             (SYS_INT_EDMAERRINT) 

/* EDMA Events */
#define MMCSD_TX_EDMA_CHAN             (EDMA3_CHA_MMCSD0_TX)
#define MMCSD_RX_EDMA_CHAN             (EDMA3_CHA_MMCSD0_RX)

/* EDMA callback function array */
static void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);

/*=======================================================================*/
/*  Definition of all global Data                                        */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/* Global flags for interrupt handling */
static volatile unsigned int callbackOccured = 0; 
static volatile unsigned int xferCompFlag = 0; 
static volatile unsigned int dataTimeout = 0;
static volatile unsigned int cmdCompFlag = 0;
static volatile unsigned int cmdTimeout = 0; 

/* SD card info structure */
static mmcsdCardInfo sdCard;

/* SD Controller info structure */
static mmcsdCtrlInfo ctrlInfo;


static DSTATUS DiskStatus = STA_NOINIT;
static OS_SEMA FSSema;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

static void InitCardDetect (void)
{
   uint32_t Value;

//   GPIO0ModuleClkConfig();
   
   /* Setup Pad Control Registers */
   Value = (7 << CONTROL_CONF_MMODE_SHIFT)     |   /* Mode 7 */
           (1 << CONTROL_CONF_PUDEN_SHIFT)     |   /* Pull disabled */
           (1 << CONTROL_CONF_PUTYPESEL_SHIFT) |   /* Pullup */
           (1 << CONTROL_CONF_RXACTIVE_SHIFT);     /* Receiver enabled */ /*lint !e845*/

   HWREG(SOC_CONTROL_REGS + CONTROL_CONF_SPI0_CS1) = Value;

   /* Module is enabled, clocks are not gated. */
   HWREG(SOC_GPIO_0_REGS + GPIO_CTRL) &= ~GPIO_CTRL_DISABLEMODULE;

   /* The corresponding GPIO ports are configured as an output. */
   HWREG(SOC_GPIO_0_REGS + GPIO_OE) |= (1<<6);
   
} /* InitCardDetect */

/*
 * Check command status
 */

static unsigned int HSMMCSDCDPinStatusGet(mmcsdCtrlInfo *ctrl)
{
   unsigned int status = 0;

   (void)ctrl;
   
   if (0 == (HWREG(SOC_GPIO_0_REGS + GPIO_DATAIN) & (1 << 6)))
   {
      status = 1;
   }
   
    return status;
}

static unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int status = 0;

    (void)ctrl;

    while ((cmdCompFlag == 0) && (cmdTimeout == 0)) { __asm__ volatile ("nop"); };

    if (cmdCompFlag)
    {
        status = 1;
        cmdCompFlag = 0;
    }

    if (cmdTimeout)
    {
        status = 0;
        cmdTimeout = 0;
    }

    return status;
}

static unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int status = 0;
    volatile unsigned int timeOut = 0xFFFF;

    while ((xferCompFlag == 0) && (dataTimeout == 0)) { __asm__ volatile ("nop"); };

    if (xferCompFlag)
    {
        status = 1;
        xferCompFlag = 0;
    }

    if (dataTimeout)
    {
        status = 0;
        dataTimeout = 0;
    }

    /* Also, poll for the callback */
    if (HWREG(ctrl->memBase + MMCHS_CMD) & MMCHS_CMD_DP)
    {
        while(callbackOccured == 0 && ((timeOut--) != 0)) { __asm__ volatile ("nop"); };
        
        callbackOccured = 0;

        if(timeOut == 0)
        {
            status = 0;
        }
    }

    ctrlInfo.dmaEnable = 0;

    return status;
}

void HSMMCSDRxDmaConfig(void *ptr, unsigned int blkSize, unsigned int nblks)
{
    EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = ctrlInfo.memBase + MMCHS_DATA;
    paramSet.destAddr   = (unsigned int)ptr;
    paramSet.srcBIdx    = 0;
    paramSet.srcCIdx    = 0;
    paramSet.destBIdx   = 4;
    paramSet.destCIdx   = (short)blkSize;
    paramSet.aCnt       = 0x4;
    paramSet.bCnt       = (unsigned short)blkSize/4;
    paramSet.cCnt       = (unsigned short)nblks;
    paramSet.bCntReload = 0x0;
    paramSet.linkAddr   = 0xffff;
    paramSet.opt        = 0;

    /* Set OPT */
    paramSet.opt |= ((MMCSD_RX_EDMA_CHAN << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* 1. Transmission complition interrupt enable */
    paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

    /* 2. Read FIFO : SRC Constant addr mode */
    paramSet.opt |= (1 << 0);

    /* 3. SRC FIFO width is 32 bit */
    paramSet.opt |= (2 << 8);

    /* 4.  AB-Sync mode */
    paramSet.opt |= (1 << 2);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, MMCSD_RX_EDMA_CHAN, &paramSet);

    /* Enable the transfer */
    EDMA3EnableTransfer(EDMA_INST_BASE, MMCSD_RX_EDMA_CHAN, EDMA3_TRIG_MODE_EVENT);
}

void HSMMCSDTxDmaConfig(void *ptr, unsigned int blkSize, unsigned int blks)
{
    EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = (unsigned int)ptr;
    paramSet.destAddr   = ctrlInfo.memBase + MMCHS_DATA;
    paramSet.srcBIdx    = 4;
    paramSet.srcCIdx    = (short)blkSize;
    paramSet.destBIdx   = 0;
    paramSet.destCIdx   = 0;
    paramSet.aCnt       = 0x4;
    paramSet.bCnt       = (unsigned short)blkSize/4;
    paramSet.cCnt       = (unsigned short)blks;
    paramSet.bCntReload = 0x0;
    paramSet.linkAddr   = 0xffff;
    paramSet.opt        = 0;

    /* Set OPT */
    paramSet.opt |= ((MMCSD_TX_EDMA_CHAN << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* 1. Transmission complition interrupt enable */
    paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

    /* 2. Read FIFO : DST Constant addr mode */
    paramSet.opt |= (1 << 1);

    /* 3. DST FIFO width is 32 bit */
    paramSet.opt |= (2 << 8);

    /* 4.  AB-Sync mode */
    paramSet.opt |= (1 << 2);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, MMCSD_TX_EDMA_CHAN, &paramSet);

    /* Enable the transfer */
    EDMA3EnableTransfer(EDMA_INST_BASE, MMCSD_TX_EDMA_CHAN, EDMA3_TRIG_MODE_EVENT);
}

static void HSMMCSDXferSetup(mmcsdCtrlInfo *ctrl, unsigned char rwFlag, void *ptr,
                             unsigned int blkSize, unsigned int nBlks)
{
    callbackOccured = 0;
    xferCompFlag = 0;

    if (rwFlag == 1)
    {
        HSMMCSDRxDmaConfig(ptr, blkSize, nBlks);
    }
    else
    {
        HSMMCSDTxDmaConfig(ptr, blkSize, nBlks);
    }

    ctrl->dmaEnable = 1;
    HSMMCSDBlkLenSet(ctrl->memBase, blkSize);
}

/*
** This function is used as a callback from EDMA3 Completion Handler.
*/
static void callback(unsigned int tccNum, unsigned int status)
{
    (void)status;

    callbackOccured = 1;
    EDMA3DisableTransfer(EDMA_INST_BASE, tccNum, EDMA3_TRIG_MODE_EVENT);
}

static void Edma3CompletionIsr(void)
{
    volatile unsigned int pendingIrqs;
    volatile unsigned int isIPR;

    unsigned int indexl;
    unsigned int Cnt = 0;
    
    TAL_CPU_IRQ_ENTER();

    indexl = 1;

    isIPR = EDMA3GetIntrStatus(EDMA_INST_BASE);

    if(isIPR)
    {
        while ((Cnt < EDMA3CC_COMPL_HANDLER_RETRY_COUNT)&& (indexl != 0u))
        {
            indexl = 0u;
            pendingIrqs = EDMA3GetIntrStatus(EDMA_INST_BASE);

            while (pendingIrqs)
            {
                if((pendingIrqs & 1u) == TRUE)
                {
                    /**
                    * If the user has not given any callback function
                    * while requesting the TCC, its TCC specific bit
                    * in the IPR register will NOT be cleared.
                    */
                    /* here write to ICR to clear the corresponding IPR bits */

                    EDMA3ClrIntr(EDMA_INST_BASE, indexl);

                    if (cb_Fxn[indexl] != NULL)
                    {
                        (*cb_Fxn[indexl])(indexl, EDMA3_XFER_COMPLETE);
                    }
                }
                ++indexl;
                pendingIrqs >>= 1u;
            }
            Cnt++;
        }
    }
}

static void Edma3CCErrorIsr(void)
{
   volatile unsigned int pendingIrqs;
   volatile unsigned int evtqueNum = 0;  /* Event Queue Num */
   volatile unsigned int isIPRH;
   volatile unsigned int isIPR;
   volatile unsigned int Cnt = 0u;
   volatile unsigned int index;
   
   TAL_CPU_IRQ_ENTER();

   pendingIrqs = 0u;
   index = 1u;

   isIPR  = EDMA3GetIntrStatus(EDMA_INST_BASE);
   isIPRH = EDMA3IntrStatusHighGet(EDMA_INST_BASE);

   if((isIPR | isIPRH ) || (EDMA3QdmaGetErrIntrStatus(EDMA_INST_BASE) != 0)
       || (EDMA3GetCCErrStatus(EDMA_INST_BASE) != 0))
   {
      /* Loop for EDMA3CC_ERR_HANDLER_RETRY_COUNT number of time,
       * breaks when no pending interrupt is found
       */
      while ((Cnt < EDMA3CC_ERR_HANDLER_RETRY_COUNT)
                  && (index != 0u))
      {
         index = 0u;

         if(isIPR)
         {
            pendingIrqs = EDMA3GetErrIntrStatus(EDMA_INST_BASE);
         }
         else
         {
            pendingIrqs = EDMA3ErrIntrHighStatusGet(EDMA_INST_BASE);
         }

         while (pendingIrqs)
         {
            /*Process all the pending interrupts*/
            if(TRUE == (pendingIrqs & 1u))
            {
               /* Write to EMCR to clear the corresponding EMR bits. */
               /*Clear any SER*/

               if(isIPR)
               {
                  EDMA3ClrMissEvt(EDMA_INST_BASE, index);
               }
               else
               {
                  EDMA3ClrMissEvt(EDMA_INST_BASE, index + 32);
               }
            }
            
            ++index;
            pendingIrqs >>= 1u;
         }
         index = 0u;
         pendingIrqs = EDMA3QdmaGetErrIntrStatus(EDMA_INST_BASE);
         while (pendingIrqs)
         {
            /*Process all the pending interrupts*/
            if(TRUE == (pendingIrqs & 1u))
            {
               /* Here write to QEMCR to clear the corresponding QEMR bits*/
               /*Clear any QSER*/
               EDMA3QdmaClrMissEvt(EDMA_INST_BASE, index);
            }
            ++index;
            pendingIrqs >>= 1u;
         }
         index = 0u;

         pendingIrqs = EDMA3GetCCErrStatus(EDMA_INST_BASE);
         if (pendingIrqs != 0u)
         {
            /* Process all the pending CC error interrupts. */
            /* Queue threshold error for different event queues.*/
            for (evtqueNum = 0u; evtqueNum < SOC_EDMA3_NUM_EVQUE; evtqueNum++)
            {
               if((pendingIrqs & (1u << evtqueNum)) != 0u)
               {
                  /* Clear the error interrupt. */
                  EDMA3ClrCCErr(EDMA_INST_BASE, (1u << evtqueNum));
               }
            }

            /* Transfer completion code error. */
            if ((pendingIrqs & (1 << EDMA3CC_CCERR_TCCERR_SHIFT)) != 0u)
            {
               EDMA3ClrCCErr(EDMA_INST_BASE, (0x01u << EDMA3CC_CCERR_TCCERR_SHIFT));
            }
            ++index;
         }
         Cnt++;
      }
   }

}

static void HSMMCSDIsr(void)
{
   volatile unsigned int status;
   
   TAL_CPU_IRQ_ENTER();

   status = HSMMCSDIntrStatusGet(ctrlInfo.memBase, 0xFFFFFFFF);
   
   HSMMCSDIntrStatusClear(ctrlInfo.memBase, status);

   if (status & HS_MMCSD_STAT_CMDCOMP)
   {
      cmdCompFlag = 1;
   }

   if (status & HS_MMCSD_STAT_ERR)
   {
      //errFlag = status & 0xFFFF0000;

      if (status & HS_MMCSD_STAT_CMDTIMEOUT)
      {
          cmdTimeout = 1;
      }

      if (status & HS_MMCSD_STAT_DATATIMEOUT)
      {
          dataTimeout = 1;
      }
   }

   if (status & HS_MMCSD_STAT_TRNFCOMP)
   {
      xferCompFlag = 1;
   }
   
   TAL_CPU_IRQ_EXIT()   
}

/*
** This function configures the AINTC to receive EDMA3 interrupts.
*/
static void EDMA3AINTCConfigure(void)
{
    /* Initializing the ARM Interrupt Controller. */
//    IntAINTCInit();

    /* Registering EDMA3 Channel Controller transfer completion interrupt.  */
    IntRegister(EDMA_COMPLTN_INT_NUM, Edma3CompletionIsr);

    /* Setting the priority for EDMA3CC completion interrupt in AINTC. */
    IntPrioritySet(EDMA_COMPLTN_INT_NUM, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Registering EDMA3 Channel Controller Error Interrupt. */
    IntRegister(EDMA_ERROR_INT_NUM, Edma3CCErrorIsr);

    /* Setting the priority for EDMA3CC Error interrupt in AINTC. */
    IntPrioritySet(EDMA_ERROR_INT_NUM, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the EDMA3CC completion interrupt in AINTC. */
    IntSystemEnable(EDMA_COMPLTN_INT_NUM);
    
    /* Enabling the EDMA3CC Error interrupt in AINTC. */
    IntSystemEnable(EDMA_ERROR_INT_NUM);

    /* Registering HSMMC Interrupt handler */
    IntRegister(MMCSD_INT_NUM, HSMMCSDIsr);

    /* Setting the priority for EDMA3CC completion interrupt in AINTC. */
    IntPrioritySet(MMCSD_INT_NUM, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the HSMMC interrupt in AINTC. */
    IntSystemEnable(MMCSD_INT_NUM);

    /* Enabling IRQ in CPSR of ARM processor. */
//    IntMasterIRQEnable();
}

/* 
** Powering up, initializing and registering interrupts for EDMA.
*/

static void EDMA3Initialize(void)
{
    /* Initialization of EDMA3 */
    EDMA3Init(EDMA_INST_BASE, EVT_QUEUE_NUM);

    /* Configuring the AINTC to receive EDMA3 interrupts. */ 
    EDMA3AINTCConfigure();
}

static void HSMMCSDEdmaInit(void)
{
    /* Initializing the EDMA. */
    EDMA3Initialize();

    /* Request DMA Channel and TCC for MMCSD Transmit*/
    EDMA3RequestChannel(EDMA_INST_BASE, EDMA3_CHANNEL_TYPE_DMA,
                        MMCSD_TX_EDMA_CHAN, MMCSD_TX_EDMA_CHAN,
                        EVT_QUEUE_NUM);

    /* Registering Callback Function for TX*/
    cb_Fxn[MMCSD_TX_EDMA_CHAN] = callback;

    /* Request DMA Channel and TCC for MMCSD Receive */
    EDMA3RequestChannel(EDMA_INST_BASE, EDMA3_CHANNEL_TYPE_DMA,
                        MMCSD_RX_EDMA_CHAN, MMCSD_RX_EDMA_CHAN,
                        EVT_QUEUE_NUM);

    /* Registering Callback Function for RX*/
    cb_Fxn[MMCSD_RX_EDMA_CHAN] = callback;
}

/*
** Initialize the MMCSD controller structure for use
*/
static void HSMMCSDControllerSetup(void)
{
   memset(&ctrlInfo, 0x00, sizeof(ctrlInfo));

   ctrlInfo.memBase        = MMCSD_INST_BASE;
   ctrlInfo.ctrlInit       = HSMMCSDControllerInit;
   ctrlInfo.xferSetup      = HSMMCSDXferSetup;
   ctrlInfo.cmdStatusGet   = HSMMCSDCmdStatusGet;
   ctrlInfo.xferStatusGet  = HSMMCSDXferStatusGet;
   ctrlInfo.cardPresent    = HSMMCSDCDPinStatusGet;
   ctrlInfo.cmdSend        = HSMMCSDCmdSend;
   ctrlInfo.busWidthConfig = HSMMCSDBusWidthConfig;
   ctrlInfo.busFreqConfig  = HSMMCSDBusFreqConfig;
   ctrlInfo.intrMask       = (HS_MMCSD_INTR_CMDCOMP | HS_MMCSD_INTR_CMDTIMEOUT |
                              HS_MMCSD_INTR_DATATIMEOUT | HS_MMCSD_INTR_TRNFCOMP);
   ctrlInfo.intrEnable     = HSMMCSDIntEnable;
   ctrlInfo.busWidth       = (SD_BUS_WIDTH_1BIT | SD_BUS_WIDTH_4BIT);
   ctrlInfo.highspeed      = 1;
   ctrlInfo.ocr            = (SD_OCR_VDD_3P0_3P1 | SD_OCR_VDD_3P1_3P2);
   ctrlInfo.card           = &sdCard;
   ctrlInfo.ipClk          = HSMMCSD_IN_FREQ;
   ctrlInfo.opClk          = HSMMCSD_INIT_FREQ;
   sdCard.ctrl             = &ctrlInfo;

   callbackOccured = 0;
   xferCompFlag    = 0;
   dataTimeout     = 0;
   cmdCompFlag     = 0;
   cmdTimeout      = 0;
}

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  ff_memalloc                                                          */
/*                                                                       */
/*  Allocate a memory block                                              */
/*                                                                       */
/*  In    : msize                                                        */
/*  Out   : none                                                         */
/*  Return: Returns pointer to the allocated memory block                */
/*************************************************************************/
void *ff_memalloc (UINT msize)
{
   return(malloc(msize));
} /* ff_memalloc */

/*************************************************************************/
/*  ff_memfree                                                           */
/*                                                                       */
/*  Free a memory block                                                  */
/*                                                                       */
/*  In    : mblock                                                       */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void ff_memfree (void *mblock)
{
   free(mblock);
} /* ff_memfree */

#if (FFS_USE_REAL_TIME_OS >= 1)
/*************************************************************************/
/*  ff_cre_syncobj                                                       */
/*                                                                       */
/*  Create a Synchronization Object                                      */
/*                                                                       */
/*  In    : vol,  corresponding logical drive being processed.           */
/*          sobj, pointer to return the created sync object.             */
/*  Out   : none                                                         */
/*  Return: 0 = Error / 1 = OK                                           */
/*************************************************************************/
int ff_cre_syncobj (BYTE vol, FF_SYNC_t *sobj)
{
   (void)vol;
   
   /* Init the semaphore */
   OS_SemaCreate(&FSSema, 1, 1);
   *sobj = &FSSema;

   return(1);
} /* ff_cre_syncobj */

/*************************************************************************/
/*  ff_del_syncobj                                                       */
/*                                                                       */
/*  Delete a Synchronization Object                                      */
/*                                                                       */
/*  In    : sobj, sync object tied to the logical drive to be deleted.   */
/*  Out   : none                                                         */
/*  Return: 0 = Error / 1 = OK                                           */
/*************************************************************************/
int ff_del_syncobj (FF_SYNC_t sobj)
{
   OS_SemaDelete(sobj);
   
   return(1);
} /* ff_del_syncobj */

/*************************************************************************/
/*  ff_req_grant                                                         */
/*                                                                       */
/*  Request Grant to Access the Volume                                   */
/*                                                                       */
/*  In    : sobj, sync object to wait.                                   */
/*  Out   : none                                                         */
/*  Return: 1 = Got a grant / 0 =  Could not get a grant                 */
/*************************************************************************/
int ff_req_grant (FF_SYNC_t sobj)
{
   OS_SemaWait(sobj, 0);
   
   return(1);
} /* ff_req_grant */

/*************************************************************************/
/*  ff_rel_grant                                                         */
/*                                                                       */
/*  Request Grant to Access the Volume                                   */
/*                                                                       */
/*  In    : sobj, Sync object to be signaled.                            */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void ff_rel_grant (FF_SYNC_t sobj)
{
   OS_SemaSignal(sobj);
} /* ff_rel_grant */
#endif /* (FFS_USE_REAL_TIME_OS >= 1) */

/*************************************************************************/
/*  disk_initialize                                                      */
/*                                                                       */
/*  Initialize a Drive                                                   */
/*                                                                       */
/*  In    : pdrv, physical drive nmuber to identify the drive            */
/*  Out   : none                                                         */
/*  Return: Status of Disk Functions                                     */
/*************************************************************************/
DSTATUS disk_initialize (uint8_t pdrv)
{
   static uint8_t InitDone = 0;
   unsigned int   Status;
   mmcsdCardInfo *card = &sdCard;  
   
   if (DiskStatus & STA_NODISK) 
   {
      DiskStatus |= STA_NOINIT;
      
      /* No card in the socket */
      return(DiskStatus);
   } 
   
   SD_LED_ON();

   /*
    * HW init only once
    */
   if ((0 == pdrv) && (0 == InitDone))
   {
      InitDone = 1;
   
      /* Configure the EDMA clocks. */
      EDMAModuleClkConfig();

      /* Configure EDMA to service the HSMMCSD events. */
      HSMMCSDEdmaInit();

      /* Perform pin-mux for HSMMCSD pins. */
      HSMMCSDPinMuxSetup();

      /* Enable module clock for HSMMCSD. */
      HSMMCSDModuleClkConfig();
      
      /* Basic controller initializations */
      HSMMCSDControllerSetup();
      
      InitCardDetect();
   }
   
   /*
    * Check if card must be init
    */
   if ((0 == pdrv) && (DiskStatus & STA_NOINIT))
   {   
      /* Initialize the MMCSD controller */
      MMCSDCtrlInit(&ctrlInfo);
   
      MMCSDIntEnable(&ctrlInfo);
   
      /* SD Card init */
      Status = MMCSDCardInit(card->ctrl);
      if (0 == Status)
      {
         term_printf("\r\nCard Init Failed \r\n");
         DiskStatus = STA_NOINIT;
      }
      else
      {
#if 0     
         if (card->cardType == MMCSD_CARD_SD)
         {
            term_printf("SD Card version : %d", card->sd_ver);
   
            if (card->highCap)
            {
               term_printf(", High Capacity");
            }
   
            if (card->tranSpeed == SD_TRANSPEED_50MBPS)
            {
                term_printf(", High Speed");
            }
         
            term_printf("\r\n");
         }
         else if (card->cardType == MMCSD_CARD_MMC)
         {
            term_printf("MMC Card\r\n");
         }
#endif
         /* Set bus width */
         if (card->cardType == MMCSD_CARD_SD)
         {
            MMCSDBusWidthSet(card->ctrl);
         }
   
         /* Transfer speed */
         MMCSDTranSpeedSet(card->ctrl);
      
         DiskStatus &= ~STA_NOINIT;
      }
   } /* end if (DiskStatus & STA_NOINIT) */

   SD_LED_OFF();

   return(DiskStatus);
} /* disk_initialize */

/*************************************************************************/
/*  disk_removed                                                         */
/*                                                                       */
/*  Medium was removed                                                   */
/*                                                                       */
/*  In    : pdrv, physical drive nmuber to identify the drive            */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void disk_removed (BYTE pdrv)
{
   if (0 == pdrv)
   {
      DiskStatus |= STA_NODISK;
      DiskStatus |= STA_NOINIT;
   }
} /* disk_removed */

/*************************************************************************/
/*  disk_status                                                          */
/*                                                                       */
/*  Get Drive Status                                                     */
/*                                                                       */
/*  In    : pdrv, physical drive nmuber to identify the drive            */
/*  Out   : none                                                         */
/*  Return: Status of Disk Functions                                     */
/*************************************************************************/
DSTATUS disk_status (uint8_t pdrv)
{
   if (0 == pdrv)
   {
      DiskStatus |= STA_NODISK;

      if (1 == MMCSDCardPresent(sdCard.ctrl))
      {
         DiskStatus &= ~STA_NODISK;
      }
   }
   else
   {
      return(STA_NOINIT);
   } 

   return(DiskStatus);
} /* disk_status */

/*************************************************************************/
/*  disk_read                                                            */
/*                                                                       */
/*  Read Sector(s)                                                       */
/*                                                                       */
/*  In    : pdrv,   physical drive nmuber to identify the drive          */
/*          buff,   data buffer to store read data                       */
/*          sector, sector address in LBA                                */
/*          count,  number of sectors to read                            */
/*  Out   : none                                                         */
/*  Return: Results of Disk Functions                                    */
/*************************************************************************/
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
   DRESULT        Result = RES_ERROR;
   mmcsdCardInfo *card   = &sdCard;  
 
   if (0 == pdrv)
   {
      /* Check drive status */
      if (DiskStatus & STA_NOINIT)
      {
         return(RES_NOTRDY);
      }  
      
      SD_LED_ON();
      
      CacheDataCleanBuff((uint32_t)buff, (uint32_t)(count * BLOCK_SIZE));
   
      if (MMCSDReadCmdSend(card->ctrl, buff, sector, count) == 1)
      {
      
         Result = RES_OK;
      }
      
      SD_LED_OFF();
   }

   return(Result);
} /* disk_read */

/*************************************************************************/
/*  disk_write                                                           */
/*                                                                       */
/*  Write Sector(s)                                                      */
/*                                                                       */
/*  In    : pdrv,   physical drive nmuber to identify the drive          */
/*          buff,   data to be written                                   */
/*          sector, sector address in LBA                                */
/*          count,  number of sectors to write                           */
/*  Out   : none                                                         */
/*  Return: Results of Disk Functions                                    */
/*************************************************************************/
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
   DRESULT        Result = RES_ERROR;
   mmcsdCardInfo *card   = &sdCard;  
 
   if (0 == pdrv)
   {
      /* Check drive status */
      if (DiskStatus & STA_NOINIT)
      {
         return(RES_NOTRDY);
      }
      
      SD_LED_ON();
      
      CacheDataCleanBuff((uint32_t)buff, (uint32_t)(count * BLOCK_SIZE));

      if(MMCSDWriteCmdSend(card->ctrl, (BYTE*)buff, sector, count) == 1)
      {
         Result = RES_OK;
      }
      
      SD_LED_OFF();
   }

   return(Result);
} /* disk_write */

/*************************************************************************/
/*  disk_ioctl                                                           */
/*                                                                       */
/*  In    : pdrv, physical drive nmuber to identify the drive            */
/*          cmd,  control code                                           */
/*          buff, buffer to send/receive control data                    */
/*  Out   : none                                                         */
/*  Return: Results of Disk Functions                                    */
/*************************************************************************/
DRESULT disk_ioctl (uint8_t pdrv, uint8_t cmd, void *buff)
{
   DRESULT Result = RES_PARERR;
  
   if (0 == pdrv)
   {
      /* Check drive status */
      if (DiskStatus & STA_NOINIT)
      {
         return(RES_NOTRDY);
      }
      
      SD_LED_ON();
      
      switch(cmd)
      {
         case CTRL_SYNC:         /* Make sure that no pending write process */
            Result = RES_OK;
            break;
            
#if 0            
         case GET_SECTOR_COUNT:  /* Get number of sectors on the disk (DWORD) */
            SD_GetCardInfo(&SDCardInfo);  
            *(DWORD*)buff = (DWORD)(SDCardInfo.CardCapacity / 512);
            Result = RES_OK;
            break;
#endif            

         case GET_SECTOR_SIZE:   /* Get R/W sector size (WORD) */
            *(WORD*)buff = BLOCK_SIZE;
            Result = RES_OK;
            break;
            
         case GET_BLOCK_SIZE:    /* Get erase block size in unit of sector (DWORD) */
            *(DWORD*)buff = BLOCK_SIZE;
            Result = RES_OK;
            break;

         default:
            Result = RES_PARERR;
            break;
      }
      
      SD_LED_OFF();
   }
   
   return(Result);
} /* disk_ioctl */

/*************************************************************************/
/*  get_fattime                                                          */
/*                                                                       */
/*  Gets Time from RTC                                                   */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: Time                                                         */
/*************************************************************************/
DWORD get_fattime (void)
{
   return(0);
} /* get_fattime */

uint8_t SD_Detect (void)
{
   uint8_t status = SD_PRESENT;
      
   if (disk_status(0) & STA_NODISK)
   {
      status = SD_NOT_PRESENT;
   }
   
   return(status);
}

/*** EOF ***/
