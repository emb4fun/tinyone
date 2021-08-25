/**************************************************************************
*  Copyright (c) 2016 by Michael Fischer (www.emb4fun.de).
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
*  05.03.2016  mifi  First Version.
**************************************************************************/
#define __ETH_PHY_SMSC8720_C__

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
#define PHY_BCR         0x00  /* Basic Control Register */
#define PHY_BSR         0x01  /* Basic Status Register */
#define PHY_PHYIDR1     0x02  /* PHY Identifier Register #1 */
#define PHY_PHYIDR2     0x03  /* PHY Identifier Register #2 */
#define PHY_ANAR        0x04  /* Auto-Negotiation Advertisement Register */
#define PHY_ANLPAR      0x05  /* Auto-Negotiation Link Partner Ability Register */
#define PHY_ANER        0x06  /* Auto-Negotiation Expansion Register */

/*
 * Extended register
 */
#define PHY_PHYSCSR     0x1F  /* PHY Special Control/Status Register */


/*
 * Register bit definitions
 */
#define PHY_BCR_AUTO_NEG_RESTART 0x0200
#define PHY_BCR_AUTO_NEG_EN      0x1000 

#define PHY_BSR_LINK_UP          0x0004
 
#define PHY_ANAR_10              0x0020
#define PHY_ANAR_10_FD           0x0040
#define PHY_ANAR_100_TX          0x0080
#define PHY_ANAR_100_TX_FD       0x0100
  
#define PHY_PHYSCSR_AUTO_DONE    0x1000
#define PHY_PHYSCSR_SPEED_MASK   0x001C  
#define PHY_PHYSCSR_SPEED_10_HD  0x0004
#define PHY_PHYSCSR_SPEED_10_FD  0x0014
#define PHY_PHYSCSR_SPEED_100_HD 0x0008
#define PHY_PHYSCSR_SPEED_100_FD 0x0018


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
   uint16_t Value;

   /*
    * Set Negotiation Advertisement Register 
    */
   Value  = pPHY->ReadReg(pPHY, PHY_ANAR);
   Value |= (PHY_ANAR_10 | PHY_ANAR_10_FD | PHY_ANAR_100_TX | PHY_ANAR_100_TX_FD);
   pPHY->WriteReg(pPHY, PHY_ANAR, Value);
    
   /*
    * Start Auto-Negotiation
    */
   Value  = pPHY->ReadReg(pPHY, PHY_BCR);
   Value |= (PHY_BCR_AUTO_NEG_EN | PHY_BCR_AUTO_NEG_RESTART);
   pPHY->WriteReg(pPHY, PHY_BCR, Value);

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
   static uint16_t OldLinkStatus = 0;
   uint16_t        LinkStatus;
   
   uint8_t  Status = 0;
   uint16_t Value = 0;
   
   LinkStatus = pPHY->ReadReg(pPHY, PHY_BSR);
   if (LinkStatus != OldLinkStatus)
   {
      OldLinkStatus = LinkStatus;
      
      if (LinkStatus & PHY_BSR_LINK_UP)
      {
         Value  = pPHY->ReadReg(pPHY, PHY_PHYSCSR);
         Value &= PHY_PHYSCSR_SPEED_MASK;
         
         if (PHY_PHYSCSR_SPEED_10_HD == Value)
         {
            Status = PHY_LINK_STATUS_SPEED_10M | PHY_LINK_STATUS_MODE_HALF;
         } 
         
         if (PHY_PHYSCSR_SPEED_10_FD == Value)
         {
            Status = PHY_LINK_STATUS_SPEED_10M | PHY_LINK_STATUS_MODE_FULL;
         }
         
         if (PHY_PHYSCSR_SPEED_100_HD == Value)
         {
            Status = PHY_LINK_STATUS_SPEED_100M | PHY_LINK_STATUS_MODE_HALF;
         } 
         
         if (PHY_PHYSCSR_SPEED_100_FD == Value)
         {
            Status = PHY_LINK_STATUS_SPEED_100M | PHY_LINK_STATUS_MODE_FULL;
         }
      }

      pPHY->Status = Status;
   }
   else
   {      
      Status = pPHY->Status;
   } /* end if (LinkStatus != OldLinkStatus) */
   
   return(Status);
} /* GetLinkStatus */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

ETH_PHY_DRIVER PHY_SMSC8720 =
{
   Init,
   GetLinkStatus,
   NULL,
   NULL,
   0,
   0
};

/*** EOF ***/
