/**************************************************************************
*  This file is part of the TAL project (Tiny Abstraction Layer)
*
*  Copyright (c) 2013-2023 by Michael Fischer (www.emb4fun.de).
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
#define __TALCOM_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>
#include "tal.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * If virtual com port support is not needed
 */
#if !defined(TAL_COM_PORT_VIRTUAL)
#define COMInit            cpu_COMInit
#define COMIoctl           cpu_COMIoctl
#define COMOpen            cpu_COMOpen
#define COMClose           cpu_COMClose
#define COMStartTx         cpu_COMStartTx
#define COMTxIsRunning     cpu_COMTxIsRunning
#define COMSendStringASS   cpu_COMSendStringASS
#endif 

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

#if defined(TAL_COM_PORT_VIRTUAL)
static TAL_COM_FUNC FuncTable[TAL_COM_PORT_MAX];
#endif

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

#if defined(TAL_COM_PORT_VIRTUAL)

/*************************************************************************/
/*  Virtual com port support                                             */
/*************************************************************************/
static TAL_RESULT COMInit (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      Error = cpu_COMInit(pDCB);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMInit != NULL))
      {
         Error = FuncTable[pDCB->ePort].COMInit(pDCB);
      }   
   }

   return(Error);   
} /* COMInit */

static TAL_RESULT COMIoctl (TAL_COM_DCB *pDCB, TAL_COM_IOCTL eFunc, uint32_t *pParam)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      Error = cpu_COMIoctl(pDCB, eFunc, pParam);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMIoctl != NULL))
      {
         Error = FuncTable[pDCB->ePort].COMIoctl(pDCB, eFunc, pParam);
      }         
   }

   return(Error);   
} /* COMIoctl */

static TAL_RESULT COMOpen (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      Error = cpu_COMOpen(pDCB);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMOpen != NULL))
      {
         Error = FuncTable[pDCB->ePort].COMOpen(pDCB);
      }   
   }

   return(Error);   
} /* COMOpen */

static TAL_RESULT COMClose (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      Error = cpu_COMClose(pDCB);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMClose != NULL))
      {
         Error = FuncTable[pDCB->ePort].COMClose(pDCB);
      }         
   }

   return(Error);   
} /* COMClose */

static TAL_RESULT COMStartTx (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      Error = cpu_COMStartTx(pDCB);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMStartTx != NULL))
      {
         Error = FuncTable[pDCB->ePort].COMStartTx(pDCB);
      }         
   }

   return(Error);   
} /* COMStartTx */

static TAL_RESULT COMTxIsRunning (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      Error = cpu_COMTxIsRunning(pDCB);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMTxIsRunning != NULL))
      {
         Error = FuncTable[pDCB->ePort].COMTxIsRunning(pDCB);
      }         
   }

   return(Error);   
} /* COMTxIsRunning */

static void COMSendStringASS (TAL_COM_DCB *pDCB, char *pString)
{
   if ((int)pDCB->ePort < (int)TAL_COM_PORT_V1)
   {
      cpu_COMSendStringASS(pDCB, pString);
   }
   else
   {
      if ((pDCB->ePort < TAL_COM_PORT_MAX) && (FuncTable[pDCB->ePort].COMSendStringASS != NULL))
      {
         FuncTable[pDCB->ePort].COMSendStringASS(pDCB, pString);
      }         
   }

} /* COMSendStringASS */ 

#endif /* TAL_COM_PORT_VIRTUAL */

/*************************************************************************/
/*  GetRxData                                                            */
/*                                                                       */
/*  Get data if available.                                               */
/*                                                                       */
/*  In    : pDCB, pData                                                  */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
static TAL_RESULT GetRxData (TAL_COM_DCB *pDCB, uint8_t *pData)
{
   TAL_RESULT Error;
   
   Error = tal_MISCRingGet(&pDCB->RxRing, pData);
   if (TAL_ERR_RING_EMPTY == Error)
   {
      /* Ring is empty */
      if (TAL_FALSE == pDCB->bRxOverflow)
      {
         Error = TAL_ERR_COM_EMPTY;
      }
      else
      {
         Error = TAL_ERR_COM_OVERFLOW_EMPTY;
      }
   }
   else if (TAL_OK == Error)
   {
      /* Ring OK */
      if (TAL_FALSE == pDCB->bRxOverflow)
      {
         Error = TAL_OK;
      }
      else
      {
         Error = TAL_ERR_COM_OVERFLOW;
      }
   }
   
   return(Error);
} /* GetRxData */ 

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  tal_COMInit                                                          */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_COMInit (void)
{
#if defined(TAL_COM_PORT_VIRTUAL)
   memset(FuncTable, 0, sizeof(FuncTable));
#endif

} /* tal_COMInit */

