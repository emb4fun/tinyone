/**************************************************************************
*  Copyright (c) 2018 by Michael Fischer (www.emb4fun.de).
*  All rights reserved.

*  The source is partial based on the TI example.
*  Therefore partial Copyright(c) 2010 Texas Instruments Incorporated
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
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
*  17.06.2018  mifi  First Version.
*  23.12.2018  mifi  New version based on previous NutOS port.
*  14.07.2019  mifi  Corrected the internal switch initialization. 
**************************************************************************/

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#define __ETHERNETIF_C__

/*=======================================================================*/
/*  Include                                                              */
/*=======================================================================*/
#include <string.h>

#include "tal.h"
#include "tcts.h"

#include "ethernetif.h"
#include "project.h"

#include "lwip\opt.h"
#include "lwip\tcpip.h"
#include "netif\etharp.h"
#include "lwip\stats.h"
#include "lwip\igmp.h"

#include "arch/sys_arch.h"

#include "soc_AM335x.h"
#include "cpsw.h"
#include "mdio.h"
#include "interrupt.h"
#include "phy.h" 
#include "cache.h"

/* Forward declarations. */
static void ethernetif_input (void *arg);

/*=======================================================================*/
/*  Extern                                                               */
/*=======================================================================*/

extern void CPSWPinMuxSetup (void);
extern void CPSWClkEnable (void);
extern void EVMMACAddrGet (unsigned int addrIdx, unsigned char *macAddr);
extern void EVMPortMIIModeSelect (void);

/*=======================================================================*/
/*  All Structures and Common Constants                                  */
/*=======================================================================*/

/*
 * Some generic port defines
 */
 
#define PORT_1       0x0
#define PORT_2       0x1
#define PORT_0_MASK  0x1
#define PORT_1_MASK  0x2
#define PORT_2_MASK  0x4


/*
 * Buffer descriptor defines
 */

#define SIZE_CPPI_RAM                     0x2000   /* CPPI RAM size in bytes */

#define NUM_BUFF_RXDESCS                  ((SIZE_CPPI_RAM >> 1) / sizeof(cpdma_bd_t))
#define NUM_BUFF_TXDESCS                  ((SIZE_CPPI_RAM >> 1) / sizeof(cpdma_bd_t))

#define LAST_BUFF_RXDESCS                 (NUM_BUFF_RXDESCS-1)     
#define LAST_BUFF_TXDESCS                 (NUM_BUFF_TXDESCS-1)

#define CPDMA_BUF_DESC_OWNER              0x20000000
#define CPDMA_BUF_DESC_SOP                0x80000000
#define CPDMA_BUF_DESC_EOP                0x40000000
#define CPDMA_BUF_DESC_EOQ                0x10000000
#define CPDMA_BUF_DESC_FROM_PORT          0x70000
#define CPDMA_BUF_DESC_FROM_PORT_SHIFT    16
#define CPDMA_BUF_DESC_TO_PORT(port)      ((port << 16) | 0x100000)
#define CPDMA_BD_LEN_MASK                 0xFFFF
#define CPDMA_BD_PKTLEN_MASK              0xFFFF


/* 
 * MDIO input and output frequencies in Hz 
 */
#define MDIO_FREQ_INPUT                   125000000
#define MDIO_FREQ_OUTPUT                  1000000


/*
 * Some ethernet length defines
 */
#define ETH_MAX_BUFF_LEN   1536
#define MIN_PKT_LEN        60


/*
 * Multicast defines
 */

#define MAX_ALE_ENTRIES                         (1024)
#define ALE_ENTRY_NUM_WORDS                     (3)

#define ENTRY_TYPE                               0x30
#define ENTRY_TYPE_IDX                           7
#define ENTRY_FREE                               0
 
#define ALE_ENTRY_UCAST                          0x10
#define ALE_ENTRY_MCAST                          0xD0
#define ALE_ENTRY_OUI                            (0x80)
#define ALE_ENTRY_ADDR                           (0x10)
#define ALE_UCAST_ENTRY_TYPE                     7
//#define ALE_UCAST_TYPE_MASK                      (0xC0)
//#define ALE_UCAST_TYPE_SHIFT                     (6)
//#define ALE_UCAST_TYPE_PERSISTANT                (0x00)
//#define ALE_UCAST_TYPE_UNTOUCHED                 (0x40)
//#define ALE_UCAST_TYPE_OUI                       (0x80)
//#define ALE_UCAST_TYPE_TOUCHED                   (0xC0)
#define ALE_UCAST_ENTRY_DLR_PORT_BLK_SEC         8
//#define ALE_UCAST_ENTRY_DLR_BLK_SEC_MASK         (0x03)
#define ALE_UCAST_ENTRY_PORT_SHIFT               2
#define ALE_MCAST_ENTRY_TYPE_FWD_STATE           7
//#define ALE_MCAST_ENTRY_TYPE_FWD_STATE_SHIFT     (6)
#define ALE_MCAST_ENTRY_PORTMASK_SUP             8
//#define ALE_MCAST_ENTRY_SUPER_MASK               (0x02)
//#define ALE_MCAST_ENTRY_SUPER_SHIFT              (1)
//#define ALE_MCAST_ENTRY_PORT_MASK                (0x1C)
#define ALE_MCAST_ENTRY_PORTMASK_SHIFT           2


/*
 * Buffer descriptor data structure 
 */
typedef struct _cpdma_bd_
{
   volatile struct _cpdma_bd_ *next;
   volatile uint32_t           bufptr;
   volatile uint32_t           bufoff_len;
   volatile uint32_t           flags_pktlen;
} cpdma_bd_t;


/*
 * Slave port information
 */
 
struct cpswportif 
{
   uint8_t  eth_addr[6];
} cpswportif;
 
struct cpswport
{ 
   uint32_t port_base;
   uint32_t sliver_base;
   uint32_t phy_addr;
  
   /* The PHY is capable of GitaBit or Not */
   uint32_t phy_gbps;
   
   struct cpswportif  cpswif;
} cpswport;

/*
 * CPSW instance information 
 */
 
