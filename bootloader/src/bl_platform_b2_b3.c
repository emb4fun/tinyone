/**************************************************************************
*  Copyright (c) 2015-2024 by Michael Fischer (www.emb4fun.de).
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
*  12.04.2015  mifi  First Version.
*                    Based on the original bl_platform.c from StarterWare
*                    with rework and cleanup for the BeagleBone Black.
**************************************************************************/


/**
 * \file  bl_platform.c
 *
 * \brief Initializes AM335x Device Peripherals.
 *
 */

/*
* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "hw_types.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"
#include "hw_control_AM335x.h"
#include "hw_emif4d.h"
#include "hw_tps65217.h"
#include "bl_platform.h"
#include "bl_mmcsd.h"
#include "watchdog.h"
#include "hsi2c.h"
#include "device.h"

/******************************************************************************
**                     Internal Macro Definitions
*******************************************************************************/

/*	I2C instance	*/
#define I2C_0				                     (0x0u)
/*	System clock fed to I2C module - 48Mhz	*/
#define I2C_SYSTEM_CLOCK		               (48000000u)
/*	Internal clock used by I2C module - 12Mhz	*/
#define I2C_INTERNAL_CLOCK		               (12000000u)
/*	I2C bus speed or frequency - 100Khz	*/
#define I2C_OUTPUT_CLOCK		               (100000u)
/*	I2C interrupt flags to clear	*/
#define I2C_INTERRUPT_FLAG_TO_CLR            (0x7FF)
#define PMIC_SR_I2C_SLAVE_ADDR               (0x12)
#define SMPS_DRIVE_SCLSR_EN1	               (0x0u)
#define SMPS_DRIVE_SDASR_EN2	               (0x1u)

/* TODO : These are not there in the control module header file */
#define DDR_PHY_CTRL_REGS                    (SOC_CONTROL_REGS + 0x2000)
#define CMD0_SLAVE_RATIO_0                   (DDR_PHY_CTRL_REGS + 0x1C)
#define CMD0_SLAVE_FORCE_0                   (DDR_PHY_CTRL_REGS + 0x20)
#define CMD0_SLAVE_DELAY_0                   (DDR_PHY_CTRL_REGS + 0x24)
#define CMD0_LOCK_DIFF_0                     (DDR_PHY_CTRL_REGS + 0x28)
#define CMD0_INVERT_CLKOUT_0                 (DDR_PHY_CTRL_REGS + 0x2C)
#define CMD1_SLAVE_RATIO_0                   (DDR_PHY_CTRL_REGS + 0x50)
#define CMD1_SLAVE_FORCE_0                   (DDR_PHY_CTRL_REGS + 0x54)
#define CMD1_SLAVE_DELAY_0                   (DDR_PHY_CTRL_REGS + 0x58)
#define CMD1_LOCK_DIFF_0                     (DDR_PHY_CTRL_REGS + 0x5C)
#define CMD1_INVERT_CLKOUT_0                 (DDR_PHY_CTRL_REGS + 0x60)
#define CMD2_SLAVE_RATIO_0                   (DDR_PHY_CTRL_REGS + 0x84)
#define CMD2_SLAVE_FORCE_0                   (DDR_PHY_CTRL_REGS + 0x88)
#define CMD2_SLAVE_DELAY_0                   (DDR_PHY_CTRL_REGS + 0x8C)
#define CMD2_LOCK_DIFF_0                     (DDR_PHY_CTRL_REGS + 0x90)
#define CMD2_INVERT_CLKOUT_0                 (DDR_PHY_CTRL_REGS + 0x94)
#define DATA0_RD_DQS_SLAVE_RATIO_0           (DDR_PHY_CTRL_REGS + 0xC8)
#define DATA0_RD_DQS_SLAVE_RATIO_1           (DDR_PHY_CTRL_REGS + 0xCC)
#define DATA0_WR_DQS_SLAVE_RATIO_0           (DDR_PHY_CTRL_REGS + 0xDC)
#define DATA0_WR_DQS_SLAVE_RATIO_1           (DDR_PHY_CTRL_REGS + 0xE0)
#define DATA0_WRLVL_INIT_RATIO_0             (DDR_PHY_CTRL_REGS + 0xF0)
#define DATA0_WRLVL_INIT_RATIO_1             (DDR_PHY_CTRL_REGS + 0xF4)
#define DATA0_GATELVL_INIT_RATIO_0           (DDR_PHY_CTRL_REGS + 0xFC)
#define DATA0_GATELVL_INIT_RATIO_1           (DDR_PHY_CTRL_REGS + 0x100)
#define DATA0_FIFO_WE_SLAVE_RATIO_0          (DDR_PHY_CTRL_REGS + 0x108)
#define DATA0_FIFO_WE_SLAVE_RATIO_1          (DDR_PHY_CTRL_REGS + 0x10C)
#define DATA0_WR_DATA_SLAVE_RATIO_0          (DDR_PHY_CTRL_REGS + 0x120)
#define DATA0_WR_DATA_SLAVE_RATIO_1          (DDR_PHY_CTRL_REGS + 0x124)
#define DATA0_USE_RANK0_DELAYS_0             (DDR_PHY_CTRL_REGS + 0x134)
#define DATA0_LOCK_DIFF_0                    (DDR_PHY_CTRL_REGS + 0x138)
#define DATA1_RD_DQS_SLAVE_RATIO_0           (DDR_PHY_CTRL_REGS + 0x16c)
#define DATA1_RD_DQS_SLAVE_RATIO_1           (DDR_PHY_CTRL_REGS + 0x170)
#define DATA1_WR_DQS_SLAVE_RATIO_0           (DDR_PHY_CTRL_REGS + 0x180)
#define DATA1_WR_DQS_SLAVE_RATIO_1           (DDR_PHY_CTRL_REGS + 0x184)
#define DATA1_WRLVL_INIT_RATIO_0             (DDR_PHY_CTRL_REGS + 0x194)
#define DATA1_WRLVL_INIT_RATIO_1             (DDR_PHY_CTRL_REGS + 0x198)
#define DATA1_GATELVL_INIT_RATIO_0           (DDR_PHY_CTRL_REGS + 0x1a0)
#define DATA1_GATELVL_INIT_RATIO_1           (DDR_PHY_CTRL_REGS + 0x1a4)
#define DATA1_FIFO_WE_SLAVE_RATIO_0          (DDR_PHY_CTRL_REGS + 0x1ac)
#define DATA1_FIFO_WE_SLAVE_RATIO_1          (DDR_PHY_CTRL_REGS + 0x1b0)
#define DATA1_WR_DATA_SLAVE_RATIO_0          (DDR_PHY_CTRL_REGS + 0x1c4)
#define DATA1_WR_DATA_SLAVE_RATIO_1          (DDR_PHY_CTRL_REGS + 0x1c8)
#define DATA1_USE_RANK0_DELAYS_0             (DDR_PHY_CTRL_REGS + 0x1d8)
#define DATA1_LOCK_DIFF_0                    (DDR_PHY_CTRL_REGS + 0x1dc)