#if defined(TAL_COM_PORT_VIRTUAL)
/*************************************************************************/
/*  tal_COMAdd                                                           */
/*                                                                       */
/*  Add the COM functionality for the given port.                        */
/*                                                                       */
/*  In    : ePort, pFunc                                                 */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMAdd (TAL_COM_PORT ePort, TAL_COM_FUNC *pFunc)
{
   TAL_RESULT Error = TAL_ERROR;
   
   if ((ePort < TAL_COM_PORT_MAX) && (pFunc != NULL))
   {
      if( (NULL != pFunc->COMInit)          &&
          (NULL != pFunc->COMIoctl)         &&
          (NULL != pFunc->COMOpen)          &&
          (NULL != pFunc->COMClose)         &&
          (NULL != pFunc->COMStartTx)       &&
          (NULL != pFunc->COMTxIsRunning)   &&
          (NULL != pFunc->COMSendStringASS) )
      {
         Error = TAL_OK;
         
         FuncTable[ePort].COMInit          = pFunc->COMInit;
         FuncTable[ePort].COMIoctl         = pFunc->COMIoctl;
         FuncTable[ePort].COMOpen          = pFunc->COMOpen;
         FuncTable[ePort].COMClose         = pFunc->COMClose;
         FuncTable[ePort].COMStartTx       = pFunc->COMStartTx;
         FuncTable[ePort].COMTxIsRunning   = pFunc->COMTxIsRunning;
         FuncTable[ePort].COMSendStringASS = pFunc->COMSendStringASS;
      }          
   }

   return(Error);
} /* tal_COMAdd */
#endif /* defined(TAL_COM_PORT_VIRTUAL) */

/*************************************************************************/
/*  tal_COMInitDCB                                                       */
/*                                                                       */
/*  Initialize the Device Control Block.                                 */
/*                                                                       */
/*  In    : pDCB, ePort                                                  */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMInitDCB (TAL_COM_DCB *pDCB, TAL_COM_PORT ePort)
{
   TAL_RESULT Error = TAL_ERR_NULL_POINTER;
   
   if (pDCB != 0) 
   {   
      /* Clear DCB first */
      memset(pDCB, 0x00, sizeof(TAL_COM_DCB));

      /* Store Magic and Port information */
      pDCB->eMagic = TAL_MAGIC_COM; 
      pDCB->ePort  = ePort;
      
      /* Init the COM hardware layer */
      Error = COMInit(pDCB);
      if (TAL_OK == Error)
      {
         OS_RES_CREATE(&pDCB->Sema);
         OS_RES_CREATE(&pDCB->TxSema);
         
         OS_SemaCreate(&pDCB->RxRdySema, 0, OS_SEMA_COUNTER_MAX);
         
         pDCB->bDCBInitDone = TAL_TRUE;
      }
   }
   
   return(Error);
} /* tal_COMInitDCB */

/*************************************************************************/
/*  tal_COMIoctl                                                         */
/*                                                                       */
/*  Call a IOCTL function.                                               */
/*                                                                       */
/*  In    : pDCB, eFunc, pParam                                          */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMIoctl (TAL_COM_DCB *pDCB, TAL_COM_IOCTL eFunc, uint32_t *pParam)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (pParam        != 0)                  &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      OS_RES_LOCK(&pDCB->Sema);
      
      Error = COMIoctl(pDCB, eFunc, pParam);
   
      OS_RES_FREE(&pDCB->Sema);
   }
   
   return(Error);
} /* tal_COMIoctl */