typedef struct _cpswinst_
{
   /* Base addresses */
   uint32_t ss_base;
   uint32_t mdio_base;
   uint32_t wrpr_base;
   uint32_t ale_base;
   uint32_t cpdma_base;
   uint32_t cppi_ram_base;
   uint32_t host_port_base;

   /* Slave port information */ 
   struct cpswport port[2];

   /* The tx/rx channels for the interface */
   uint32_t next_rx_index;
   uint32_t tx_running;
   uint32_t tx_read_index;
   uint32_t tx_write_index;
} cpswinst_t;

/*************************************************************************/

#define IFNAME0 'e'
#define IFNAME1 'n'

#define NETIF_EVENT_RX              0x0001
#define NETIF_EVENT_TX              0x0002
#define NETIF_EVENT_LINK_UP         0x0004   /* Not used */
#define NETIF_EVENT_LINK_DOWN       0x0008   /* Not used */
#define NETIF_EVENT_RX_OVERRUN      0x0010   /* Not used */
#define NETIF_EVENT_TX_UNDERRUN     0x0020   /* Not used */

#define NETIF_EVENT_WAIT_ALL        (NETIF_EVENT_RX         | \
                                     NETIF_EVENT_TX         | \
                                     NETIF_EVENT_LINK_UP    | \
                                     NETIF_EVENT_LINK_DOWN  | \
                                     NETIF_EVENT_RX_OVERRUN | \
                                     NETIF_EVENT_TX_UNDERRUN)

#define LINK_TMR_INTERVAL           1000

#define PHY_LINK_STATUS_NO_LINK     0x00
#define PHY_LINK_STATUS_SPEED_10M   0x01
#define PHY_LINK_STATUS_SPEED_100M  0x02
#define PHY_LINK_STATUS_MODE_HALF   0x04
#define PHY_LINK_STATUS_MODE_FULL   0x08

/*=======================================================================*/
/*  Definition of all local Data                                         */
/*=======================================================================*/

/*
 * This is the stack for the ReceiverTask
 */
static OS_STACK (ReceiverStack, TASK_IP_RX_STK_SIZE);
static OS_TCB      tcb_task_rx;

static OS_EVENT    NetIfEvent;

static uint8_t     OldLinkStatus = PHY_LINK_STATUS_NO_LINK; 

/* Local DMA Descriptors */
static cpdma_bd_t *RxDesc = (cpdma_bd_t*)(SOC_CPSW_CPPI_RAM_REGS);
static cpdma_bd_t *TxDesc = (cpdma_bd_t*)(SOC_CPSW_CPPI_RAM_REGS + (SIZE_CPPI_RAM >> 1));

/* Local DMA buffers. */
static uint32_t    RxBuf[NUM_BUFF_RXDESCS][ETH_MAX_BUFF_LEN>>2];
static uint32_t    TxBuf[NUM_BUFF_TXDESCS][ETH_MAX_BUFF_LEN>>2];

static void       *RxMboxBuffer[NUM_BUFF_RXDESCS];
static OS_MBOX     RxMbox;

/* Defining set for all the instances */
static cpswinst_t cpsw_inst_data;

static uint32_t MulticastFilterList[MAX_ALE_ENTRIES];

static char     Hostname[16];

/*=======================================================================*/
/*  Definition of all local Procedures                                   */
/*=======================================================================*/

/* Forward prototypes */
static void CPSWCore0RxIsr (void);
static void CPSWCore0TxIsr (void);

#if 0
static void cpsw_ale_dump_table (cpswinst_t *cpswinst)
{
   int i;
   uint32_t ale_entry[3];
   
   for (i = 0; i < MAX_ALE_ENTRIES; i++)
   {
      CPSWALETableEntryGet(cpswinst->ale_base, (uint32_t)i, ale_entry);
      
      if (ale_entry[0] || ale_entry[1] || ale_entry[2]) 
      {
         term_printf("ALE[%4u] %08x %08x %08x ", i, ale_entry[0],
            ale_entry[1], ale_entry[2]);
         term_printf("mac: %02x:%02x:%02x:%02x:%02x:%02x ",
            (ale_entry[1] >> 8) & 0xFF,
            (ale_entry[1] >> 0) & 0xFF,
            (ale_entry[0] >>24) & 0xFF,
            (ale_entry[0] >>16) & 0xFF,
            (ale_entry[0] >> 8) & 0xFF,
            (ale_entry[0] >> 0) & 0xFF);
         term_printf(((ale_entry[1] >> 8) & 1) ? "mcast " : "ucast ");
         term_printf("type: %u ", (ale_entry[1] >> 28) & 3);
         term_printf("port: %u ", (ale_entry[2] >> 2) & 7);
         term_printf("\n");
      }
   }
   term_printf("\n");
}
#endif

/**
* Function to setup the instance parameters inside the interface
* @param  cpswif  The interface structure pointer
* @return None. 
*/
static void cpsw_inst_config (struct cpswportif *cpswif)
{
   cpswinst_t *cpswinst = &cpsw_inst_data;

   (void)cpswif;

   /**
    * Code is added for only instance 0. If more instances
    * are there, assign base addresses and phy info here
    */
   cpswinst->ss_base                  = SOC_CPSW_SS_REGS;
   cpswinst->mdio_base                = SOC_CPSW_MDIO_REGS;
   cpswinst->wrpr_base                = SOC_CPSW_WR_REGS;
   cpswinst->cpdma_base               = SOC_CPSW_CPDMA_REGS;
   cpswinst->ale_base                 = SOC_CPSW_ALE_REGS;
   cpswinst->cppi_ram_base            = SOC_CPSW_CPPI_RAM_REGS;
   cpswinst->host_port_base           = SOC_CPSW_PORT_0_REGS;
    
   cpswinst->port[PORT_1].port_base   = SOC_CPSW_PORT_1_REGS;
   cpswinst->port[PORT_1].sliver_base = SOC_CPSW_SLIVER_1_REGS;
   cpswinst->port[PORT_1].phy_addr    = 0;
   cpswinst->port[PORT_1].phy_gbps    = FALSE;

   cpswinst->port[PORT_2].port_base   = SOC_CPSW_PORT_2_REGS;
   cpswinst->port[PORT_2].sliver_base = SOC_CPSW_SLIVER_2_REGS;
}