/*
 * DDR3 init values 
 */

/*******************************************************/
/*
 * Timing from U-Boot:
 * arch\arm\include\asm\arch-am33xx\ddr_defs.h
 */
  
/* Micron MT41K256M16HA-125E */
#define MT41K256M16HA125E_EMIF_READ_LATENCY  0x100007
#define MT41K256M16HA125E_EMIF_TIM1		      0x0AAAD4DB
#define MT41K256M16HA125E_EMIF_TIM2		      0x266B7FDA
#define MT41K256M16HA125E_EMIF_TIM3		      0x501F867F
#define MT41K256M16HA125E_EMIF_SDCFG		   0x61C05332
#define MT41K256M16HA125E_EMIF_SDREF		   0xC30
#define MT41K256M16HA125E_ZQ_CFG	         	0x50074BE4
#define MT41K256M16HA125E_RATIO			      0x80
#define MT41K256M16HA125E_INVERT_CLKOUT		0x0
#define MT41K256M16HA125E_RD_DQS		         0x38
#define MT41K256M16HA125E_WR_DQS		         0x44
#define MT41K256M16HA125E_PHY_WR_DATA		   0x7D
#define MT41K256M16HA125E_PHY_FIFO_WE		   0x94
#define MT41K256M16HA125E_IOCTRL_VALUE		   0x18B
/*******************************************************/ 

#define DDR3_CMD0_SLAVE_RATIO_0              MT41K256M16HA125E_RATIO
#define DDR3_CMD0_INVERT_CLKOUT_0            MT41K256M16HA125E_INVERT_CLKOUT
#define DDR3_CMD1_SLAVE_RATIO_0              MT41K256M16HA125E_RATIO
#define DDR3_CMD1_INVERT_CLKOUT_0            MT41K256M16HA125E_INVERT_CLKOUT
#define DDR3_CMD2_SLAVE_RATIO_0              MT41K256M16HA125E_RATIO
#define DDR3_CMD2_INVERT_CLKOUT_0            MT41K256M16HA125E_INVERT_CLKOUT

#define DDR3_DATA0_RD_DQS_SLAVE_RATIO_0      MT41K256M16HA125E_RD_DQS
#define DDR3_DATA0_WR_DQS_SLAVE_RATIO_0      MT41K256M16HA125E_WR_DQS
#define DDR3_DATA0_WR_DATA_SLAVE_RATIO_0     MT41K256M16HA125E_PHY_WR_DATA
#define DDR3_DATA0_FIFO_WE_SLAVE_RATIO_0     MT41K256M16HA125E_PHY_FIFO_WE

#define DDR3_DATA0_RD_DQS_SLAVE_RATIO_1      MT41K256M16HA125E_RD_DQS
#define DDR3_DATA0_WR_DQS_SLAVE_RATIO_1      MT41K256M16HA125E_WR_DQS
#define DDR3_DATA0_FIFO_WE_SLAVE_RATIO_1     MT41K256M16HA125E_PHY_WR_DATA
#define DDR3_DATA0_WR_DATA_SLAVE_RATIO_1     MT41K256M16HA125E_PHY_FIFO_WE

#define DDR3_CONTROL_DDR_CMD_IOCTRL_0        MT41K256M16HA125E_IOCTRL_VALUE
#define DDR3_CONTROL_DDR_CMD_IOCTRL_1        MT41K256M16HA125E_IOCTRL_VALUE
#define DDR3_CONTROL_DDR_CMD_IOCTRL_2        MT41K256M16HA125E_IOCTRL_VALUE

#define DDR3_CONTROL_DDR_DATA_IOCTRL_0       MT41K256M16HA125E_IOCTRL_VALUE
#define DDR3_CONTROL_DDR_DATA_IOCTRL_1       MT41K256M16HA125E_IOCTRL_VALUE

#define DDR3_CONTROL_DDR_IO_CTRL             (0xefffffff)

#define DDR3_EMIF_DDR_PHY_CTRL_1             MT41K256M16HA125E_EMIF_READ_LATENCY
#define DDR3_EMIF_DDR_PHY_CTRL_1_SHDW        MT41K256M16HA125E_EMIF_READ_LATENCY

#define DDR3_EMIF_SDRAM_TIM_1                MT41K256M16HA125E_EMIF_TIM1
#define DDR3_EMIF_SDRAM_TIM_1_SHDW           MT41K256M16HA125E_EMIF_TIM1

#define DDR3_EMIF_SDRAM_TIM_2                MT41K256M16HA125E_EMIF_TIM2
#define DDR3_EMIF_SDRAM_TIM_2_SHDW           MT41K256M16HA125E_EMIF_TIM2

#define DDR3_EMIF_SDRAM_TIM_3                MT41K256M16HA125E_EMIF_TIM3
#define DDR3_EMIF_SDRAM_TIM_3_SHDM           MT41K256M16HA125E_EMIF_TIM3

#define DDR3_EMIF_SDRAM_REF_CTRL_VAL1        MT41K256M16HA125E_EMIF_SDREF
#define DDR3_EMIF_SDRAM_REF_CTRL_SHDW_VAL1   MT41K256M16HA125E_EMIF_SDREF

#define DDR3_EMIF_ZQ_CONFIG_VAL              MT41K256M16HA125E_ZQ_CFG

#define DDR3_EMIF_SDRAM_CONFIG               MT41K256M16HA125E_EMIF_SDCFG


/*
 * DDR2 init values
 */
#define DDR2_CMD0_SLAVE_RATIO_0              (0x80)
#define DDR2_CMD0_SLAVE_FORCE_0              (0x0)
#define DDR2_CMD0_SLAVE_DELAY_0              (0x0)
#define DDR2_CMD0_LOCK_DIFF_0                (0x4)
#define DDR2_CMD0_INVERT_CLKOUT_0            (0x0)

#define DDR2_CMD1_SLAVE_RATIO_0              (0x80)
#define DDR2_CMD1_SLAVE_FORCE_0              (0x0)
#define DDR2_CMD1_SLAVE_DELAY_0              (0x0)
#define DDR2_CMD1_LOCK_DIFF_0                (0x4)
#define DDR2_CMD1_INVERT_CLKOUT_0            (0x0)

#define DDR2_CMD2_SLAVE_RATIO_0              (0x80)
#define DDR2_CMD2_SLAVE_FORCE_0              (0x0)
#define DDR2_CMD2_SLAVE_DELAY_0              (0x0)
#define DDR2_CMD2_LOCK_DIFF_0                (0x4)
#define DDR2_CMD2_INVERT_CLKOUT_0            (0x0)

#define DDR2_DATA0_RD_DQS_SLAVE_RATIO_0      (0x12)
#define DDR2_DATA0_WR_DQS_SLAVE_RATIO_0      (0x0)
#define DDR2_DATA0_FIFO_WE_SLAVE_RATIO_0     (0x80)
#define DDR2_DATA0_WR_DATA_SLAVE_RATIO_0     (0x40)