/*************************************************************************/
/*  tal_COMSetRingBuffer                                                 */
/*                                                                       */
/*  Set the ring buffer values.                                          */
/*                                                                       */
/*  In    : pDCB, eBuffer, pBuffer, wBufferSize                          */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMSetRingBuffer (TAL_COM_DCB   *pDCB, 
                                 TAL_COM_BUFFER eBuffer,
                                 uint8_t       *pBuffer, 
                                 uint16_t       wBufferSize)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;
   
   /* Check for valid parameters */
   if ( (pDCB          != NULL)               &&
        (pBuffer       != NULL)               &&
        (wBufferSize   != 0)                  &&  
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      OS_RES_LOCK(&pDCB->Sema);
   
      if (TAL_COM_BUFFER_RX == eBuffer)
      {
         tal_MISCRingSetup(&pDCB->RxRing, pBuffer, sizeof(uint8_t), wBufferSize);  
         
         Error = TAL_OK;
      }

      if (TAL_COM_BUFFER_TX == eBuffer)
      {
         tal_MISCRingSetup(&pDCB->TxRing, pBuffer, sizeof(uint8_t), wBufferSize);  
         
         Error = TAL_OK;
      }
      
      OS_RES_FREE(&pDCB->Sema);
   }
   
   return(Error);
} /* tal_COMSetRingBuffer */

/*************************************************************************/
/*  tal_COMClearOverflow                                                 */
/*                                                                       */
/*  Clear the ring buffer.                                               */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMClearOverflow (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->Sema);
         
         tal_MISCRingReset(&pDCB->RxRing);
         
         /* Clear counting semaphore */
         OS_SemaDelete(&pDCB->RxRdySema);
         OS_SemaCreate(&pDCB->RxRdySema, 0, OS_SEMA_COUNTER_MAX);
         
         OS_RES_FREE(&pDCB->Sema);
         
         Error = TAL_OK;
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }        

   return(Error);
} /* tal_COMClearOverflow */

/*************************************************************************/
/*  tal_COMOpen                                                          */
/*                                                                       */
/*  Open the COM port with the given settings.                           */
/*                                                                       */
/*  In    : pDCB, pSettings                                              */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMOpen (TAL_COM_DCB *pDCB, TAL_COM_SETTINGS *pSettings)
{
   TAL_RESULT  Error = TAL_ERR_PARAMETER;
   
   /* Check for valid parameters */
   if ( (pDCB          != NULL)               &&
        (pSettings     != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_FALSE == pDCB->bIsOpen)
      {
         /* Check if RX ring buffer is available */
         if (NULL == pDCB->RxRing.pBuffer)
         {
            Error = TAL_ERR_COM_NO_RX_BUFF;
            goto COMOpenEnd;  /*lint !e801*/
         }

         /* Check if TX ring buffer is available */
         if (NULL == pDCB->TxRing.pBuffer)
         {
            Error = TAL_ERR_COM_NO_TX_BUFF;
            goto COMOpenEnd;  /*lint !e801*/
         }

         OS_RES_LOCK(&pDCB->Sema);

         /* Reset ring buffer */         
         tal_MISCRingReset(&pDCB->RxRing);
         tal_MISCRingReset(&pDCB->TxRing);
         
         /* Clear counting semaphore */
         OS_SemaDelete(&pDCB->RxRdySema);
         OS_SemaCreate(&pDCB->RxRdySema, 0, OS_SEMA_COUNTER_MAX);
      
         /* Copy settings */
         pDCB->Settings = *pSettings;
         
         Error = COMOpen(pDCB);
         if (TAL_OK == Error)
         {
            pDCB->bIsOpen = TAL_TRUE;
         }
         
         OS_RES_FREE(&pDCB->Sema);
      }
      else
      {
         Error = TAL_ERR_COM_NOT_CLOSED;
      }
   }   
   
COMOpenEnd:   
   return(Error);
} /* tal_COMOpen */

/*************************************************************************/
/*  tal_COMClose                                                         */
/*                                                                       */
/*  Close the COM port.                                                  */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMClose (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->Sema);
         
         Error = COMClose(pDCB);
         if (TAL_OK == Error)
         {
            pDCB->bIsOpen = TAL_FALSE;
         }
         
         OS_RES_FREE(&pDCB->Sema);
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }        

   return(Error);
} /* tal_COMClose */

/*************************************************************************/
/*  tal_COMSendBlock                                                     */
/*                                                                       */
/*  Send a block of data.                                                */
/*                                                                       */
/*  There is currently an issue with the NEORV32. If the data is output  */
/*  under interrupt control, it could be possible that some data are     */
/*  lost.                                                                */
/*                                                                       */
/*  In    : pDCB, pData, wSize                                           */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMSendBlock (TAL_COM_DCB *pDCB, uint8_t *pData, uint16_t wSize)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;

   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (pData         != NULL)               &&
        (wSize         != 0)                  &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->TxSema);