/**
 * Gives the index of the ALE entry which is free
 * @param  cpswinst  The CPSW instance structure pointer
 *
 * @return index of the ALE entry which is free
 *         ERR_VAL if entry not found
 */
static int cpsw_ale_entry_match_free (cpswinst_t *cpswinst)
{
   uint32_t ale_entry[ALE_ENTRY_NUM_WORDS];
   int32_t  idx;

   /* Check which ALE entry is free starting from 0th entry */
   for (idx = 0; idx < MAX_ALE_ENTRIES; idx++) 
   {
      CPSWALETableEntryGet(cpswinst->ale_base, (uint32_t)idx, ale_entry);

      /* Break if the table entry is free */
      if (((*(((uint8_t *)ale_entry) + ENTRY_TYPE_IDX)) & ENTRY_TYPE) == ENTRY_FREE) 
      {
         return idx;
      }
   }

   return ERR_VAL;
}

/**
 * Sets a unicast entry in the ALE table.
 * @param cpswinst   The CPSW instance structure pointer
 * @param port_num   The slave port number
 * @param eth_addr   Ethernet address
 *
 * @return None
 */
static void cpsw_ale_unicastentry_set (cpswinst_t *cpswinst, uint32_t port_num, uint8_t *eth_addr)
{
   volatile uint32_t cnt;
   volatile int32_t idx;
   uint32_t ale_entry[ALE_ENTRY_NUM_WORDS] = {0, 0, 0};

   for(cnt = 0; cnt < ETHARP_HWADDR_LEN; cnt++) 
   {
      *(((uint8_t *)ale_entry) + cnt) = eth_addr[(ETHARP_HWADDR_LEN - cnt) -1];
   }

   *(((uint8_t *)ale_entry) + ALE_UCAST_ENTRY_TYPE) = ALE_ENTRY_UCAST;
   *(((uint8_t *)ale_entry) + ALE_UCAST_ENTRY_DLR_PORT_BLK_SEC) = (uint8_t)(port_num << ALE_UCAST_ENTRY_PORT_SHIFT);

   idx = cpsw_ale_entry_match_free(cpswinst);

   if (idx < MAX_ALE_ENTRIES ) 
   {
      CPSWALETableEntrySet(cpswinst->ale_base, (uint32_t)idx, ale_entry);
   }
}

/**
 * Sets a multicast entry in the ALE table
 * @param cpswinst   The CPSW instance structure pointer
 * @param portmask   The port mask for the port number
 * @param eth_addr   Ethernet Address
 *
 * @return index of the ALE entry added
 *         ERR_VAL if table entry is not free
 */
static void cpsw_ale_multicastentry_set (cpswinst_t *cpswinst, uint32_t portmask, uint8_t *eth_addr)
{
   volatile uint32_t cnt;
   volatile int32_t  idx;
   uint32_t ale_entry[ALE_ENTRY_NUM_WORDS] = {0, 0, 0};

   idx = cpsw_ale_entry_match_free(cpswinst);
   if (idx < MAX_ALE_ENTRIES )
   {
      for (cnt = 0; cnt < ETHARP_HWADDR_LEN; cnt++) 
      {
         *(((uint8_t *)ale_entry) + cnt) = eth_addr[(ETHARP_HWADDR_LEN - cnt) -1];
      }

      *(((uint8_t *)ale_entry) + ALE_MCAST_ENTRY_TYPE_FWD_STATE) = ALE_ENTRY_MCAST;
      *(((uint8_t *)ale_entry) + ALE_MCAST_ENTRY_PORTMASK_SUP) |= (uint8_t)(portmask << ALE_MCAST_ENTRY_PORTMASK_SHIFT);

      CPSWALETableEntrySet(cpswinst->ale_base, (uint32_t)idx, ale_entry);
   }
}

static void cpsw_ale_remove_all_entries (cpswinst_t *cpswinst, uint32_t start)
{
   uint32_t idx;
   uint32_t ale_entry[ALE_ENTRY_NUM_WORDS] = {0, 0, 0};

   for (idx=start; idx<MAX_ALE_ENTRIES; idx++)
   {
      CPSWALETableEntrySet(cpswinst->ale_base, idx, ale_entry);
   }
}

static int cpsw_autoneg_config (uint32_t port_num)
{
   cpswinst_t *cpswinst = &cpsw_inst_data;
   uint16_t adv_val, gig_adv_val;

   /* We advertise for 10/100 Mbps both half and full duplex */
   adv_val = (PHY_100BTX | PHY_100BTX_FD | PHY_10BT | PHY_10BT_FD);

   /**
    * Not all the PHYs can operate at 1000 Mbps. So advertise only
    * if the PHY is capable
    */
   if(TRUE == cpswinst->port[port_num -1].phy_gbps)
   {
      gig_adv_val = PHY_1000BT_FD;
   } 
   else 
   {
      gig_adv_val = 0;
   }

   TAL_DEBUG(TAL_DBG_NIC, "NIC: Performing Auto-Negotiation...");

   /**
    * Now start Autonegotiation. PHY will talk to its partner
    * and give us what the partner can handle
    */
   if (FALSE == PhyAutoNegotiate(cpswinst->mdio_base, cpswinst->port[port_num -1].phy_addr, &adv_val, &gig_adv_val))
   {
      return(ERR_CONN);                                    
   }                                 

   return(ERR_OK);
}

static int cpsw_phylink_config (struct cpswportif * cpswif, uint32_t slv_port_num)
{
   cpswinst_t *cpswinst = &cpsw_inst_data;
   int         err;

   (void)cpswif;
  

   /* Check if ethernet PHY is present or not */
   if(0 == (MDIOPhyAliveStatusGet(cpswinst->mdio_base) & (1u << cpswinst->port[slv_port_num - 1].phy_addr)))
   {
      TAL_DEBUG(TAL_DBG_NIC, "NIC: No PHY found at address %d for port %d.",
                cpswinst->port[slv_port_num - 1].phy_addr, slv_port_num);
      return ERR_CONN;
   }

   TAL_DEBUG(TAL_DBG_NIC, "NIC: PHY found at address %d for port %d.",
             cpswinst->port[slv_port_num - 1].phy_addr, slv_port_num);

   /**
    * PHY is alive. So autonegotiate and get the speed and duplex
    * parameters, set it in the sliver
    */
   err = cpsw_autoneg_config(slv_port_num);
   (void)err;
   
   return(ERR_OK);
}