#define DDR2_DATA1_RD_DQS_SLAVE_RATIO_0      (0x12)
#define DDR2_DATA1_WR_DQS_SLAVE_RATIO_0      (0x0)
#define DDR2_DATA1_FIFO_WE_SLAVE_RATIO_0     (0x80)
#define DDR2_DATA1_WR_DATA_SLAVE_RATIO_0     (0x40)

#define DDR2_CONTROL_DDR_CMD_IOCTRL_0        (0x18B)
#define DDR2_CONTROL_DDR_CMD_IOCTRL_1        (0x18B)
#define DDR2_CONTROL_DDR_CMD_IOCTRL_2        (0x18B)
#define DDR2_CONTROL_DDR_DATA_IOCTRL_0       (0x18B)
#define DDR2_CONTROL_DDR_DATA_IOCTRL_1       (0x18B)

#define DDR2_CONTROL_DDR_IO_CTRL             (0x0fffffff)
#define DDR2_EMIF_DDR_PHY_CTRL_1             (0x05)
#define DDR2_EMIF_DDR_PHY_CTRL_1_DY_PWRDN    (0x00100000)
#define DDR2_EMIF_DDR_PHY_CTRL_1_SHDW        (0x05)
#define DDR2_EMIF_DDR_PHY_CTRL_1_SHDW_DY_PWRDN  (0x00100000)
#define DDR2_EMIF_DDR_PHY_CTRL_2             (0x05)

#define DDR2_EMIF_SDRAM_TIM_1		            (0x0666B3D6)
#define DDR2_EMIF_SDRAM_TIM_1_SHDW           (0x0666B3D6)
#define DDR2_EMIF_SDRAM_TIM_2                (0x143731DA)
#define DDR2_EMIF_SDRAM_TIM_2_SHDW	         (0x143731DA)
#define DDR2_EMIF_SDRAM_TIM_3                (0x00000347)
#define DDR2_EMIF_SDRAM_TIM_3_SHDM           (0x00000347)
#define DDR2_EMIF_SDRAM_CONFIG               (0x41805332)
#define DDR2_EMIF_SDRAM_REF_CTRL_VAL1	      (0x00004650)
#define DDR2_EMIF_SDRAM_REF_CTRL_SHDW_VAL1   (0x00004650)

#define DDR2_EMIF_SDRAM_REF_CTRL_VAL2	      (0x0000081a)
#define DDR2_EMIF_SDRAM_REF_CTRL_SHDW_VAL2   (0x0000081a)

/******************************************************************************
**                     Local function Declarations
*******************************************************************************/

/******************************************************************************
**                     Global variable Definitions
*******************************************************************************/

volatile unsigned char dataFromSlave[2];
volatile unsigned char dataToSlave[3];
volatile unsigned int tCount;
volatile unsigned int rCount;
volatile unsigned int oppMaxIdx;
volatile unsigned int deviceVersion;
volatile unsigned int freqMultDDR;

/*
** OPP table for mpu multiplier and pmic voltage select.
*/
tOPPConfig oppTable[] =
{
   {MPUPLL_M_275_MHZ,  PMIC_VOLT_SEL_1100MV},   /* OPP100 275Mhz - 1.1v */
   {MPUPLL_M_500_MHZ,  PMIC_VOLT_SEL_1100MV},   /* OPP100 500Mhz - 1.1v */
   {MPUPLL_M_600_MHZ,  PMIC_VOLT_SEL_1200MV},   /* OPP120 600Mhz - 1.2v */
   {MPUPLL_M_720_MHZ,  PMIC_VOLT_SEL_1260MV},   /* OPP TURBO 720Mhz - 1.26v */
   {MPUPLL_M_300_MHZ,  PMIC_VOLT_SEL_0950MV},   /* OPP50 300Mhz - 950mv */
   {MPUPLL_M_300_MHZ,  PMIC_VOLT_SEL_1100MV},   /* OPP100 300Mhz - 1.1v */
   {MPUPLL_M_600_MHZ,  PMIC_VOLT_SEL_1100MV},   /* OPP100 600Mhz - 1.1v */
   {MPUPLL_M_720_MHZ,  PMIC_VOLT_SEL_1200MV},   /* OPP120 720Mhz - 1.2v */
   {MPUPLL_M_800_MHZ,  PMIC_VOLT_SEL_1260MV},   /* OPP TURBO 800Mhz - 1.26v */
   {MPUPLL_M_1000_MHZ, PMIC_VOLT_SEL_1325MV}    /* OPP NITRO 1000Mhz - 1.325v */
};

/******************************************************************************
**                     Local variable Definitions
*******************************************************************************/

/******************************************************************************
**                     Function Definitions
*******************************************************************************/

/*
** Read the device version from the DEVICE ID register
*/
unsigned int DeviceVersionGet(void)
{
   unsigned int deviceVersion;

   deviceVersion = (HWREG(SOC_CONTROL_REGS + CONTROL_DEVICE_ID) >>
                          CONTROL_DEVICE_ID_DEVREV_SHIFT);

   return deviceVersion;
}

/*
** Read the device OPP support from EFUSE SMA control register
*/
unsigned int SysConfigOppDataGet(void)
{
   return (HWREG(SOC_CONTROL_REGS + CONTROL_EFUSE_SMA) & EFUSE_OPP_MASK);
}

/*
** Determine maximum OPP configuration of SoC.
*/
unsigned int BootMaxOppGet(void)
{
   unsigned int oppIdx;
   unsigned int oppSupport = SysConfigOppDataGet();
   
   if(DEVICE_VERSION_1_0 == deviceVersion)
   {
      oppIdx = EFUSE_OPPTB_720;
   }
   else
   {
      switch(oppSupport)
      {
         case 0x1FEF: oppIdx = EFUSE_OPP100_300; break;
         case 0x1FAF: oppIdx = EFUSE_OPP100_600; break;
         case 0x1F2F: oppIdx = EFUSE_OPP120_720; break;
         case 0x1E2F: oppIdx = EFUSE_OPPTB_800;  break;
         case 0x1C2F: oppIdx = EFUSE_OPPNT_1000; break;
         case 0x1FDF: oppIdx = EFUSE_OPP100_300; break;
         case 0x1F9F: oppIdx = EFUSE_OPP100_600; break;
      
         default:
         {
            oppIdx = OPP_NONE;
            break;
         }
      }
   }
   
   return(oppIdx);
}

/* \brief This function initializes the CORE PLL 
 * 
 * \param none
 *
 * \return none
 *
 */
