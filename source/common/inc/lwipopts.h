/*
 * @file
 *
 * lwIP Options Configuration
 */

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
#ifndef __LWIPOPT_H__
#define __LWIPOPT_H__

#include <stdlib.h>
#include "project.h"

/*
 * How to tune, see: http://lwip.wikia.com/wiki/Tuning_TCP
 */

/*
 * Some notes from my tests here. 
 *
 * TCP_MSS
 * ========
 * If the RX performance should be increased, change TCP_MSS
 * from 536 to 1460. This change the RX performance e.g. from
 * 2.8 MByte/s to 7 MByte/s. Measured with JPerf 2.0.2.
 *
 * MEM_SIZE
 * =========
 * If the MEM_SIZE was changed from 1600 to 4096,
 * the TX performance could be increased from
 * 7 KByte/s to 2 MByte/s.
 *
 * In general take a look at the lines marked with "@perf@".
 */

/*
 * Random generator function
 */
unsigned long xrand (void); 
#define LWIP_RAND()  xrand() 

/*
 * DHCP, and IGMP should be supported
 */
#define LWIP_DHCP                   1
#define LWIP_IGMP                   1
#define LWIP_DNS                    1

#define LWIP_DHCP_DOES_ACD_CHECK    0


/*
 * Set to 0, because conflict with time.h from CrossWorks
 */
#define LWIP_TIMEVAL_PRIVATE        0

/*-----------------------------------------------------------------------*/
/*    Platform specific locking                                          */
/*-----------------------------------------------------------------------*/

/*
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT        0


/*-----------------------------------------------------------------------*/
/*    Memory options                                                     */
/*-----------------------------------------------------------------------*/

/*
 * MEM_LIBC_MALLOC==1: Use malloc/free/realloc provided by your C-library
 * instead of the lwip internal allocator. Can save code size if you
 * already use it.
 */
#define MEM_LIBC_MALLOC             1

#if (MEM_LIBC_MALLOC >= 1)
#include <stddef.h>
extern void *lwip_malloc(size_t size);
extern void *lwip_calloc(size_t n, size_t size);
extern void  lwip_free(void *ptr);

#define mem_clib_malloc    lwip_malloc
#define mem_clib_calloc    lwip_calloc
#define mem_clib_free      lwip_free
#endif


/*
 * MEMP_MEM_MALLOC==1: Use mem_malloc/mem_free instead of the lwip pool allocator.
 * Especially useful with MEM_LIBC_MALLOC but handle with care regarding execution
 * speed and usage from interrupts!
 */
#define MEMP_MEM_MALLOC             1

/*
 * MEMP_NUM_SYS_TIMEOUT: the number of simultaneously active timeouts.
 * The default number of timeouts is calculated here for all enabled modules.
 * The formula expects settings to be either '0' or '1'.
 */
#define MEMP_NUM_SYS_TIMEOUT        16

/*
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */
#define MEM_ALIGNMENT               4

/*
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */
#define MEM_SIZE                    (4*1024)


/*-----------------------------------------------------------------------*/
/*    Internal Memory Pool Sizes                                         */
/*-----------------------------------------------------------------------*/

/*
 * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
 * If the application sends a lot of data out of ROM (or other static memory),
 * this should be set high.
 */
#define MEMP_NUM_PBUF               16

/*
 * MEMP_NUM_NETCONN: the number of struct netconns.
 * (only needed if you use the sequential API, like api_lib.c)
 */
#define MEMP_NUM_NETCONN            16

/*
 * MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
 * per active UDP "connection".
 * (requires the LWIP_UDP option)
 */
#define MEMP_NUM_UDP_PCB            8

/*
 * MEMP_NUM_TCP_PCB: the number of simultaneously active TCP connections.
 * (requires the LWIP_TCP option)
 */
#define MEMP_NUM_TCP_PCB            16

/*
 * MEMP_NUM_TCPIP_MSG_INPKT: the number of struct tcpip_msg, which are used
 * for incoming packets.
 * (only needed if you use tcpip.c)
 */
#define MEMP_NUM_TCPIP_MSG_INPKT    16

/*
 * PBUF_POOL_SIZE: the number of buffers in the pbuf pool.
 */
#define PBUF_POOL_SIZE              32


/*-----------------------------------------------------------------------*/
/*    Network Interfaces options                                         */
/*-----------------------------------------------------------------------*/

/*
 * LWIP_NETIF_API==1: Support netif api (in netifapi.c)
 */
#define LWIP_NETIF_API              1

/*
 * LWIP_NETIF_STATUS_CALLBACK==1: Support a callback function whenever an interface
 * changes its up/down status (i.e., due to DHCP IP acquisition)
 */