/**
 * This function intializes the CPDMA.
 * The CPPI RAM will be initialized for transmit and receive
 * buffer descriptor rings.
 *
 * @param cpswinst   The CPSW instance structure pointer
 * @return None
 */
static void cpsw_cpdma_init (cpswinst_t *cpswinst)
{
   uint32_t i;
   
   for(i=0; i<NUM_BUFF_TXDESCS; i++)
   {
      TxDesc[i].next         = NULL;
      TxDesc[i].bufptr       = (uint32_t)&TxBuf[i][0]; 
      TxDesc[i].bufoff_len   = ETH_MAX_BUFF_LEN;
      TxDesc[i].flags_pktlen = 0;
   } 
   TxDesc[LAST_BUFF_TXDESCS].next = NULL;

   cpswinst->tx_running     = 0;
   cpswinst->tx_read_index  = 0;
   cpswinst->tx_write_index = 0;


   for(i=0; i<NUM_BUFF_RXDESCS; i++)
   {
      RxDesc[i].next         = &RxDesc[i+1];   
      RxDesc[i].bufptr       = (uint32_t)&RxBuf[i][0]; 
      RxDesc[i].bufoff_len   = ETH_MAX_BUFF_LEN;
      RxDesc[i].flags_pktlen = CPDMA_BUF_DESC_OWNER;
   } 
   RxDesc[LAST_BUFF_RXDESCS].next = NULL;

   cpswinst->next_rx_index  = 0;


   CPSWCPDMARxHdrDescPtrWrite(cpswinst->cpdma_base, (uint32_t)&RxDesc[0], 0);
}

/**
 * In this function, the hardware should be initialized.
 * Called from cpsw_init().
 *
 * @param cpswportif  The CPSW port interface structure pointer
 * @return None
 */
static void cpsw_inst_init (struct cpswportif *cpswif)
{
   cpswinst_t *cpswinst = &cpsw_inst_data;

   /* Reset the different modules */
   CPSWSSReset(cpswinst->ss_base);
   CPSWWrReset(cpswinst->wrpr_base);
   CPSWSlReset(cpswinst->port[PORT_1].sliver_base);
   CPSWSlReset(cpswinst->port[PORT_2].sliver_base);
   CPSWCPDMAReset(cpswinst->cpdma_base);

   /* Initialize MDIO */
   MDIOInit(cpswinst->mdio_base, MDIO_FREQ_INPUT, MDIO_FREQ_OUTPUT);
   OS_TimeDly(2);

   CPSWALEInit(cpswinst->ale_base);

   /* Set the port 0, 1 to FORWARD and DISABLE port 2 */
   CPSWALEPortStateSet(cpswinst->ale_base, 0, CPSW_ALE_PORT_STATE_FWD);
   CPSWALEPortStateSet(cpswinst->ale_base, 1, CPSW_ALE_PORT_STATE_FWD);
   CPSWALEPortStateSet(cpswinst->ale_base, 2, CPSW_ALE_PORT_STATE_DISABLED);

   /* Clear ALE first */
   cpsw_ale_remove_all_entries(cpswinst, 0);

   /* For normal CPSW switch mode, set multicast entry. */
   uint8_t bcast_addr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   cpsw_ale_multicastentry_set(cpswinst, (PORT_0_MASK | PORT_1_MASK | PORT_2_MASK), bcast_addr);
   cpsw_ale_unicastentry_set(cpswinst, 0, cpswif->eth_addr);

   /* Set the ethernet address for both the ports */
   CPSWPortSrcAddrSet(cpswinst->port[0].port_base, cpswif->eth_addr);
   CPSWPortSrcAddrSet(cpswinst->port[1].port_base, cpswif->eth_addr);

   /* Enable the statistics. Lets see in case we come across any issues */
   CPSWStatisticsEnable(cpswinst->ss_base);

   /* Initialize the buffer descriptors for CPDMA */
   cpsw_cpdma_init(cpswinst);

   /* Acknowledge receive and transmit interrupts for proper interrupt pulsing*/
   CPSWCPDMAEndOfIntVectorWrite(cpswinst->cpdma_base, CPSW_EOI_TX_PULSE);
   CPSWCPDMAEndOfIntVectorWrite(cpswinst->cpdma_base, CPSW_EOI_RX_PULSE);

   /* Enable the transmission and reception */
   CPSWCPDMATxEnable(cpswinst->cpdma_base);
   CPSWCPDMARxEnable(cpswinst->cpdma_base);

   /* Enable the interrupts for channel 0 and for control core 0 */
   CPSWCPDMATxIntEnable(cpswinst->cpdma_base, 0);
   CPSWWrCoreIntEnable(cpswinst->wrpr_base, 0, 0, CPSW_CORE_INT_TX_PULSE);

   CPSWCPDMARxIntEnable(cpswinst->cpdma_base, 0);
   CPSWWrCoreIntEnable(cpswinst->wrpr_base, 0, 0, CPSW_CORE_INT_RX_PULSE);
}

/**
 * Initializes the CPSW port
 * @param netif   The cpsw interface
 *
 * @return ERR_OK    if port initialization is successful
 *                   an error status if failed
 */