void CorePLLInit(void)
{
   volatile unsigned int regVal = 0;

   /* Enable the Core PLL */

   /* Put the PLL in bypass mode */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_CORE) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_CORE_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_CORE_DPLL_EN_DPLL_MN_BYP_MODE;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_CORE) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_CORE) &
                 CM_WKUP_CM_IDLEST_DPLL_CORE_ST_MN_BYPASS));

   /* Set the multipler and divider values for the PLL */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_CORE) =
         ((COREPLL_M << CM_WKUP_CM_CLKSEL_DPLL_CORE_DPLL_MULT_SHIFT) |
          (COREPLL_N << CM_WKUP_CM_CLKSEL_DPLL_CORE_DPLL_DIV_SHIFT));

   /* Configure the High speed dividers */
   /* Set M4 divider */    
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M4_DPLL_CORE);
   regVal = regVal & ~CM_WKUP_CM_DIV_M4_DPLL_CORE_HSDIVIDER_CLKOUT1_DIV;
   regVal = regVal | (COREPLL_HSD_M4 << CM_WKUP_CM_DIV_M4_DPLL_CORE_HSDIVIDER_CLKOUT1_DIV_SHIFT);
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M4_DPLL_CORE) = regVal;
   
   /* Set M5 divider */    
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M5_DPLL_CORE);
   regVal = regVal & ~CM_WKUP_CM_DIV_M5_DPLL_CORE_HSDIVIDER_CLKOUT2_DIV;
   regVal = regVal | (COREPLL_HSD_M5 << CM_WKUP_CM_DIV_M5_DPLL_CORE_HSDIVIDER_CLKOUT2_DIV_SHIFT);
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M5_DPLL_CORE) = regVal;        
       
   /* Set M6 divider */    
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M6_DPLL_CORE);
   regVal = regVal & ~CM_WKUP_CM_DIV_M6_DPLL_CORE_HSDIVIDER_CLKOUT3_DIV;
   regVal = regVal | (COREPLL_HSD_M6 << CM_WKUP_CM_DIV_M6_DPLL_CORE_HSDIVIDER_CLKOUT3_DIV_SHIFT);
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M6_DPLL_CORE) = regVal;         

   /* Now LOCK the PLL by enabling it */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_CORE) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_CORE_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_CORE_DPLL_EN;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_CORE) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_CORE) &
                 CM_WKUP_CM_IDLEST_DPLL_CORE_ST_DPLL_CLK));
}

/* \brief This function initializes the DISPLAY PLL
 * 
 * \param none
 *
 * \return none
 *
 */
void DisplayPLLInit(void)
{
   volatile unsigned int regVal = 0;

   /* Put the PLL in bypass mode */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DISP) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_DISP_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_DISP_DPLL_EN_DPLL_MN_BYP_MODE;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DISP) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_DISP) &
                 CM_WKUP_CM_IDLEST_DPLL_DISP_ST_MN_BYPASS));

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_DISP) &=
         ~(CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_DIV |
           CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_MULT);

   /* Set the multipler and divider values for the PLL */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_DISP) |=
         ((DISPLL_M << CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_MULT_SHIFT) |
          (DISPLL_N << CM_WKUP_CM_CLKSEL_DPLL_DISP_DPLL_DIV_SHIFT));

   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_DISP);
   regVal = regVal & ~CM_WKUP_CM_DIV_M2_DPLL_DISP_DPLL_CLKOUT_DIV;
   regVal = regVal | DISPLL_M2;

   /* Set the CLKOUT2 divider */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_DISP) = regVal;

   /* Now LOCK the PLL by enabling it */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DISP) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_DISP_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_DISP_DPLL_EN;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DISP) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_DISP) &
                 CM_WKUP_CM_IDLEST_DPLL_DISP_ST_DPLL_CLK));
}

/* \brief This function initializes the PER PLL
 * 
 * \param none
 *
 * \return none
 *
 */
void PerPLLInit(void)
{
   volatile unsigned int regVal = 0;

   /* Put the PLL in bypass mode */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_PER) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN_DPLL_MN_BYP_MODE;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_PER) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_PER) &
                 CM_WKUP_CM_IDLEST_DPLL_PER_ST_MN_BYPASS));

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_PERIPH) &=
         ~(CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_MULT | CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_DIV);

   /* Set the multipler and divider values for the PLL */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_PERIPH) |=
       ((PERPLL_M << CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_MULT_SHIFT) |
        (PERPLL_N << CM_WKUP_CM_CLKSEL_DPLL_PERIPH_DPLL_DIV_SHIFT));

   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_PER);
   regVal = regVal & ~CM_WKUP_CM_DIV_M2_DPLL_PER_DPLL_CLKOUT_DIV;
   regVal = regVal | PERPLL_M2;

   /* Set the CLKOUT2 divider */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_PER) = regVal;
   
   /* Now LOCK the PLL by enabling it */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_PER) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_PER_DPLL_EN;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_PER) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_PER) &
                 CM_WKUP_CM_IDLEST_DPLL_PER_ST_DPLL_CLK));
}

/* \brief This function initializes the DDR PLL
 * 
 * \param none
 *
 * \return none
 *
 */
void DDRPLLInit(unsigned int freqMult)
{
   volatile unsigned int regVal = 0;

   /* Put the PLL in bypass mode */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DDR) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_DDR_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN_DPLL_MN_BYP_MODE;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DDR) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_DDR) &
                 CM_WKUP_CM_IDLEST_DPLL_DDR_ST_MN_BYPASS));

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_DDR) &=
         ~(CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_MULT | CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_DIV);

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_DDR) |=
         ((freqMult << CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_MULT_SHIFT) |
          (DDRPLL_N << CM_WKUP_CM_CLKSEL_DPLL_DDR_DPLL_DIV_SHIFT));

   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_DDR);
   regVal = regVal & ~CM_WKUP_CM_DIV_M2_DPLL_DDR_DPLL_CLKOUT_DIV;
   regVal = regVal | DDRPLL_M2;

   /* Set the CLKOUT2 divider */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_DDR) = regVal;

   /* Now LOCK the PLL by enabling it */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DDR) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_DDR_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_DDR_DPLL_EN;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_DDR) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_DDR) &
                 CM_WKUP_CM_IDLEST_DPLL_DDR_ST_DPLL_CLK));
}

/*
 * \brief This function initializes the MPU PLL
 *
 * \param  none
 *
 * \return none
 */
void MPUPLLInit(unsigned int freqMult)
{
   volatile unsigned int regVal = 0;

   /* Put the PLL in bypass mode */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_MPU) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN_DPLL_MN_BYP_MODE;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_MPU) = regVal;

   /* Wait for DPLL to go in to bypass mode */
   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_MPU) &
                 CM_WKUP_CM_IDLEST_DPLL_MPU_ST_MN_BYPASS));

   /* Clear the MULT and DIV field of DPLL_MPU register */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_MPU) &=
         ~(CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_MULT | CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_DIV);

   /* Set the multiplier and divider values for the PLL */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKSEL_DPLL_MPU) |=
         ((freqMult << CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_MULT_SHIFT) |
          (MPUPLL_N << CM_WKUP_CM_CLKSEL_DPLL_MPU_DPLL_DIV_SHIFT));

   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_MPU);

   regVal = regVal & ~CM_WKUP_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_DIV;

   regVal = regVal | MPUPLL_M2;

   /* Set the CLKOUT2 divider */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_DIV_M2_DPLL_MPU) = regVal;

   /* Now LOCK the PLL by enabling it */
   regVal = HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_MPU) &
                  ~CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN;

   regVal |= CM_WKUP_CM_CLKMODE_DPLL_MPU_DPLL_EN;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKMODE_DPLL_MPU) = regVal;

   while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_IDLEST_DPLL_MPU) &
                 CM_WKUP_CM_IDLEST_DPLL_MPU_ST_DPLL_CLK));
}