#define LWIP_NETIF_STATUS_CALLBACK  1

/*
 * LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */
#define LWIP_NETIF_LINK_CALLBACK    1

/*
 * LWIP_NETIF_HOSTNAME==1: use DHCP_OPTION_HOSTNAME with netif's hostname field.
 */
#define LWIP_NETIF_HOSTNAME         1


/*-----------------------------------------------------------------------*/
/*    TCP options                                                        */
/*-----------------------------------------------------------------------*/

/*
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */
#define TCP_MSS                     1460  // RX @perf@ 536/1460

/*
 * TCP_WND: The size of a TCP window.  This must be at least
 * (2 * TCP_MSS) for things to work well
 */
#define TCP_WND                     (6 * TCP_MSS)

/*
 * TCP_SND_BUF: TCP sender buffer space (bytes).
 * To achieve good performance, this should be at least 2 * TCP_MSS.
 */
#define TCP_SND_BUF                 (6 * TCP_MSS) // TX @perf@

/*
 * TCP_LISTEN_BACKLOG: Enable the backlog option for tcp listen pcb.
 */
#define TCP_LISTEN_BACKLOG          1


/*-----------------------------------------------------------------------*/
/*    Thread options                                                     */    
/*-----------------------------------------------------------------------*/

#define TCPIP_THREAD_NAME           "IP"
#define TCPIP_THREAD_STACKSIZE      TASK_IP_STK_SIZE
#define TCPIP_THREAD_PRIO           TASK_IP_PRIORITY

/*
 * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when tcpip_init is called.
 */
#define TCPIP_MBOX_SIZE             MBOX_SIZE_MAX

/*
 * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_RAW_RECVMBOX_SIZE   MBOX_SIZE_MAX

/*
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_UDP_RECVMBOX_SIZE   MBOX_SIZE_MAX

/*
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */
#define DEFAULT_TCP_RECVMBOX_SIZE   MBOX_SIZE_MAX

/*
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */
#define DEFAULT_ACCEPTMBOX_SIZE     MBOX_SIZE_MAX


/*-----------------------------------------------------------------------*/
/*    Socket options                                                     */    
/*-----------------------------------------------------------------------*/

/* 
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO            1

/*
 * LWIP_SO_SNDRCVTIMEO_NONSTANDARD==1: SO_RCVTIMEO/SO_SNDTIMEO take an int
 * (milliseconds, much like winsock does) instead of a struct timeval (default).
 */
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD   1

/*
 * LWIP_SO_RCVBUF==1: Enable SO_RCVBUF processing.
 */
#define LWIP_SO_RCVBUF              1


#define LWIP_TCP_KEEPALIVE          1

/**
 * SO_REUSE==1: Enable SO_REUSEADDR option.
 */
#define SO_REUSE                    1


/*-----------------------------------------------------------------------*/
/*    Statistics options                                                 */    
/*-----------------------------------------------------------------------*/

#define LWIP_STATS                  1
#define MEM_STATS                   1 
#define MEMP_STATS                  1 


/*-----------------------------------------------------------------------*/
/*    Debugging options                                                  */    
/*-----------------------------------------------------------------------*/

//#define LWIP_DEBUG
#define LWIP_DBG_MIN_LEVEL          LWIP_DBG_LEVEL_ALL 
#define LWIP_DBG_TYPES_ON           LWIP_DBG_ON 
#define MEM_DEBUG                   LWIP_DBG_ON
#define MEMP_DEBUG                  LWIP_DBG_ON
//#define PBUF_DEBUG                  LWIP_DBG_ON
//#define TCP_DEBUG                   LWIP_DBG_ON
//#define IGMP_DEBUG                  LWIP_DBG_ON


/*-----------------------------------------------------------------------*/
/*    Sequential layer options                                           */    
/*-----------------------------------------------------------------------*/

/*
 * !!! Must be set to 0 !!!
 * No support for mutexes priority inversion available.
 */
#define LWIP_TCPIP_CORE_LOCKING     0

#define IP_SOF_BROADCAST            1
#define IP_SOF_BROADCAST_RECV       1


/*-----------------------------------------------------------------------*/
/*    Misc options                                                       */    
/*-----------------------------------------------------------------------*/

#define LWIP_DHCP_GET_NTP_SRV       1


/*-----------------------------------------------------------------------*/
/*    Function prototypes                                                */    
/*-----------------------------------------------------------------------*/

void _IP_DHCP_CallbackBound (void);

#endif /* __LWIPOPT_H__ */
/*** EOF ***/