static int cpsw_port_init (struct netif *netif)
{
   struct cpswportif *cpswif = &cpsw_inst_data.port[0].cpswif;
   int err;
   unsigned char MACAddress[6];
   
   (void)netif;
   
   /* Get the MAC address */
   EVMMACAddrGet(0, MACAddress);    
   cpswif->eth_addr[0] = MACAddress[5];
   cpswif->eth_addr[1] = MACAddress[4];
   cpswif->eth_addr[2] = MACAddress[3];
   cpswif->eth_addr[3] = MACAddress[2];
   cpswif->eth_addr[4] = MACAddress[1];
   cpswif->eth_addr[5] = MACAddress[0];
   
   /* 
    * Initialize the hardware
    */
   CPSWPinMuxSetup();
   CPSWClkEnable();
   
   /* Chip configuration RGMII selection */
   EVMPortMIIModeSelect();

   /*
    * Set up the ARM Interrupt Controller for generating timer interrupt
    */
   /* Register the Receive ISR for Core 0 */
   IntRegister(SYS_INT_3PGSWRXINT0, CPSWCore0RxIsr);
  
   /* Register the Transmit ISR for Core 0 */
   IntRegister(SYS_INT_3PGSWTXINT0, CPSWCore0TxIsr);
   
   /* Set the priority */
   IntPrioritySet(SYS_INT_3PGSWTXINT0, 0, AINTC_HOSTINT_ROUTE_IRQ);
   IntPrioritySet(SYS_INT_3PGSWRXINT0, 0, AINTC_HOSTINT_ROUTE_IRQ);
   
   /**
    * Initialize an instance only once. Port initialization will be
    * done separately.
    */
   cpsw_inst_config(cpswif);
   cpsw_inst_init(cpswif);

   err = cpsw_phylink_config(cpswif, 1);

   return(err);
} /* cpsw_port_init */

/*
 * Handler for Receive interrupt.
 */
static void cpsw_rx_inthandler (void)
{
   uint32_t             i;
   uint32_t             start;
   uint32_t             tot_len;
   cpswinst_t          *cpswinst = &cpsw_inst_data;
   volatile cpdma_bd_t *curr_bd;

   start = cpswinst->next_rx_index;

  /*
   * Process the receive buffer descriptors. When the DMA completes
   * reception, OWNERSHIP flag will be cleared.
   */
   for(i=start; i<NUM_BUFF_RXDESCS; i++)
   {
      curr_bd = &RxDesc[i];
      
      if ((curr_bd->flags_pktlen & CPDMA_BUF_DESC_OWNER) != CPDMA_BUF_DESC_OWNER)
      {
         /* Save the index for the next start of the rx_inthandler */
         cpswinst->next_rx_index = i + 1;
         
         /* Get the total length of the packet */
         tot_len = curr_bd->flags_pktlen & CPDMA_BD_PKTLEN_MASK;

         /*
          * Invalidate the cache lines of the pbuf including payload. Because
          * the memory contents got changed by DMA.
          */
         CacheDataInvalidateBuff((uint32_t)curr_bd->bufptr, ETH_MAX_BUFF_LEN);

         /* Add RxDesc to the mailbox */
         if (OS_MboxPostFromInt(&RxMbox, (void*)curr_bd) != OS_RC_OK) {
            // Ignore error
            //while(1) {}
         }

         /* Acknowledge that this packet is processed */
         curr_bd->bufoff_len   = ETH_MAX_BUFF_LEN;
         curr_bd->flags_pktlen = CPDMA_BUF_DESC_OWNER | tot_len;  /* tot_len is used for NicGetPacket */
         CPSWCPDMARxCPWrite(cpswinst->cpdma_base, 0, (unsigned int)curr_bd);
         
         /* Check if this is the last packet */
         if (LAST_BUFF_RXDESCS == i)
         {
            /* Set the new start */
            cpswinst->next_rx_index = 0;
            CPSWCPDMARxHdrDescPtrWrite(cpswinst->cpdma_base, (uint32_t)&RxDesc[0], 0);
         }
      }
      else
      {
         break;
      }
   }

   /* Send RX event */
   OS_EventSetFromInt(&NetIfEvent, NETIF_EVENT_RX);
  
   /* Acknowledge interrupt */
   CPSWCPDMAEndOfIntVectorWrite(cpswinst->cpdma_base, CPSW_EOI_RX_PULSE);
}

/**
 * Handler for CPSW Transmit interrupt
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return none
 */
static void cpsw_tx_inthandler (void)
{
   cpswinst_t          *cpswinst = &cpsw_inst_data;
   volatile cpdma_bd_t *curr_bd;

   curr_bd = &TxDesc[cpswinst->tx_read_index];
   if ((curr_bd->flags_pktlen & 0xF0000000) != 0xD0000000)
   {
      while(1) {}
   }

   /* Acknowledge CPSW */
   curr_bd->flags_pktlen = 0;
   CPSWCPDMATxCPWrite(cpswinst->cpdma_base, 0, (uint32_t)curr_bd);


   /* Check if a new packet is available to send */
   cpswinst->tx_read_index++;
   if (cpswinst->tx_read_index > LAST_BUFF_TXDESCS)
   {
      cpswinst->tx_read_index = 0;
   }

   curr_bd = &TxDesc[cpswinst->tx_read_index];
   if (curr_bd->flags_pktlen & CPDMA_BUF_DESC_OWNER)
   {
      /* Start new packet */
      CPSWCPDMATxHdrDescPtrWrite(cpswinst->cpdma_base, (uint32_t)curr_bd, 0);
   }
   else
   {
      /* Ups, no packet available */
      cpswinst->tx_running = 0;
   }

   CPSWCPDMAEndOfIntVectorWrite(cpswinst->cpdma_base, CPSW_EOI_TX_PULSE);
}

/*
** Interrupt Handler for Core 0 Receive interrupt
*/
static void CPSWCore0RxIsr (void)
{
   TAL_CPU_IRQ_ENTER();
   cpsw_rx_inthandler();
   TAL_CPU_IRQ_EXIT();
}

/*
** Interrupt Handler for Core 0 Transmit interrupt
*/
static void CPSWCore0TxIsr (void)
{
   TAL_CPU_IRQ_ENTER();
   cpsw_tx_inthandler();
   TAL_CPU_IRQ_EXIT();   
}


/*
 * Return the Link status
 */
