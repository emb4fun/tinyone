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
*  06.06.2013  mifi  First Version.
*  19.08.2013  mifi  Some rework and cleanup.
*  05.01.2016  mifi  Rename IP_PHY_to ETH_PHY_.
**************************************************************************/
#define __ETH_PHY_DP83848_C__

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
                              
/*
 * Extended register
 */
#define PHY_PHYSTS      0x10  /* PHY Status Register */
#define PHY_MICR        0x11  /* MII Interrupt Control Register */
#define PHY_MISR        0x12  /* MII Interrupt Status Register */
#define PHY_FCSCR       0x14  /* False Carrier Sense Counter Register */
#define PHY_RECR        0x15  /* Receive Error Counter Register */
#define PHY_PCSR        0x16  /* PCS Sub-Layer Configuration and Status Register */
#define PHY_RBR         0x17  /* RMII and Bypass Register */
#define PHY_LEDCR       0x18  /* LED Direct Control Register */
#define PHY_PHYCR       0x19  /* PHY Control Register */
#define PHY_10BTSCR     0x1A  /* 10Base-T Status/Control Register */
#define PHY_CDCTRL1     0x1B  /* CD Test Control Register and BIST Extensions Register */
#define PHY_EDCR        0x1D  /* Energy Detect Control Register */ 
 
/*
 * Register bit definitions
 */
#define PHY_BMCR_AUTO_NEG_START  0x0200
#define PHY_BMCR_AUTO_NEG_EN     0x1000 
 
#define PHY_ANAR_10              0x0020
#define PHY_ANAR_10_FD           0x0040
#define PHY_ANAR_TX              0x0080
#define PHY_ANAR_TX_FD           0x0100
  
#define PHY_PHYSTS_LINK_STATUS   0x0001
#define PHY_PHYSTS_SPEED_STATUS  0x0002
#define PHY_PHYSTS_DUPLEX_STATUS 0x0004


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

   Value = pPHY->ReadReg(pPHY, PHY_PHYSTS);
   if (Value & PHY_PHYSTS_LINK_STATUS)
   {
      if (Value & PHY_PHYSTS_SPEED_STATUS)
      {
         Status |= PHY_LINK_STATUS_SPEED_10M;
      }
      else
      {
         Status |= PHY_LINK_STATUS_SPEED_100M;
      }
      
      if (Value & PHY_PHYSTS_DUPLEX_STATUS)
      {
         Status |= PHY_LINK_STATUS_MODE_FULL;
      }
      else
      {
         Status |= PHY_LINK_STATUS_MODE_HALF;
      }
   }
   
   pPHY->Status = Status;
   
   return(Status);
} /* GetLinkStatus */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

ETH_PHY_DRIVER PHY_DP83848 =
{
   Init,
   GetLinkStatus,
   NULL,
   NULL,
   0,
   0
};

/*** EOF ***/
