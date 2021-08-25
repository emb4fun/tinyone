/**************************************************************************
*  Copyright (c) 2013 by Michael Fischer (www.emb4fun.de).
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
*  20.08.2013  mifi  First Version.
*  05.01.2016  mifi  Rename IP_PHY_to ETH_PHY_.
**************************************************************************/
#define __ETH_PHY_KSZ8721_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <stdint.h>
#include <string.h>
#include "eth_phy_driver.h"

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Standard register
 */
#define PHY_BMCR        0x00  /* Basic Mode Control Register */
#define PHY_BMSR        0x01  /* Basic Mode Status Register */
#define PHY_PHYIDR1     0x02  /* PHY Identifier Register #1 */
#define PHY_PHYIDR2     0x03  /* PHY Identifier Register #2 */
#define PHY_ANAR        0x04  /* Auto-Negotiation Advertisement Register */
#define PHY_ANLPAR      0x05  /* Auto-Negotiation Link Partner Ability Register (Base Page) */
#define PHY_ANLPARNP    0x05  /* Auto-Negotiation Link Partner Ability Register (Next Page) */
#define PHY_ANER        0x06  /* Auto-Negotiation Expansion Register */
#define PHY_ANNPTP      0x07  /* Auto-Negotiation Next Page TX */
#define PHY_LPNPA       0x08  /* Link Partner Next Page Ability */
#define PHY_RECR        0x15  /* Receive Error Counter Register */
                               
/*
 * Register bit definitions
 */
#define PHY_BMCR_AUTO_NEG_START  0x0200
#define PHY_BMCR_AUTO_NEG_EN     0x1000 

#define PHY_BMSR_LIU             0x0004   /* Link is up */
#define PHY_BMSR_100FD           0x4000
#define PHY_BMSR_100HD           0x2000  
#define PHY_BMSR_10FD            0x1000  
#define PHY_BMSR_10HD            0x0800  
 
#define PHY_ANAR_10              0x0020
#define PHY_ANAR_10_FD           0x0040
#define PHY_ANAR_TX              0x0080
#define PHY_ANAR_TX_FD           0x0100
  

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/*************************************************************************/
/*  Init                                                                 */
/*                                                                       */
/*  In    : pPHY                                                         */
/*  Out   : none                                                         */
/*  Return: none                                                         */
/*************************************************************************/
static void Init (ETH_PHY_DRIVER *pPHY)
{
   uint16_t Value = 0;

   /*
    * Set Negotiation Advertisement Register 
    */
   Value  = pPHY->ReadReg(pPHY, PHY_ANAR);
   Value |= (PHY_ANAR_10 | PHY_ANAR_10_FD | PHY_ANAR_TX | PHY_ANAR_TX_FD);
   pPHY->WriteReg(pPHY, PHY_ANAR, Value);
    
   /*
    * Start Auto-Negotiation
    */
   Value  = pPHY->ReadReg(pPHY, PHY_BMCR);
   Value |= (PHY_BMCR_AUTO_NEG_EN | PHY_BMCR_AUTO_NEG_START);
   pPHY->WriteReg(pPHY, PHY_BMCR, Value);

} /* Init */

/*************************************************************************/
/*  GetLinkStatus                                                        */
/*                                                                       */
/*  In    : pPHY                                                         */
/*  Out   : none                                                         */
/*  Return: Status                                                       */
/*************************************************************************/
static uint8_t GetLinkStatus (ETH_PHY_DRIVER *pPHY)
{
   uint8_t  Status = 0;
   uint16_t Value = 0;

   Value = pPHY->ReadReg(pPHY, PHY_BMSR);
   if (Value & PHY_BMSR_LIU)
   {
      if      (Value & PHY_BMSR_100FD)
      {
         Status |= PHY_LINK_STATUS_SPEED_100M;
         Status |= PHY_LINK_STATUS_MODE_FULL;
      }
      else if (Value & PHY_BMSR_100HD)
      {
         Status |= PHY_LINK_STATUS_SPEED_100M;
         Status |= PHY_LINK_STATUS_MODE_HALF;
      }
      else if (Value & PHY_BMSR_10FD)
      {
         Status |= PHY_LINK_STATUS_SPEED_10M;
         Status |= PHY_LINK_STATUS_MODE_FULL;
      }
      else if (Value & PHY_BMSR_10HD)
      {
         Status |= PHY_LINK_STATUS_SPEED_10M;
         Status |= PHY_LINK_STATUS_MODE_HALF;
      }
   }
   
   pPHY->Status = Status;
   
   return(Status);
} /* GetLinkStatus */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

ETH_PHY_DRIVER PHY_KSZ8721 =
{
   Init,
   GetLinkStatus,
   NULL,
   NULL,
   0,
   0
};

/*** EOF ***/