/* \brief This function initializes the interface clock 
 * 
 * \param none
 *
 * \return none
 *
 */
void InterfaceClkInit(void)
{
   HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |= CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

   while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
                CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) |= CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

   while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
                CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_CM_PER_REGS + CM_PER_L4FW_CLKCTRL) |= CM_PER_L4FW_CLKCTRL_MODULEMODE_ENABLE;

   while((HWREG(SOC_CM_PER_REGS + CM_PER_L4FW_CLKCTRL) &
                CM_PER_L4FW_CLKCTRL_MODULEMODE) != CM_PER_L4FW_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) |=
                                         CM_PER_L4FW_CLKCTRL_MODULEMODE_ENABLE;
   while((HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
                CM_WKUP_L4WKUP_CLKCTRL_MODULEMODE) != CM_PER_L4FW_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |= CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

   while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) != CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_CM_PER_REGS + CM_PER_L4HS_CLKCTRL) |= CM_PER_L4HS_CLKCTRL_MODULEMODE_ENABLE;

   while((HWREG(SOC_CM_PER_REGS + CM_PER_L4HS_CLKCTRL) &
                CM_PER_L4HS_CLKCTRL_MODULEMODE) != CM_PER_L4HS_CLKCTRL_MODULEMODE_ENABLE);
}

/* \brief This function initializes the power domain transition.
 * 
 * \param none
 *
 * \return none
 *
 */
void PowerDomainTransitionInit(void)
{
   HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL)   |= CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) |= CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL)    |= CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   HWREG(SOC_CM_PER_REGS + CM_PER_L4FW_CLKSTCTRL) |= CM_PER_L4FW_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL)  |= CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;
}

/*
 * \brief This function sets up various PLLs  
 *
 * \param  none
 *
 * \return none
 */
void PLLInit(void)
{
   MPUPLLInit(oppTable[oppMaxIdx].pllMult);
   CorePLLInit();
   PerPLLInit();
   DDRPLLInit(freqMultDDR);
   InterfaceClkInit();
   PowerDomainTransitionInit();
   DisplayPLLInit();
}

/*
 * \brief Configure I2C0 on which the PMIC is interfaced
 *
 * \param  none
 *
 * \return none
 */
void SetupI2C(void)
{
   I2C0ModuleClkConfig();

   I2CPinMuxSetup(I2C_0);

   /* Put i2c in reset/disabled state */
   I2CMasterDisable(SOC_I2C_0_REGS);

   I2CSoftReset(SOC_I2C_0_REGS);

   /* Disable auto Idle functionality */
   I2CAutoIdleDisable(SOC_I2C_0_REGS);

   /* Configure i2c bus speed to 100khz */
   I2CMasterInitExpClk(SOC_I2C_0_REGS, I2C_SYSTEM_CLOCK, I2C_INTERNAL_CLOCK, I2C_OUTPUT_CLOCK);
   I2CMasterEnable(SOC_I2C_0_REGS);

   while(!I2CSystemStatusGet(SOC_I2C_0_REGS));
}

/*
 * \brief Clear the status of all interrupts 
 *
 * \param  none.
 *
 * \return none
 */

void CleanupInterrupts(void)
{
   I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INTERRUPT_FLAG_TO_CLR);
}

/*
 * \brief Transmits data over I2C0 bus 
 *
 * \param  none
 *
 * \return none
 */
void SetupI2CTransmit(unsigned int dcount)
{
   I2CSetDataCount(SOC_I2C_0_REGS, dcount);

   CleanupInterrupts();

   I2CMasterControl(SOC_I2C_0_REGS, I2C_CFG_MST_TX);

   I2CMasterStart(SOC_I2C_0_REGS);

   while(I2CMasterBusBusy(SOC_I2C_0_REGS) == 0);

   while((I2C_INT_TRANSMIT_READY == (I2CMasterIntRawStatus(SOC_I2C_0_REGS)
                                    & I2C_INT_TRANSMIT_READY)) && dcount--)
   {
      I2CMasterDataPut(SOC_I2C_0_REGS, dataToSlave[tCount++]);

      I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_TRANSMIT_READY);
   }

   I2CMasterStop(SOC_I2C_0_REGS);

   while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_STOP_CONDITION));

   I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_STOP_CONDITION);
}

/*
 * \brief Receives data over I2C0 bus 
 *
 * \param  dcount - Number of bytes to receive.
 *
 * \return none
 */

void SetupReception(unsigned int dcount)
{
   I2CSetDataCount(SOC_I2C_0_REGS, 1);

   CleanupInterrupts();

   I2CMasterControl(SOC_I2C_0_REGS, I2C_CFG_MST_TX);

   I2CMasterStart(SOC_I2C_0_REGS);

   while(I2CMasterBusBusy(SOC_I2C_0_REGS) == 0);

   I2CMasterDataPut(SOC_I2C_0_REGS, dataToSlave[tCount]);

   I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_TRANSMIT_READY);

   while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_ADRR_READY_ACESS));

   I2CSetDataCount(SOC_I2C_0_REGS, dcount);

   CleanupInterrupts();

   I2CMasterControl(SOC_I2C_0_REGS, I2C_CFG_MST_RX);

   I2CMasterStart(SOC_I2C_0_REGS);

   /* Wait till the bus if free */
   while(I2CMasterBusBusy(SOC_I2C_0_REGS) == 0);

   /* Read the data from slave of dcount */
   while((dcount--))
   {
      while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_RECV_READY));

      dataFromSlave[rCount++] = I2CMasterDataGet(SOC_I2C_0_REGS);

      I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_RECV_READY);
   }

   I2CMasterStop(SOC_I2C_0_REGS);

   while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_STOP_CONDITION));

   I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_STOP_CONDITION);
}

/*
 * \brief Generic function that can read a TPS65217 register
 *
 * \param regOffset -  Source register address
 *
 * \return dest     -  Place holder for read bytes.
 */
void TPS65217RegRead(unsigned char regOffset, unsigned char* dest)
{
   dataToSlave[0] = regOffset;
   tCount = 0;

   SetupReception(1);

   *dest = dataFromSlave[0];
}

/**
 *  \brief            - Generic function that can write a TPS65217 PMIC
 *                      register or bit field regardless of protection
 *                      level.
 *
 * \param prot_level  - Register password protection.
 *                      use PROT_LEVEL_NONE, PROT_LEVEL_1, or PROT_LEVEL_2
 * \param regOffset:  - Register address to write.
 *
 * \param dest_val    - Value to write.
 *
 * \param mask        - Bit mask (8 bits) to be applied.  Function will only
 *                      change bits that are set in the bit mask.
 *
 * \return:            None.
 */
