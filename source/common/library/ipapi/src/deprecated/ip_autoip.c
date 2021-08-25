/**************************************************************************
*  Copyright (c) 2014 by Michael Fischer (www.emb4fun.de).
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
*  08.02.2014  mifi  First Version.
*  07.02.2015  mifi  Rework to use lwip.
**************************************************************************/
#define __IP_AUTOIP_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include "ipstack.h"
#include "terminal.h"

#include "lwip\netifapi.h"

/*=======================================================================*/
/*  All extern definitions                                               */
/*=======================================================================*/

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

static int AutoIPInUse = 0;

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/*************************************************************************/
/*  IPAutoIPStart                                                        */
/*                                                                       */
/*  Start the AutoIP client.                                             */
/*                                                                       */
/*  In    : addr                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IPAutoIPStart (uint32_t addr)
{
   struct netif *netif;
   
   (void)addr;
   
   if (0 == AutoIPInUse)
   {
      netif = netif_find("en0");
      if (netif != NULL)
      {
         AutoIPInUse = 1;
         netifapi_autoip_start(netif);
      }   
   }   
} /* IPAutoIPStart */

/*************************************************************************/
/*  IPAutoIPStop                                                         */
/*                                                                       */
/*  Stop the AutoIP client.                                              */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IPAutoIPStop (void)
{
   struct netif *netif;
   
   if (1 == AutoIPInUse)
   {
      netif = netif_find("en0");
      if (netif != NULL)
      {
         AutoIPInUse = 0;
         netifapi_autoip_stop(netif);
      }   
   }
} /* IPAutoIPStop */

/*************************************************************************/
/*  IPAutoIPIsStarted                                                    */
/*                                                                       */
/*  In    : none                                                         */
/*  Out   : none                                                         */
/*  Return: 0 not started / 1 otherwise                                  */
/*************************************************************************/
int IPAutoIPIsStarted (void)
{
   return(AutoIPInUse);
} /* IPAutoIPIsStarted */

/*************************************************************************/
/*  IPAutoIPCallbackSet                                                  */
/*                                                                       */
/*  In    : Callback                                                     */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
void IPAutoIPCallbackSet (ip_autoip_callback_t callback)
{
   (void)callback;
} /* IPAutoIPCallbackSet */

/*** EOF ***/
