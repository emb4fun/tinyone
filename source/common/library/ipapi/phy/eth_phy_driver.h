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
*  05.01.2016  mifi  Rename IP_PHY_DRIVER to ETH_PHY_DRIVER.
**************************************************************************/
#if !defined(__ETH_PHY_DRIVER_H__)
#define __ETH_PHY_DRIVER_H__

/**************************************************************************
*  Includes
**************************************************************************/
#include <stdint.h>

/**************************************************************************
*  Global Definitions
**************************************************************************/

/*
 * Defines for the Status value of GetLinkStatus
 */
#define PHY_LINK_STATUS_NO_LINK     0x00
#define PHY_LINK_STATUS_SPEED_10M   0x01
#define PHY_LINK_STATUS_SPEED_100M  0x02
#define PHY_LINK_STATUS_MODE_HALF   0x04
#define PHY_LINK_STATUS_MODE_FULL   0x08

#define PHY_OK          0
#define PHY_ERROR       1

typedef uint16_t PHY_RESULT;

struct _eth_phy_driver_;

typedef void      (*phy_init_fn)(struct _eth_phy_driver_ *pPHY);
typedef uint8_t   (*phy_get_link_status_fn)(struct _eth_phy_driver_ *pPHY);
typedef uint16_t  (*phy_read_reg_fn)(struct _eth_phy_driver_ *pPHY, uint16_t Reg);
typedef void      (*phy_write_reg_fn)(struct _eth_phy_driver_ *pPHY, uint16_t Reg, uint16_t Value);

typedef struct _eth_phy_driver_
{
   phy_init_fn             Init;   
   phy_get_link_status_fn  GetLinkStatus;
   phy_read_reg_fn         ReadReg; 
   phy_write_reg_fn        WriteReg;
   uint8_t                 Address;
   uint8_t                 Status;   
} ETH_PHY_DRIVER;


/*
 * Define some PHY-IDs which are supported
 */
#define PHY_ID_MASK        0xFFFFFFF0 
#define PHY_ID_DP83848C    (0x20005C90 & PHY_ID_MASK)
#define PHY_ID_KSZ8721BL   (0x00221610 & PHY_ID_MASK)
#define PHY_ID_KSZ8081RN   (0x00221560 & PHY_ID_MASK)
#define PHY_ID_SMCS8720    (0x0007C0F0 & PHY_ID_MASK)
#define PHY_ID_LAN8742A    (0x0007C130 & PHY_ID_MASK)

/**************************************************************************
*  Macro Definitions
**************************************************************************/

/**************************************************************************
*  Funtions Definitions
**************************************************************************/
extern ETH_PHY_DRIVER PHY_DP83848;
extern ETH_PHY_DRIVER PHY_KSZ8721;
extern ETH_PHY_DRIVER PHY_KSZ8081;
extern ETH_PHY_DRIVER PHY_SMSC8720;
extern ETH_PHY_DRIVER PHY_LAN8742;

#endif /* !__ETH_PHY_DRIVER_H__ */

/*** EOF ***/