void TPS65217RegWrite(unsigned char port_level, unsigned char regOffset,
                      unsigned char dest_val, unsigned char mask)
{
   unsigned char read_val;
   unsigned xor_reg;

   dataToSlave[0] = regOffset;
   tCount = 0;
   rCount = 0;

   if(mask != MASK_ALL_BITS)
   {
      SetupReception(1);

      read_val = dataFromSlave[0];
      read_val &= (~mask);
      read_val |= (dest_val & mask);
      dest_val = read_val;
   }

   if(port_level > 0)
   {
      xor_reg = regOffset ^ PASSWORD_UNLOCK;

      dataToSlave[0] = PASSWORD;
      dataToSlave[1] = xor_reg;
      tCount = 0;

      SetupI2CTransmit(2);
   }

   dataToSlave[0] = regOffset;
   dataToSlave[1] = dest_val;
   tCount = 0;

   SetupI2CTransmit(2);

   if(port_level == PROT_LEVEL_2)
   {
      dataToSlave[0] = PASSWORD;
      dataToSlave[1] = xor_reg;
      tCount = 0;

      SetupI2CTransmit(2);

      dataToSlave[0] = regOffset;
      dataToSlave[1] = dest_val;
      tCount = 0;

      SetupI2CTransmit(2);
   }
}

/**
 *  \brief              - Controls output voltage setting for the DCDC1,
 *                        DCDC2, or DCDC3 control registers in the PMIC.
 *
 * \param  dc_cntrl_reg   DCDC Control Register address.
 *                        Must be DEFDCDC1, DEFDCDC2, or DEFDCDC3.
 *
 * \param  volt_sel       Register value to set.  See PMIC TRM for value set.
 *
 * \return:               None.
 */
void TPS65217VoltageUpdate(unsigned char dc_cntrl_reg, unsigned char volt_sel)
{
   /* set voltage level */
   TPS65217RegWrite(PROT_LEVEL_2, dc_cntrl_reg, volt_sel, MASK_ALL_BITS);

   /* set GO bit to initiate voltage transition */
   TPS65217RegWrite(PROT_LEVEL_2, DEFSLEW, DCDC_GO, DCDC_GO);
}

/**
 *  \brief set VDD1_OP voltage value.
 *
 * \param  opVolSelector  - VDD2_OP voltage value.
 *
 * \return None.
 */
void SetVdd1OpVoltage(unsigned int opVolSelector)
{
   /* Set DCDC2 (MPU) voltage */
   TPS65217VoltageUpdate(DEFDCDC2, opVolSelector);
}

/*
 * \brief Configures the VDD OP voltage. 
 *
 * \param  none.
 *
 * \return none
 */

void ConfigVddOpVoltage(void)
{
   SetupI2C();

   unsigned char pmic_status = 0;

   /* Configure PMIC slave address */
   I2CMasterSlaveAddrSet(SOC_I2C_0_REGS, PMIC_TPS65217_I2C_SLAVE_ADDR);

   TPS65217RegRead(STATUS, &pmic_status);

   /* Increase USB current limit to 1300mA */
   TPS65217RegWrite(PROT_LEVEL_NONE, POWER_PATH, USB_INPUT_CUR_LIMIT_1300MA,
                    USB_INPUT_CUR_LIMIT_MASK);

   /* Set DCDC2 (MPU) voltage to 1.275V */
   TPS65217VoltageUpdate(DEFDCDC2, DCDC_VOLT_SEL_1275MV);
   
   /*
    * The LDO3 and LDO4 does not need to set, because the
    * default values from the TPS65217 will be used.
    *
    * The BeagleBone use a TPS65217B with the following default voltages:
    *   LDO3 = 3.3V
    *   LDO4 = 3.3V
    *
    * The BeagleBone Black use a TPS65217C with the following default voltages:
    *   LDO3 = 1.8V
    *   LDO4 = 3.3V
    */

#if 0 /* Old BeagleBone code */    
   /* Set LDO3, LDO4 output voltage to 3.3V */
   //TPS65217RegWrite(PROT_LEVEL_2, DEFLS1, LDO_VOLTAGE_OUT_3_3, LDO_MASK);
   //TPS65217RegWrite(PROT_LEVEL_2, DEFLS2, LDO_VOLTAGE_OUT_3_3, LDO_MASK);
#endif    
}

/*
 * \brief This function sets up the DDR PHY
 *
 * \param  none
 *
 * \return none
 */
static void DDR2PhyInit(void)
{
   /* Enable VTP */
   HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) |= CONTROL_VTP_CTRL_ENABLE;
   HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) &= ~CONTROL_VTP_CTRL_CLRZ;
   HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) |= CONTROL_VTP_CTRL_CLRZ;
   while((HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) & CONTROL_VTP_CTRL_READY) !=
                CONTROL_VTP_CTRL_READY);

   /* DDR PHY CMD0 Register configuration */
   HWREG(CMD0_SLAVE_RATIO_0)   = DDR2_CMD0_SLAVE_RATIO_0;
   HWREG(CMD0_SLAVE_FORCE_0)   = DDR2_CMD0_SLAVE_FORCE_0;
   HWREG(CMD0_SLAVE_DELAY_0)   = DDR2_CMD0_SLAVE_DELAY_0;
   HWREG(CMD0_LOCK_DIFF_0)     = DDR2_CMD0_LOCK_DIFF_0;
   HWREG(CMD0_INVERT_CLKOUT_0) = DDR2_CMD0_INVERT_CLKOUT_0;

   /* DDR PHY CMD1 Register configuration */
   HWREG(CMD1_SLAVE_RATIO_0)   = DDR2_CMD1_SLAVE_RATIO_0;
   HWREG(CMD1_SLAVE_FORCE_0)   = DDR2_CMD1_SLAVE_FORCE_0;
   HWREG(CMD1_SLAVE_DELAY_0)   = DDR2_CMD1_SLAVE_DELAY_0;
   HWREG(CMD1_LOCK_DIFF_0)     = DDR2_CMD1_LOCK_DIFF_0;
   HWREG(CMD1_INVERT_CLKOUT_0) = DDR2_CMD1_INVERT_CLKOUT_0;

   /* DDR PHY CMD2 Register configuration */
   HWREG(CMD2_SLAVE_RATIO_0)   = DDR2_CMD2_SLAVE_RATIO_0;
   HWREG(CMD2_SLAVE_FORCE_0)   = DDR2_CMD2_SLAVE_FORCE_0;
   HWREG(CMD2_SLAVE_DELAY_0)   = DDR2_CMD2_SLAVE_DELAY_0;
   HWREG(CMD2_LOCK_DIFF_0)     = DDR2_CMD2_LOCK_DIFF_0;
   HWREG(CMD2_INVERT_CLKOUT_0) = DDR2_CMD2_INVERT_CLKOUT_0;

   /* DATA macro configuration */
   HWREG(DATA0_RD_DQS_SLAVE_RATIO_0)  = DDR2_DATA0_RD_DQS_SLAVE_RATIO_0;
   HWREG(DATA0_WR_DQS_SLAVE_RATIO_0)  = DDR2_DATA0_WR_DQS_SLAVE_RATIO_0;
   HWREG(DATA0_FIFO_WE_SLAVE_RATIO_0) = DDR2_DATA0_FIFO_WE_SLAVE_RATIO_0;
   HWREG(DATA0_WR_DATA_SLAVE_RATIO_0) = DDR2_DATA0_WR_DATA_SLAVE_RATIO_0;

   HWREG(DATA1_RD_DQS_SLAVE_RATIO_0)  = DDR2_DATA1_RD_DQS_SLAVE_RATIO_0;
   HWREG(DATA1_WR_DQS_SLAVE_RATIO_0)  = DDR2_DATA1_WR_DQS_SLAVE_RATIO_0;
   HWREG(DATA1_FIFO_WE_SLAVE_RATIO_0) = DDR2_DATA1_FIFO_WE_SLAVE_RATIO_0;
   HWREG(DATA1_WR_DATA_SLAVE_RATIO_0) = DDR2_DATA1_WR_DATA_SLAVE_RATIO_0;
}
/*
 * \brief This function sets up the DDR PHY
 *
 * \param  none
 *
 * \return none
 */