static uint8_t cpsw_GetLinkStatus (uint8_t OldLinkStat)
{
   static uint32_t  OldLink = FALSE;
   uint32_t         Link;
   
   cpswinst_t      *cpswinst = &cpsw_inst_data;
   uint32_t         slv_port_num = 1;
   uint32_t         LinkStatus = PHY_LINK_STATUS_NO_LINK;
   
   uint16_t adv_val, partnr_ablty, gbps_partnr_ablty;
   uint32_t auto_stat, transfer_mode = 0;

   Link = PhyLinkStatusGet(cpswinst->mdio_base, cpswinst->port[slv_port_num - 1].phy_addr, 3);
   if (Link != OldLink)
   {
      OldLink = Link;
      
      if (FALSE == Link)
      {
         LinkStatus = PHY_LINK_STATUS_NO_LINK;   
      }
      else
      {
         /**
          * Not all the PHYs can operate at 1000 Mbps. So advertise only
          * if the PHY is capable
          */
         if(TRUE == cpswinst->port[slv_port_num -1].phy_gbps)
         {
            partnr_ablty = TRUE;
            gbps_partnr_ablty = TRUE;
         } 
         else 
         {
            partnr_ablty = TRUE;
            gbps_partnr_ablty = FALSE;
         }

         /* We advertise for 10/100 Mbps both half and full duplex */
         adv_val = (PHY_100BTX | PHY_100BTX_FD | PHY_10BT | PHY_10BT_FD);
  
         auto_stat = PhyAutoNegStatusGet(cpswinst->mdio_base,
                                         cpswinst->port[slv_port_num -1].phy_addr);
         if(TRUE == auto_stat) 
         {
            TAL_DEBUG(TAL_DBG_NIC, "NIC: Auto-Negotiation Successful.");

            /* Get what the partner supports */
            PhyPartnerAbilityGet(cpswinst->mdio_base,
                                 cpswinst->port[slv_port_num -1].phy_addr,
                                 &partnr_ablty, &gbps_partnr_ablty);
            if(gbps_partnr_ablty & PHY_LINK_PARTNER_1000BT_FD)
            {
               TAL_DEBUG(TAL_DBG_NIC, "NIC: Transfer Mode : 1000 Mbps.");
               transfer_mode = CPSW_SLIVER_GIG_FULL_DUPLEX;
            } 
            else 
            {
               if ((adv_val & partnr_ablty) & PHY_100BTX_FD) 
               {
                  TAL_DEBUG(TAL_DBG_NIC, "NIC: Transfer Mode : 100 Mbps Full Duplex.");
                  transfer_mode = CPSW_SLIVER_NON_GIG_FULL_DUPLEX;
                  LinkStatus = PHY_LINK_STATUS_SPEED_100M | PHY_LINK_STATUS_MODE_FULL;
               } 
               else if ((adv_val & partnr_ablty) & PHY_100BTX) {
                  TAL_DEBUG(TAL_DBG_NIC, "NIC: Transfer Mode : 100 Mbps Half Duplex.");
                  transfer_mode = CPSW_SLIVER_NON_GIG_HALF_DUPLEX;
                  LinkStatus = PHY_LINK_STATUS_SPEED_100M | PHY_LINK_STATUS_MODE_HALF;
               } 
               else if ((adv_val & partnr_ablty) & PHY_10BT_FD) {
                  TAL_DEBUG(TAL_DBG_NIC, "NIC: Transfer Mode : 10 Mbps Full Duplex.");
                  transfer_mode = CPSW_SLIVER_INBAND | CPSW_SLIVER_NON_GIG_FULL_DUPLEX;
                  LinkStatus = PHY_LINK_STATUS_SPEED_10M | PHY_LINK_STATUS_MODE_FULL;
               } 
               else if ((adv_val & partnr_ablty) & PHY_10BT) {
                  TAL_DEBUG(TAL_DBG_NIC, "NIC: Transfer Mode : 10 Mbps Half Duplex.");
                  transfer_mode = CPSW_SLIVER_INBAND | CPSW_SLIVER_NON_GIG_HALF_DUPLEX;
                  LinkStatus = PHY_LINK_STATUS_SPEED_10M | PHY_LINK_STATUS_MODE_HALF;
               } 
               else 
               {
                  TAL_DEBUG(TAL_DBG_NIC, "NIC: No Valid Transfer Mode is detected.");
               }
            }

            /**
             * Set the Sliver with the negotiation results if autonegotiation
             * is successful
             */
            CPSWSlTransferModeSet(cpswinst->port[slv_port_num - 1].sliver_base,
                                  transfer_mode);

            CPSWSlRGMIIEnable(cpswinst->port[slv_port_num - 1].sliver_base);
         }
         else
         {
            TAL_DEBUG(TAL_DBG_NIC, "NIC: Auto-Negotiation Not Successful.");
         }
      }         
   }
   else
   {
      LinkStatus = OldLinkStat;   
   }

   return((uint8_t)LinkStatus);
} /* cpsw_GetLinkStatus */

void cpsw_GetMACAddress (uint8_t *pAddress)
{
   unsigned char MACAddress[6];
   
   /* Get the MAC address */
   EVMMACAddrGet(0, MACAddress);    

   pAddress[0] = MACAddress[5];
   pAddress[1] = MACAddress[4];
   pAddress[2] = MACAddress[3];
   pAddress[3] = MACAddress[2];
   pAddress[4] = MACAddress[1];
   pAddress[5] = MACAddress[0];
   
}

/*************************************************************************/

/*
 *  MulticastAddFilter
 */
static int MulticastAddFilter (uint32_t address)
{
   int          rc = -1;
   int          i;
   uint8_t      mac[6];
   cpswinst_t  *cpswinst = &cpsw_inst_data;
   
   /*
    * Check if address is still in list
    */
   for (i=0; i<MAX_ALE_ENTRIES; i++)
   {
      if (MulticastFilterList[i] == address)
      {
         return(-1);
      }
   }    
   
   /*
    * Check for a free entry
    */
   for (i=0; i<MAX_ALE_ENTRIES; i++)
   {
      if (0 == MulticastFilterList[i])
      {
         rc = 0;
         
         /* Save entry */
         MulticastFilterList[i] = address;
         
         /* Create MAC address */
         mac[0] = 0x01;
         mac[1] = 0x00;
         mac[2] = 0x5E;
         mac[3] = ((uint8_t *) &address)[1] & 0x7f;
         mac[4] = ((uint8_t *) &address)[2];
         mac[5] = ((uint8_t *) &address)[3];

         /* Add new mcast to the hardware */
         cpsw_ale_multicastentry_set(cpswinst, (PORT_0_MASK | PORT_1_MASK | PORT_2_MASK), mac);
         break;
      }
   }  

   return(rc);
} /* MulticastAddFilter */