#if defined(__NEORV32_FAMILY)
         /*
          * Use polling instead of interrupt control
          */
         neorv32_uart_t *UARTx;
         UARTx = (neorv32_uart_t*)pDCB->HW.dBaseAddress;
         while (wSize)
         {
            neorv32_uart_putc(UARTx, *pData);
            pData++;
            wSize--;
         }
#else
#if 0
         do
         {
            /* Add data to the ring buffer */
            Error = tal_MISCRingAdd(&pDCB->TxRing, pData);
            if (Error != TAL_OK)
            {
               /*
                * The data was not added to the ring buffer!
                *
                * Ring buffer is full, wait for an free entry.
                */
               while ( 0 == tal_MISCRingGetFreeCount(&pDCB->TxRing) )
               {
                  /* Check if the transmitter is still runing */
                  if (TAL_ERROR == COMTxIsRunning(pDCB))
                  {
                     /* No, start the transmitter */
                     COMStartTx(pDCB);
                  }
                  OS_TimeDly(1);
               }

               /*
                * A free entry is available, we can add
                * the data to the ring buffer now.
                */
               tal_MISCRingAdd(&pDCB->TxRing, pData);
            }

            /*
             * The data was added, switch to the next one
             * and start the transmitter.
             */
            pData++;
            wSize--;

            COMStartTx(pDCB);
         }
         while (wSize != 0);
#else
         uint16_t wFreeCount = tal_MISCRingGetFreeCount(&pDCB->TxRing);

         /* Check if we can put all data in the ring buffer */
         if (wFreeCount >= wSize)
         {
            /* Put all data to send into the ring buffer */
            while (wSize)
            {
               tal_MISCRingAdd(&pDCB->TxRing, pData);
               pData++;
               wSize--;
            }
            /* Start Tx if needed */
            COMStartTx(pDCB);
         }
         else
         {
            /* Ups, no space for all data, add byte by byte into the ring buffer */
            do
            {
               /* Add data to the ring buffer */
               Error = tal_MISCRingAdd(&pDCB->TxRing, pData);
               if (Error != TAL_OK)
               {
                  /*
                   * The data was not added to the ring buffer!
                   *
                   * Ring buffer is full, wait for an free entry.
                   */
                  while ( 0 == tal_MISCRingGetFreeCount(&pDCB->TxRing) )
                  {
                     /* Check if the transmitter is still runing */
                     if (TAL_ERROR == COMTxIsRunning(pDCB))
                     {
                        /* No, start the transmitter */
                        COMStartTx(pDCB);
                     }
                     OS_TimeDly(1);
                  }

                  /*
                   * A free entry is available, we can add
                   * the data to the ring buffer now.
                   */
                  tal_MISCRingAdd(&pDCB->TxRing, pData);
               }

               /*
                * The data was added, switch to the next one
                * and start the transmitter.
                */
               pData++;
               wSize--;

               /* Start Tx if needed */
               COMStartTx(pDCB);
            }
            while (wSize != 0);
         } /* end if (wFreeCount >= wSize) */
#endif
#endif /* #if defined(__NEORV32_FAMILY) */

         OS_RES_FREE(&pDCB->TxSema);

         Error = TAL_OK;
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }

   return(Error);
} /* tal_COMSendBlock */

/*************************************************************************/
/*  tal_COMSendString                                                    */
/*                                                                       */
/*  Send a String.                                                       */
/*                                                                       */
/*  In    : pDCB, pString                                                */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMSendString (TAL_COM_DCB *pDCB, char *pString)
{
   TAL_RESULT Error;
   
   Error = tal_COMSendBlock(pDCB, (uint8_t*)pString, (uint16_t)strlen(pString));
   
   return(Error);
} /* tal_COMSendString */

/*************************************************************************/
/*  tal_COMSendChar                                                      */
/*                                                                       */
/*  Send one character.                                                  */
/*                                                                       */
/*  In    : pDCB, cData                                                  */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMSendChar (TAL_COM_DCB *pDCB, char cData)
{
   TAL_RESULT Error;
   
   Error = tal_COMSendBlock(pDCB, (uint8_t*)&cData, 1);
   
   return(Error);
} /* tal_COMSendChar */