static void DDR3PhyInit(void)
{
   /* Enable VTP */
   HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) |= CONTROL_VTP_CTRL_ENABLE;
   HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) &= ~CONTROL_VTP_CTRL_CLRZ;
   HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) |= CONTROL_VTP_CTRL_CLRZ;
   while((HWREG(SOC_CONTROL_REGS + CONTROL_VTP_CTRL) & CONTROL_VTP_CTRL_READY) !=
                CONTROL_VTP_CTRL_READY);

   /* DDR PHY CMD0 Register configuration */
   HWREG(CMD0_SLAVE_RATIO_0)   = DDR3_CMD0_SLAVE_RATIO_0;
   HWREG(CMD0_INVERT_CLKOUT_0) = DDR3_CMD0_INVERT_CLKOUT_0;

   /* DDR PHY CMD1 Register configuration */
   HWREG(CMD1_SLAVE_RATIO_0)   = DDR3_CMD1_SLAVE_RATIO_0;
   HWREG(CMD1_INVERT_CLKOUT_0) = DDR3_CMD1_INVERT_CLKOUT_0;

   /* DDR PHY CMD2 Register configuration */
   HWREG(CMD2_SLAVE_RATIO_0)   = DDR3_CMD2_SLAVE_RATIO_0;
   HWREG(CMD2_INVERT_CLKOUT_0) = DDR3_CMD2_INVERT_CLKOUT_0;

   /* DATA macro configuration */
   HWREG(DATA0_RD_DQS_SLAVE_RATIO_0)  = DDR3_DATA0_RD_DQS_SLAVE_RATIO_0;
   HWREG(DATA0_WR_DQS_SLAVE_RATIO_0)  = DDR3_DATA0_WR_DQS_SLAVE_RATIO_0;
   HWREG(DATA0_FIFO_WE_SLAVE_RATIO_0) = DDR3_DATA0_FIFO_WE_SLAVE_RATIO_0;
   HWREG(DATA0_WR_DATA_SLAVE_RATIO_0) = DDR3_DATA0_WR_DATA_SLAVE_RATIO_0;
   HWREG(DATA1_RD_DQS_SLAVE_RATIO_0)  = DDR3_DATA0_RD_DQS_SLAVE_RATIO_1;
   HWREG(DATA1_WR_DQS_SLAVE_RATIO_0)  = DDR3_DATA0_WR_DQS_SLAVE_RATIO_1;
   HWREG(DATA1_FIFO_WE_SLAVE_RATIO_0) = DDR3_DATA0_FIFO_WE_SLAVE_RATIO_1;
   HWREG(DATA1_WR_DATA_SLAVE_RATIO_0) = DDR3_DATA0_WR_DATA_SLAVE_RATIO_1;
}


/* \brief This function initializes the DDR2
 * 
 * \param none
 *
 * \return none
 *
 */
void DDR3Init(void)
{
   /* DDR3 Phy Initialization */
   DDR3PhyInit();

   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CMD_IOCTRL(0))  = DDR3_CONTROL_DDR_CMD_IOCTRL_0;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CMD_IOCTRL(1))  = DDR3_CONTROL_DDR_CMD_IOCTRL_1;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CMD_IOCTRL(2))  = DDR3_CONTROL_DDR_CMD_IOCTRL_2;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_DATA_IOCTRL(0)) = DDR3_CONTROL_DDR_DATA_IOCTRL_0;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_DATA_IOCTRL(1)) = DDR3_CONTROL_DDR_DATA_IOCTRL_1;

   /* IO to work for DDR3 */
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_IO_CTRL) &= DDR3_CONTROL_DDR_IO_CTRL;

   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CKE_CTRL) |= CONTROL_DDR_CKE_CTRL_DDR_CKE_CTRL;

   HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_1)      = DDR3_EMIF_DDR_PHY_CTRL_1;
   HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_1_SHDW) = DDR3_EMIF_DDR_PHY_CTRL_1_SHDW;

   /*
    * EMIF_DDR_PHY_CTRL_2 does not exist, see:
    * https://e2e.ti.com/support/embedded/starterware/f/790/t/397867 
    */
//   HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_2)      = DDR3_EMIF_DDR_PHY_CTRL_2;

   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_1)         = DDR3_EMIF_SDRAM_TIM_1;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_1_SHDW)    = DDR3_EMIF_SDRAM_TIM_1_SHDW;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_2)         = DDR3_EMIF_SDRAM_TIM_2;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_2_SHDW)    = DDR3_EMIF_SDRAM_TIM_2_SHDW;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_3)         = DDR3_EMIF_SDRAM_TIM_3;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_3_SHDW)    = DDR3_EMIF_SDRAM_TIM_3_SHDM;

   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_REF_CTRL)      = DDR3_EMIF_SDRAM_REF_CTRL_VAL1;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_REF_CTRL_SHDW) = DDR3_EMIF_SDRAM_REF_CTRL_SHDW_VAL1;
   
   HWREG(SOC_EMIF_0_REGS + EMIF_ZQ_CONFIG)           = DDR3_EMIF_ZQ_CONFIG_VAL;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_CONFIG)        = DDR3_EMIF_SDRAM_CONFIG;
  
   /* The CONTROL_SECURE_EMIF_SDRAM_CONFIG register exports SDRAM configuration 
      information to the EMIF */
   HWREG(SOC_CONTROL_REGS + CONTROL_SECURE_EMIF_SDRAM_CONFIG) = DDR3_EMIF_SDRAM_CONFIG;
}

/* \brief This function initializes the DDR2
 * 
 * \param none
 *
 * \return none
 *
 */