/*
 * MulticastDelFilter
 */
static int MulticastDelFilter (uint32_t address)
{
   int          rc = -1;
   int          i;
   uint8_t      mac[6];
   int          RebuildList = 0;
   cpswinst_t  *cpswinst = &cpsw_inst_data;
   
   /*
    * Check if address is still in list
    */
   for (i=0; i<MAX_ALE_ENTRIES; i++)
   {
      if (MulticastFilterList[i] == address)
      {
         RebuildList = 1;
         MulticastFilterList[i] = 0;
         break;
      }
   }    

   if (1 == RebuildList)
   {
      /* Remove all entries from the hardware starting at 2 */
      cpsw_ale_remove_all_entries(cpswinst, 2);
      
      /* Add multicast addresses from the list */
      for (i=0; i<MAX_ALE_ENTRIES; i++)
      {
         if (MulticastFilterList[i] != 0)
         {
            address = MulticastFilterList[i];
            
            /* Create MAC address */
            mac[0] = 0x01;
            mac[1] = 0x00;
            mac[2] = 0x5E;
            mac[3] = ((uint8_t *) &address)[1] & 0x7f;
            mac[4] = ((uint8_t *) &address)[2];
            mac[5] = ((uint8_t *) &address)[3];

            /* Add new mcast to the hardware */
            cpsw_ale_multicastentry_set(cpswinst, (PORT_0_MASK | PORT_1_MASK | PORT_2_MASK), mac);
         }
      }
      
      rc = 0;
   }

   return(rc);
} /* MulticastDelFilter */


/**
 * This function handle the IGMP filter ADD/DEL functionality.
 */
static err_t igmp_mac_filter (struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action)
{
   err_t error = ERR_OK;
   
   (void)netif;

   switch (action)
   {
      case IGMP_ADD_MAC_FILTER:
      {
         MulticastAddFilter(group->addr);
         break;
      }
      
      case IGMP_DEL_MAC_FILTER:
      {
         MulticastDelFilter(group->addr);
         break;
      }
   
      default:
      {
         error = ERR_IF;
         break;
      }
   }
   
   return(error);
} /* igmp_mac_filter */


/**
 * Timer callback function for checking the Ethernet Link.
 *
 */
static void CheckLink (struct netif *netif)
{
   uint8_t LinkStatus;
   
   /* Get actual Link status, and check if the status has changed */
   LinkStatus = cpsw_GetLinkStatus(OldLinkStatus);
   if (LinkStatus != OldLinkStatus)
   {
      /* Check for DOWN/UP */
      if (PHY_LINK_STATUS_NO_LINK == LinkStatus)
      {
         tcpip_try_callback((tcpip_callback_fn)netif_set_link_down, netif);
      }
      else
      {
         tcpip_try_callback((tcpip_callback_fn)netif_set_link_up, netif);
      }
   } /* end if (LinkStatus != OldLinkStatus) */
   
   OldLinkStatus = LinkStatus;      
} /* CheckLink */


/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static err_t low_level_init (struct netif *netif)
{
   int rc;
   int i;

   netif->flags = 0;
   
   /* Configure the HW of the board */
   rc = cpsw_port_init(netif);
   if (rc != 0)
   {
      /* Error */
      return(ERR_MEM);
   }
   
   /* Clear multicast filter list */
   for(i=0; i<MAX_ALE_ENTRIES; i++)
   {
      MulticastFilterList[i] = 0;
   }   
   
   /* Set netif MAC hardware address length */
   netif->hwaddr_len = ETHARP_HWADDR_LEN;
   
   /* Set netif MAC hardware address */
   memcpy(netif->hwaddr, &cpsw_inst_data.port[0].cpswif.eth_addr[0], ETHARP_HWADDR_LEN); 
   
   /* Set netif maximum transfer unit */
   netif->mtu = 1500;

   /* Accept broadcast address, ARP traffic and Multicast */
   netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;
   
#if (LWIP_IGMP >= 1)   
   /* Add IGMP support */   
   netif->flags |= NETIF_FLAG_IGMP;
   netif->igmp_mac_filter = igmp_mac_filter;
#endif   
   
   /* Create RX mailbox */
   OS_MboxCreate(&RxMbox, RxMboxBuffer, NUM_BUFF_RXDESCS);     
   
   /* Create the event used for RX and LinkStatus */
   OS_EventCreate(&NetIfEvent);

   /* Start the receiver thread. */
   OS_TaskCreate(&tcb_task_rx, ethernetif_input, netif, TASK_IP_RX_PRIORITY, 
                 ReceiverStack, sizeof(ReceiverStack), 
                 "IPRX");

   return(ERR_OK);
} /* low_level_init */