/*************************************************************************/
/*  tal_COMReceiveChar                                                   */
/*                                                                       */
/*  Get one data if available.                                           */
/*                                                                       */
/*  In    : pDCB, pData                                                  */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMReceiveChar (TAL_COM_DCB *pDCB, uint8_t *pData)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (pData         != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->Sema);
         
         Error = GetRxData(pDCB, pData);
         
         OS_RES_FREE(&pDCB->Sema);
         
         if ((TAL_OK == Error) || (TAL_ERR_COM_OVERFLOW == Error))
         {
            /* Data was available, RxRdySema must be decrease */
            OS_SemaWait(&pDCB->RxRdySema, 1);
         }
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }
   
   return(Error);        
} /* tal_COMReceiveChar */

/*************************************************************************/
/*  tal_COMReceiveCharWait                                               */
/*                                                                       */
/*  Get one data if available, but with timeout.                         */
/*                                                                       */
/*  In    : pDCB, pData, dTimeoutMs                                      */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMReceiveCharWait (TAL_COM_DCB *pDCB, uint8_t *pData, uint32_t dTimeoutMs)
{
   TAL_RESULT Error = TAL_ERR_PARAMETER;
   int        rc;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (pData         != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->Sema);

         /* Check if data is available */         
         Error = GetRxData(pDCB, pData);
         if ((TAL_OK == Error) || (TAL_ERR_COM_OVERFLOW == Error))
         {
            /* Data was available, RxRdySema must be decrease */
            OS_SemaWait(&pDCB->RxRdySema, 1);
         }
         else
         {
            /* Data is not available, wait for it */
            rc = OS_SemaWait(&pDCB->RxRdySema, dTimeoutMs);
            if (OS_RC_OK == rc)
            {
               /* Data is available */
               Error = GetRxData(pDCB, pData);
            }
         }

         OS_RES_FREE(&pDCB->Sema);
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }
   
   return(Error);        
} /* tal_COMReceiveCharWait */

/*************************************************************************/
/*  tal_COMReceiveCharTest                                               */
/*                                                                       */
/*  Test if a received char is available.                                */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMReceiveCharTest (TAL_COM_DCB *pDCB)
{
   TAL_RESULT Error = TAL_ERROR;
   uint16_t  wCount;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->Sema);
         
         wCount = tal_MISCRingGetUseCount(&pDCB->RxRing);
         if (wCount != 0)
         {
            Error = TAL_OK;
         }

         OS_RES_FREE(&pDCB->Sema);
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }
   
   return(Error);        
} /* tal_COMReceiveCharTest */

/*************************************************************************/
/*  tal_COMReceiveCharTestWait                                           */
/*                                                                       */
/*  Test if a received char is available, but with timeout.              */
/*                                                                       */
/*  In    : pDCB                                                         */
/*  Out   : none                                                         */
/*  Return: TAL_OK / error cause                                         */
/*************************************************************************/
TAL_RESULT tal_COMReceiveCharTestWait (TAL_COM_DCB *pDCB, uint32_t dTimeoutMs)
{
   TAL_RESULT Error = TAL_ERROR;
   uint16_t  wCount;
   int        rc;
   
   /* Check for valid conditions */
   if ( (pDCB          != NULL)               &&
        (TAL_TRUE      == pDCB->bDCBInitDone) &&
        (TAL_MAGIC_COM == pDCB->eMagic)       )
   {
      if (TAL_TRUE == pDCB->bIsOpen)
      {
         OS_RES_LOCK(&pDCB->Sema);
         
         wCount = tal_MISCRingGetUseCount(&pDCB->RxRing);
         if (wCount != 0)
         {
            Error = TAL_OK;
         }
         else
         {
            /* Data is not available, wait for it */
            rc = OS_SemaWait(&pDCB->RxRdySema, dTimeoutMs);
            if (OS_RC_OK == rc)
            {
               /* Data is available */

               /* 
                * We have consumed the semaphore but does not read 
                * any data therefore we must signal the semaphore again.
                */
               OS_SemaSignal(&pDCB->RxRdySema);

               Error = TAL_OK;
            }
         }

         OS_RES_FREE(&pDCB->Sema);
      }
      else
      {
         Error = TAL_ERR_COM_NOT_OPEN;
      }
   }
   
   return(Error);        
} /* tal_COMReceiveCharTestWait */

/*************************************************************************/
/*  tal_COMSendStringASS                                                 */
/*                                                                       */
/*  Send a String.                                                       */
/*                                                                       */
/*  In    : pDCB, pString                                                */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void tal_COMSendStringASS (TAL_COM_DCB *pDCB, char *pString)
{
   COMSendStringASS(pDCB, pString);
} /* tal_COMSendString */

/*** EOF ***/