void DDR2Init(void)
{
   volatile unsigned int delay = 5000;

   /* DDR2 Phy Initialization */
   DDR2PhyInit();

   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CMD_IOCTRL(0))  = DDR2_CONTROL_DDR_CMD_IOCTRL_0;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CMD_IOCTRL(1))  = DDR2_CONTROL_DDR_CMD_IOCTRL_1;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CMD_IOCTRL(2))  = DDR2_CONTROL_DDR_CMD_IOCTRL_2;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_DATA_IOCTRL(0)) = DDR2_CONTROL_DDR_DATA_IOCTRL_0;
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_DATA_IOCTRL(1)) = DDR2_CONTROL_DDR_DATA_IOCTRL_1;

   /* IO to work for DDR2 */
   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_IO_CTRL) &= DDR2_CONTROL_DDR_IO_CTRL;

   HWREG(SOC_CONTROL_REGS + CONTROL_DDR_CKE_CTRL) |= CONTROL_DDR_CKE_CTRL_DDR_CKE_CTRL;


   HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_1) = DDR2_EMIF_DDR_PHY_CTRL_1;

   /* Dynamic Power Down */
   if((DEVICE_VERSION_2_0 == deviceVersion) ||
      (DEVICE_VERSION_2_1 == deviceVersion))
   {
      HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_1) |= DDR2_EMIF_DDR_PHY_CTRL_1_DY_PWRDN;
   }

   HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_1_SHDW) = DDR2_EMIF_DDR_PHY_CTRL_1_SHDW;

   /* Dynamic Power Down */
   if((DEVICE_VERSION_2_0 == deviceVersion) ||
      (DEVICE_VERSION_2_1 == deviceVersion))
   {
       HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_1_SHDW) |= DDR2_EMIF_DDR_PHY_CTRL_1_SHDW_DY_PWRDN;
   }

   HWREG(SOC_EMIF_0_REGS + EMIF_DDR_PHY_CTRL_2) = DDR2_EMIF_DDR_PHY_CTRL_2;

   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_1)         = DDR2_EMIF_SDRAM_TIM_1;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_1_SHDW)    = DDR2_EMIF_SDRAM_TIM_1_SHDW;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_2)         = DDR2_EMIF_SDRAM_TIM_2;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_2_SHDW)    = DDR2_EMIF_SDRAM_TIM_2_SHDW;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_3)         = DDR2_EMIF_SDRAM_TIM_3;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_TIM_3_SHDW)    = DDR2_EMIF_SDRAM_TIM_3_SHDM;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_CONFIG)        = DDR2_EMIF_SDRAM_CONFIG;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_REF_CTRL)      = DDR2_EMIF_SDRAM_REF_CTRL_VAL1;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_REF_CTRL_SHDW) = DDR2_EMIF_SDRAM_REF_CTRL_SHDW_VAL1;

   while(delay--);

   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_REF_CTRL)      = DDR2_EMIF_SDRAM_REF_CTRL_VAL2;
   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_REF_CTRL_SHDW) = DDR2_EMIF_SDRAM_REF_CTRL_SHDW_VAL2;

   HWREG(SOC_EMIF_0_REGS + EMIF_SDRAM_CONFIG) = DDR2_EMIF_SDRAM_CONFIG;
  
   /* The CONTROL_SECURE_EMIF_SDRAM_CONFIG register exports SDRAM configuration 
      information to the EMIF */
   HWREG(SOC_CONTROL_REGS + CONTROL_SECURE_EMIF_SDRAM_CONFIG) = DDR2_EMIF_SDRAM_CONFIG;
}

/* \brief This function initializes the EMIF
 * 
 * \param none
 *
 * \return none
 *
 */
void EMIFInit(void)
{
   volatile unsigned int regVal;

   /* Enable the clocks for EMIF */
   regVal = HWREG(SOC_CM_PER_REGS + CM_PER_EMIF_FW_CLKCTRL) &
               ~(CM_PER_EMIF_FW_CLKCTRL_MODULEMODE);

   regVal |= CM_PER_EMIF_FW_CLKCTRL_MODULEMODE_ENABLE;

   HWREG(SOC_CM_PER_REGS + CM_PER_EMIF_FW_CLKCTRL) = regVal;

   regVal = HWREG(SOC_CM_PER_REGS + CM_PER_EMIF_CLKCTRL) &
               ~(CM_PER_EMIF_CLKCTRL_MODULEMODE);

   regVal |= CM_PER_EMIF_CLKCTRL_MODULEMODE_ENABLE;

   HWREG(SOC_CM_PER_REGS + CM_PER_EMIF_CLKCTRL) = regVal;

   while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
         (CM_PER_L3_CLKSTCTRL_CLKACTIVITY_EMIF_GCLK |
          CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK)) !=
         (CM_PER_L3_CLKSTCTRL_CLKACTIVITY_EMIF_GCLK |
          CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));
}

/*
 * \brief This function is used to initialize and configure the MMC.
 *
 * \param  none.
 *
 * \return none
*/
void BlPlatformMMCSDSetup(void)
{
   unsigned int          DelayCnt;
   volatile unsigned int Value = 0;

   /* Enable clock for MMCSD and Do the PINMUXing */
   HSMMCSDPinMuxSetup();
   HSMMCSDModuleClkConfig();

   /* Wait for the MMCSD clock */   
   for (DelayCnt = 0; DelayCnt < 1000000; DelayCnt++)
   {
      Value++;
   }
   
   (void)Value;
}

/*
 * \brief This function copies the image form MMCSD to RAM
 *
 * \param  none
 *
 * \return Status of the copy operation.
*/
unsigned int BlPlatformMMCSDImageCopy()
{
   int ret;
   
   ret = HSMMCSDInit();
   if (TRUE == ret)
   {
      ret = HSMMCSDImageCopy();
   }   

   return(ret);
}

/*
 * \brief This function Initializes Pll, DDR and Uart
 *
 * \param  none
 *
 * \return none
*/
void BlPlatformConfig(void)
{
   unsigned short BeagleBoneBlack = 0;
   
   deviceVersion = DeviceVersionGet();
   ConfigVddOpVoltage();

   oppMaxIdx = BootMaxOppGet();

   /* The BeagleBone Black use a 1000 MHz CPU */   
   if (EFUSE_OPPNT_1000 == oppMaxIdx)
   {
      BeagleBoneBlack = 1;
   }
   
   /* Set the MPU voltage */
   SetVdd1OpVoltage(oppTable[oppMaxIdx].pmicVolt);

   /* Disable watchdog */
   HWREG(SOC_WDT_1_REGS + WDT_WSPR) = 0xAAAAu;
   while(HWREG(SOC_WDT_1_REGS + WDT_WWPS) != 0x00);

   HWREG(SOC_WDT_1_REGS + WDT_WSPR) = 0x5555u;
   while(HWREG(SOC_WDT_1_REGS + WDT_WWPS) != 0x00);

   /* Configure DDR frequency */
   if (0 == BeagleBoneBlack)
   {
      freqMultDDR = DDRPLL_M_DDR2;
   }
   else
   {
      freqMultDDR = 400;
   }   
     
   /* Set all PLLs */   
   PLLInit();

   /* Enable the control module */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) =
         CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

   /* EMIF Initialization */
   EMIFInit();

   /* DDR Initialization */
   if (0 == BeagleBoneBlack)
   {
      DDR2Init();
   }
   else
   {
      DDR3Init();
   }   
}

/******************************************************************************
**                              END OF FILE
*******************************************************************************/