/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output (struct netif *netif, struct pbuf *p)
{
   int          sz = 0;
   cpdma_bd_t  *bd;
   cpswinst_t  *cpswinst = &cpsw_inst_data;
   uint8_t     *buffer;
   struct pbuf *q;

   (void)netif;
   
   /* Get TX descriptor */   
   bd = &TxDesc[cpswinst->tx_write_index];
   
   /* Check if this descriptor can be used */
   if (bd->flags_pktlen != 0)
   {
      return(ERR_MEM);
   }

   /* Get descriptor buffer */
   buffer = (uint8_t*)bd->bufptr;

   for(q = p; q != NULL; q = q->next) 
   {
      memcpy(&buffer[sz], q->payload, q->len);
      sz = sz + q->len;
   }

   if (sz < MIN_PKT_LEN)
   {
      sz = MIN_PKT_LEN;
   }

   bd->bufoff_len   = sz; //ETH_MAX_BUFF_LEN;
   bd->flags_pktlen = sz & CPDMA_BD_PKTLEN_MASK;

   /*
    * Make sure that the payload is written to memory. Clean
    * the portion of cache to make it coherent with the memory.
    */
   CacheDataCleanBuff(bd->bufptr, (uint32_t)sz);

   IntSystemDisable(SYS_INT_3PGSWTXINT0);

   /* Indicate the start of the packet */
   bd->flags_pktlen |= (CPDMA_BUF_DESC_SOP | CPDMA_BUF_DESC_EOP | CPDMA_BUF_DESC_OWNER);

   /* Check if TX is running */
   if (0 == cpswinst->tx_running)
   {
      /* Not running, must be started */
      cpswinst->tx_running = 1;
      CPSWCPDMATxHdrDescPtrWrite(cpswinst->cpdma_base, (uint32_t)bd, 0);
   }
   else
   {
      /* Do nothing, TX is running */
   } 
   
   cpswinst->tx_write_index++;
   if (cpswinst->tx_write_index > LAST_BUFF_TXDESCS)
   {
      cpswinst->tx_write_index = 0;
   }

   IntSystemEnable(SYS_INT_3PGSWTXINT0);

   
   LINK_STATS_INC(link.xmit);

   return(ERR_OK);
} /* low_level_output */


/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input (struct netif *netif)
{
   int         rc;
   cpdma_bd_t *bd;
   uint16_t    len;
   uint8_t    *buffer;

   struct pbuf *p, *q;
   uint16_t i;
        
   (void)netif;
   
   p = NULL;   
   
   /* Get RX descriptor */   
   rc = OS_MboxWait(&RxMbox, (void*)&bd, 0);
   if (rc != OS_RC_OK)
   {
      len    = 0; 
      buffer = NULL;
   }
   else
   {
      len    = bd->flags_pktlen & CPDMA_BD_PKTLEN_MASK;
      buffer = (uint8_t*)bd->bufptr;
   }
   
   if (buffer != NULL)
   {
      /* We allocate a pbuf chain of pbufs from the pool. */
      p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
 
      /* Copy received frame from ethernet driver buffer to stack buffer */
      if (p != NULL)
      { 
         i = 0;
         for (q = p; q != NULL; q = q->next)
         {
            memcpy((u8_t*)q->payload, &buffer[i], q->len);
            i = i + q->len;
         } 
         
         LINK_STATS_INC(link.recv);
      }
   }

   return(p);
} /* low_level_input */


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void ethernetif_input (void *arg)
{
   struct pbuf    *p;
   struct netif   *netif = (struct netif*)arg;
   struct eth_hdr *ethhdr;
   unsigned        Event;
   uint32_t        Checktime = OS_TimeGet();

   /* 
    * Now the MAC can be started, enable the system interrupt.
    */
   IntSystemEnable(SYS_INT_3PGSWTXINT0);
   IntSystemEnable(SYS_INT_3PGSWRXINT0);

   while (1)
   {
      /* Wait for an event */
      Event = 0;
      OS_EventWait(&NetIfEvent, NETIF_EVENT_WAIT_ALL, OS_EVENT_MODE_OR, &Event, 500); 

#if 0      
      if (Event & NETIF_EVENT_RX_OVERRUN)
      {
         enet_RxOverrun();
      }
#endif 

      if (Event & NETIF_EVENT_RX)
      {
         while (1)
         {
            /* move received packet into a new pbuf */
            p = low_level_input( netif );
            
            /* no packet could be read, silently ignore this */
            if (p == NULL) break;
            
            /* points to packet payload, which starts with an Ethernet header */
            ethhdr = p->payload;
            
            switch (htons(ethhdr->type)) 
            {
               /* IP or ARP packet? */
               case ETHTYPE_IP:
               case ETHTYPE_ARP:
               
                  /* full packet send to tcpip_thread to process */
                  if (netif->input(p, netif) != ERR_OK)
                  {
                     LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
                     pbuf_free(p);
                  }
                  
                  /* 
                   * In case we have receive a packet, we can
                   * restart the timeout for the CheckLink.
                   */
                  if (PHY_LINK_STATUS_NO_LINK == OldLinkStatus)
                  {
                     CheckLink(netif);
                  }
                  else
                  {
                     Checktime = OS_TimeGet();               
                  }             
                  break;
               
               default:
                  pbuf_free(p);
                  break;                  
            } /* end switch (htons(ethhdr->type)) */               
         } /* while (1), loop over low_level_input */
      } /* if (Event & NETIF_EVENT_RX) */


      /* Check timeout for CheckLink */
      if ((OS_TimeGet() - Checktime) > LINK_TMR_INTERVAL)
      {
         Checktime = OS_TimeGet();
         CheckLink(netif);
      }

   } /* end while (1), task loop */

} /* ethernetif_input */

/*=======================================================================*/
/*  All code exported                                                    */
/*=======================================================================*/

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethernetif_init (struct netif *netif)
{
   err_t error = ERR_MEM;
   
   if (netif != NULL)
   {
      /* Descriptive abbreviation for this interface */
      netif->name[0] = IFNAME0;
      netif->name[1] = IFNAME1;
  
      /* 
       * We directly use etharp_output() here to save a function call.
       * You can instead declare your own function an call etharp_output()
       * from it if you have to do some checks before sending (e.g. if link
       * is available...) 
       */
      netif->output     = etharp_output;
      netif->linkoutput = low_level_output;
  
      /* Initialize the hardware */
      error = low_level_init(netif);
      
#if LWIP_NETIF_HOSTNAME
      if (ERR_OK == error)
      {
         if (NULL == netif->hostname)
         {
            /* Initialize interface hostname */
            snprintf(Hostname, sizeof(Hostname), "%02X%02X%02X%02X%02X%02X", 
                     netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], 
                     netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
            netif->hostname = Hostname;
         }
      }         
#endif /* LWIP_NETIF_HOSTNAME */
      
   }      

   return(error);
} /* ethernetif_init */

/*** EOF ***/
