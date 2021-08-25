// NAME=OMAP35x register and bit definitions.
//
// Copyright (c) 2015 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef OMAP35x_h
#define OMAP35x_h


#ifndef SCM_INTERFACE_BASE
#define SCM_INTERFACE_BASE 0x48002000
#endif

#define CONTROL_REVISION (*(volatile unsigned long *)0x48002000)
#define CONTROL_REVISION_OFFSET 0x0

#define CONTROL_SYSCONFIG (*(volatile unsigned long *)0x48002010)
#define CONTROL_SYSCONFIG_OFFSET 0x10

#ifndef SCM_PADCONFS_BASE
#define SCM_PADCONFS_BASE 0x48002030
#endif

#define CONTROL_PADCONF_SDRC_D0 (*(volatile unsigned long *)0x48002030)
#define CONTROL_PADCONF_SDRC_D0_OFFSET 0x0

#define CONTROL_PADCONF_SDRC_D2 (*(volatile unsigned long *)0x48002034)
#define CONTROL_PADCONF_SDRC_D2_OFFSET 0x4

#define CONTROL_PADCONF_SDRC_D4 (*(volatile unsigned long *)0x48002038)
#define CONTROL_PADCONF_SDRC_D4_OFFSET 0x8

#define CONTROL_PADCONF_SDRC_D6 (*(volatile unsigned long *)0x4800203C)
#define CONTROL_PADCONF_SDRC_D6_OFFSET 0xC

#define CONTROL_PADCONF_SDRC_D8 (*(volatile unsigned long *)0x48002040)
#define CONTROL_PADCONF_SDRC_D8_OFFSET 0x10

#define CONTROL_PADCONF_SDRC_D10 (*(volatile unsigned long *)0x48002044)
#define CONTROL_PADCONF_SDRC_D10_OFFSET 0x14

#define CONTROL_PADCONF_SDRC_D12 (*(volatile unsigned long *)0x48002048)
#define CONTROL_PADCONF_SDRC_D12_OFFSET 0x18

#define CONTROL_PADCONF_SDRC_D14 (*(volatile unsigned long *)0x4800204C)
#define CONTROL_PADCONF_SDRC_D14_OFFSET 0x1C

#define CONTROL_PADCONF_SDRC_D16 (*(volatile unsigned long *)0x48002050)
#define CONTROL_PADCONF_SDRC_D16_OFFSET 0x20

#define CONTROL_PADCONF_SDRC_D18 (*(volatile unsigned long *)0x48002054)
#define CONTROL_PADCONF_SDRC_D18_OFFSET 0x24

#define CONTROL_PADCONF_SDRC_D20 (*(volatile unsigned long *)0x48002058)
#define CONTROL_PADCONF_SDRC_D20_OFFSET 0x28

#define CONTROL_PADCONF_SDRC_D22 (*(volatile unsigned long *)0x4800205C)
#define CONTROL_PADCONF_SDRC_D22_OFFSET 0x2C

#define CONTROL_PADCONF_SDRC_D24 (*(volatile unsigned long *)0x48002060)
#define CONTROL_PADCONF_SDRC_D24_OFFSET 0x30

#define CONTROL_PADCONF_SDRC_D26 (*(volatile unsigned long *)0x48002064)
#define CONTROL_PADCONF_SDRC_D26_OFFSET 0x34

#define CONTROL_PADCONF_SDRC_D28 (*(volatile unsigned long *)0x48002068)
#define CONTROL_PADCONF_SDRC_D28_OFFSET 0x38

#define CONTROL_PADCONF_SDRC_D30 (*(volatile unsigned long *)0x4800206C)
#define CONTROL_PADCONF_SDRC_D30_OFFSET 0x3C

#define CONTROL_PADCONF_SDRC_CLK (*(volatile unsigned long *)0x48002070)
#define CONTROL_PADCONF_SDRC_CLK_OFFSET 0x40

#define CONTROL_PADCONF_SDRC_DQS1 (*(volatile unsigned long *)0x48002074)
#define CONTROL_PADCONF_SDRC_DQS1_OFFSET 0x44

#define CONTROL_PADCONF_SDRC_DQS3 (*(volatile unsigned long *)0x48002078)
#define CONTROL_PADCONF_SDRC_DQS3_OFFSET 0x48

#define CONTROL_PADCONF_GPMC_A2 (*(volatile unsigned long *)0x4800207C)
#define CONTROL_PADCONF_GPMC_A2_OFFSET 0x4C

#define CONTROL_PADCONF_GPMC_A4 (*(volatile unsigned long *)0x48002080)
#define CONTROL_PADCONF_GPMC_A4_OFFSET 0x50

#define CONTROL_PADCONF_GPMC_A6 (*(volatile unsigned long *)0x48002084)
#define CONTROL_PADCONF_GPMC_A6_OFFSET 0x54

#define CONTROL_PADCONF_GPMC_A8 (*(volatile unsigned long *)0x48002088)
#define CONTROL_PADCONF_GPMC_A8_OFFSET 0x58

#define CONTROL_PADCONF_GPMC_A10 (*(volatile unsigned long *)0x4800208C)
#define CONTROL_PADCONF_GPMC_A10_OFFSET 0x5C

#define CONTROL_PADCONF_GPMC_D1 (*(volatile unsigned long *)0x48002090)
#define CONTROL_PADCONF_GPMC_D1_OFFSET 0x60

#define CONTROL_PADCONF_GPMC_D3 (*(volatile unsigned long *)0x48002094)
#define CONTROL_PADCONF_GPMC_D3_OFFSET 0x64

#define CONTROL_PADCONF_GPMC_D5 (*(volatile unsigned long *)0x48002098)
#define CONTROL_PADCONF_GPMC_D5_OFFSET 0x68

#define CONTROL_PADCONF_GPMC_D7 (*(volatile unsigned long *)0x4800209C)
#define CONTROL_PADCONF_GPMC_D7_OFFSET 0x6C

#define CONTROL_PADCONF_GPMC_D9 (*(volatile unsigned long *)0x480020A0)
#define CONTROL_PADCONF_GPMC_D9_OFFSET 0x70

#define CONTROL_PADCONF_GPMC_D11 (*(volatile unsigned long *)0x480020A4)
#define CONTROL_PADCONF_GPMC_D11_OFFSET 0x74

#define CONTROL_PADCONF_GPMC_D13 (*(volatile unsigned long *)0x480020A8)
#define CONTROL_PADCONF_GPMC_D13_OFFSET 0x78

#define CONTROL_PADCONF_GPMC_D15 (*(volatile unsigned long *)0x480020AC)
#define CONTROL_PADCONF_GPMC_D15_OFFSET 0x7C

#define CONTROL_PADCONF_GPMC_NCS1 (*(volatile unsigned long *)0x480020B0)
#define CONTROL_PADCONF_GPMC_NCS1_OFFSET 0x80

#define CONTROL_PADCONF_GPMC_NCS3 (*(volatile unsigned long *)0x480020B4)
#define CONTROL_PADCONF_GPMC_NCS3_OFFSET 0x84

#define CONTROL_PADCONF_GPMC_NCS5 (*(volatile unsigned long *)0x480020B8)
#define CONTROL_PADCONF_GPMC_NCS5_OFFSET 0x88

#define CONTROL_PADCONF_GPMC_NCS7 (*(volatile unsigned long *)0x480020BC)
#define CONTROL_PADCONF_GPMC_NCS7_OFFSET 0x8C

#define CONTROL_PADCONF_GPMC_NADV_ALE (*(volatile unsigned long *)0x480020C0)
#define CONTROL_PADCONF_GPMC_NADV_ALE_OFFSET 0x90

#define CONTROL_PADCONF_GPMC_NWE (*(volatile unsigned long *)0x480020C4)
#define CONTROL_PADCONF_GPMC_NWE_OFFSET 0x94

#define CONTROL_PADCONF_GPMC_NBE1 (*(volatile unsigned long *)0x480020C8)
#define CONTROL_PADCONF_GPMC_NBE1_OFFSET 0x98

#define CONTROL_PADCONF_GPMC_WAIT0 (*(volatile unsigned long *)0x480020CC)
#define CONTROL_PADCONF_GPMC_WAIT0_OFFSET 0x9C

#define CONTROL_PADCONF_GPMC_WAIT2 (*(volatile unsigned long *)0x480020D0)
#define CONTROL_PADCONF_GPMC_WAIT2_OFFSET 0xA0

#define CONTROL_PADCONF_DSS_PCLK (*(volatile unsigned long *)0x480020D4)
#define CONTROL_PADCONF_DSS_PCLK_OFFSET 0xA4

#define CONTROL_PADCONF_DSS_VSYNC (*(volatile unsigned long *)0x480020D8)
#define CONTROL_PADCONF_DSS_VSYNC_OFFSET 0xA8

#define CONTROL_PADCONF_DSS_DATA0 (*(volatile unsigned long *)0x480020DC)
#define CONTROL_PADCONF_DSS_DATA0_OFFSET 0xAC

#define CONTROL_PADCONF_DSS_DATA2 (*(volatile unsigned long *)0x480020E0)
#define CONTROL_PADCONF_DSS_DATA2_OFFSET 0xB0

#define CONTROL_PADCONF_DSS_DATA4 (*(volatile unsigned long *)0x480020E4)
#define CONTROL_PADCONF_DSS_DATA4_OFFSET 0xB4

#define CONTROL_PADCONF_DSS_DATA6 (*(volatile unsigned long *)0x480020E8)
#define CONTROL_PADCONF_DSS_DATA6_OFFSET 0xB8

#define CONTROL_PADCONF_DSS_DATA8 (*(volatile unsigned long *)0x480020EC)
#define CONTROL_PADCONF_DSS_DATA8_OFFSET 0xBC

#define CONTROL_PADCONF_DSS_DATA10 (*(volatile unsigned long *)0x480020F0)
#define CONTROL_PADCONF_DSS_DATA10_OFFSET 0xC0

#define CONTROL_PADCONF_DSS_DATA12 (*(volatile unsigned long *)0x480020F4)
#define CONTROL_PADCONF_DSS_DATA12_OFFSET 0xC4

#define CONTROL_PADCONF_DSS_DATA14 (*(volatile unsigned long *)0x480020F8)
#define CONTROL_PADCONF_DSS_DATA14_OFFSET 0xC8

#define CONTROL_PADCONF_DSS_DATA16 (*(volatile unsigned long *)0x480020FC)
#define CONTROL_PADCONF_DSS_DATA16_OFFSET 0xCC

#define CONTROL_PADCONF_DSS_DATA18 (*(volatile unsigned long *)0x48002100)
#define CONTROL_PADCONF_DSS_DATA18_OFFSET 0xD0

#define CONTROL_PADCONF_DSS_DATA20 (*(volatile unsigned long *)0x48002104)
#define CONTROL_PADCONF_DSS_DATA20_OFFSET 0xD4

#define CONTROL_PADCONF_DSS_DATA22 (*(volatile unsigned long *)0x48002108)
#define CONTROL_PADCONF_DSS_DATA22_OFFSET 0xD8

#define CONTROL_PADCONF_MCBSP2_FSX (*(volatile unsigned long *)0x4800213C)
#define CONTROL_PADCONF_MCBSP2_FSX_OFFSET 0x10C

#define CONTROL_PADCONF_MCBSP2_DR (*(volatile unsigned long *)0x48002140)
#define CONTROL_PADCONF_MCBSP2_DR_OFFSET 0x110

#define CONTROL_PADCONF_MMC1_CLK (*(volatile unsigned long *)0x48002144)
#define CONTROL_PADCONF_MMC1_CLK_OFFSET 0x114

#define CONTROL_PADCONF_MMC1_DAT0 (*(volatile unsigned long *)0x48002148)
#define CONTROL_PADCONF_MMC1_DAT0_OFFSET 0x118

#define CONTROL_PADCONF_MMC1_DAT2 (*(volatile unsigned long *)0x4800214C)
#define CONTROL_PADCONF_MMC1_DAT2_OFFSET 0x11C

#define CONTROL_PADCONF_MMC1_DAT4 (*(volatile unsigned long *)0x48002150)
#define CONTROL_PADCONF_MMC1_DAT4_OFFSET 0x120

#define CONTROL_PADCONF_MMC1_DAT6 (*(volatile unsigned long *)0x48002154)
#define CONTROL_PADCONF_MMC1_DAT6_OFFSET 0x124

#define CONTROL_PADCONF_MMC2_CLK (*(volatile unsigned long *)0x48002158)
#define CONTROL_PADCONF_MMC2_CLK_OFFSET 0x128

#define CONTROL_PADCONF_MMC2_DAT0 (*(volatile unsigned long *)0x4800215C)
#define CONTROL_PADCONF_MMC2_DAT0_OFFSET 0x12C

#define CONTROL_PADCONF_MMC2_DAT2 (*(volatile unsigned long *)0x48002160)
#define CONTROL_PADCONF_MMC2_DAT2_OFFSET 0x130

#define CONTROL_PADCONF_MMC2_DAT4 (*(volatile unsigned long *)0x48002164)
#define CONTROL_PADCONF_MMC2_DAT4_OFFSET 0x134

#define CONTROL_PADCONF_MMC2_DAT6 (*(volatile unsigned long *)0x48002168)
#define CONTROL_PADCONF_MMC2_DAT6_OFFSET 0x138

#define CONTROL_PADCONF_MCBSP3_DX (*(volatile unsigned long *)0x4800216C)
#define CONTROL_PADCONF_MCBSP3_DX_OFFSET 0x13C

#define CONTROL_PADCONF_MCBSP3_CLKX (*(volatile unsigned long *)0x48002170)
#define CONTROL_PADCONF_MCBSP3_CLKX_OFFSET 0x140

#define CONTROL_PADCONF_UART2_CTS (*(volatile unsigned long *)0x48002174)
#define CONTROL_PADCONF_UART2_CTS_OFFSET 0x144

#define CONTROL_PADCONF_UART2_TX (*(volatile unsigned long *)0x48002178)
#define CONTROL_PADCONF_UART2_TX_OFFSET 0x148

#define CONTROL_PADCONF_UART1_TX (*(volatile unsigned long *)0x4800217C)
#define CONTROL_PADCONF_UART1_TX_OFFSET 0x14C

#define CONTROL_PADCONF_UART1_CTS (*(volatile unsigned long *)0x48002180)
#define CONTROL_PADCONF_UART1_CTS_OFFSET 0x150

#define CONTROL_PADCONF_MCBSP4_CLKX (*(volatile unsigned long *)0x48002184)
#define CONTROL_PADCONF_MCBSP4_CLKX_OFFSET 0x154

#define CONTROL_PADCONF_MCBSP4_DX (*(volatile unsigned long *)0x48002188)
#define CONTROL_PADCONF_MCBSP4_DX_OFFSET 0x158

#define CONTROL_PADCONF_MCBSP1_CLKR (*(volatile unsigned long *)0x4800218C)
#define CONTROL_PADCONF_MCBSP1_CLKR_OFFSET 0x15C

#define CONTROL_PADCONF_MCBSP1_DX (*(volatile unsigned long *)0x48002190)
#define CONTROL_PADCONF_MCBSP1_DX_OFFSET 0x160

#define CONTROL_PADCONF_MCBSP_CLKS (*(volatile unsigned long *)0x48002194)
#define CONTROL_PADCONF_MCBSP_CLKS_OFFSET 0x164

#define CONTROL_PADCONF_MCBSP1_CLKX (*(volatile unsigned long *)0x48002198)
#define CONTROL_PADCONF_MCBSP1_CLKX_OFFSET 0x168

#define CONTROL_PADCONF_UART3_RTS_SD (*(volatile unsigned long *)0x4800219C)
#define CONTROL_PADCONF_UART3_RTS_SD_OFFSET 0x16C

#define CONTROL_PADCONF_UART3_TX_IRTX (*(volatile unsigned long *)0x480021A0)
#define CONTROL_PADCONF_UART3_TX_IRTX_OFFSET 0x170

#define CONTROL_PADCONF_I2C1_SDA (*(volatile unsigned long *)0x480021BC)
#define CONTROL_PADCONF_I2C1_SDA_OFFSET 0x18C

#define CONTROL_PADCONF_I2C2_SDA (*(volatile unsigned long *)0x480021C0)
#define CONTROL_PADCONF_I2C2_SDA_OFFSET 0x190

#define CONTROL_PADCONF_I2C3_SDA (*(volatile unsigned long *)0x480021C4)
#define CONTROL_PADCONF_I2C3_SDA_OFFSET 0x194

#define CONTROL_PADCONF_MCSPI1_CLK (*(volatile unsigned long *)0x480021C8)
#define CONTROL_PADCONF_MCSPI1_CLK_OFFSET 0x198

#define CONTROL_PADCONF_MCSPI1_SOMI (*(volatile unsigned long *)0x480021CC)
#define CONTROL_PADCONF_MCSPI1_SOMI_OFFSET 0x19C

#define CONTROL_PADCONF_MCSPI1_CS1 (*(volatile unsigned long *)0x480021D0)
#define CONTROL_PADCONF_MCSPI1_CS1_OFFSET 0x1A0

#define CONTROL_PADCONF_MCSPI1_CS3 (*(volatile unsigned long *)0x480021D4)
#define CONTROL_PADCONF_MCSPI1_CS3_OFFSET 0x1A4

#define CONTROL_PADCONF_MCSPI2_SIMO (*(volatile unsigned long *)0x480021D8)
#define CONTROL_PADCONF_MCSPI2_SIMO_OFFSET 0x1A8

#define CONTROL_PADCONF_MCSPI2_CS0 (*(volatile unsigned long *)0x480021DC)
#define CONTROL_PADCONF_MCSPI2_CS0_OFFSET 0x1AC

#define CONTROL_PADCONF_SYS_NIRQ (*(volatile unsigned long *)0x480021E0)
#define CONTROL_PADCONF_SYS_NIRQ_OFFSET 0x1B0

#define CONTROL_PADCONF_SDRC_CKE1 (*(volatile unsigned long *)0x48002264)
#define CONTROL_PADCONF_SDRC_CKE1_OFFSET 0x234

#define CONTROL_PADCONF_ETK_CLK (*(volatile unsigned long *)0x480025D8)
#define CONTROL_PADCONF_ETK_CLK_OFFSET 0x5A8

#define CONTROL_PADCONF_ETK_D0 (*(volatile unsigned long *)0x480025DC)
#define CONTROL_PADCONF_ETK_D0_OFFSET 0x5AC

#define CONTROL_PADCONF_ETK_D2 (*(volatile unsigned long *)0x480025E0)
#define CONTROL_PADCONF_ETK_D2_OFFSET 0x5B0

#define CONTROL_PADCONF_ETK_D4 (*(volatile unsigned long *)0x480025E4)
#define CONTROL_PADCONF_ETK_D4_OFFSET 0x5B4

#define CONTROL_PADCONF_ETK_D6 (*(volatile unsigned long *)0x480025E8)
#define CONTROL_PADCONF_ETK_D6_OFFSET 0x5B8

#define CONTROL_PADCONF_ETK_D8 (*(volatile unsigned long *)0x480025EC)
#define CONTROL_PADCONF_ETK_D8_OFFSET 0x5BC

#define CONTROL_PADCONF_ETK_D10 (*(volatile unsigned long *)0x480025F0)
#define CONTROL_PADCONF_ETK_D10_OFFSET 0x5C0

#define CONTROL_PADCONF_ETK_D12 (*(volatile unsigned long *)0x480025F4)
#define CONTROL_PADCONF_ETK_D12_OFFSET 0x5C4

#define CONTROL_PADCONF_ETK_D14 (*(volatile unsigned long *)0x480025F8)
#define CONTROL_PADCONF_ETK_D14_OFFSET 0x5C8

#ifndef SCM_GENERAL_BASE
#define SCM_GENERAL_BASE 0x48002270
#endif

#define CONTROL_PADCONF_OFF (*(volatile unsigned long *)0x48002270)
#define CONTROL_PADCONF_OFF_OFFSET 0x0

#define CONTROL_DEVCONF0 (*(volatile unsigned long *)0x48002274)
#define CONTROL_DEVCONF0_OFFSET 0x4

#define CONTROL_MEM_DFTRW0 (*(volatile unsigned long *)0x48002278)
#define CONTROL_MEM_DFTRW0_OFFSET 0x8

#define CONTROL_MEM_DFTRW1 (*(volatile unsigned long *)0x4800227C)
#define CONTROL_MEM_DFTRW1_OFFSET 0xC

#define CONTROL_MSUSPENDMUX_0 (*(volatile unsigned long *)0x48002290)
#define CONTROL_MSUSPENDMUX_0_OFFSET 0x20

#define CONTROL_MSUSPENDMUX_1 (*(volatile unsigned long *)0x48002294)
#define CONTROL_MSUSPENDMUX_1_OFFSET 0x24

#define CONTROL_MSUSPENDMUX_2 (*(volatile unsigned long *)0x48002298)
#define CONTROL_MSUSPENDMUX_2_OFFSET 0x28

#define CONTROL_MSUSPENDMUX_3 (*(volatile unsigned long *)0x4800229C)
#define CONTROL_MSUSPENDMUX_3_OFFSET 0x2C

#define CONTROL_MSUSPENDMUX_4 (*(volatile unsigned long *)0x480022A0)
#define CONTROL_MSUSPENDMUX_4_OFFSET 0x30

#define CONTROL_MSUSPENDMUX_5 (*(volatile unsigned long *)0x480022A4)
#define CONTROL_MSUSPENDMUX_5_OFFSET 0x34

#define CONTROL_PROT_CTRL (*(volatile unsigned long *)0x480022B0)
#define CONTROL_PROT_CTRL_OFFSET 0x40

#define CONTROL_DEVCONF1 (*(volatile unsigned long *)0x480022D8)
#define CONTROL_DEVCONF1_OFFSET 0x68

#define CONTROL_CSIRXFE (*(volatile unsigned long *)0x480022DC)
#define CONTROL_CSIRXFE_OFFSET 0x6C

#define CONTROL_PROT_STATUS (*(volatile unsigned long *)0x480022E0)
#define CONTROL_PROT_STATUS_OFFSET 0x70

#define CONTROL_PROT_ERR_STATUS (*(volatile unsigned long *)0x480022E4)
#define CONTROL_PROT_ERR_STATUS_OFFSET 0x74

#define CONTROL_PROT_ERR_STATUS_DEBUG (*(volatile unsigned long *)0x480022E8)
#define CONTROL_PROT_ERR_STATUS_DEBUG_OFFSET 0x78

#define CONTROL_STATUS (*(volatile unsigned long *)0x480022F0)
#define CONTROL_STATUS_OFFSET 0x80

#define CONTROL_GENERAL_PURPOSE_STATUS (*(volatile unsigned long *)0x480022F4)
#define CONTROL_GENERAL_PURPOSE_STATUS_OFFSET 0x84

#define CONTROL_RPUB_KEY_H_0 (*(volatile unsigned long *)0x48002300)
#define CONTROL_RPUB_KEY_H_0_OFFSET 0x90

#define CONTROL_RPUB_KEY_H_1 (*(volatile unsigned long *)0x48002304)
#define CONTROL_RPUB_KEY_H_1_OFFSET 0x94

#define CONTROL_RPUB_KEY_H_2 (*(volatile unsigned long *)0x48002308)
#define CONTROL_RPUB_KEY_H_2_OFFSET 0x98

#define CONTROL_RPUB_KEY_H_3 (*(volatile unsigned long *)0x4800230C)
#define CONTROL_RPUB_KEY_H_3_OFFSET 0x9C

#define CONTROL_RPUB_KEY_H_4 (*(volatile unsigned long *)0x48002310)
#define CONTROL_RPUB_KEY_H_4_OFFSET 0xA0

#define CONTROL_USB_CONF_0 (*(volatile unsigned long *)0x48002370)
#define CONTROL_USB_CONF_0_OFFSET 0x100

#define CONTROL_USB_CONF_1 (*(volatile unsigned long *)0x48002374)
#define CONTROL_USB_CONF_1_OFFSET 0x104

#define CONTROL_DEBOBS_0 (*(volatile unsigned long *)0x48002420)
#define CONTROL_DEBOBS_0_OFFSET 0x1B0

#define CONTROL_DEBOBS_1 (*(volatile unsigned long *)0x48002424)
#define CONTROL_DEBOBS_1_OFFSET 0x1B4

#define CONTROL_DEBOBS_2 (*(volatile unsigned long *)0x48002428)
#define CONTROL_DEBOBS_2_OFFSET 0x1B8

#define CONTROL_DEBOBS_3 (*(volatile unsigned long *)0x4800242C)
#define CONTROL_DEBOBS_3_OFFSET 0x1BC

#define CONTROL_DEBOBS_4 (*(volatile unsigned long *)0x48002430)
#define CONTROL_DEBOBS_4_OFFSET 0x1C0

#define CONTROL_DEBOBS_5 (*(volatile unsigned long *)0x48002434)
#define CONTROL_DEBOBS_5_OFFSET 0x1C4

#define CONTROL_DEBOBS_6 (*(volatile unsigned long *)0x48002438)
#define CONTROL_DEBOBS_6_OFFSET 0x1C8

#define CONTROL_DEBOBS_7 (*(volatile unsigned long *)0x4800243C)
#define CONTROL_DEBOBS_7_OFFSET 0x1CC

#define CONTROL_DEBOBS_8 (*(volatile unsigned long *)0x48002440)
#define CONTROL_DEBOBS_8_OFFSET 0x1D0

#define CONTROL_DSS_DPLL_SPREADING (*(volatile unsigned long *)0x48002450)
#define CONTROL_DSS_DPLL_SPREADING_OFFSET 0x1E0

#define CONTROL_CORE_DPLL_SPREADING (*(volatile unsigned long *)0x48002454)
#define CONTROL_CORE_DPLL_SPREADING_OFFSET 0x1E4

#define CONTROL_PER_DPLL_SPREADING (*(volatile unsigned long *)0x48002458)
#define CONTROL_PER_DPLL_SPREADING_OFFSET 0x1E8

#define CONTROL_USBHOST_DPLL_SPREADING (*(volatile unsigned long *)0x4800245C)
#define CONTROL_USBHOST_DPLL_SPREADING_OFFSET 0x1EC

#define CONTROL_DPF_OCM_RAM_FW_ADDR_MATCH (*(volatile unsigned long *)0x48002498)
#define CONTROL_DPF_OCM_RAM_FW_ADDR_MATCH_OFFSET 0x228

#define CONTROL_DPF_OCM_RAM_FW_REQINFO (*(volatile unsigned long *)0x4800249C)
#define CONTROL_DPF_OCM_RAM_FW_REQINFO_OFFSET 0x22C

#define CONTROL_DPF_OCM_RAM_FW_WR (*(volatile unsigned long *)0x480024A0)
#define CONTROL_DPF_OCM_RAM_FW_WR_OFFSET 0x230

#define CONTROL_DPF_R4_GPMC_FW_ADDR_MCH (*(volatile unsigned long *)0x480024A4)
#define CONTROL_DPF_R4_GPMC_FW_ADDR_MCH_OFFSET 0x234

#define CONTROL_DPF_R4_GPMC_FW_REQINFO (*(volatile unsigned long *)0x480024A8)
#define CONTROL_DPF_R4_GPMC_FW_REQINFO_OFFSET 0x238

#define CONTROL_DPF_R4_GPMC_FW_WR (*(volatile unsigned long *)0x480024AC)
#define CONTROL_DPF_R4_GPMC_FW_WR_OFFSET 0x23C

#define CONTROL_APE_FW_DEFAULT_SECURE_LOCKN (*(volatile unsigned long *)0x480024BC)
#define CONTROL_APE_FW_DEFAULT_SECURE_LOCKN_OFFSET 0x24C

#define CONTROL_OCMROM_SECURE_DEBUG (*(volatile unsigned long *)0x480024C0)
#define CONTROL_OCMROM_SECURE_DEBUG_OFFSET 0x250

#define CONTROL_EXT_SEC_CONTROL (*(volatile unsigned long *)0x480024D4)
#define CONTROL_EXT_SEC_CONTROL_OFFSET 0x264

#define CONTROL_IDCODE (*(volatile unsigned long *)0x4830A204)
#define CONTROL_IDCODE_OFFSET 0x307F94

#ifndef SCM_MEM_WKUP_BASE
#define SCM_MEM_WKUP_BASE 0x48002600
#endif

#define SAVE_RESTORE_MEM (*(volatile unsigned long *)0x48002600)
#define SAVE_RESTORE_MEM_OFFSET 0x0

#ifndef SCM_PADCONFS_WKUP_BASE
#define SCM_PADCONFS_WKUP_BASE 0x48002A00
#endif

#define CONTROL_PADCONF_I2C4_SCL (*(volatile unsigned long *)0x48002A00)
#define CONTROL_PADCONF_I2C4_SCL_OFFSET 0x0

#define CONTROL_PADCONF_SYS_32K (*(volatile unsigned long *)0x48002A04)
#define CONTROL_PADCONF_SYS_32K_OFFSET 0x4

#define CONTROL_PADCONF_SYS_NRESWARM (*(volatile unsigned long *)0x48002A08)
#define CONTROL_PADCONF_SYS_NRESWARM_OFFSET 0x8

#define CONTROL_PADCONF_SYS_BOOT1 (*(volatile unsigned long *)0x48002A0C)
#define CONTROL_PADCONF_SYS_BOOT1_OFFSET 0xC

#define CONTROL_PADCONF_SYS_BOOT3 (*(volatile unsigned long *)0x48002A10)
#define CONTROL_PADCONF_SYS_BOOT3_OFFSET 0x10

#define CONTROL_PADCONF_SYS_BOOT5 (*(volatile unsigned long *)0x48002A14)
#define CONTROL_PADCONF_SYS_BOOT5_OFFSET 0x14

#define CONTROL_PADCONF_SYS_OFF_MODE (*(volatile unsigned long *)0x48002A18)
#define CONTROL_PADCONF_SYS_OFF_MODE_OFFSET 0x18

#define CONTROL_PADCONF_JTAG_NTRST (*(volatile unsigned long *)0x48002A1C)
#define CONTROL_PADCONF_JTAG_NTRST_OFFSET 0x1C

#define CONTROL_PADCONF_JTAG_TMS_TMSC (*(volatile unsigned long *)0x48002A20)
#define CONTROL_PADCONF_JTAG_TMS_TMSC_OFFSET 0x20

#define CONTROL_PADCONF_JTAG_EMU0 (*(volatile unsigned long *)0x48002A24)
#define CONTROL_PADCONF_JTAG_EMU0_OFFSET 0x24

#define CONTROL_PADCONF_SAD2D_SWAKEUP (*(volatile unsigned long *)0x48002A4C)
#define CONTROL_PADCONF_SAD2D_SWAKEUP_OFFSET 0x4C

#define CONTROL_PADCONF_JTAG_TDO (*(volatile unsigned long *)0x48002A50)
#define CONTROL_PADCONF_JTAG_TDO_OFFSET 0x50

#ifndef SCM_GENERAL_WKUP_BASE
#define SCM_GENERAL_WKUP_BASE 0x48002A00
#endif

#define CONTROL_WKUP_CTRL (*(volatile unsigned long *)0x48002A5C)
#define CONTROL_WKUP_CTRL_OFFSET 0x5C

#define CONTROL_WKUP_DEBOBS_0 (*(volatile unsigned long *)0x48002A68)
#define CONTROL_WKUP_DEBOBS_0_OFFSET 0x68

#define CONTROL_WKUP_DEBOBS_1 (*(volatile unsigned long *)0x48002A6C)
#define CONTROL_WKUP_DEBOBS_1_OFFSET 0x6C

#define CONTROL_WKUP_DEBOBS_2 (*(volatile unsigned long *)0x48002A70)
#define CONTROL_WKUP_DEBOBS_2_OFFSET 0x70

#define CONTROL_WKUP_DEBOBS_3 (*(volatile unsigned long *)0x48002A74)
#define CONTROL_WKUP_DEBOBS_3_OFFSET 0x74

#define CONTROL_WKUP_DEBOBS_4 (*(volatile unsigned long *)0x48002A78)
#define CONTROL_WKUP_DEBOBS_4_OFFSET 0x78

#ifndef CM_IVA2_BASE
#define CM_IVA2_BASE 0x48004000
#endif

#define CM_FCLKEN_IVA2 (*(volatile unsigned *)0x48004000)
#define CM_FCLKEN_IVA2_OFFSET 0x0

#define CM_CLKEN_PLL_IVA2 (*(volatile unsigned *)0x48004004)
#define CM_CLKEN_PLL_IVA2_OFFSET 0x4

#define CM_IDLEST_IVA2 (*(volatile unsigned *)0x48004020)
#define CM_IDLEST_IVA2_OFFSET 0x20

#define CM_IDLEST_PLL_IVA2 (*(volatile unsigned *)0x48004024)
#define CM_IDLEST_PLL_IVA2_OFFSET 0x24

#define CM_AUTOIDLE_PLL_IVA2 (*(volatile unsigned *)0x48004034)
#define CM_AUTOIDLE_PLL_IVA2_OFFSET 0x34

#define CM_CLKSEL1_PLL_IVA2 (*(volatile unsigned *)0x48004040)
#define CM_CLKSEL1_PLL_IVA2_OFFSET 0x40

#define CM_CLKSEL2_PLL_IVA2 (*(volatile unsigned *)0x48004044)
#define CM_CLKSEL2_PLL_IVA2_OFFSET 0x44

#define CM_CLKSTCTRL_IVA2 (*(volatile unsigned *)0x48004048)
#define CM_CLKSTCTRL_IVA2_OFFSET 0x48

#define CM_CLKSTST_IVA2 (*(volatile unsigned *)0x4800404C)
#define CM_CLKSTST_IVA2_OFFSET 0x4C

#ifndef CM_OCP_BASE
#define CM_OCP_BASE 0x48004800
#endif

#define CM_REVISION (*(volatile unsigned *)0x48004800)
#define CM_REVISION_OFFSET 0x0

#define CM_SYSCONFIG (*(volatile unsigned *)0x48004810)
#define CM_SYSCONFIG_OFFSET 0x10

#ifndef CM_MPU_BASE
#define CM_MPU_BASE 0x48004900
#endif

#define CM_CLKEN_PLL_MPU (*(volatile unsigned *)0x48004904)
#define CM_CLKEN_PLL_MPU_OFFSET 0x4

#define CM_IDLEST_MPU (*(volatile unsigned *)0x48004920)
#define CM_IDLEST_MPU_OFFSET 0x20

#define CM_IDLEST_PLL_MPU (*(volatile unsigned *)0x48004924)
#define CM_IDLEST_PLL_MPU_OFFSET 0x24

#define CM_AUTOIDLE_PLL_MPU (*(volatile unsigned *)0x48004934)
#define CM_AUTOIDLE_PLL_MPU_OFFSET 0x34

#define CM_CLKSEL1_PLL_MPU (*(volatile unsigned *)0x48004940)
#define CM_CLKSEL1_PLL_MPU_OFFSET 0x40

#define CM_CLKSEL2_PLL_MPU (*(volatile unsigned *)0x48004944)
#define CM_CLKSEL2_PLL_MPU_OFFSET 0x44

#define CM_CLKSTCTRL_MPU (*(volatile unsigned *)0x48004948)
#define CM_CLKSTCTRL_MPU_OFFSET 0x48

#define CM_CLKSTST_MPU (*(volatile unsigned *)0x4800494C)
#define CM_CLKSTST_MPU_OFFSET 0x4C

#ifndef CM_CORE_BASE
#define CM_CORE_BASE 0x48004A00
#endif

#define CM_FCLKEN1_CORE (*(volatile unsigned *)0x48004A00)
#define CM_FCLKEN1_CORE_OFFSET 0x0

#define CM_FCLKEN3_CORE (*(volatile unsigned *)0x48004A08)
#define CM_FCLKEN3_CORE_OFFSET 0x8

#define CM_ICLKEN1_CORE (*(volatile unsigned *)0x48004A10)
#define CM_ICLKEN1_CORE_OFFSET 0x10

#define CM_ICLKEN3_CORE (*(volatile unsigned *)0x48004A18)
#define CM_ICLKEN3_CORE_OFFSET 0x18

#define CM_IDLEST1_CORE (*(volatile unsigned *)0x48004A20)
#define CM_IDLEST1_CORE_OFFSET 0x20

#define CM_IDLEST3_CORE (*(volatile unsigned *)0x48004A28)
#define CM_IDLEST3_CORE_OFFSET 0x28

#define CM_AUTOIDLE1_CORE (*(volatile unsigned *)0x48004A30)
#define CM_AUTOIDLE1_CORE_OFFSET 0x30

#define CM_AUTOIDLE3_CORE (*(volatile unsigned *)0x48004A38)
#define CM_AUTOIDLE3_CORE_OFFSET 0x38

#define CM_CLKSEL_CORE (*(volatile unsigned *)0x48004A40)
#define CM_CLKSEL_CORE_OFFSET 0x40

#define CM_CLKSTCTRL_CORE (*(volatile unsigned *)0x48004A48)
#define CM_CLKSTCTRL_CORE_OFFSET 0x48

#define CM_CLKSTST_CORE (*(volatile unsigned *)0x48004A4C)
#define CM_CLKSTST_CORE_OFFSET 0x4C

#ifndef CM_SGX_BASE
#define CM_SGX_BASE 0x48004B00
#endif

#define CM_FCLKEN_SGX (*(volatile unsigned *)0x48004B00)
#define CM_FCLKEN_SGX_OFFSET 0x0

#define CM_ICLKEN_SGX (*(volatile unsigned *)0x48004B10)
#define CM_ICLKEN_SGX_OFFSET 0x10

#define CM_IDLEST_SGX (*(volatile unsigned *)0x48004B20)
#define CM_IDLEST_SGX_OFFSET 0x20

#define CM_CLKSEL_SGX (*(volatile unsigned *)0x48004B40)
#define CM_CLKSEL_SGX_OFFSET 0x40

#define CM_SLEEPDEP_SGX (*(volatile unsigned *)0x48004B44)
#define CM_SLEEPDEP_SGX_OFFSET 0x44

#define CM_CLKSTCTRL_SGX (*(volatile unsigned *)0x48004B48)
#define CM_CLKSTCTRL_SGX_OFFSET 0x48

#define CM_CLKSTST_SGX (*(volatile unsigned *)0x48004B4C)
#define CM_CLKSTST_SGX_OFFSET 0x4C

#ifndef CM_WKUP_BASE
#define CM_WKUP_BASE 0x48004C00
#endif

#define CM_FCLKEN_WKUP (*(volatile unsigned *)0x48004C00)
#define CM_FCLKEN_WKUP_OFFSET 0x0

#define CM_ICLKEN_WKUP (*(volatile unsigned *)0x48004C10)
#define CM_ICLKEN_WKUP_OFFSET 0x10

#define CM_IDLEST_WKUP (*(volatile unsigned *)0x48004C20)
#define CM_IDLEST_WKUP_OFFSET 0x20

#define CM_AUTOIDLE_WKUP (*(volatile unsigned *)0x48004C30)
#define CM_AUTOIDLE_WKUP_OFFSET 0x30

#define CM_CLKSEL_WKUP (*(volatile unsigned *)0x48004C40)
#define CM_CLKSEL_WKUP_OFFSET 0x40

#ifndef CM_CLK_BASE
#define CM_CLK_BASE 0x48004D00
#endif

#define CM_CLKEN_PLL (*(volatile unsigned *)0x48004D00)
#define CM_CLKEN_PLL_OFFSET 0x0

#define CM_CLKEN2_PLL (*(volatile unsigned *)0x48004D04)
#define CM_CLKEN2_PLL_OFFSET 0x4

#define CM_IDLEST_CKGEN (*(volatile unsigned *)0x48004D20)
#define CM_IDLEST_CKGEN_OFFSET 0x20

#define CM_IDLEST2_CKGEN (*(volatile unsigned *)0x48004D24)
#define CM_IDLEST2_CKGEN_OFFSET 0x24

#define CM_AUTOIDLE_PLL (*(volatile unsigned *)0x48004D30)
#define CM_AUTOIDLE_PLL_OFFSET 0x30

#define CM_AUTOIDLE2_PLL (*(volatile unsigned *)0x48004D34)
#define CM_AUTOIDLE2_PLL_OFFSET 0x34

#define CM_CLKSEL1_PLL (*(volatile unsigned *)0x48004D40)
#define CM_CLKSEL1_PLL_OFFSET 0x40

#define CM_CLKSEL2_PLL (*(volatile unsigned *)0x48004D44)
#define CM_CLKSEL2_PLL_OFFSET 0x44

#define CM_CLKSEL3_PLL (*(volatile unsigned *)0x48004D48)
#define CM_CLKSEL3_PLL_OFFSET 0x48

#define CM_CLKSEL4_PLL (*(volatile unsigned *)0x48004D4C)
#define CM_CLKSEL4_PLL_OFFSET 0x4C

#define CM_CLKSEL5_PLL (*(volatile unsigned *)0x48004D50)
#define CM_CLKSEL5_PLL_OFFSET 0x50

#define CM_CLKOUT_CTRL (*(volatile unsigned *)0x48004D70)
#define CM_CLKOUT_CTRL_OFFSET 0x70

#ifndef CM_DSS_BASE
#define CM_DSS_BASE 0x48004E00
#endif

#define CM_FCLKEN_DSS (*(volatile unsigned *)0x48004E00)
#define CM_FCLKEN_DSS_OFFSET 0x0

#define CM_ICLKEN_DSS (*(volatile unsigned *)0x48004E10)
#define CM_ICLKEN_DSS_OFFSET 0x10

#define CM_IDLEST_DSS (*(volatile unsigned *)0x48004E20)
#define CM_IDLEST_DSS_OFFSET 0x20

#define CM_AUTOIDLE_DSS (*(volatile unsigned *)0x48004E30)
#define CM_AUTOIDLE_DSS_OFFSET 0x30

#define CM_CLKSEL_DSS (*(volatile unsigned *)0x48004E40)
#define CM_CLKSEL_DSS_OFFSET 0x40

#define CM_SLEEPDEP_DSS (*(volatile unsigned *)0x48004E44)
#define CM_SLEEPDEP_DSS_OFFSET 0x44

#define CM_CLKSTCTRL_DSS (*(volatile unsigned *)0x48004E48)
#define CM_CLKSTCTRL_DSS_OFFSET 0x48

#define CM_CLKSTST_DSS (*(volatile unsigned *)0x48004E4C)
#define CM_CLKSTST_DSS_OFFSET 0x4C

#ifndef CM_CAM_BASE
#define CM_CAM_BASE 0x48004F00
#endif

#define CM_FCLKEN_CAM (*(volatile unsigned *)0x48004F00)
#define CM_FCLKEN_CAM_OFFSET 0x0

#define CM_ICLKEN_CAM (*(volatile unsigned *)0x48004F10)
#define CM_ICLKEN_CAM_OFFSET 0x10

#define CM_IDLEST_CAM (*(volatile unsigned *)0x48004F20)
#define CM_IDLEST_CAM_OFFSET 0x20

#define CM_AUTOIDLE_CAM (*(volatile unsigned *)0x48004F30)
#define CM_AUTOIDLE_CAM_OFFSET 0x30

#define CM_CLKSEL_CAM (*(volatile unsigned *)0x48004F40)
#define CM_CLKSEL_CAM_OFFSET 0x40

#define CM_SLEEPDEP_CAM (*(volatile unsigned *)0x48004F44)
#define CM_SLEEPDEP_CAM_OFFSET 0x44

#define CM_CLKSTCTRL_CAM (*(volatile unsigned *)0x48004F48)
#define CM_CLKSTCTRL_CAM_OFFSET 0x48

#define CM_CLKSTST_CAM (*(volatile unsigned *)0x48004F4C)
#define CM_CLKSTST_CAM_OFFSET 0x4C

#ifndef CM_PER_BASE
#define CM_PER_BASE 0x48005000
#endif

#define CM_FCLKEN_PER (*(volatile unsigned *)0x48005000)
#define CM_FCLKEN_PER_OFFSET 0x0

#define CM_ICLKEN_PER (*(volatile unsigned *)0x48005010)
#define CM_ICLKEN_PER_OFFSET 0x10

#define CM_IDLEST_PER (*(volatile unsigned *)0x48005020)
#define CM_IDLEST_PER_OFFSET 0x20

#define CM_AUTOIDLE_PER (*(volatile unsigned *)0x48005030)
#define CM_AUTOIDLE_PER_OFFSET 0x30

#define CM_CLKSEL_PER (*(volatile unsigned *)0x48005040)
#define CM_CLKSEL_PER_OFFSET 0x40

#define CM_SLEEPDEP_PER (*(volatile unsigned *)0x48005044)
#define CM_SLEEPDEP_PER_OFFSET 0x44

#define CM_CLKSTCTRL_PER (*(volatile unsigned *)0x48005048)
#define CM_CLKSTCTRL_PER_OFFSET 0x48

#define CM_CLKSTST_PER (*(volatile unsigned *)0x4800504C)
#define CM_CLKSTST_PER_OFFSET 0x4C

#ifndef CM_EMU_BASE
#define CM_EMU_BASE 0x48005100
#endif

#define CM_CLKSEL1_EMU (*(volatile unsigned *)0x48005140)
#define CM_CLKSEL1_EMU_OFFSET 0x40

#define CM_CLKSTCTRL_EMU (*(volatile unsigned *)0x48005148)
#define CM_CLKSTCTRL_EMU_OFFSET 0x48

#define CM_CLKSTST_EMU (*(volatile unsigned *)0x4800514C)
#define CM_CLKSTST_EMU_OFFSET 0x4C

#define CM_CLKSEL2_EMU (*(volatile unsigned *)0x48005150)
#define CM_CLKSEL2_EMU_OFFSET 0x50

#define CM_CLKSEL3_EMU (*(volatile unsigned *)0x48005154)
#define CM_CLKSEL3_EMU_OFFSET 0x54

#ifndef CM_Global_BASE
#define CM_Global_BASE 0x48005200
#endif

#define CM_POLCTRL (*(volatile unsigned *)0x4800529C)
#define CM_POLCTRL_OFFSET 0x9C

#ifndef CM_NEON_BASE
#define CM_NEON_BASE 0x48005300
#endif

#define CM_IDLETEST_NEON (*(volatile unsigned *)0x48005320)
#define CM_IDLETEST_NEON_OFFSET 0x20

#define CM_CLKSTCTRL_NEON (*(volatile unsigned *)0x48005348)
#define CM_CLKSTCTRL_NEON_OFFSET 0x48

#ifndef CM_USBHOST_BASE
#define CM_USBHOST_BASE 0x48005400
#endif

#define CM_FCLKEN_USBHOST (*(volatile unsigned *)0x48005400)
#define CM_FCLKEN_USBHOST_OFFSET 0x0

#define CM_ICLKEN_USBHOST (*(volatile unsigned *)0x48005410)
#define CM_ICLKEN_USBHOST_OFFSET 0x10

#define CM_IDLEST_USBHOST (*(volatile unsigned *)0x48005420)
#define CM_IDLEST_USBHOST_OFFSET 0x20

#define CM_AUTOIDLE_USBHOST (*(volatile unsigned *)0x48005430)
#define CM_AUTOIDLE_USBHOST_OFFSET 0x30

#define CM_SLEEPDEP_USBHOST (*(volatile unsigned *)0x48005444)
#define CM_SLEEPDEP_USBHOST_OFFSET 0x44

#define CM_CLKSTCTRL_USBHOST (*(volatile unsigned *)0x48005448)
#define CM_CLKSTCTRL_USBHOST_OFFSET 0x48

#define CM_CLKSTST_USBHOST (*(volatile unsigned *)0x4800544C)
#define CM_CLKSTST_USBHOST_OFFSET 0x4C

#ifndef DSS_BASE
#define DSS_BASE 0x48050000
#endif

#define DSS_REVISIONNUMBER (*(volatile unsigned *)0x48050000)
#define DSS_REVISIONNUMBER_OFFSET 0x0

#define DSS_SYSCONFIG (*(volatile unsigned long *)0x48050010)
#define DSS_SYSCONFIG_OFFSET 0x10

#define DSS_SYSSTATUS (*(volatile unsigned long *)0x48050014)
#define DSS_SYSSTATUS_OFFSET 0x14

#define DSS_IRQSTATUS (*(volatile unsigned long *)0x48050018)
#define DSS_IRQSTATUS_OFFSET 0x18

#define DSS_CONTROL (*(volatile unsigned long *)0x48050040)
#define DSS_CONTROL_OFFSET 0x40

#define DSS_SDI_CONTROL (*(volatile unsigned long *)0x48050044)
#define DSS_SDI_CONTROL_OFFSET 0x44

#define DSS_PLL_CONTROL (*(volatile unsigned long *)0x48050048)
#define DSS_PLL_CONTROL_OFFSET 0x48

#define DSS_SDI_STATUS (*(volatile unsigned long *)0x4805005C)
#define DSS_SDI_STATUS_OFFSET 0x5C

#ifndef DSS_DISPC_BASE
#define DSS_DISPC_BASE 0x48050400
#endif

#define DISPC_REVISIONNUMBER (*(volatile unsigned *)0x48050400)
#define DISPC_REVISIONNUMBER_OFFSET 0x0

#define DISPC_SYSCONFIG (*(volatile unsigned long *)0x48050410)
#define DISPC_SYSCONFIG_OFFSET 0x10

#define DISPC_SYSSTATUS (*(volatile unsigned long *)0x48050414)
#define DISPC_SYSSTATUS_OFFSET 0x14

#define DISPC_IRQSTATUS (*(volatile unsigned long *)0x48050418)
#define DISPC_IRQSTATUS_OFFSET 0x18

#define DISPC_IRQENABLE (*(volatile unsigned long *)0x4805041C)
#define DISPC_IRQENABLE_OFFSET 0x1C

#define DISPC_CONTROL (*(volatile unsigned long *)0x48050440)
#define DISPC_CONTROL_OFFSET 0x40

#define DISPC_CONFIG (*(volatile unsigned long *)0x48050444)
#define DISPC_CONFIG_OFFSET 0x44

#define DISPC_CAPABLE (*(volatile unsigned long *)0x48050448)
#define DISPC_CAPABLE_OFFSET 0x48

#define DISPC_DEFAULT_COLOR0 (*(volatile unsigned long *)0x4805044C)
#define DISPC_DEFAULT_COLOR0_OFFSET 0x4C

#define DISPC_DEFAULT_COLOR1 (*(volatile unsigned long *)0x48050450)
#define DISPC_DEFAULT_COLOR1_OFFSET 0x50

#define DISPC_TRANS_COLOR0 (*(volatile unsigned long *)0x48050454)
#define DISPC_TRANS_COLOR0_OFFSET 0x54

#define DISPC_TRANS_COLOR1 (*(volatile unsigned long *)0x48050458)
#define DISPC_TRANS_COLOR1_OFFSET 0x58

#define DISPC_LINE_STATUS (*(volatile unsigned long *)0x4805045C)
#define DISPC_LINE_STATUS_OFFSET 0x5C

#define DISPC_LINE_NUMBER (*(volatile unsigned long *)0x48050460)
#define DISPC_LINE_NUMBER_OFFSET 0x60

#define DISPC_TIMING_H (*(volatile unsigned long *)0x48050464)
#define DISPC_TIMING_H_OFFSET 0x64

#define DISPC_TIMING_V (*(volatile unsigned long *)0x48050468)
#define DISPC_TIMING_V_OFFSET 0x68

#define DISPC_POL_FREQ (*(volatile unsigned long *)0x4805046C)
#define DISPC_POL_FREQ_OFFSET 0x6C

#define DISPC_DIVISOR (*(volatile unsigned long *)0x48050470)
#define DISPC_DIVISOR_OFFSET 0x70

#define DISPC_GLOBAL_ALPHA (*(volatile unsigned long *)0x48050474)
#define DISPC_GLOBAL_ALPHA_OFFSET 0x74

#define DISPC_SIZE_DIG (*(volatile unsigned long *)0x48050478)
#define DISPC_SIZE_DIG_OFFSET 0x78

#define DISPC_SIZE_LCD (*(volatile unsigned long *)0x4805047C)
#define DISPC_SIZE_LCD_OFFSET 0x7C

#define DISPC_GFX_BA0 (*(volatile unsigned long *)0x48050480)
#define DISPC_GFX_BA0_OFFSET 0x80

#define DISPC_GFX_BA1 (*(volatile unsigned long *)0x48050484)
#define DISPC_GFX_BA1_OFFSET 0x84

#define DISPC_GFX_POSITION (*(volatile unsigned long *)0x48050488)
#define DISPC_GFX_POSITION_OFFSET 0x88

#define DISPC_GFX_SIZE (*(volatile unsigned long *)0x4805048C)
#define DISPC_GFX_SIZE_OFFSET 0x8C

#define DISPC_GFX_ATTRIBUTES (*(volatile unsigned long *)0x480504A0)
#define DISPC_GFX_ATTRIBUTES_OFFSET 0xA0

#define DISPC_GFX_FIFO_THRESHOLD (*(volatile unsigned long *)0x480504A4)
#define DISPC_GFX_FIFO_THRESHOLD_OFFSET 0xA4

#define DISPC_GFX_FIFO_SIZE_STATUS (*(volatile unsigned long *)0x480504A8)
#define DISPC_GFX_FIFO_SIZE_STATUS_OFFSET 0xA8

#define DISPC_GFX_ROW_INC (*(volatile unsigned long *)0x480504AC)
#define DISPC_GFX_ROW_INC_OFFSET 0xAC

#define DISPC_GFX_PIXEL_INC (*(volatile unsigned long *)0x480504B0)
#define DISPC_GFX_PIXEL_INC_OFFSET 0xB0

#define DISPC_GFX_WINDOW_SKIP (*(volatile unsigned long *)0x480504B4)
#define DISPC_GFX_WINDOW_SKIP_OFFSET 0xB4

#define DISPC_GFX_TABLE_BA (*(volatile unsigned long *)0x480504B8)
#define DISPC_GFX_TABLE_BA_OFFSET 0xB8

#define DISPC_DATA_CYCLE1 (*(volatile unsigned long *)0x480505D4)
#define DISPC_DATA_CYCLE1_OFFSET 0x1D4

#define DISPC_DATA_CYCLE2 (*(volatile unsigned long *)0x480505D8)
#define DISPC_DATA_CYCLE2_OFFSET 0x1D8

#define DISPC_DATA_CYCLE3 (*(volatile unsigned long *)0x480505DC)
#define DISPC_DATA_CYCLE3_OFFSET 0x1DC

#define DISPC_CPR_COEF_R (*(volatile unsigned long *)0x48050620)
#define DISPC_CPR_COEF_R_OFFSET 0x220

#define DISPC_CPR_COEF_G (*(volatile unsigned long *)0x48050624)
#define DISPC_CPR_COEF_G_OFFSET 0x224

#define DISPC_CPR_COEF_B (*(volatile unsigned long *)0x48050628)
#define DISPC_CPR_COEF_B_OFFSET 0x228

#define DISPC_GFX_PRELOAD (*(volatile unsigned long *)0x4805062C)
#define DISPC_GFX_PRELOAD_OFFSET 0x22C

#ifndef DSS_DISPC_VID1_BASE
#define DSS_DISPC_VID1_BASE 0x48050400
#endif

#define DISPC_VID1_BA0 (*(volatile unsigned long *)0x480504BC)
#define DISPC_VID1_BA0_OFFSET 0xBC

#define DISPC_VID1_BA1 (*(volatile unsigned long *)0x480504C0)
#define DISPC_VID1_BA1_OFFSET 0xC0

#define DISPC_VID1_POSITION (*(volatile unsigned long *)0x480504C4)
#define DISPC_VID1_POSITION_OFFSET 0xC4

#define DISPC_VID1_SIZE (*(volatile unsigned long *)0x480504C8)
#define DISPC_VID1_SIZE_OFFSET 0xC8

#define DISPC_VID1_ATTRIBUTES (*(volatile unsigned long *)0x480504CC)
#define DISPC_VID1_ATTRIBUTES_OFFSET 0xCC

#define DISPC_VID1_FIFO_THRESHOLD (*(volatile unsigned long *)0x480504D0)
#define DISPC_VID1_FIFO_THRESHOLD_OFFSET 0xD0

#define DISPC_VID1_FIFO_SIZE_STATUS (*(volatile unsigned long *)0x480504D4)
#define DISPC_VID1_FIFO_SIZE_STATUS_OFFSET 0xD4

#define DISPC_VID1_ROW_INC (*(volatile unsigned long *)0x480504D8)
#define DISPC_VID1_ROW_INC_OFFSET 0xD8

#define DISPC_VID1_PIXEL_INC (*(volatile unsigned long *)0x480504DC)
#define DISPC_VID1_PIXEL_INC_OFFSET 0xDC

#define DISPC_VID1_FIR (*(volatile unsigned long *)0x480504E0)
#define DISPC_VID1_FIR_OFFSET 0xE0

#define DISPC_VID1_PICTURE_SIZE (*(volatile unsigned long *)0x480504E4)
#define DISPC_VID1_PICTURE_SIZE_OFFSET 0xE4

#define DISPC_VID1_ACCU0 (*(volatile unsigned long *)0x480504E8)
#define DISPC_VID1_ACCU0_OFFSET 0xE8

#define DISPC_VID1_ACCU1 (*(volatile unsigned long *)0x480504EC)
#define DISPC_VID1_ACCU1_OFFSET 0xEC

#define DISPC_VID1_FIR_COEF_H0 (*(volatile unsigned long *)0x480504F0)
#define DISPC_VID1_FIR_COEF_H0_OFFSET 0xF0

#define DISPC_VID1_FIR_COEF_H1 (*(volatile unsigned long *)0x480504F8)
#define DISPC_VID1_FIR_COEF_H1_OFFSET 0xF8

#define DISPC_VID1_FIR_COEF_Hv0 (*(volatile unsigned long *)0x480504F4)
#define DISPC_VID1_FIR_COEF_Hv0_OFFSET 0xF4

#define DISPC_VID1_FIR_COEF_Hv1 (*(volatile unsigned long *)0x480504FC)
#define DISPC_VID1_FIR_COEF_Hv1_OFFSET 0xFC

#define DISPC_VID1_CONV_COEF0 (*(volatile unsigned long *)0x48050530)
#define DISPC_VID1_CONV_COEF0_OFFSET 0x130

#define DISPC_VID1_CONV_COEF1 (*(volatile unsigned long *)0x48050534)
#define DISPC_VID1_CONV_COEF1_OFFSET 0x134

#define DISPC_VID1_CONV_COEF2 (*(volatile unsigned long *)0x48050538)
#define DISPC_VID1_CONV_COEF2_OFFSET 0x138

#define DISPC_VID1_CONV_COEF3 (*(volatile unsigned long *)0x4805053C)
#define DISPC_VID1_CONV_COEF3_OFFSET 0x13C

#define DISPC_VID1_CONV_COEF4 (*(volatile unsigned long *)0x48050540)
#define DISPC_VID1_CONV_COEF4_OFFSET 0x140

#define DISPC_VID1_FIR_COEF_V0 (*(volatile unsigned long *)0x480505E0)
#define DISPC_VID1_FIR_COEF_V0_OFFSET 0x1E0

#define DISPC_VID1_FIR_COEF_V1 (*(volatile unsigned long *)0x480505E4)
#define DISPC_VID1_FIR_COEF_V1_OFFSET 0x1E4

#define DISPC_VID1_FIR_COEF_V2 (*(volatile unsigned long *)0x480505E8)
#define DISPC_VID1_FIR_COEF_V2_OFFSET 0x1E8

#define DISPC_VID1_FIR_COEF_V3 (*(volatile unsigned long *)0x480505EC)
#define DISPC_VID1_FIR_COEF_V3_OFFSET 0x1EC

#define DISPC_VID1_FIR_COEF_V4 (*(volatile unsigned long *)0x480505F0)
#define DISPC_VID1_FIR_COEF_V4_OFFSET 0x1F0

#define DISPC_VID1_FIR_COEF_V5 (*(volatile unsigned long *)0x480505F4)
#define DISPC_VID1_FIR_COEF_V5_OFFSET 0x1F4

#define DISPC_VID1_FIR_COEF_V6 (*(volatile unsigned long *)0x480505F8)
#define DISPC_VID1_FIR_COEF_V6_OFFSET 0x1F8

#define DISPC_VID1_FIR_COEF_V7 (*(volatile unsigned long *)0x480505FC)
#define DISPC_VID1_FIR_COEF_V7_OFFSET 0x1FC

#define DISPC_VID1_PRELOAD (*(volatile unsigned long *)0x48050630)
#define DISPC_VID1_PRELOAD_OFFSET 0x230

#ifndef DSS_DISPC_VID2_BASE
#define DSS_DISPC_VID2_BASE 0x48050400
#endif

#define DISPC_VID2_BA0 (*(volatile unsigned long *)0x4805054C)
#define DISPC_VID2_BA0_OFFSET 0x14C

#define DISPC_VID2_BA1 (*(volatile unsigned long *)0x48050550)
#define DISPC_VID2_BA1_OFFSET 0x150

#define DISPC_VID2_POSITION (*(volatile unsigned long *)0x48050554)
#define DISPC_VID2_POSITION_OFFSET 0x154

#define DISPC_VID2_SIZE (*(volatile unsigned long *)0x48050558)
#define DISPC_VID2_SIZE_OFFSET 0x158

#define DISPC_VID2_ATTRIBUTES (*(volatile unsigned long *)0x4805055C)
#define DISPC_VID2_ATTRIBUTES_OFFSET 0x15C

#define DISPC_VID2_FIFO_THRESHOLD (*(volatile unsigned long *)0x48050560)
#define DISPC_VID2_FIFO_THRESHOLD_OFFSET 0x160

#define DISPC_VID2_FIFO_SIZE_STATUS (*(volatile unsigned long *)0x48050564)
#define DISPC_VID2_FIFO_SIZE_STATUS_OFFSET 0x164

#define DISPC_VID2_ROW_INC (*(volatile unsigned long *)0x48050568)
#define DISPC_VID2_ROW_INC_OFFSET 0x168

#define DISPC_VID2_PIXEL_INC (*(volatile unsigned long *)0x4805056C)
#define DISPC_VID2_PIXEL_INC_OFFSET 0x16C

#define DISPC_VID2_FIR (*(volatile unsigned long *)0x48050570)
#define DISPC_VID2_FIR_OFFSET 0x170

#define DISPC_VID2_PICTURE_SIZE (*(volatile unsigned long *)0x48050574)
#define DISPC_VID2_PICTURE_SIZE_OFFSET 0x174

#define DISPC_VID2_ACCU0 (*(volatile unsigned long *)0x48050578)
#define DISPC_VID2_ACCU0_OFFSET 0x178

#define DISPC_VID2_ACCU1 (*(volatile unsigned long *)0x4805057C)
#define DISPC_VID2_ACCU1_OFFSET 0x17C

#define DISPC_VID2_FIR_COEF_H0 (*(volatile unsigned long *)0x48050580)
#define DISPC_VID2_FIR_COEF_H0_OFFSET 0x180

#define DISPC_VID2_FIR_COEF_H1 (*(volatile unsigned long *)0x48050588)
#define DISPC_VID2_FIR_COEF_H1_OFFSET 0x188

#define DISPC_VID2_FIR_COEF_Hv0 (*(volatile unsigned long *)0x48050584)
#define DISPC_VID2_FIR_COEF_Hv0_OFFSET 0x184

#define DISPC_VID2_FIR_COEF_Hv1 (*(volatile unsigned long *)0x4805058C)
#define DISPC_VID2_FIR_COEF_Hv1_OFFSET 0x18C

#define DISPC_VID2_CONV_COEF0 (*(volatile unsigned long *)0x480505C0)
#define DISPC_VID2_CONV_COEF0_OFFSET 0x1C0

#define DISPC_VID2_CONV_COEF1 (*(volatile unsigned long *)0x480505C4)
#define DISPC_VID2_CONV_COEF1_OFFSET 0x1C4

#define DISPC_VID2_CONV_COEF2 (*(volatile unsigned long *)0x480505C8)
#define DISPC_VID2_CONV_COEF2_OFFSET 0x1C8

#define DISPC_VID2_CONV_COEF3 (*(volatile unsigned long *)0x480505CC)
#define DISPC_VID2_CONV_COEF3_OFFSET 0x1CC

#define DISPC_VID2_CONV_COEF4 (*(volatile unsigned long *)0x480505D0)
#define DISPC_VID2_CONV_COEF4_OFFSET 0x1D0

#define DISPC_VID2_FIR_COEF_V0 (*(volatile unsigned long *)0x48050600)
#define DISPC_VID2_FIR_COEF_V0_OFFSET 0x200

#define DISPC_VID2_FIR_COEF_V1 (*(volatile unsigned long *)0x48050604)
#define DISPC_VID2_FIR_COEF_V1_OFFSET 0x204

#define DISPC_VID2_FIR_COEF_V2 (*(volatile unsigned long *)0x48050608)
#define DISPC_VID2_FIR_COEF_V2_OFFSET 0x208

#define DISPC_VID2_FIR_COEF_V3 (*(volatile unsigned long *)0x4805060C)
#define DISPC_VID2_FIR_COEF_V3_OFFSET 0x20C

#define DISPC_VID2_FIR_COEF_V4 (*(volatile unsigned long *)0x48050610)
#define DISPC_VID2_FIR_COEF_V4_OFFSET 0x210

#define DISPC_VID2_FIR_COEF_V5 (*(volatile unsigned long *)0x48050614)
#define DISPC_VID2_FIR_COEF_V5_OFFSET 0x214

#define DISPC_VID2_FIR_COEF_V6 (*(volatile unsigned long *)0x48050618)
#define DISPC_VID2_FIR_COEF_V6_OFFSET 0x218

#define DISPC_VID2_FIR_COEF_V7 (*(volatile unsigned long *)0x4805061C)
#define DISPC_VID2_FIR_COEF_V7_OFFSET 0x21C

#define DISPC_VID2_PRELOAD (*(volatile unsigned long *)0x48050634)
#define DISPC_VID2_PRELOAD_OFFSET 0x234

#ifndef DSS_RFBI_BASE
#define DSS_RFBI_BASE 0x48050800
#endif

#define RFBI_REVISION (*(volatile unsigned long *)0x48050800)
#define RFBI_REVISION_OFFSET 0x0

#define RFBI_SYSCONFIG (*(volatile unsigned long *)0x48050810)
#define RFBI_SYSCONFIG_OFFSET 0x10

#define RFBI_SYSSTATUS (*(volatile unsigned long *)0x48050814)
#define RFBI_SYSSTATUS_OFFSET 0x14

#define RFBI_CONTROL (*(volatile unsigned long *)0x48050840)
#define RFBI_CONTROL_OFFSET 0x40

#define RFBI_PIXEL_CNT (*(volatile unsigned long *)0x48050844)
#define RFBI_PIXEL_CNT_OFFSET 0x44

#define RFBI_LINE_NUMBER (*(volatile unsigned long *)0x48050848)
#define RFBI_LINE_NUMBER_OFFSET 0x48

#define RFBI_CMD (*(volatile unsigned long *)0x4805084C)
#define RFBI_CMD_OFFSET 0x4C

#define RFBI_PARAM (*(volatile unsigned long *)0x48050850)
#define RFBI_PARAM_OFFSET 0x50

#define RFBI_DATA (*(volatile unsigned long *)0x48050854)
#define RFBI_DATA_OFFSET 0x54

#define RFBI_READ (*(volatile unsigned long *)0x48050858)
#define RFBI_READ_OFFSET 0x58

#define RFBI_STATUS (*(volatile unsigned long *)0x4805085C)
#define RFBI_STATUS_OFFSET 0x5C

#define RFBI_CONFIG0 (*(volatile unsigned long *)0x48050860)
#define RFBI_CONFIG0_OFFSET 0x60

#define RFBI_CONFIG1 (*(volatile unsigned long *)0x48050878)
#define RFBI_CONFIG1_OFFSET 0x78

#define RFBI_ONOFF_TIME0 (*(volatile unsigned long *)0x48050864)
#define RFBI_ONOFF_TIME0_OFFSET 0x64

#define RFBI_ONOFF_TIME1 (*(volatile unsigned long *)0x4805087C)
#define RFBI_ONOFF_TIME1_OFFSET 0x7C

#define RFBI_CYCLE_TIME0 (*(volatile unsigned long *)0x48050868)
#define RFBI_CYCLE_TIME0_OFFSET 0x68

#define RFBI_CYCLE_TIME1 (*(volatile unsigned long *)0x48050880)
#define RFBI_CYCLE_TIME1_OFFSET 0x80

#define RFBI_DATA_CYCLE1_0 (*(volatile unsigned long *)0x4805086C)
#define RFBI_DATA_CYCLE1_0_OFFSET 0x6C

#define RFBI_DATA_CYCLE1_1 (*(volatile unsigned long *)0x48050884)
#define RFBI_DATA_CYCLE1_1_OFFSET 0x84

#define RFBI_DATA_CYCLE2_0 (*(volatile unsigned long *)0x48050870)
#define RFBI_DATA_CYCLE2_0_OFFSET 0x70

#define RFBI_DATA_CYCLE2_1 (*(volatile unsigned long *)0x48050888)
#define RFBI_DATA_CYCLE2_1_OFFSET 0x88

#define RFBI_DATA_CYCLE3_0 (*(volatile unsigned long *)0x48050874)
#define RFBI_DATA_CYCLE3_0_OFFSET 0x74

#define RFBI_DATA_CYCLE3_1 (*(volatile unsigned long *)0x4805088C)
#define RFBI_DATA_CYCLE3_1_OFFSET 0x8C

#define RFBI_VSYNC_WIDTH (*(volatile unsigned long *)0x48050890)
#define RFBI_VSYNC_WIDTH_OFFSET 0x90

#define RFBI_HSYNC_WIDTH (*(volatile unsigned long *)0x48050894)
#define RFBI_HSYNC_WIDTH_OFFSET 0x94

#ifndef DSS_DSI_PHY_BASE
#define DSS_DSI_PHY_BASE 0x4804FE00
#endif

#define DSI_PHY_CFG0 (*(volatile unsigned long *)0x4804FE00)
#define DSI_PHY_CFG0_OFFSET 0x0

#define DSI_PHY_CFG1 (*(volatile unsigned long *)0x4804FE04)
#define DSI_PHY_CFG1_OFFSET 0x4

#define DSI_PHY_CFG2 (*(volatile unsigned long *)0x4804FE08)
#define DSI_PHY_CFG2_OFFSET 0x8

#define DSI_PHY_CFG3 (*(volatile unsigned long *)0x4804FE0C)
#define DSI_PHY_CFG3_OFFSET 0xC

#define DSI_PHY_CFG4 (*(volatile unsigned long *)0x4804FE10)
#define DSI_PHY_CFG4_OFFSET 0x10

#define DSI_PHY_CFG5 (*(volatile unsigned long *)0x4804FE14)
#define DSI_PHY_CFG5_OFFSET 0x14

#ifndef DSS_DSI_PLL_BASE
#define DSS_DSI_PLL_BASE 0x4804FF00
#endif

#define DSI_PLL_CONTROL (*(volatile unsigned long *)0x4804FF00)
#define DSI_PLL_CONTROL_OFFSET 0x0

#define DSI_PLL_STATUS (*(volatile unsigned long *)0x4804FF04)
#define DSI_PLL_STATUS_OFFSET 0x4

#define DSI_PLL_GO (*(volatile unsigned long *)0x4804FF08)
#define DSI_PLL_GO_OFFSET 0x8

#define DSI_PLL_CONFIGURATION1 (*(volatile unsigned long *)0x4804FF0C)
#define DSI_PLL_CONFIGURATION1_OFFSET 0xC

#define DSI_PLL_CONFIGURATION2 (*(volatile unsigned long *)0x4804FF10)
#define DSI_PLL_CONFIGURATION2_OFFSET 0x10

#ifndef DSS_DSI_PROTOCOL_BASE
#define DSS_DSI_PROTOCOL_BASE 0x4804FC00
#endif

#define DSI_REVISION (*(volatile unsigned long *)0x4804FC00)
#define DSI_REVISION_OFFSET 0x0

#define DSI_SYSCONFIG (*(volatile unsigned long *)0x4804FC10)
#define DSI_SYSCONFIG_OFFSET 0x10

#define DSI_SYSSTATUS (*(volatile unsigned long *)0x4804FC14)
#define DSI_SYSSTATUS_OFFSET 0x14

#define DSI_IRQSTATUS (*(volatile unsigned long *)0x4804FC18)
#define DSI_IRQSTATUS_OFFSET 0x18

#define DSI_IRQENABLE (*(volatile unsigned long *)0x4804FC1C)
#define DSI_IRQENABLE_OFFSET 0x1C

#define DSI_CTRL (*(volatile unsigned long *)0x4804FC40)
#define DSI_CTRL_OFFSET 0x40

#define DSI_COMPLEXIO_CFG1 (*(volatile unsigned long *)0x4804FC48)
#define DSI_COMPLEXIO_CFG1_OFFSET 0x48

#define DSI_COMPLEXIO_IRQSTATUS (*(volatile unsigned long *)0x4804FC4C)
#define DSI_COMPLEXIO_IRQSTATUS_OFFSET 0x4C

#define DSI_COMPLEXIO_IRQENABLE (*(volatile unsigned long *)0x4804FC50)
#define DSI_COMPLEXIO_IRQENABLE_OFFSET 0x50

#define DSI_CLK_CTRL (*(volatile unsigned long *)0x4804FC54)
#define DSI_CLK_CTRL_OFFSET 0x54

#define DSI_TIMING1 (*(volatile unsigned long *)0x4804FC58)
#define DSI_TIMING1_OFFSET 0x58

#define DSI_TIMING2 (*(volatile unsigned long *)0x4804FC5C)
#define DSI_TIMING2_OFFSET 0x5C

#define DSI_VM_TIMING1 (*(volatile unsigned long *)0x4804FC60)
#define DSI_VM_TIMING1_OFFSET 0x60

#define DSI_VM_TIMING2 (*(volatile unsigned long *)0x4804FC64)
#define DSI_VM_TIMING2_OFFSET 0x64

#define DSI_VM_TIMING3 (*(volatile unsigned long *)0x4804FC68)
#define DSI_VM_TIMING3_OFFSET 0x68

#define DSI_CLK_TIMING (*(volatile unsigned long *)0x4804FC6C)
#define DSI_CLK_TIMING_OFFSET 0x6C

#define DSI_TX_FIFO_VC_SIZE (*(volatile unsigned long *)0x4804FC70)
#define DSI_TX_FIFO_VC_SIZE_OFFSET 0x70

#define DSI_RX_FIFO_VC_SIZE (*(volatile unsigned long *)0x4804FC74)
#define DSI_RX_FIFO_VC_SIZE_OFFSET 0x74

#define DSI_COMPLEXIO_CFG2 (*(volatile unsigned long *)0x4804FC78)
#define DSI_COMPLEXIO_CFG2_OFFSET 0x78

#define DSI_RX_FIFO_VC_FULLNESS (*(volatile unsigned long *)0x4804FC7C)
#define DSI_RX_FIFO_VC_FULLNESS_OFFSET 0x7C

#define DSI_VM_TIMING4 (*(volatile unsigned long *)0x4804FC80)
#define DSI_VM_TIMING4_OFFSET 0x80

#define DSI_TX_FIFO_VC_EMPTINESS (*(volatile unsigned long *)0x4804FC84)
#define DSI_TX_FIFO_VC_EMPTINESS_OFFSET 0x84

#define DSI_VM_TIMING5 (*(volatile unsigned long *)0x4804FC88)
#define DSI_VM_TIMING5_OFFSET 0x88

#define DSI_VM_TIMING6 (*(volatile unsigned long *)0x4804FC8C)
#define DSI_VM_TIMING6_OFFSET 0x8C

#define DSI_VM_TIMING7 (*(volatile unsigned long *)0x4804FC90)
#define DSI_VM_TIMING7_OFFSET 0x90

#define DSI_STOPCLK_TIMING (*(volatile unsigned long *)0x4804FC94)
#define DSI_STOPCLK_TIMING_OFFSET 0x94

#define DSI_VC0_CTRL (*(volatile unsigned long *)0x4804FD00)
#define DSI_VC0_CTRL_OFFSET 0x100

#define DSI_VC1_CTRL (*(volatile unsigned long *)0x4804FD20)
#define DSI_VC1_CTRL_OFFSET 0x120

#define DSI_VC2_CTRL (*(volatile unsigned long *)0x4804FD40)
#define DSI_VC2_CTRL_OFFSET 0x140

#define DSI_VC3_CTRL (*(volatile unsigned long *)0x4804FD60)
#define DSI_VC3_CTRL_OFFSET 0x160

#define DSI_VC0_TE (*(volatile unsigned long *)0x4804FD04)
#define DSI_VC0_TE_OFFSET 0x104

#define DSI_VC1_TE (*(volatile unsigned long *)0x4804FD24)
#define DSI_VC1_TE_OFFSET 0x124

#define DSI_VC2_TE (*(volatile unsigned long *)0x4804FD44)
#define DSI_VC2_TE_OFFSET 0x144

#define DSI_VC3_TE (*(volatile unsigned long *)0x4804FD64)
#define DSI_VC3_TE_OFFSET 0x164

#define DSI_VC0_LONG_PACKET_HEADER (*(volatile unsigned long *)0x4804FD08)
#define DSI_VC0_LONG_PACKET_HEADER_OFFSET 0x108

#define DSI_VC1_LONG_PACKET_HEADER (*(volatile unsigned long *)0x4804FD28)
#define DSI_VC1_LONG_PACKET_HEADER_OFFSET 0x128

#define DSI_VC2_LONG_PACKET_HEADER (*(volatile unsigned long *)0x4804FD48)
#define DSI_VC2_LONG_PACKET_HEADER_OFFSET 0x148

#define DSI_VC3_LONG_PACKET_HEADER (*(volatile unsigned long *)0x4804FD68)
#define DSI_VC3_LONG_PACKET_HEADER_OFFSET 0x168

#define DSI_VC0_LONG_PACKET_PAYLOAD (*(volatile unsigned long *)0x4804FD0C)
#define DSI_VC0_LONG_PACKET_PAYLOAD_OFFSET 0x10C

#define DSI_VC1_LONG_PACKET_PAYLOAD (*(volatile unsigned long *)0x4804FD2C)
#define DSI_VC1_LONG_PACKET_PAYLOAD_OFFSET 0x12C

#define DSI_VC2_LONG_PACKET_PAYLOAD (*(volatile unsigned long *)0x4804FD4C)
#define DSI_VC2_LONG_PACKET_PAYLOAD_OFFSET 0x14C

#define DSI_VC3_LONG_PACKET_PAYLOAD (*(volatile unsigned long *)0x4804FD6C)
#define DSI_VC3_LONG_PACKET_PAYLOAD_OFFSET 0x16C

#define DSI_VC0_SHORT_PACKET_HEADER (*(volatile unsigned long *)0x4804FD10)
#define DSI_VC0_SHORT_PACKET_HEADER_OFFSET 0x110

#define DSI_VC1_SHORT_PACKET_HEADER (*(volatile unsigned long *)0x4804FD30)
#define DSI_VC1_SHORT_PACKET_HEADER_OFFSET 0x130

#define DSI_VC2_SHORT_PACKET_HEADER (*(volatile unsigned long *)0x4804FD50)
#define DSI_VC2_SHORT_PACKET_HEADER_OFFSET 0x150

#define DSI_VC3_SHORT_PACKET_HEADER (*(volatile unsigned long *)0x4804FD70)
#define DSI_VC3_SHORT_PACKET_HEADER_OFFSET 0x170

#define DSI_VC0_IRQSTATUS (*(volatile unsigned long *)0x4804FD18)
#define DSI_VC0_IRQSTATUS_OFFSET 0x118

#define DSI_VC1_IRQSTATUS (*(volatile unsigned long *)0x4804FD38)
#define DSI_VC1_IRQSTATUS_OFFSET 0x138

#define DSI_VC2_IRQSTATUS (*(volatile unsigned long *)0x4804FD58)
#define DSI_VC2_IRQSTATUS_OFFSET 0x158

#define DSI_VC3_IRQSTATUS (*(volatile unsigned long *)0x4804FD78)
#define DSI_VC3_IRQSTATUS_OFFSET 0x178

#define DSI_VC0_IRQENABLE (*(volatile unsigned long *)0x4804FD1C)
#define DSI_VC0_IRQENABLE_OFFSET 0x11C

#define DSI_VC1_IRQENABLE (*(volatile unsigned long *)0x4804FD3C)
#define DSI_VC1_IRQENABLE_OFFSET 0x13C

#define DSI_VC2_IRQENABLE (*(volatile unsigned long *)0x4804FD5C)
#define DSI_VC2_IRQENABLE_OFFSET 0x15C

#define DSI_VC3_IRQENABLE (*(volatile unsigned long *)0x4804FD7C)
#define DSI_VC3_IRQENABLE_OFFSET 0x17C

#ifndef DSS_VENC_BASE
#define DSS_VENC_BASE 0x48050C00
#endif

#define VENC_REV_ID (*(volatile unsigned long *)0x48050C00)
#define VENC_REV_ID_OFFSET 0x0

#define VENC_STATUS (*(volatile unsigned long *)0x48050C04)
#define VENC_STATUS_OFFSET 0x4

#define VENC_F_CONTROL (*(volatile unsigned long *)0x48050C08)
#define VENC_F_CONTROL_OFFSET 0x8

#define VENC_VIDOUT_CTRL (*(volatile unsigned long *)0x48050C10)
#define VENC_VIDOUT_CTRL_OFFSET 0x10

#define VENC_SYNC_CTRL (*(volatile unsigned long *)0x48050C14)
#define VENC_SYNC_CTRL_OFFSET 0x14

#define VENC_LLEN (*(volatile unsigned long *)0x48050C1C)
#define VENC_LLEN_OFFSET 0x1C

#define VENC_FLENS (*(volatile unsigned long *)0x48050C20)
#define VENC_FLENS_OFFSET 0x20

#define VENC_HFLTR_CTRL (*(volatile unsigned long *)0x48050C24)
#define VENC_HFLTR_CTRL_OFFSET 0x24

#define VENC_CC_CARR_WSS_CARR (*(volatile unsigned long *)0x48050C28)
#define VENC_CC_CARR_WSS_CARR_OFFSET 0x28

#define VENC_C_PHASE (*(volatile unsigned long *)0x48050C2C)
#define VENC_C_PHASE_OFFSET 0x2C

#define VENC_GAIN_U (*(volatile unsigned long *)0x48050C30)
#define VENC_GAIN_U_OFFSET 0x30

#define VENC_GAIN_V (*(volatile unsigned long *)0x48050C34)
#define VENC_GAIN_V_OFFSET 0x34

#define VENC_GAIN_Y (*(volatile unsigned long *)0x48050C38)
#define VENC_GAIN_Y_OFFSET 0x38

#define VENC_BLACK_LEVEL (*(volatile unsigned long *)0x48050C3C)
#define VENC_BLACK_LEVEL_OFFSET 0x3C

#define VENC_BLANK_LEVEL (*(volatile unsigned long *)0x48050C40)
#define VENC_BLANK_LEVEL_OFFSET 0x40

#define VENC_X_COLOR (*(volatile unsigned long *)0x48050C44)
#define VENC_X_COLOR_OFFSET 0x44

#define VENC_M_CONTROL (*(volatile unsigned long *)0x48050C48)
#define VENC_M_CONTROL_OFFSET 0x48

#define VENC_BSTAMP_WSS_DATA (*(volatile unsigned long *)0x48050C4C)
#define VENC_BSTAMP_WSS_DATA_OFFSET 0x4C

#define VENC_S_CARR (*(volatile unsigned long *)0x48050C50)
#define VENC_S_CARR_OFFSET 0x50

#define VENC_LINE21 (*(volatile unsigned long *)0x48050C54)
#define VENC_LINE21_OFFSET 0x54

#define VENC_LN_SEL (*(volatile unsigned long *)0x48050C58)
#define VENC_LN_SEL_OFFSET 0x58

#define VENC_L21_WC_CTL (*(volatile unsigned long *)0x48050C5C)
#define VENC_L21_WC_CTL_OFFSET 0x5C

#define VENC_HTRIGGER_VTRIGGER (*(volatile unsigned long *)0x48050C60)
#define VENC_HTRIGGER_VTRIGGER_OFFSET 0x60

#define VENC_SAVID_EAVID (*(volatile unsigned long *)0x48050C64)
#define VENC_SAVID_EAVID_OFFSET 0x64

#define VENC_FLEN_FAL (*(volatile unsigned long *)0x48050C68)
#define VENC_FLEN_FAL_OFFSET 0x68

#define VENC_LAL_PHASE_RESET (*(volatile unsigned long *)0x48050C6C)
#define VENC_LAL_PHASE_RESET_OFFSET 0x6C

#define VENC_HS_INT_START_STOP_X (*(volatile unsigned long *)0x48050C70)
#define VENC_HS_INT_START_STOP_X_OFFSET 0x70

#define VENC_HS_EXT_START_STOP_X (*(volatile unsigned long *)0x48050C74)
#define VENC_HS_EXT_START_STOP_X_OFFSET 0x74

#define VENC_VS_INT_START_X (*(volatile unsigned long *)0x48050C78)
#define VENC_VS_INT_START_X_OFFSET 0x78

#define VENC_VS_INT_STOP_X_VS_INT_START_Y (*(volatile unsigned long *)0x48050C7C)
#define VENC_VS_INT_STOP_X_VS_INT_START_Y_OFFSET 0x7C

#define VENC_VS_INT_STOP_Y_VS_EXT_START_X (*(volatile unsigned long *)0x48050C80)
#define VENC_VS_INT_STOP_Y_VS_EXT_START_X_OFFSET 0x80

#define VENC_VS_EXT_STOP_X_VS_EXT_START_Y (*(volatile unsigned long *)0x48050C84)
#define VENC_VS_EXT_STOP_X_VS_EXT_START_Y_OFFSET 0x84

#define VENC_VS_EXT_STOP_Y (*(volatile unsigned long *)0x48050C88)
#define VENC_VS_EXT_STOP_Y_OFFSET 0x88

#define VENC_AVID_START_STOP_X (*(volatile unsigned long *)0x48050C90)
#define VENC_AVID_START_STOP_X_OFFSET 0x90

#define VENC_AVID_START_STOP_Y (*(volatile unsigned long *)0x48050C94)
#define VENC_AVID_START_STOP_Y_OFFSET 0x94

#define VENC_FID_INT_START_X_FID_INT_START_Y (*(volatile unsigned long *)0x48050CA0)
#define VENC_FID_INT_START_X_FID_INT_START_Y_OFFSET 0xA0

#define VENC_FID_INT_OFFSET_Y_FID_EXT_START_X (*(volatile unsigned long *)0x48050CA4)
#define VENC_FID_INT_OFFSET_Y_FID_EXT_START_X_OFFSET 0xA4

#define VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y (*(volatile unsigned long *)0x48050CA8)
#define VENC_FID_EXT_START_Y_FID_EXT_OFFSET_Y_OFFSET 0xA8

#define VENC_TVDETGP_INT_START_STOP_X (*(volatile unsigned long *)0x48050CB0)
#define VENC_TVDETGP_INT_START_STOP_X_OFFSET 0xB0

#define VENC_TVDETGP_INT_START_STOP_Y (*(volatile unsigned long *)0x48050CB4)
#define VENC_TVDETGP_INT_START_STOP_Y_OFFSET 0xB4

#define VENC_GEN_CTRL (*(volatile unsigned long *)0x48050CB8)
#define VENC_GEN_CTRL_OFFSET 0xB8

#define VENC_OUTPUT_CONTROL (*(volatile unsigned long *)0x48050CC4)
#define VENC_OUTPUT_CONTROL_OFFSET 0xC4

#define VENC_OUTPUT_TEST (*(volatile unsigned long *)0x48050CC8)
#define VENC_OUTPUT_TEST_OFFSET 0xC8

#ifndef SDMA_BASE
#define SDMA_BASE 0x48056000
#endif

#define DMA4_REVISION (*(volatile unsigned *)0x48056000)
#define DMA4_REVISION_OFFSET 0x0

#define DMA4_IRQSTATUS_L0 (*(volatile unsigned *)0x48056008)
#define DMA4_IRQSTATUS_L0_OFFSET 0x8

#define DMA4_IRQENABLE_L0 (*(volatile unsigned *)0x48056018)
#define DMA4_IRQENABLE_L0_OFFSET 0x18

#define DMA4_IRQSTATUS_L1 (*(volatile unsigned *)0x4805600C)
#define DMA4_IRQSTATUS_L1_OFFSET 0xC

#define DMA4_IRQENABLE_L1 (*(volatile unsigned *)0x4805601C)
#define DMA4_IRQENABLE_L1_OFFSET 0x1C

#define DMA4_IRQSTATUS_L2 (*(volatile unsigned *)0x48056010)
#define DMA4_IRQSTATUS_L2_OFFSET 0x10

#define DMA4_IRQENABLE_L2 (*(volatile unsigned *)0x48056020)
#define DMA4_IRQENABLE_L2_OFFSET 0x20

#define DMA4_IRQSTATUS_L3 (*(volatile unsigned *)0x48056014)
#define DMA4_IRQSTATUS_L3_OFFSET 0x14

#define DMA4_IRQENABLE_L3 (*(volatile unsigned *)0x48056024)
#define DMA4_IRQENABLE_L3_OFFSET 0x24

#define DMA4_SYSSTATUS (*(volatile unsigned *)0x48056028)
#define DMA4_SYSSTATUS_OFFSET 0x28

#define DMA4_OCP_SYSCONFIG (*(volatile unsigned *)0x4805602C)
#define DMA4_OCP_SYSCONFIG_OFFSET 0x2C

#define DMA4_CAPS_0 (*(volatile unsigned *)0x48056064)
#define DMA4_CAPS_0_OFFSET 0x64

#define DMA4_CAPS_2 (*(volatile unsigned *)0x4805606C)
#define DMA4_CAPS_2_OFFSET 0x6C

#define DMA4_CAPS_3 (*(volatile unsigned *)0x48056070)
#define DMA4_CAPS_3_OFFSET 0x70

#define DMA4_CAPS_4 (*(volatile unsigned *)0x48056074)
#define DMA4_CAPS_4_OFFSET 0x74

#define DMA4_GCR (*(volatile unsigned *)0x48056078)
#define DMA4_GCR_OFFSET 0x78

#ifndef SDMA_CHANNEL0_BASE
#define SDMA_CHANNEL0_BASE 0x48056080
#endif

#define DMA4_CCR0 (*(volatile unsigned *)0x48056100)
#define DMA4_CCR0_OFFSET 0x80

#define DMA4_CLNK_CTRL0 (*(volatile unsigned *)0x48056104)
#define DMA4_CLNK_CTRL0_OFFSET 0x84

#define DMA4_CICR0 (*(volatile unsigned *)0x48056108)
#define DMA4_CICR0_OFFSET 0x88

#define DMA4_CSR0 (*(volatile unsigned *)0x4805610C)
#define DMA4_CSR0_OFFSET 0x8C

#define DMA4_CSDP0 (*(volatile unsigned *)0x48056110)
#define DMA4_CSDP0_OFFSET 0x90

#define DMA4_CEN0 (*(volatile unsigned *)0x48056114)
#define DMA4_CEN0_OFFSET 0x94

#define DMA4_CFN0 (*(volatile unsigned *)0x48056118)
#define DMA4_CFN0_OFFSET 0x98

#define DMA4_CSSA0 (*(volatile unsigned *)0x4805611C)
#define DMA4_CSSA0_OFFSET 0x9C

#define DMA4_CDSA0 (*(volatile unsigned *)0x48056120)
#define DMA4_CDSA0_OFFSET 0xA0

#define DMA4_CSEL0 (*(volatile unsigned *)0x48056124)
#define DMA4_CSEL0_OFFSET 0xA4

#define DMA4_CSFl0 (*(volatile unsigned *)0x48056128)
#define DMA4_CSFl0_OFFSET 0xA8

#define DMA4_CDEl0 (*(volatile unsigned *)0x4805612C)
#define DMA4_CDEl0_OFFSET 0xAC

#define DMA4_CDFl0 (*(volatile unsigned *)0x48056130)
#define DMA4_CDFl0_OFFSET 0xB0

#define DMA4_CSAC0 (*(volatile unsigned *)0x48056134)
#define DMA4_CSAC0_OFFSET 0xB4

#define DMA4_CDAC0 (*(volatile unsigned *)0x48056138)
#define DMA4_CDAC0_OFFSET 0xB8

#define DMA4_CCEN0 (*(volatile unsigned *)0x4805613C)
#define DMA4_CCEN0_OFFSET 0xBC

#define DMA4_CCFN0 (*(volatile unsigned *)0x48056140)
#define DMA4_CCFN0_OFFSET 0xC0

#define DMA4_COLOR0 (*(volatile unsigned *)0x48056144)
#define DMA4_COLOR0_OFFSET 0xC4

#ifndef SDMA_CHANNEL1_BASE
#define SDMA_CHANNEL1_BASE 0x480560E0
#endif

#define DMA4_CCR1 (*(volatile unsigned *)0x480561C0)
#define DMA4_CCR1_OFFSET 0xE0

#define DMA4_CLNK_CTRL1 (*(volatile unsigned *)0x480561C4)
#define DMA4_CLNK_CTRL1_OFFSET 0xE4

#define DMA4_CICR1 (*(volatile unsigned *)0x480561C8)
#define DMA4_CICR1_OFFSET 0xE8

#define DMA4_CSR1 (*(volatile unsigned *)0x480561CC)
#define DMA4_CSR1_OFFSET 0xEC

#define DMA4_CSDP1 (*(volatile unsigned *)0x480561D0)
#define DMA4_CSDP1_OFFSET 0xF0

#define DMA4_CEN1 (*(volatile unsigned *)0x480561D4)
#define DMA4_CEN1_OFFSET 0xF4

#define DMA4_CFN1 (*(volatile unsigned *)0x480561D8)
#define DMA4_CFN1_OFFSET 0xF8

#define DMA4_CSSA1 (*(volatile unsigned *)0x480561DC)
#define DMA4_CSSA1_OFFSET 0xFC

#define DMA4_CDSA1 (*(volatile unsigned *)0x480561E0)
#define DMA4_CDSA1_OFFSET 0x100

#define DMA4_CSEL1 (*(volatile unsigned *)0x480561E4)
#define DMA4_CSEL1_OFFSET 0x104

#define DMA4_CSFl1 (*(volatile unsigned *)0x480561E8)
#define DMA4_CSFl1_OFFSET 0x108

#define DMA4_CDEl1 (*(volatile unsigned *)0x480561EC)
#define DMA4_CDEl1_OFFSET 0x10C

#define DMA4_CDFl1 (*(volatile unsigned *)0x480561F0)
#define DMA4_CDFl1_OFFSET 0x110

#define DMA4_CSAC1 (*(volatile unsigned *)0x480561F4)
#define DMA4_CSAC1_OFFSET 0x114

#define DMA4_CDAC1 (*(volatile unsigned *)0x480561F8)
#define DMA4_CDAC1_OFFSET 0x118

#define DMA4_CCEN1 (*(volatile unsigned *)0x480561FC)
#define DMA4_CCEN1_OFFSET 0x11C

#define DMA4_CCFN1 (*(volatile unsigned *)0x48056200)
#define DMA4_CCFN1_OFFSET 0x120

#define DMA4_COLOR1 (*(volatile unsigned *)0x48056204)
#define DMA4_COLOR1_OFFSET 0x124

#ifndef SDMA_CHANNEL2_BASE
#define SDMA_CHANNEL2_BASE 0x48056140
#endif

#define DMA4_CCR2 (*(volatile unsigned *)0x48056280)
#define DMA4_CCR2_OFFSET 0x140

#define DMA4_CLNK_CTRL2 (*(volatile unsigned *)0x48056284)
#define DMA4_CLNK_CTRL2_OFFSET 0x144

#define DMA4_CICR2 (*(volatile unsigned *)0x48056288)
#define DMA4_CICR2_OFFSET 0x148

#define DMA4_CSR2 (*(volatile unsigned *)0x4805628C)
#define DMA4_CSR2_OFFSET 0x14C

#define DMA4_CSDP2 (*(volatile unsigned *)0x48056290)
#define DMA4_CSDP2_OFFSET 0x150

#define DMA4_CEN2 (*(volatile unsigned *)0x48056294)
#define DMA4_CEN2_OFFSET 0x154

#define DMA4_CFN2 (*(volatile unsigned *)0x48056298)
#define DMA4_CFN2_OFFSET 0x158

#define DMA4_CSSA2 (*(volatile unsigned *)0x4805629C)
#define DMA4_CSSA2_OFFSET 0x15C

#define DMA4_CDSA2 (*(volatile unsigned *)0x480562A0)
#define DMA4_CDSA2_OFFSET 0x160

#define DMA4_CSEL2 (*(volatile unsigned *)0x480562A4)
#define DMA4_CSEL2_OFFSET 0x164

#define DMA4_CSFl2 (*(volatile unsigned *)0x480562A8)
#define DMA4_CSFl2_OFFSET 0x168

#define DMA4_CDEl2 (*(volatile unsigned *)0x480562AC)
#define DMA4_CDEl2_OFFSET 0x16C

#define DMA4_CDFl2 (*(volatile unsigned *)0x480562B0)
#define DMA4_CDFl2_OFFSET 0x170

#define DMA4_CSAC2 (*(volatile unsigned *)0x480562B4)
#define DMA4_CSAC2_OFFSET 0x174

#define DMA4_CDAC2 (*(volatile unsigned *)0x480562B8)
#define DMA4_CDAC2_OFFSET 0x178

#define DMA4_CCEN2 (*(volatile unsigned *)0x480562BC)
#define DMA4_CCEN2_OFFSET 0x17C

#define DMA4_CCFN2 (*(volatile unsigned *)0x480562C0)
#define DMA4_CCFN2_OFFSET 0x180

#define DMA4_COLOR2 (*(volatile unsigned *)0x480562C4)
#define DMA4_COLOR2_OFFSET 0x184

#ifndef SDMA_CHANNEL3_BASE
#define SDMA_CHANNEL3_BASE 0x480561A0
#endif

#define DMA4_CCR3 (*(volatile unsigned *)0x48056340)
#define DMA4_CCR3_OFFSET 0x1A0

#define DMA4_CLNK_CTRL3 (*(volatile unsigned *)0x48056344)
#define DMA4_CLNK_CTRL3_OFFSET 0x1A4

#define DMA4_CICR3 (*(volatile unsigned *)0x48056348)
#define DMA4_CICR3_OFFSET 0x1A8

#define DMA4_CSR3 (*(volatile unsigned *)0x4805634C)
#define DMA4_CSR3_OFFSET 0x1AC

#define DMA4_CSDP3 (*(volatile unsigned *)0x48056350)
#define DMA4_CSDP3_OFFSET 0x1B0

#define DMA4_CEN3 (*(volatile unsigned *)0x48056354)
#define DMA4_CEN3_OFFSET 0x1B4

#define DMA4_CFN3 (*(volatile unsigned *)0x48056358)
#define DMA4_CFN3_OFFSET 0x1B8

#define DMA4_CSSA3 (*(volatile unsigned *)0x4805635C)
#define DMA4_CSSA3_OFFSET 0x1BC

#define DMA4_CDSA3 (*(volatile unsigned *)0x48056360)
#define DMA4_CDSA3_OFFSET 0x1C0

#define DMA4_CSEL3 (*(volatile unsigned *)0x48056364)
#define DMA4_CSEL3_OFFSET 0x1C4

#define DMA4_CSFl3 (*(volatile unsigned *)0x48056368)
#define DMA4_CSFl3_OFFSET 0x1C8

#define DMA4_CDEl3 (*(volatile unsigned *)0x4805636C)
#define DMA4_CDEl3_OFFSET 0x1CC

#define DMA4_CDFl3 (*(volatile unsigned *)0x48056370)
#define DMA4_CDFl3_OFFSET 0x1D0

#define DMA4_CSAC3 (*(volatile unsigned *)0x48056374)
#define DMA4_CSAC3_OFFSET 0x1D4

#define DMA4_CDAC3 (*(volatile unsigned *)0x48056378)
#define DMA4_CDAC3_OFFSET 0x1D8

#define DMA4_CCEN3 (*(volatile unsigned *)0x4805637C)
#define DMA4_CCEN3_OFFSET 0x1DC

#define DMA4_CCFN3 (*(volatile unsigned *)0x48056380)
#define DMA4_CCFN3_OFFSET 0x1E0

#define DMA4_COLOR3 (*(volatile unsigned *)0x48056384)
#define DMA4_COLOR3_OFFSET 0x1E4

#ifndef SDMA_CHANNEL4_BASE
#define SDMA_CHANNEL4_BASE 0x48056200
#endif

#define DMA4_CCR4 (*(volatile unsigned *)0x48056400)
#define DMA4_CCR4_OFFSET 0x200

#define DMA4_CLNK_CTRL4 (*(volatile unsigned *)0x48056404)
#define DMA4_CLNK_CTRL4_OFFSET 0x204

#define DMA4_CICR4 (*(volatile unsigned *)0x48056408)
#define DMA4_CICR4_OFFSET 0x208

#define DMA4_CSR4 (*(volatile unsigned *)0x4805640C)
#define DMA4_CSR4_OFFSET 0x20C

#define DMA4_CSDP4 (*(volatile unsigned *)0x48056410)
#define DMA4_CSDP4_OFFSET 0x210

#define DMA4_CEN4 (*(volatile unsigned *)0x48056414)
#define DMA4_CEN4_OFFSET 0x214

#define DMA4_CFN4 (*(volatile unsigned *)0x48056418)
#define DMA4_CFN4_OFFSET 0x218

#define DMA4_CSSA4 (*(volatile unsigned *)0x4805641C)
#define DMA4_CSSA4_OFFSET 0x21C

#define DMA4_CDSA4 (*(volatile unsigned *)0x48056420)
#define DMA4_CDSA4_OFFSET 0x220

#define DMA4_CSEL4 (*(volatile unsigned *)0x48056424)
#define DMA4_CSEL4_OFFSET 0x224

#define DMA4_CSFl4 (*(volatile unsigned *)0x48056428)
#define DMA4_CSFl4_OFFSET 0x228

#define DMA4_CDEl4 (*(volatile unsigned *)0x4805642C)
#define DMA4_CDEl4_OFFSET 0x22C

#define DMA4_CDFl4 (*(volatile unsigned *)0x48056430)
#define DMA4_CDFl4_OFFSET 0x230

#define DMA4_CSAC4 (*(volatile unsigned *)0x48056434)
#define DMA4_CSAC4_OFFSET 0x234

#define DMA4_CDAC4 (*(volatile unsigned *)0x48056438)
#define DMA4_CDAC4_OFFSET 0x238

#define DMA4_CCEN4 (*(volatile unsigned *)0x4805643C)
#define DMA4_CCEN4_OFFSET 0x23C

#define DMA4_CCFN4 (*(volatile unsigned *)0x48056440)
#define DMA4_CCFN4_OFFSET 0x240

#define DMA4_COLOR4 (*(volatile unsigned *)0x48056444)
#define DMA4_COLOR4_OFFSET 0x244

#ifndef SDMA_CHANNEL5_BASE
#define SDMA_CHANNEL5_BASE 0x48056260
#endif

#define DMA4_CCR5 (*(volatile unsigned *)0x480564C0)
#define DMA4_CCR5_OFFSET 0x260

#define DMA4_CLNK_CTRL5 (*(volatile unsigned *)0x480564C4)
#define DMA4_CLNK_CTRL5_OFFSET 0x264

#define DMA4_CICR5 (*(volatile unsigned *)0x480564C8)
#define DMA4_CICR5_OFFSET 0x268

#define DMA4_CSR5 (*(volatile unsigned *)0x480564CC)
#define DMA4_CSR5_OFFSET 0x26C

#define DMA4_CSDP5 (*(volatile unsigned *)0x480564D0)
#define DMA4_CSDP5_OFFSET 0x270

#define DMA4_CEN5 (*(volatile unsigned *)0x480564D4)
#define DMA4_CEN5_OFFSET 0x274

#define DMA4_CFN5 (*(volatile unsigned *)0x480564D8)
#define DMA4_CFN5_OFFSET 0x278

#define DMA4_CSSA5 (*(volatile unsigned *)0x480564DC)
#define DMA4_CSSA5_OFFSET 0x27C

#define DMA4_CDSA5 (*(volatile unsigned *)0x480564E0)
#define DMA4_CDSA5_OFFSET 0x280

#define DMA4_CSEL5 (*(volatile unsigned *)0x480564E4)
#define DMA4_CSEL5_OFFSET 0x284

#define DMA4_CSFl5 (*(volatile unsigned *)0x480564E8)
#define DMA4_CSFl5_OFFSET 0x288

#define DMA4_CDEl5 (*(volatile unsigned *)0x480564EC)
#define DMA4_CDEl5_OFFSET 0x28C

#define DMA4_CDFl5 (*(volatile unsigned *)0x480564F0)
#define DMA4_CDFl5_OFFSET 0x290

#define DMA4_CSAC5 (*(volatile unsigned *)0x480564F4)
#define DMA4_CSAC5_OFFSET 0x294

#define DMA4_CDAC5 (*(volatile unsigned *)0x480564F8)
#define DMA4_CDAC5_OFFSET 0x298

#define DMA4_CCEN5 (*(volatile unsigned *)0x480564FC)
#define DMA4_CCEN5_OFFSET 0x29C

#define DMA4_CCFN5 (*(volatile unsigned *)0x48056500)
#define DMA4_CCFN5_OFFSET 0x2A0

#define DMA4_COLOR5 (*(volatile unsigned *)0x48056504)
#define DMA4_COLOR5_OFFSET 0x2A4

#ifndef SDMA_CHANNEL6_BASE
#define SDMA_CHANNEL6_BASE 0x480562C0
#endif

#define DMA4_CCR6 (*(volatile unsigned *)0x48056580)
#define DMA4_CCR6_OFFSET 0x2C0

#define DMA4_CLNK_CTRL6 (*(volatile unsigned *)0x48056584)
#define DMA4_CLNK_CTRL6_OFFSET 0x2C4

#define DMA4_CICR6 (*(volatile unsigned *)0x48056588)
#define DMA4_CICR6_OFFSET 0x2C8

#define DMA4_CSR6 (*(volatile unsigned *)0x4805658C)
#define DMA4_CSR6_OFFSET 0x2CC

#define DMA4_CSDP6 (*(volatile unsigned *)0x48056590)
#define DMA4_CSDP6_OFFSET 0x2D0

#define DMA4_CEN6 (*(volatile unsigned *)0x48056594)
#define DMA4_CEN6_OFFSET 0x2D4

#define DMA4_CFN6 (*(volatile unsigned *)0x48056598)
#define DMA4_CFN6_OFFSET 0x2D8

#define DMA4_CSSA6 (*(volatile unsigned *)0x4805659C)
#define DMA4_CSSA6_OFFSET 0x2DC

#define DMA4_CDSA6 (*(volatile unsigned *)0x480565A0)
#define DMA4_CDSA6_OFFSET 0x2E0

#define DMA4_CSEL6 (*(volatile unsigned *)0x480565A4)
#define DMA4_CSEL6_OFFSET 0x2E4

#define DMA4_CSFl6 (*(volatile unsigned *)0x480565A8)
#define DMA4_CSFl6_OFFSET 0x2E8

#define DMA4_CDEl6 (*(volatile unsigned *)0x480565AC)
#define DMA4_CDEl6_OFFSET 0x2EC

#define DMA4_CDFl6 (*(volatile unsigned *)0x480565B0)
#define DMA4_CDFl6_OFFSET 0x2F0

#define DMA4_CSAC6 (*(volatile unsigned *)0x480565B4)
#define DMA4_CSAC6_OFFSET 0x2F4

#define DMA4_CDAC6 (*(volatile unsigned *)0x480565B8)
#define DMA4_CDAC6_OFFSET 0x2F8

#define DMA4_CCEN6 (*(volatile unsigned *)0x480565BC)
#define DMA4_CCEN6_OFFSET 0x2FC

#define DMA4_CCFN6 (*(volatile unsigned *)0x480565C0)
#define DMA4_CCFN6_OFFSET 0x300

#define DMA4_COLOR6 (*(volatile unsigned *)0x480565C4)
#define DMA4_COLOR6_OFFSET 0x304

#ifndef SDMA_CHANNEL7_BASE
#define SDMA_CHANNEL7_BASE 0x48056320
#endif

#define DMA4_CCR7 (*(volatile unsigned *)0x48056640)
#define DMA4_CCR7_OFFSET 0x320

#define DMA4_CLNK_CTRL7 (*(volatile unsigned *)0x48056644)
#define DMA4_CLNK_CTRL7_OFFSET 0x324

#define DMA4_CICR7 (*(volatile unsigned *)0x48056648)
#define DMA4_CICR7_OFFSET 0x328

#define DMA4_CSR7 (*(volatile unsigned *)0x4805664C)
#define DMA4_CSR7_OFFSET 0x32C

#define DMA4_CSDP7 (*(volatile unsigned *)0x48056650)
#define DMA4_CSDP7_OFFSET 0x330

#define DMA4_CEN7 (*(volatile unsigned *)0x48056654)
#define DMA4_CEN7_OFFSET 0x334

#define DMA4_CFN7 (*(volatile unsigned *)0x48056658)
#define DMA4_CFN7_OFFSET 0x338

#define DMA4_CSSA7 (*(volatile unsigned *)0x4805665C)
#define DMA4_CSSA7_OFFSET 0x33C

#define DMA4_CDSA7 (*(volatile unsigned *)0x48056660)
#define DMA4_CDSA7_OFFSET 0x340

#define DMA4_CSEL7 (*(volatile unsigned *)0x48056664)
#define DMA4_CSEL7_OFFSET 0x344

#define DMA4_CSFl7 (*(volatile unsigned *)0x48056668)
#define DMA4_CSFl7_OFFSET 0x348

#define DMA4_CDEl7 (*(volatile unsigned *)0x4805666C)
#define DMA4_CDEl7_OFFSET 0x34C

#define DMA4_CDFl7 (*(volatile unsigned *)0x48056670)
#define DMA4_CDFl7_OFFSET 0x350

#define DMA4_CSAC7 (*(volatile unsigned *)0x48056674)
#define DMA4_CSAC7_OFFSET 0x354

#define DMA4_CDAC7 (*(volatile unsigned *)0x48056678)
#define DMA4_CDAC7_OFFSET 0x358

#define DMA4_CCEN7 (*(volatile unsigned *)0x4805667C)
#define DMA4_CCEN7_OFFSET 0x35C

#define DMA4_CCFN7 (*(volatile unsigned *)0x48056680)
#define DMA4_CCFN7_OFFSET 0x360

#define DMA4_COLOR7 (*(volatile unsigned *)0x48056684)
#define DMA4_COLOR7_OFFSET 0x364

#ifndef SDMA_CHANNEL8_BASE
#define SDMA_CHANNEL8_BASE 0x48056380
#endif

#define DMA4_CCR8 (*(volatile unsigned *)0x48056700)
#define DMA4_CCR8_OFFSET 0x380

#define DMA4_CLNK_CTRL8 (*(volatile unsigned *)0x48056704)
#define DMA4_CLNK_CTRL8_OFFSET 0x384

#define DMA4_CICR8 (*(volatile unsigned *)0x48056708)
#define DMA4_CICR8_OFFSET 0x388

#define DMA4_CSR8 (*(volatile unsigned *)0x4805670C)
#define DMA4_CSR8_OFFSET 0x38C

#define DMA4_CSDP8 (*(volatile unsigned *)0x48056710)
#define DMA4_CSDP8_OFFSET 0x390

#define DMA4_CEN8 (*(volatile unsigned *)0x48056714)
#define DMA4_CEN8_OFFSET 0x394

#define DMA4_CFN8 (*(volatile unsigned *)0x48056718)
#define DMA4_CFN8_OFFSET 0x398

#define DMA4_CSSA8 (*(volatile unsigned *)0x4805671C)
#define DMA4_CSSA8_OFFSET 0x39C

#define DMA4_CDSA8 (*(volatile unsigned *)0x48056720)
#define DMA4_CDSA8_OFFSET 0x3A0

#define DMA4_CSEL8 (*(volatile unsigned *)0x48056724)
#define DMA4_CSEL8_OFFSET 0x3A4

#define DMA4_CSFl8 (*(volatile unsigned *)0x48056728)
#define DMA4_CSFl8_OFFSET 0x3A8

#define DMA4_CDEl8 (*(volatile unsigned *)0x4805672C)
#define DMA4_CDEl8_OFFSET 0x3AC

#define DMA4_CDFl8 (*(volatile unsigned *)0x48056730)
#define DMA4_CDFl8_OFFSET 0x3B0

#define DMA4_CSAC8 (*(volatile unsigned *)0x48056734)
#define DMA4_CSAC8_OFFSET 0x3B4

#define DMA4_CDAC8 (*(volatile unsigned *)0x48056738)
#define DMA4_CDAC8_OFFSET 0x3B8

#define DMA4_CCEN8 (*(volatile unsigned *)0x4805673C)
#define DMA4_CCEN8_OFFSET 0x3BC

#define DMA4_CCFN8 (*(volatile unsigned *)0x48056740)
#define DMA4_CCFN8_OFFSET 0x3C0

#define DMA4_COLOR8 (*(volatile unsigned *)0x48056744)
#define DMA4_COLOR8_OFFSET 0x3C4

#ifndef SDMA_CHANNEL9_BASE
#define SDMA_CHANNEL9_BASE 0x480563E0
#endif

#define DMA4_CCR9 (*(volatile unsigned *)0x480567C0)
#define DMA4_CCR9_OFFSET 0x3E0

#define DMA4_CLNK_CTRL9 (*(volatile unsigned *)0x480567C4)
#define DMA4_CLNK_CTRL9_OFFSET 0x3E4

#define DMA4_CICR9 (*(volatile unsigned *)0x480567C8)
#define DMA4_CICR9_OFFSET 0x3E8

#define DMA4_CSR9 (*(volatile unsigned *)0x480567CC)
#define DMA4_CSR9_OFFSET 0x3EC

#define DMA4_CSDP9 (*(volatile unsigned *)0x480567D0)
#define DMA4_CSDP9_OFFSET 0x3F0

#define DMA4_CEN9 (*(volatile unsigned *)0x480567D4)
#define DMA4_CEN9_OFFSET 0x3F4

#define DMA4_CFN9 (*(volatile unsigned *)0x480567D8)
#define DMA4_CFN9_OFFSET 0x3F8

#define DMA4_CSSA9 (*(volatile unsigned *)0x480567DC)
#define DMA4_CSSA9_OFFSET 0x3FC

#define DMA4_CDSA9 (*(volatile unsigned *)0x480567E0)
#define DMA4_CDSA9_OFFSET 0x400

#define DMA4_CSEL9 (*(volatile unsigned *)0x480567E4)
#define DMA4_CSEL9_OFFSET 0x404

#define DMA4_CSFl9 (*(volatile unsigned *)0x480567E8)
#define DMA4_CSFl9_OFFSET 0x408

#define DMA4_CDEl9 (*(volatile unsigned *)0x480567EC)
#define DMA4_CDEl9_OFFSET 0x40C

#define DMA4_CDFl9 (*(volatile unsigned *)0x480567F0)
#define DMA4_CDFl9_OFFSET 0x410

#define DMA4_CSAC9 (*(volatile unsigned *)0x480567F4)
#define DMA4_CSAC9_OFFSET 0x414

#define DMA4_CDAC9 (*(volatile unsigned *)0x480567F8)
#define DMA4_CDAC9_OFFSET 0x418

#define DMA4_CCEN9 (*(volatile unsigned *)0x480567FC)
#define DMA4_CCEN9_OFFSET 0x41C

#define DMA4_CCFN9 (*(volatile unsigned *)0x48056800)
#define DMA4_CCFN9_OFFSET 0x420

#define DMA4_COLOR9 (*(volatile unsigned *)0x48056804)
#define DMA4_COLOR9_OFFSET 0x424

#ifndef SDMA_CHANNEL10_BASE
#define SDMA_CHANNEL10_BASE 0x48056440
#endif

#define DMA4_CCR10 (*(volatile unsigned *)0x48056880)
#define DMA4_CCR10_OFFSET 0x440

#define DMA4_CLNK_CTRL10 (*(volatile unsigned *)0x48056884)
#define DMA4_CLNK_CTRL10_OFFSET 0x444

#define DMA4_CICR10 (*(volatile unsigned *)0x48056888)
#define DMA4_CICR10_OFFSET 0x448

#define DMA4_CSR10 (*(volatile unsigned *)0x4805688C)
#define DMA4_CSR10_OFFSET 0x44C

#define DMA4_CSDP10 (*(volatile unsigned *)0x48056890)
#define DMA4_CSDP10_OFFSET 0x450

#define DMA4_CEN10 (*(volatile unsigned *)0x48056894)
#define DMA4_CEN10_OFFSET 0x454

#define DMA4_CFN10 (*(volatile unsigned *)0x48056898)
#define DMA4_CFN10_OFFSET 0x458

#define DMA4_CSSA10 (*(volatile unsigned *)0x4805689C)
#define DMA4_CSSA10_OFFSET 0x45C

#define DMA4_CDSA10 (*(volatile unsigned *)0x480568A0)
#define DMA4_CDSA10_OFFSET 0x460

#define DMA4_CSEL10 (*(volatile unsigned *)0x480568A4)
#define DMA4_CSEL10_OFFSET 0x464

#define DMA4_CSFl10 (*(volatile unsigned *)0x480568A8)
#define DMA4_CSFl10_OFFSET 0x468

#define DMA4_CDEl10 (*(volatile unsigned *)0x480568AC)
#define DMA4_CDEl10_OFFSET 0x46C

#define DMA4_CDFl10 (*(volatile unsigned *)0x480568B0)
#define DMA4_CDFl10_OFFSET 0x470

#define DMA4_CSAC10 (*(volatile unsigned *)0x480568B4)
#define DMA4_CSAC10_OFFSET 0x474

#define DMA4_CDAC10 (*(volatile unsigned *)0x480568B8)
#define DMA4_CDAC10_OFFSET 0x478

#define DMA4_CCEN10 (*(volatile unsigned *)0x480568BC)
#define DMA4_CCEN10_OFFSET 0x47C

#define DMA4_CCFN10 (*(volatile unsigned *)0x480568C0)
#define DMA4_CCFN10_OFFSET 0x480

#define DMA4_COLOR10 (*(volatile unsigned *)0x480568C4)
#define DMA4_COLOR10_OFFSET 0x484

#ifndef SDMA_CHANNEL11_BASE
#define SDMA_CHANNEL11_BASE 0x480564A0
#endif

#define DMA4_CCR11 (*(volatile unsigned *)0x48056940)
#define DMA4_CCR11_OFFSET 0x4A0

#define DMA4_CLNK_CTRL11 (*(volatile unsigned *)0x48056944)
#define DMA4_CLNK_CTRL11_OFFSET 0x4A4

#define DMA4_CICR11 (*(volatile unsigned *)0x48056948)
#define DMA4_CICR11_OFFSET 0x4A8

#define DMA4_CSR11 (*(volatile unsigned *)0x4805694C)
#define DMA4_CSR11_OFFSET 0x4AC

#define DMA4_CSDP11 (*(volatile unsigned *)0x48056950)
#define DMA4_CSDP11_OFFSET 0x4B0

#define DMA4_CEN11 (*(volatile unsigned *)0x48056954)
#define DMA4_CEN11_OFFSET 0x4B4

#define DMA4_CFN11 (*(volatile unsigned *)0x48056958)
#define DMA4_CFN11_OFFSET 0x4B8

#define DMA4_CSSA11 (*(volatile unsigned *)0x4805695C)
#define DMA4_CSSA11_OFFSET 0x4BC

#define DMA4_CDSA11 (*(volatile unsigned *)0x48056960)
#define DMA4_CDSA11_OFFSET 0x4C0

#define DMA4_CSEL11 (*(volatile unsigned *)0x48056964)
#define DMA4_CSEL11_OFFSET 0x4C4

#define DMA4_CSFl11 (*(volatile unsigned *)0x48056968)
#define DMA4_CSFl11_OFFSET 0x4C8

#define DMA4_CDEl11 (*(volatile unsigned *)0x4805696C)
#define DMA4_CDEl11_OFFSET 0x4CC

#define DMA4_CDFl11 (*(volatile unsigned *)0x48056970)
#define DMA4_CDFl11_OFFSET 0x4D0

#define DMA4_CSAC11 (*(volatile unsigned *)0x48056974)
#define DMA4_CSAC11_OFFSET 0x4D4

#define DMA4_CDAC11 (*(volatile unsigned *)0x48056978)
#define DMA4_CDAC11_OFFSET 0x4D8

#define DMA4_CCEN11 (*(volatile unsigned *)0x4805697C)
#define DMA4_CCEN11_OFFSET 0x4DC

#define DMA4_CCFN11 (*(volatile unsigned *)0x48056980)
#define DMA4_CCFN11_OFFSET 0x4E0

#define DMA4_COLOR11 (*(volatile unsigned *)0x48056984)
#define DMA4_COLOR11_OFFSET 0x4E4

#ifndef SDMA_CHANNEL12_BASE
#define SDMA_CHANNEL12_BASE 0x48056500
#endif

#define DMA4_CCR12 (*(volatile unsigned *)0x48056A00)
#define DMA4_CCR12_OFFSET 0x500

#define DMA4_CLNK_CTRL12 (*(volatile unsigned *)0x48056A04)
#define DMA4_CLNK_CTRL12_OFFSET 0x504

#define DMA4_CICR12 (*(volatile unsigned *)0x48056A08)
#define DMA4_CICR12_OFFSET 0x508

#define DMA4_CSR12 (*(volatile unsigned *)0x48056A0C)
#define DMA4_CSR12_OFFSET 0x50C

#define DMA4_CSDP12 (*(volatile unsigned *)0x48056A10)
#define DMA4_CSDP12_OFFSET 0x510

#define DMA4_CEN12 (*(volatile unsigned *)0x48056A14)
#define DMA4_CEN12_OFFSET 0x514

#define DMA4_CFN12 (*(volatile unsigned *)0x48056A18)
#define DMA4_CFN12_OFFSET 0x518

#define DMA4_CSSA12 (*(volatile unsigned *)0x48056A1C)
#define DMA4_CSSA12_OFFSET 0x51C

#define DMA4_CDSA12 (*(volatile unsigned *)0x48056A20)
#define DMA4_CDSA12_OFFSET 0x520

#define DMA4_CSEL12 (*(volatile unsigned *)0x48056A24)
#define DMA4_CSEL12_OFFSET 0x524

#define DMA4_CSFl12 (*(volatile unsigned *)0x48056A28)
#define DMA4_CSFl12_OFFSET 0x528

#define DMA4_CDEl12 (*(volatile unsigned *)0x48056A2C)
#define DMA4_CDEl12_OFFSET 0x52C

#define DMA4_CDFl12 (*(volatile unsigned *)0x48056A30)
#define DMA4_CDFl12_OFFSET 0x530

#define DMA4_CSAC12 (*(volatile unsigned *)0x48056A34)
#define DMA4_CSAC12_OFFSET 0x534

#define DMA4_CDAC12 (*(volatile unsigned *)0x48056A38)
#define DMA4_CDAC12_OFFSET 0x538

#define DMA4_CCEN12 (*(volatile unsigned *)0x48056A3C)
#define DMA4_CCEN12_OFFSET 0x53C

#define DMA4_CCFN12 (*(volatile unsigned *)0x48056A40)
#define DMA4_CCFN12_OFFSET 0x540

#define DMA4_COLOR12 (*(volatile unsigned *)0x48056A44)
#define DMA4_COLOR12_OFFSET 0x544

#ifndef SDMA_CHANNEL13_BASE
#define SDMA_CHANNEL13_BASE 0x48056560
#endif

#define DMA4_CCR13 (*(volatile unsigned *)0x48056AC0)
#define DMA4_CCR13_OFFSET 0x560

#define DMA4_CLNK_CTRL13 (*(volatile unsigned *)0x48056AC4)
#define DMA4_CLNK_CTRL13_OFFSET 0x564

#define DMA4_CICR13 (*(volatile unsigned *)0x48056AC8)
#define DMA4_CICR13_OFFSET 0x568

#define DMA4_CSR13 (*(volatile unsigned *)0x48056ACC)
#define DMA4_CSR13_OFFSET 0x56C

#define DMA4_CSDP13 (*(volatile unsigned *)0x48056AD0)
#define DMA4_CSDP13_OFFSET 0x570

#define DMA4_CEN13 (*(volatile unsigned *)0x48056AD4)
#define DMA4_CEN13_OFFSET 0x574

#define DMA4_CFN13 (*(volatile unsigned *)0x48056AD8)
#define DMA4_CFN13_OFFSET 0x578

#define DMA4_CSSA13 (*(volatile unsigned *)0x48056ADC)
#define DMA4_CSSA13_OFFSET 0x57C

#define DMA4_CDSA13 (*(volatile unsigned *)0x48056AE0)
#define DMA4_CDSA13_OFFSET 0x580

#define DMA4_CSEL13 (*(volatile unsigned *)0x48056AE4)
#define DMA4_CSEL13_OFFSET 0x584

#define DMA4_CSFl13 (*(volatile unsigned *)0x48056AE8)
#define DMA4_CSFl13_OFFSET 0x588

#define DMA4_CDEl13 (*(volatile unsigned *)0x48056AEC)
#define DMA4_CDEl13_OFFSET 0x58C

#define DMA4_CDFl13 (*(volatile unsigned *)0x48056AF0)
#define DMA4_CDFl13_OFFSET 0x590

#define DMA4_CSAC13 (*(volatile unsigned *)0x48056AF4)
#define DMA4_CSAC13_OFFSET 0x594

#define DMA4_CDAC13 (*(volatile unsigned *)0x48056AF8)
#define DMA4_CDAC13_OFFSET 0x598

#define DMA4_CCEN13 (*(volatile unsigned *)0x48056AFC)
#define DMA4_CCEN13_OFFSET 0x59C

#define DMA4_CCFN13 (*(volatile unsigned *)0x48056B00)
#define DMA4_CCFN13_OFFSET 0x5A0

#define DMA4_COLOR13 (*(volatile unsigned *)0x48056B04)
#define DMA4_COLOR13_OFFSET 0x5A4

#ifndef SDMA_CHANNEL14_BASE
#define SDMA_CHANNEL14_BASE 0x480565C0
#endif

#define DMA4_CCR14 (*(volatile unsigned *)0x48056B80)
#define DMA4_CCR14_OFFSET 0x5C0

#define DMA4_CLNK_CTRL14 (*(volatile unsigned *)0x48056B84)
#define DMA4_CLNK_CTRL14_OFFSET 0x5C4

#define DMA4_CICR14 (*(volatile unsigned *)0x48056B88)
#define DMA4_CICR14_OFFSET 0x5C8

#define DMA4_CSR14 (*(volatile unsigned *)0x48056B8C)
#define DMA4_CSR14_OFFSET 0x5CC

#define DMA4_CSDP14 (*(volatile unsigned *)0x48056B90)
#define DMA4_CSDP14_OFFSET 0x5D0

#define DMA4_CEN14 (*(volatile unsigned *)0x48056B94)
#define DMA4_CEN14_OFFSET 0x5D4

#define DMA4_CFN14 (*(volatile unsigned *)0x48056B98)
#define DMA4_CFN14_OFFSET 0x5D8

#define DMA4_CSSA14 (*(volatile unsigned *)0x48056B9C)
#define DMA4_CSSA14_OFFSET 0x5DC

#define DMA4_CDSA14 (*(volatile unsigned *)0x48056BA0)
#define DMA4_CDSA14_OFFSET 0x5E0

#define DMA4_CSEL14 (*(volatile unsigned *)0x48056BA4)
#define DMA4_CSEL14_OFFSET 0x5E4

#define DMA4_CSFl14 (*(volatile unsigned *)0x48056BA8)
#define DMA4_CSFl14_OFFSET 0x5E8

#define DMA4_CDEl14 (*(volatile unsigned *)0x48056BAC)
#define DMA4_CDEl14_OFFSET 0x5EC

#define DMA4_CDFl14 (*(volatile unsigned *)0x48056BB0)
#define DMA4_CDFl14_OFFSET 0x5F0

#define DMA4_CSAC14 (*(volatile unsigned *)0x48056BB4)
#define DMA4_CSAC14_OFFSET 0x5F4

#define DMA4_CDAC14 (*(volatile unsigned *)0x48056BB8)
#define DMA4_CDAC14_OFFSET 0x5F8

#define DMA4_CCEN14 (*(volatile unsigned *)0x48056BBC)
#define DMA4_CCEN14_OFFSET 0x5FC

#define DMA4_CCFN14 (*(volatile unsigned *)0x48056BC0)
#define DMA4_CCFN14_OFFSET 0x600

#define DMA4_COLOR14 (*(volatile unsigned *)0x48056BC4)
#define DMA4_COLOR14_OFFSET 0x604

#ifndef SDMA_CHANNEL15_BASE
#define SDMA_CHANNEL15_BASE 0x48056620
#endif

#define DMA4_CCR15 (*(volatile unsigned *)0x48056C40)
#define DMA4_CCR15_OFFSET 0x620

#define DMA4_CLNK_CTRL15 (*(volatile unsigned *)0x48056C44)
#define DMA4_CLNK_CTRL15_OFFSET 0x624

#define DMA4_CICR15 (*(volatile unsigned *)0x48056C48)
#define DMA4_CICR15_OFFSET 0x628

#define DMA4_CSR15 (*(volatile unsigned *)0x48056C4C)
#define DMA4_CSR15_OFFSET 0x62C

#define DMA4_CSDP15 (*(volatile unsigned *)0x48056C50)
#define DMA4_CSDP15_OFFSET 0x630

#define DMA4_CEN15 (*(volatile unsigned *)0x48056C54)
#define DMA4_CEN15_OFFSET 0x634

#define DMA4_CFN15 (*(volatile unsigned *)0x48056C58)
#define DMA4_CFN15_OFFSET 0x638

#define DMA4_CSSA15 (*(volatile unsigned *)0x48056C5C)
#define DMA4_CSSA15_OFFSET 0x63C

#define DMA4_CDSA15 (*(volatile unsigned *)0x48056C60)
#define DMA4_CDSA15_OFFSET 0x640

#define DMA4_CSEL15 (*(volatile unsigned *)0x48056C64)
#define DMA4_CSEL15_OFFSET 0x644

#define DMA4_CSFl15 (*(volatile unsigned *)0x48056C68)
#define DMA4_CSFl15_OFFSET 0x648

#define DMA4_CDEl15 (*(volatile unsigned *)0x48056C6C)
#define DMA4_CDEl15_OFFSET 0x64C

#define DMA4_CDFl15 (*(volatile unsigned *)0x48056C70)
#define DMA4_CDFl15_OFFSET 0x650

#define DMA4_CSAC15 (*(volatile unsigned *)0x48056C74)
#define DMA4_CSAC15_OFFSET 0x654

#define DMA4_CDAC15 (*(volatile unsigned *)0x48056C78)
#define DMA4_CDAC15_OFFSET 0x658

#define DMA4_CCEN15 (*(volatile unsigned *)0x48056C7C)
#define DMA4_CCEN15_OFFSET 0x65C

#define DMA4_CCFN15 (*(volatile unsigned *)0x48056C80)
#define DMA4_CCFN15_OFFSET 0x660

#define DMA4_COLOR15 (*(volatile unsigned *)0x48056C84)
#define DMA4_COLOR15_OFFSET 0x664

#ifndef SDMA_CHANNEL16_BASE
#define SDMA_CHANNEL16_BASE 0x48056680
#endif

#define DMA4_CCR16 (*(volatile unsigned *)0x48056D00)
#define DMA4_CCR16_OFFSET 0x680

#define DMA4_CLNK_CTRL16 (*(volatile unsigned *)0x48056D04)
#define DMA4_CLNK_CTRL16_OFFSET 0x684

#define DMA4_CICR16 (*(volatile unsigned *)0x48056D08)
#define DMA4_CICR16_OFFSET 0x688

#define DMA4_CSR16 (*(volatile unsigned *)0x48056D0C)
#define DMA4_CSR16_OFFSET 0x68C

#define DMA4_CSDP16 (*(volatile unsigned *)0x48056D10)
#define DMA4_CSDP16_OFFSET 0x690

#define DMA4_CEN16 (*(volatile unsigned *)0x48056D14)
#define DMA4_CEN16_OFFSET 0x694

#define DMA4_CFN16 (*(volatile unsigned *)0x48056D18)
#define DMA4_CFN16_OFFSET 0x698

#define DMA4_CSSA16 (*(volatile unsigned *)0x48056D1C)
#define DMA4_CSSA16_OFFSET 0x69C

#define DMA4_CDSA16 (*(volatile unsigned *)0x48056D20)
#define DMA4_CDSA16_OFFSET 0x6A0

#define DMA4_CSEL16 (*(volatile unsigned *)0x48056D24)
#define DMA4_CSEL16_OFFSET 0x6A4

#define DMA4_CSFl16 (*(volatile unsigned *)0x48056D28)
#define DMA4_CSFl16_OFFSET 0x6A8

#define DMA4_CDEl16 (*(volatile unsigned *)0x48056D2C)
#define DMA4_CDEl16_OFFSET 0x6AC

#define DMA4_CDFl16 (*(volatile unsigned *)0x48056D30)
#define DMA4_CDFl16_OFFSET 0x6B0

#define DMA4_CSAC16 (*(volatile unsigned *)0x48056D34)
#define DMA4_CSAC16_OFFSET 0x6B4

#define DMA4_CDAC16 (*(volatile unsigned *)0x48056D38)
#define DMA4_CDAC16_OFFSET 0x6B8

#define DMA4_CCEN16 (*(volatile unsigned *)0x48056D3C)
#define DMA4_CCEN16_OFFSET 0x6BC

#define DMA4_CCFN16 (*(volatile unsigned *)0x48056D40)
#define DMA4_CCFN16_OFFSET 0x6C0

#define DMA4_COLOR16 (*(volatile unsigned *)0x48056D44)
#define DMA4_COLOR16_OFFSET 0x6C4

#ifndef SDMA_CHANNEL17_BASE
#define SDMA_CHANNEL17_BASE 0x480566E0
#endif

#define DMA4_CCR17 (*(volatile unsigned *)0x48056DC0)
#define DMA4_CCR17_OFFSET 0x6E0

#define DMA4_CLNK_CTRL17 (*(volatile unsigned *)0x48056DC4)
#define DMA4_CLNK_CTRL17_OFFSET 0x6E4

#define DMA4_CICR17 (*(volatile unsigned *)0x48056DC8)
#define DMA4_CICR17_OFFSET 0x6E8

#define DMA4_CSR17 (*(volatile unsigned *)0x48056DCC)
#define DMA4_CSR17_OFFSET 0x6EC

#define DMA4_CSDP17 (*(volatile unsigned *)0x48056DD0)
#define DMA4_CSDP17_OFFSET 0x6F0

#define DMA4_CEN17 (*(volatile unsigned *)0x48056DD4)
#define DMA4_CEN17_OFFSET 0x6F4

#define DMA4_CFN17 (*(volatile unsigned *)0x48056DD8)
#define DMA4_CFN17_OFFSET 0x6F8

#define DMA4_CSSA17 (*(volatile unsigned *)0x48056DDC)
#define DMA4_CSSA17_OFFSET 0x6FC

#define DMA4_CDSA17 (*(volatile unsigned *)0x48056DE0)
#define DMA4_CDSA17_OFFSET 0x700

#define DMA4_CSEL17 (*(volatile unsigned *)0x48056DE4)
#define DMA4_CSEL17_OFFSET 0x704

#define DMA4_CSFl17 (*(volatile unsigned *)0x48056DE8)
#define DMA4_CSFl17_OFFSET 0x708

#define DMA4_CDEl17 (*(volatile unsigned *)0x48056DEC)
#define DMA4_CDEl17_OFFSET 0x70C

#define DMA4_CDFl17 (*(volatile unsigned *)0x48056DF0)
#define DMA4_CDFl17_OFFSET 0x710

#define DMA4_CSAC17 (*(volatile unsigned *)0x48056DF4)
#define DMA4_CSAC17_OFFSET 0x714

#define DMA4_CDAC17 (*(volatile unsigned *)0x48056DF8)
#define DMA4_CDAC17_OFFSET 0x718

#define DMA4_CCEN17 (*(volatile unsigned *)0x48056DFC)
#define DMA4_CCEN17_OFFSET 0x71C

#define DMA4_CCFN17 (*(volatile unsigned *)0x48056E00)
#define DMA4_CCFN17_OFFSET 0x720

#define DMA4_COLOR17 (*(volatile unsigned *)0x48056E04)
#define DMA4_COLOR17_OFFSET 0x724

#ifndef SDMA_CHANNEL18_BASE
#define SDMA_CHANNEL18_BASE 0x48056740
#endif

#define DMA4_CCR18 (*(volatile unsigned *)0x48056E80)
#define DMA4_CCR18_OFFSET 0x740

#define DMA4_CLNK_CTRL18 (*(volatile unsigned *)0x48056E84)
#define DMA4_CLNK_CTRL18_OFFSET 0x744

#define DMA4_CICR18 (*(volatile unsigned *)0x48056E88)
#define DMA4_CICR18_OFFSET 0x748

#define DMA4_CSR18 (*(volatile unsigned *)0x48056E8C)
#define DMA4_CSR18_OFFSET 0x74C

#define DMA4_CSDP18 (*(volatile unsigned *)0x48056E90)
#define DMA4_CSDP18_OFFSET 0x750

#define DMA4_CEN18 (*(volatile unsigned *)0x48056E94)
#define DMA4_CEN18_OFFSET 0x754

#define DMA4_CFN18 (*(volatile unsigned *)0x48056E98)
#define DMA4_CFN18_OFFSET 0x758

#define DMA4_CSSA18 (*(volatile unsigned *)0x48056E9C)
#define DMA4_CSSA18_OFFSET 0x75C

#define DMA4_CDSA18 (*(volatile unsigned *)0x48056EA0)
#define DMA4_CDSA18_OFFSET 0x760

#define DMA4_CSEL18 (*(volatile unsigned *)0x48056EA4)
#define DMA4_CSEL18_OFFSET 0x764

#define DMA4_CSFl18 (*(volatile unsigned *)0x48056EA8)
#define DMA4_CSFl18_OFFSET 0x768

#define DMA4_CDEl18 (*(volatile unsigned *)0x48056EAC)
#define DMA4_CDEl18_OFFSET 0x76C

#define DMA4_CDFl18 (*(volatile unsigned *)0x48056EB0)
#define DMA4_CDFl18_OFFSET 0x770

#define DMA4_CSAC18 (*(volatile unsigned *)0x48056EB4)
#define DMA4_CSAC18_OFFSET 0x774

#define DMA4_CDAC18 (*(volatile unsigned *)0x48056EB8)
#define DMA4_CDAC18_OFFSET 0x778

#define DMA4_CCEN18 (*(volatile unsigned *)0x48056EBC)
#define DMA4_CCEN18_OFFSET 0x77C

#define DMA4_CCFN18 (*(volatile unsigned *)0x48056EC0)
#define DMA4_CCFN18_OFFSET 0x780

#define DMA4_COLOR18 (*(volatile unsigned *)0x48056EC4)
#define DMA4_COLOR18_OFFSET 0x784

#ifndef SDMA_CHANNEL19_BASE
#define SDMA_CHANNEL19_BASE 0x480567A0
#endif

#define DMA4_CCR19 (*(volatile unsigned *)0x48056F40)
#define DMA4_CCR19_OFFSET 0x7A0

#define DMA4_CLNK_CTRL19 (*(volatile unsigned *)0x48056F44)
#define DMA4_CLNK_CTRL19_OFFSET 0x7A4

#define DMA4_CICR19 (*(volatile unsigned *)0x48056F48)
#define DMA4_CICR19_OFFSET 0x7A8

#define DMA4_CSR19 (*(volatile unsigned *)0x48056F4C)
#define DMA4_CSR19_OFFSET 0x7AC

#define DMA4_CSDP19 (*(volatile unsigned *)0x48056F50)
#define DMA4_CSDP19_OFFSET 0x7B0

#define DMA4_CEN19 (*(volatile unsigned *)0x48056F54)
#define DMA4_CEN19_OFFSET 0x7B4

#define DMA4_CFN19 (*(volatile unsigned *)0x48056F58)
#define DMA4_CFN19_OFFSET 0x7B8

#define DMA4_CSSA19 (*(volatile unsigned *)0x48056F5C)
#define DMA4_CSSA19_OFFSET 0x7BC

#define DMA4_CDSA19 (*(volatile unsigned *)0x48056F60)
#define DMA4_CDSA19_OFFSET 0x7C0

#define DMA4_CSEL19 (*(volatile unsigned *)0x48056F64)
#define DMA4_CSEL19_OFFSET 0x7C4

#define DMA4_CSFl19 (*(volatile unsigned *)0x48056F68)
#define DMA4_CSFl19_OFFSET 0x7C8

#define DMA4_CDEl19 (*(volatile unsigned *)0x48056F6C)
#define DMA4_CDEl19_OFFSET 0x7CC

#define DMA4_CDFl19 (*(volatile unsigned *)0x48056F70)
#define DMA4_CDFl19_OFFSET 0x7D0

#define DMA4_CSAC19 (*(volatile unsigned *)0x48056F74)
#define DMA4_CSAC19_OFFSET 0x7D4

#define DMA4_CDAC19 (*(volatile unsigned *)0x48056F78)
#define DMA4_CDAC19_OFFSET 0x7D8

#define DMA4_CCEN19 (*(volatile unsigned *)0x48056F7C)
#define DMA4_CCEN19_OFFSET 0x7DC

#define DMA4_CCFN19 (*(volatile unsigned *)0x48056F80)
#define DMA4_CCFN19_OFFSET 0x7E0

#define DMA4_COLOR19 (*(volatile unsigned *)0x48056F84)
#define DMA4_COLOR19_OFFSET 0x7E4

#ifndef SDMA_CHANNEL20_BASE
#define SDMA_CHANNEL20_BASE 0x48056800
#endif

#define DMA4_CCR20 (*(volatile unsigned *)0x48057000)
#define DMA4_CCR20_OFFSET 0x800

#define DMA4_CLNK_CTRL20 (*(volatile unsigned *)0x48057004)
#define DMA4_CLNK_CTRL20_OFFSET 0x804

#define DMA4_CICR20 (*(volatile unsigned *)0x48057008)
#define DMA4_CICR20_OFFSET 0x808

#define DMA4_CSR20 (*(volatile unsigned *)0x4805700C)
#define DMA4_CSR20_OFFSET 0x80C

#define DMA4_CSDP20 (*(volatile unsigned *)0x48057010)
#define DMA4_CSDP20_OFFSET 0x810

#define DMA4_CEN20 (*(volatile unsigned *)0x48057014)
#define DMA4_CEN20_OFFSET 0x814

#define DMA4_CFN20 (*(volatile unsigned *)0x48057018)
#define DMA4_CFN20_OFFSET 0x818

#define DMA4_CSSA20 (*(volatile unsigned *)0x4805701C)
#define DMA4_CSSA20_OFFSET 0x81C

#define DMA4_CDSA20 (*(volatile unsigned *)0x48057020)
#define DMA4_CDSA20_OFFSET 0x820

#define DMA4_CSEL20 (*(volatile unsigned *)0x48057024)
#define DMA4_CSEL20_OFFSET 0x824

#define DMA4_CSFl20 (*(volatile unsigned *)0x48057028)
#define DMA4_CSFl20_OFFSET 0x828

#define DMA4_CDEl20 (*(volatile unsigned *)0x4805702C)
#define DMA4_CDEl20_OFFSET 0x82C

#define DMA4_CDFl20 (*(volatile unsigned *)0x48057030)
#define DMA4_CDFl20_OFFSET 0x830

#define DMA4_CSAC20 (*(volatile unsigned *)0x48057034)
#define DMA4_CSAC20_OFFSET 0x834

#define DMA4_CDAC20 (*(volatile unsigned *)0x48057038)
#define DMA4_CDAC20_OFFSET 0x838

#define DMA4_CCEN20 (*(volatile unsigned *)0x4805703C)
#define DMA4_CCEN20_OFFSET 0x83C

#define DMA4_CCFN20 (*(volatile unsigned *)0x48057040)
#define DMA4_CCFN20_OFFSET 0x840

#define DMA4_COLOR20 (*(volatile unsigned *)0x48057044)
#define DMA4_COLOR20_OFFSET 0x844

#ifndef SDMA_CHANNEL21_BASE
#define SDMA_CHANNEL21_BASE 0x48056860
#endif

#define DMA4_CCR21 (*(volatile unsigned *)0x480570C0)
#define DMA4_CCR21_OFFSET 0x860

#define DMA4_CLNK_CTRL21 (*(volatile unsigned *)0x480570C4)
#define DMA4_CLNK_CTRL21_OFFSET 0x864

#define DMA4_CICR21 (*(volatile unsigned *)0x480570C8)
#define DMA4_CICR21_OFFSET 0x868

#define DMA4_CSR21 (*(volatile unsigned *)0x480570CC)
#define DMA4_CSR21_OFFSET 0x86C

#define DMA4_CSDP21 (*(volatile unsigned *)0x480570D0)
#define DMA4_CSDP21_OFFSET 0x870

#define DMA4_CEN21 (*(volatile unsigned *)0x480570D4)
#define DMA4_CEN21_OFFSET 0x874

#define DMA4_CFN21 (*(volatile unsigned *)0x480570D8)
#define DMA4_CFN21_OFFSET 0x878

#define DMA4_CSSA21 (*(volatile unsigned *)0x480570DC)
#define DMA4_CSSA21_OFFSET 0x87C

#define DMA4_CDSA21 (*(volatile unsigned *)0x480570E0)
#define DMA4_CDSA21_OFFSET 0x880

#define DMA4_CSEL21 (*(volatile unsigned *)0x480570E4)
#define DMA4_CSEL21_OFFSET 0x884

#define DMA4_CSFl21 (*(volatile unsigned *)0x480570E8)
#define DMA4_CSFl21_OFFSET 0x888

#define DMA4_CDEl21 (*(volatile unsigned *)0x480570EC)
#define DMA4_CDEl21_OFFSET 0x88C

#define DMA4_CDFl21 (*(volatile unsigned *)0x480570F0)
#define DMA4_CDFl21_OFFSET 0x890

#define DMA4_CSAC21 (*(volatile unsigned *)0x480570F4)
#define DMA4_CSAC21_OFFSET 0x894

#define DMA4_CDAC21 (*(volatile unsigned *)0x480570F8)
#define DMA4_CDAC21_OFFSET 0x898

#define DMA4_CCEN21 (*(volatile unsigned *)0x480570FC)
#define DMA4_CCEN21_OFFSET 0x89C

#define DMA4_CCFN21 (*(volatile unsigned *)0x48057100)
#define DMA4_CCFN21_OFFSET 0x8A0

#define DMA4_COLOR21 (*(volatile unsigned *)0x48057104)
#define DMA4_COLOR21_OFFSET 0x8A4

#ifndef SDMA_CHANNEL22_BASE
#define SDMA_CHANNEL22_BASE 0x480568C0
#endif

#define DMA4_CCR22 (*(volatile unsigned *)0x48057180)
#define DMA4_CCR22_OFFSET 0x8C0

#define DMA4_CLNK_CTRL22 (*(volatile unsigned *)0x48057184)
#define DMA4_CLNK_CTRL22_OFFSET 0x8C4

#define DMA4_CICR22 (*(volatile unsigned *)0x48057188)
#define DMA4_CICR22_OFFSET 0x8C8

#define DMA4_CSR22 (*(volatile unsigned *)0x4805718C)
#define DMA4_CSR22_OFFSET 0x8CC

#define DMA4_CSDP22 (*(volatile unsigned *)0x48057190)
#define DMA4_CSDP22_OFFSET 0x8D0

#define DMA4_CEN22 (*(volatile unsigned *)0x48057194)
#define DMA4_CEN22_OFFSET 0x8D4

#define DMA4_CFN22 (*(volatile unsigned *)0x48057198)
#define DMA4_CFN22_OFFSET 0x8D8

#define DMA4_CSSA22 (*(volatile unsigned *)0x4805719C)
#define DMA4_CSSA22_OFFSET 0x8DC

#define DMA4_CDSA22 (*(volatile unsigned *)0x480571A0)
#define DMA4_CDSA22_OFFSET 0x8E0

#define DMA4_CSEL22 (*(volatile unsigned *)0x480571A4)
#define DMA4_CSEL22_OFFSET 0x8E4

#define DMA4_CSFl22 (*(volatile unsigned *)0x480571A8)
#define DMA4_CSFl22_OFFSET 0x8E8

#define DMA4_CDEl22 (*(volatile unsigned *)0x480571AC)
#define DMA4_CDEl22_OFFSET 0x8EC

#define DMA4_CDFl22 (*(volatile unsigned *)0x480571B0)
#define DMA4_CDFl22_OFFSET 0x8F0

#define DMA4_CSAC22 (*(volatile unsigned *)0x480571B4)
#define DMA4_CSAC22_OFFSET 0x8F4

#define DMA4_CDAC22 (*(volatile unsigned *)0x480571B8)
#define DMA4_CDAC22_OFFSET 0x8F8

#define DMA4_CCEN22 (*(volatile unsigned *)0x480571BC)
#define DMA4_CCEN22_OFFSET 0x8FC

#define DMA4_CCFN22 (*(volatile unsigned *)0x480571C0)
#define DMA4_CCFN22_OFFSET 0x900

#define DMA4_COLOR22 (*(volatile unsigned *)0x480571C4)
#define DMA4_COLOR22_OFFSET 0x904

#ifndef SDMA_CHANNEL23_BASE
#define SDMA_CHANNEL23_BASE 0x48056920
#endif

#define DMA4_CCR23 (*(volatile unsigned *)0x48057240)
#define DMA4_CCR23_OFFSET 0x920

#define DMA4_CLNK_CTRL23 (*(volatile unsigned *)0x48057244)
#define DMA4_CLNK_CTRL23_OFFSET 0x924

#define DMA4_CICR23 (*(volatile unsigned *)0x48057248)
#define DMA4_CICR23_OFFSET 0x928

#define DMA4_CSR23 (*(volatile unsigned *)0x4805724C)
#define DMA4_CSR23_OFFSET 0x92C

#define DMA4_CSDP23 (*(volatile unsigned *)0x48057250)
#define DMA4_CSDP23_OFFSET 0x930

#define DMA4_CEN23 (*(volatile unsigned *)0x48057254)
#define DMA4_CEN23_OFFSET 0x934

#define DMA4_CFN23 (*(volatile unsigned *)0x48057258)
#define DMA4_CFN23_OFFSET 0x938

#define DMA4_CSSA23 (*(volatile unsigned *)0x4805725C)
#define DMA4_CSSA23_OFFSET 0x93C

#define DMA4_CDSA23 (*(volatile unsigned *)0x48057260)
#define DMA4_CDSA23_OFFSET 0x940

#define DMA4_CSEL23 (*(volatile unsigned *)0x48057264)
#define DMA4_CSEL23_OFFSET 0x944

#define DMA4_CSFl23 (*(volatile unsigned *)0x48057268)
#define DMA4_CSFl23_OFFSET 0x948

#define DMA4_CDEl23 (*(volatile unsigned *)0x4805726C)
#define DMA4_CDEl23_OFFSET 0x94C

#define DMA4_CDFl23 (*(volatile unsigned *)0x48057270)
#define DMA4_CDFl23_OFFSET 0x950

#define DMA4_CSAC23 (*(volatile unsigned *)0x48057274)
#define DMA4_CSAC23_OFFSET 0x954

#define DMA4_CDAC23 (*(volatile unsigned *)0x48057278)
#define DMA4_CDAC23_OFFSET 0x958

#define DMA4_CCEN23 (*(volatile unsigned *)0x4805727C)
#define DMA4_CCEN23_OFFSET 0x95C

#define DMA4_CCFN23 (*(volatile unsigned *)0x48057280)
#define DMA4_CCFN23_OFFSET 0x960

#define DMA4_COLOR23 (*(volatile unsigned *)0x48057284)
#define DMA4_COLOR23_OFFSET 0x964

#ifndef SDMA_CHANNEL24_BASE
#define SDMA_CHANNEL24_BASE 0x48056980
#endif

#define DMA4_CCR24 (*(volatile unsigned *)0x48057300)
#define DMA4_CCR24_OFFSET 0x980

#define DMA4_CLNK_CTRL24 (*(volatile unsigned *)0x48057304)
#define DMA4_CLNK_CTRL24_OFFSET 0x984

#define DMA4_CICR24 (*(volatile unsigned *)0x48057308)
#define DMA4_CICR24_OFFSET 0x988

#define DMA4_CSR24 (*(volatile unsigned *)0x4805730C)
#define DMA4_CSR24_OFFSET 0x98C

#define DMA4_CSDP24 (*(volatile unsigned *)0x48057310)
#define DMA4_CSDP24_OFFSET 0x990

#define DMA4_CEN24 (*(volatile unsigned *)0x48057314)
#define DMA4_CEN24_OFFSET 0x994

#define DMA4_CFN24 (*(volatile unsigned *)0x48057318)
#define DMA4_CFN24_OFFSET 0x998

#define DMA4_CSSA24 (*(volatile unsigned *)0x4805731C)
#define DMA4_CSSA24_OFFSET 0x99C

#define DMA4_CDSA24 (*(volatile unsigned *)0x48057320)
#define DMA4_CDSA24_OFFSET 0x9A0

#define DMA4_CSEL24 (*(volatile unsigned *)0x48057324)
#define DMA4_CSEL24_OFFSET 0x9A4

#define DMA4_CSFl24 (*(volatile unsigned *)0x48057328)
#define DMA4_CSFl24_OFFSET 0x9A8

#define DMA4_CDEl24 (*(volatile unsigned *)0x4805732C)
#define DMA4_CDEl24_OFFSET 0x9AC

#define DMA4_CDFl24 (*(volatile unsigned *)0x48057330)
#define DMA4_CDFl24_OFFSET 0x9B0

#define DMA4_CSAC24 (*(volatile unsigned *)0x48057334)
#define DMA4_CSAC24_OFFSET 0x9B4

#define DMA4_CDAC24 (*(volatile unsigned *)0x48057338)
#define DMA4_CDAC24_OFFSET 0x9B8

#define DMA4_CCEN24 (*(volatile unsigned *)0x4805733C)
#define DMA4_CCEN24_OFFSET 0x9BC

#define DMA4_CCFN24 (*(volatile unsigned *)0x48057340)
#define DMA4_CCFN24_OFFSET 0x9C0

#define DMA4_COLOR24 (*(volatile unsigned *)0x48057344)
#define DMA4_COLOR24_OFFSET 0x9C4

#ifndef SDMA_CHANNEL25_BASE
#define SDMA_CHANNEL25_BASE 0x480569E0
#endif

#define DMA4_CCR25 (*(volatile unsigned *)0x480573C0)
#define DMA4_CCR25_OFFSET 0x9E0

#define DMA4_CLNK_CTRL25 (*(volatile unsigned *)0x480573C4)
#define DMA4_CLNK_CTRL25_OFFSET 0x9E4

#define DMA4_CICR25 (*(volatile unsigned *)0x480573C8)
#define DMA4_CICR25_OFFSET 0x9E8

#define DMA4_CSR25 (*(volatile unsigned *)0x480573CC)
#define DMA4_CSR25_OFFSET 0x9EC

#define DMA4_CSDP25 (*(volatile unsigned *)0x480573D0)
#define DMA4_CSDP25_OFFSET 0x9F0

#define DMA4_CEN25 (*(volatile unsigned *)0x480573D4)
#define DMA4_CEN25_OFFSET 0x9F4

#define DMA4_CFN25 (*(volatile unsigned *)0x480573D8)
#define DMA4_CFN25_OFFSET 0x9F8

#define DMA4_CSSA25 (*(volatile unsigned *)0x480573DC)
#define DMA4_CSSA25_OFFSET 0x9FC

#define DMA4_CDSA25 (*(volatile unsigned *)0x480573E0)
#define DMA4_CDSA25_OFFSET 0xA00

#define DMA4_CSEL25 (*(volatile unsigned *)0x480573E4)
#define DMA4_CSEL25_OFFSET 0xA04

#define DMA4_CSFl25 (*(volatile unsigned *)0x480573E8)
#define DMA4_CSFl25_OFFSET 0xA08

#define DMA4_CDEl25 (*(volatile unsigned *)0x480573EC)
#define DMA4_CDEl25_OFFSET 0xA0C

#define DMA4_CDFl25 (*(volatile unsigned *)0x480573F0)
#define DMA4_CDFl25_OFFSET 0xA10

#define DMA4_CSAC25 (*(volatile unsigned *)0x480573F4)
#define DMA4_CSAC25_OFFSET 0xA14

#define DMA4_CDAC25 (*(volatile unsigned *)0x480573F8)
#define DMA4_CDAC25_OFFSET 0xA18

#define DMA4_CCEN25 (*(volatile unsigned *)0x480573FC)
#define DMA4_CCEN25_OFFSET 0xA1C

#define DMA4_CCFN25 (*(volatile unsigned *)0x48057400)
#define DMA4_CCFN25_OFFSET 0xA20

#define DMA4_COLOR25 (*(volatile unsigned *)0x48057404)
#define DMA4_COLOR25_OFFSET 0xA24

#ifndef SDMA_CHANNEL26_BASE
#define SDMA_CHANNEL26_BASE 0x48056A40
#endif

#define DMA4_CCR26 (*(volatile unsigned *)0x48057480)
#define DMA4_CCR26_OFFSET 0xA40

#define DMA4_CLNK_CTRL26 (*(volatile unsigned *)0x48057484)
#define DMA4_CLNK_CTRL26_OFFSET 0xA44

#define DMA4_CICR26 (*(volatile unsigned *)0x48057488)
#define DMA4_CICR26_OFFSET 0xA48

#define DMA4_CSR26 (*(volatile unsigned *)0x4805748C)
#define DMA4_CSR26_OFFSET 0xA4C

#define DMA4_CSDP26 (*(volatile unsigned *)0x48057490)
#define DMA4_CSDP26_OFFSET 0xA50

#define DMA4_CEN26 (*(volatile unsigned *)0x48057494)
#define DMA4_CEN26_OFFSET 0xA54

#define DMA4_CFN26 (*(volatile unsigned *)0x48057498)
#define DMA4_CFN26_OFFSET 0xA58

#define DMA4_CSSA26 (*(volatile unsigned *)0x4805749C)
#define DMA4_CSSA26_OFFSET 0xA5C

#define DMA4_CDSA26 (*(volatile unsigned *)0x480574A0)
#define DMA4_CDSA26_OFFSET 0xA60

#define DMA4_CSEL26 (*(volatile unsigned *)0x480574A4)
#define DMA4_CSEL26_OFFSET 0xA64

#define DMA4_CSFl26 (*(volatile unsigned *)0x480574A8)
#define DMA4_CSFl26_OFFSET 0xA68

#define DMA4_CDEl26 (*(volatile unsigned *)0x480574AC)
#define DMA4_CDEl26_OFFSET 0xA6C

#define DMA4_CDFl26 (*(volatile unsigned *)0x480574B0)
#define DMA4_CDFl26_OFFSET 0xA70

#define DMA4_CSAC26 (*(volatile unsigned *)0x480574B4)
#define DMA4_CSAC26_OFFSET 0xA74

#define DMA4_CDAC26 (*(volatile unsigned *)0x480574B8)
#define DMA4_CDAC26_OFFSET 0xA78

#define DMA4_CCEN26 (*(volatile unsigned *)0x480574BC)
#define DMA4_CCEN26_OFFSET 0xA7C

#define DMA4_CCFN26 (*(volatile unsigned *)0x480574C0)
#define DMA4_CCFN26_OFFSET 0xA80

#define DMA4_COLOR26 (*(volatile unsigned *)0x480574C4)
#define DMA4_COLOR26_OFFSET 0xA84

#ifndef SDMA_CHANNEL27_BASE
#define SDMA_CHANNEL27_BASE 0x48056AA0
#endif

#define DMA4_CCR27 (*(volatile unsigned *)0x48057540)
#define DMA4_CCR27_OFFSET 0xAA0

#define DMA4_CLNK_CTRL27 (*(volatile unsigned *)0x48057544)
#define DMA4_CLNK_CTRL27_OFFSET 0xAA4

#define DMA4_CICR27 (*(volatile unsigned *)0x48057548)
#define DMA4_CICR27_OFFSET 0xAA8

#define DMA4_CSR27 (*(volatile unsigned *)0x4805754C)
#define DMA4_CSR27_OFFSET 0xAAC

#define DMA4_CSDP27 (*(volatile unsigned *)0x48057550)
#define DMA4_CSDP27_OFFSET 0xAB0

#define DMA4_CEN27 (*(volatile unsigned *)0x48057554)
#define DMA4_CEN27_OFFSET 0xAB4

#define DMA4_CFN27 (*(volatile unsigned *)0x48057558)
#define DMA4_CFN27_OFFSET 0xAB8

#define DMA4_CSSA27 (*(volatile unsigned *)0x4805755C)
#define DMA4_CSSA27_OFFSET 0xABC

#define DMA4_CDSA27 (*(volatile unsigned *)0x48057560)
#define DMA4_CDSA27_OFFSET 0xAC0

#define DMA4_CSEL27 (*(volatile unsigned *)0x48057564)
#define DMA4_CSEL27_OFFSET 0xAC4

#define DMA4_CSFl27 (*(volatile unsigned *)0x48057568)
#define DMA4_CSFl27_OFFSET 0xAC8

#define DMA4_CDEl27 (*(volatile unsigned *)0x4805756C)
#define DMA4_CDEl27_OFFSET 0xACC

#define DMA4_CDFl27 (*(volatile unsigned *)0x48057570)
#define DMA4_CDFl27_OFFSET 0xAD0

#define DMA4_CSAC27 (*(volatile unsigned *)0x48057574)
#define DMA4_CSAC27_OFFSET 0xAD4

#define DMA4_CDAC27 (*(volatile unsigned *)0x48057578)
#define DMA4_CDAC27_OFFSET 0xAD8

#define DMA4_CCEN27 (*(volatile unsigned *)0x4805757C)
#define DMA4_CCEN27_OFFSET 0xADC

#define DMA4_CCFN27 (*(volatile unsigned *)0x48057580)
#define DMA4_CCFN27_OFFSET 0xAE0

#define DMA4_COLOR27 (*(volatile unsigned *)0x48057584)
#define DMA4_COLOR27_OFFSET 0xAE4

#ifndef SDMA_CHANNEL28_BASE
#define SDMA_CHANNEL28_BASE 0x48056B00
#endif

#define DMA4_CCR28 (*(volatile unsigned *)0x48057600)
#define DMA4_CCR28_OFFSET 0xB00

#define DMA4_CLNK_CTRL28 (*(volatile unsigned *)0x48057604)
#define DMA4_CLNK_CTRL28_OFFSET 0xB04

#define DMA4_CICR28 (*(volatile unsigned *)0x48057608)
#define DMA4_CICR28_OFFSET 0xB08

#define DMA4_CSR28 (*(volatile unsigned *)0x4805760C)
#define DMA4_CSR28_OFFSET 0xB0C

#define DMA4_CSDP28 (*(volatile unsigned *)0x48057610)
#define DMA4_CSDP28_OFFSET 0xB10

#define DMA4_CEN28 (*(volatile unsigned *)0x48057614)
#define DMA4_CEN28_OFFSET 0xB14

#define DMA4_CFN28 (*(volatile unsigned *)0x48057618)
#define DMA4_CFN28_OFFSET 0xB18

#define DMA4_CSSA28 (*(volatile unsigned *)0x4805761C)
#define DMA4_CSSA28_OFFSET 0xB1C

#define DMA4_CDSA28 (*(volatile unsigned *)0x48057620)
#define DMA4_CDSA28_OFFSET 0xB20

#define DMA4_CSEL28 (*(volatile unsigned *)0x48057624)
#define DMA4_CSEL28_OFFSET 0xB24

#define DMA4_CSFl28 (*(volatile unsigned *)0x48057628)
#define DMA4_CSFl28_OFFSET 0xB28

#define DMA4_CDEl28 (*(volatile unsigned *)0x4805762C)
#define DMA4_CDEl28_OFFSET 0xB2C

#define DMA4_CDFl28 (*(volatile unsigned *)0x48057630)
#define DMA4_CDFl28_OFFSET 0xB30

#define DMA4_CSAC28 (*(volatile unsigned *)0x48057634)
#define DMA4_CSAC28_OFFSET 0xB34

#define DMA4_CDAC28 (*(volatile unsigned *)0x48057638)
#define DMA4_CDAC28_OFFSET 0xB38

#define DMA4_CCEN28 (*(volatile unsigned *)0x4805763C)
#define DMA4_CCEN28_OFFSET 0xB3C

#define DMA4_CCFN28 (*(volatile unsigned *)0x48057640)
#define DMA4_CCFN28_OFFSET 0xB40

#define DMA4_COLOR28 (*(volatile unsigned *)0x48057644)
#define DMA4_COLOR28_OFFSET 0xB44

#ifndef SDMA_CHANNEL29_BASE
#define SDMA_CHANNEL29_BASE 0x48056B60
#endif

#define DMA4_CCR29 (*(volatile unsigned *)0x480576C0)
#define DMA4_CCR29_OFFSET 0xB60

#define DMA4_CLNK_CTRL29 (*(volatile unsigned *)0x480576C4)
#define DMA4_CLNK_CTRL29_OFFSET 0xB64

#define DMA4_CICR29 (*(volatile unsigned *)0x480576C8)
#define DMA4_CICR29_OFFSET 0xB68

#define DMA4_CSR29 (*(volatile unsigned *)0x480576CC)
#define DMA4_CSR29_OFFSET 0xB6C

#define DMA4_CSDP29 (*(volatile unsigned *)0x480576D0)
#define DMA4_CSDP29_OFFSET 0xB70

#define DMA4_CEN29 (*(volatile unsigned *)0x480576D4)
#define DMA4_CEN29_OFFSET 0xB74

#define DMA4_CFN29 (*(volatile unsigned *)0x480576D8)
#define DMA4_CFN29_OFFSET 0xB78

#define DMA4_CSSA29 (*(volatile unsigned *)0x480576DC)
#define DMA4_CSSA29_OFFSET 0xB7C

#define DMA4_CDSA29 (*(volatile unsigned *)0x480576E0)
#define DMA4_CDSA29_OFFSET 0xB80

#define DMA4_CSEL29 (*(volatile unsigned *)0x480576E4)
#define DMA4_CSEL29_OFFSET 0xB84

#define DMA4_CSFl29 (*(volatile unsigned *)0x480576E8)
#define DMA4_CSFl29_OFFSET 0xB88

#define DMA4_CDEl29 (*(volatile unsigned *)0x480576EC)
#define DMA4_CDEl29_OFFSET 0xB8C

#define DMA4_CDFl29 (*(volatile unsigned *)0x480576F0)
#define DMA4_CDFl29_OFFSET 0xB90

#define DMA4_CSAC29 (*(volatile unsigned *)0x480576F4)
#define DMA4_CSAC29_OFFSET 0xB94

#define DMA4_CDAC29 (*(volatile unsigned *)0x480576F8)
#define DMA4_CDAC29_OFFSET 0xB98

#define DMA4_CCEN29 (*(volatile unsigned *)0x480576FC)
#define DMA4_CCEN29_OFFSET 0xB9C

#define DMA4_CCFN29 (*(volatile unsigned *)0x48057700)
#define DMA4_CCFN29_OFFSET 0xBA0

#define DMA4_COLOR29 (*(volatile unsigned *)0x48057704)
#define DMA4_COLOR29_OFFSET 0xBA4

#ifndef SDMA_CHANNEL30_BASE
#define SDMA_CHANNEL30_BASE 0x48056BC0
#endif

#define DMA4_CCR30 (*(volatile unsigned *)0x48057780)
#define DMA4_CCR30_OFFSET 0xBC0

#define DMA4_CLNK_CTRL30 (*(volatile unsigned *)0x48057784)
#define DMA4_CLNK_CTRL30_OFFSET 0xBC4

#define DMA4_CICR30 (*(volatile unsigned *)0x48057788)
#define DMA4_CICR30_OFFSET 0xBC8

#define DMA4_CSR30 (*(volatile unsigned *)0x4805778C)
#define DMA4_CSR30_OFFSET 0xBCC

#define DMA4_CSDP30 (*(volatile unsigned *)0x48057790)
#define DMA4_CSDP30_OFFSET 0xBD0

#define DMA4_CEN30 (*(volatile unsigned *)0x48057794)
#define DMA4_CEN30_OFFSET 0xBD4

#define DMA4_CFN30 (*(volatile unsigned *)0x48057798)
#define DMA4_CFN30_OFFSET 0xBD8

#define DMA4_CSSA30 (*(volatile unsigned *)0x4805779C)
#define DMA4_CSSA30_OFFSET 0xBDC

#define DMA4_CDSA30 (*(volatile unsigned *)0x480577A0)
#define DMA4_CDSA30_OFFSET 0xBE0

#define DMA4_CSEL30 (*(volatile unsigned *)0x480577A4)
#define DMA4_CSEL30_OFFSET 0xBE4

#define DMA4_CSFl30 (*(volatile unsigned *)0x480577A8)
#define DMA4_CSFl30_OFFSET 0xBE8

#define DMA4_CDEl30 (*(volatile unsigned *)0x480577AC)
#define DMA4_CDEl30_OFFSET 0xBEC

#define DMA4_CDFl30 (*(volatile unsigned *)0x480577B0)
#define DMA4_CDFl30_OFFSET 0xBF0

#define DMA4_CSAC30 (*(volatile unsigned *)0x480577B4)
#define DMA4_CSAC30_OFFSET 0xBF4

#define DMA4_CDAC30 (*(volatile unsigned *)0x480577B8)
#define DMA4_CDAC30_OFFSET 0xBF8

#define DMA4_CCEN30 (*(volatile unsigned *)0x480577BC)
#define DMA4_CCEN30_OFFSET 0xBFC

#define DMA4_CCFN30 (*(volatile unsigned *)0x480577C0)
#define DMA4_CCFN30_OFFSET 0xC00

#define DMA4_COLOR30 (*(volatile unsigned *)0x480577C4)
#define DMA4_COLOR30_OFFSET 0xC04

#ifndef SDMA_CHANNEL31_BASE
#define SDMA_CHANNEL31_BASE 0x48056C20
#endif

#define DMA4_CCR31 (*(volatile unsigned *)0x48057840)
#define DMA4_CCR31_OFFSET 0xC20

#define DMA4_CLNK_CTRL31 (*(volatile unsigned *)0x48057844)
#define DMA4_CLNK_CTRL31_OFFSET 0xC24

#define DMA4_CICR31 (*(volatile unsigned *)0x48057848)
#define DMA4_CICR31_OFFSET 0xC28

#define DMA4_CSR31 (*(volatile unsigned *)0x4805784C)
#define DMA4_CSR31_OFFSET 0xC2C

#define DMA4_CSDP31 (*(volatile unsigned *)0x48057850)
#define DMA4_CSDP31_OFFSET 0xC30

#define DMA4_CEN31 (*(volatile unsigned *)0x48057854)
#define DMA4_CEN31_OFFSET 0xC34

#define DMA4_CFN31 (*(volatile unsigned *)0x48057858)
#define DMA4_CFN31_OFFSET 0xC38

#define DMA4_CSSA31 (*(volatile unsigned *)0x4805785C)
#define DMA4_CSSA31_OFFSET 0xC3C

#define DMA4_CDSA31 (*(volatile unsigned *)0x48057860)
#define DMA4_CDSA31_OFFSET 0xC40

#define DMA4_CSEL31 (*(volatile unsigned *)0x48057864)
#define DMA4_CSEL31_OFFSET 0xC44

#define DMA4_CSFl31 (*(volatile unsigned *)0x48057868)
#define DMA4_CSFl31_OFFSET 0xC48

#define DMA4_CDEl31 (*(volatile unsigned *)0x4805786C)
#define DMA4_CDEl31_OFFSET 0xC4C

#define DMA4_CDFl31 (*(volatile unsigned *)0x48057870)
#define DMA4_CDFl31_OFFSET 0xC50

#define DMA4_CSAC31 (*(volatile unsigned *)0x48057874)
#define DMA4_CSAC31_OFFSET 0xC54

#define DMA4_CDAC31 (*(volatile unsigned *)0x48057878)
#define DMA4_CDAC31_OFFSET 0xC58

#define DMA4_CCEN31 (*(volatile unsigned *)0x4805787C)
#define DMA4_CCEN31_OFFSET 0xC5C

#define DMA4_CCFN31 (*(volatile unsigned *)0x48057880)
#define DMA4_CCFN31_OFFSET 0xC60

#define DMA4_COLOR31 (*(volatile unsigned *)0x48057884)
#define DMA4_COLOR31_OFFSET 0xC64

#ifndef I2C3_BASE
#define I2C3_BASE 0x48060000
#endif

#define I2C3_REV (*(volatile unsigned *)0x48060000)
#define I2C3_REV_OFFSET 0x0

#define I2C3_IE (*(volatile unsigned short *)0x48060004)
#define I2C3_IE_OFFSET 0x4

#define I2C3_STAT (*(volatile unsigned short *)0x48060008)
#define I2C3_STAT_OFFSET 0x8

#define I2C3_WE (*(volatile unsigned short *)0x4806000C)
#define I2C3_WE_OFFSET 0xC

#define I2C3_SYSS (*(volatile unsigned short *)0x48060010)
#define I2C3_SYSS_OFFSET 0x10

#define I2C3_BUF (*(volatile unsigned short *)0x48060014)
#define I2C3_BUF_OFFSET 0x14

#define I2C3_CNT (*(volatile unsigned short *)0x48060018)
#define I2C3_CNT_OFFSET 0x18

#define I2C3_DATA (*(volatile unsigned short *)0x4806001C)
#define I2C3_DATA_OFFSET 0x1C

#define I2C3_SYSC (*(volatile unsigned short *)0x48060020)
#define I2C3_SYSC_OFFSET 0x20

#define I2C3_CON (*(volatile unsigned short *)0x48060024)
#define I2C3_CON_OFFSET 0x24

#define I2C3_OA0 (*(volatile unsigned short *)0x48060028)
#define I2C3_OA0_OFFSET 0x28

#define I2C3_SA (*(volatile unsigned short *)0x4806002C)
#define I2C3_SA_OFFSET 0x2C

#define I2C3_PSC (*(volatile unsigned short *)0x48060030)
#define I2C3_PSC_OFFSET 0x30

#define I2C3_SCLL (*(volatile unsigned short *)0x48060034)
#define I2C3_SCLL_OFFSET 0x34

#define I2C3_SCLH (*(volatile unsigned short *)0x48060038)
#define I2C3_SCLH_OFFSET 0x38

#define I2C3_SYSTEST (*(volatile unsigned short *)0x4806003C)
#define I2C3_SYSTEST_OFFSET 0x3C

#define I2C3_BUFSTAT (*(volatile unsigned short *)0x48060040)
#define I2C3_BUFSTAT_OFFSET 0x40

#define I2C3_OA1 (*(volatile unsigned short *)0x48060044)
#define I2C3_OA1_OFFSET 0x44

#define I2C3_OA2 (*(volatile unsigned short *)0x48060048)
#define I2C3_OA2_OFFSET 0x48

#define I2C3_OA3 (*(volatile unsigned short *)0x4806004C)
#define I2C3_OA3_OFFSET 0x4C

#define I2C3_ACTOA (*(volatile unsigned short *)0x48060050)
#define I2C3_ACTOA_OFFSET 0x50

#define I2C3_SBLOCK (*(volatile unsigned short *)0x48060054)
#define I2C3_SBLOCK_OFFSET 0x54

#ifndef HS_USB_USBTLL_BASE
#define HS_USB_USBTLL_BASE 0x48062000
#endif

#define USBTLL_REVISION (*(volatile unsigned *)0x48062000)
#define USBTLL_REVISION_OFFSET 0x0

#define USBTLL_SYSCONFIG (*(volatile unsigned *)0x48062010)
#define USBTLL_SYSCONFIG_OFFSET 0x10

#define USBTLL_SYSSTATUS (*(volatile unsigned *)0x48062014)
#define USBTLL_SYSSTATUS_OFFSET 0x14

#define USBTLL_IRQSTATUS (*(volatile unsigned *)0x48062018)
#define USBTLL_IRQSTATUS_OFFSET 0x18

#define USBTLL_IRQENABLE (*(volatile unsigned *)0x4806201C)
#define USBTLL_IRQENABLE_OFFSET 0x1C

#define TLL_SHARED_CONF (*(volatile unsigned *)0x48062030)
#define TLL_SHARED_CONF_OFFSET 0x30

#define TLL_CHANNEL_CONF_0 (*(volatile unsigned *)0x48062000)
#define TLL_CHANNEL_CONF_0_OFFSET 0x0

#define TLL_CHANNEL_CONF_1 (*(volatile unsigned *)0x48062000)
#define TLL_CHANNEL_CONF_1_OFFSET 0x0

#define TLL_CHANNEL_CONF_2 (*(volatile unsigned *)0x48062000)
#define TLL_CHANNEL_CONF_2_OFFSET 0x0

#ifndef HS_USB_USBTLL_ULPI0_BASE
#define HS_USB_USBTLL_ULPI0_BASE 0x48062800
#endif

#define ULPI_VENDOR_ID_LO_0 (*(volatile unsigned char *)0x48062800)
#define ULPI_VENDOR_ID_LO_0_OFFSET 0x0

#define ULPI_VENDOR_ID_HI_0 (*(volatile unsigned char *)0x48062801)
#define ULPI_VENDOR_ID_HI_0_OFFSET 0x1

#define ULPI_PRODUCT_ID_LO_0 (*(volatile unsigned char *)0x48062802)
#define ULPI_PRODUCT_ID_LO_0_OFFSET 0x2

#define ULPI_PRODUCT_ID_HI_0 (*(volatile unsigned char *)0x48062803)
#define ULPI_PRODUCT_ID_HI_0_OFFSET 0x3

#define ULPI_FUNCTION_CTRL_0 (*(volatile unsigned char *)0x48062804)
#define ULPI_FUNCTION_CTRL_0_OFFSET 0x4

#define ULPI_FUNCTION_CTRL_SET_0 (*(volatile unsigned char *)0x48062805)
#define ULPI_FUNCTION_CTRL_SET_0_OFFSET 0x5

#define ULPI_FUNCTION_CTRL_CLR_0 (*(volatile unsigned char *)0x48062806)
#define ULPI_FUNCTION_CTRL_CLR_0_OFFSET 0x6

#define ULPI_INTERFACE_CTRL_0 (*(volatile unsigned char *)0x48062807)
#define ULPI_INTERFACE_CTRL_0_OFFSET 0x7

#define ULPI_INTERFACE_CTRL_SET_0 (*(volatile unsigned char *)0x48062808)
#define ULPI_INTERFACE_CTRL_SET_0_OFFSET 0x8

#define ULPI_INTERFACE_CTRL_CLR_0 (*(volatile unsigned char *)0x48062809)
#define ULPI_INTERFACE_CTRL_CLR_0_OFFSET 0x9

#define ULPI_OTG_CTRL_0 (*(volatile unsigned char *)0x4806280A)
#define ULPI_OTG_CTRL_0_OFFSET 0xA

#define ULPI_OTG_CTRL_SET_0 (*(volatile unsigned char *)0x4806280B)
#define ULPI_OTG_CTRL_SET_0_OFFSET 0xB

#define ULPI_OTG_CTRL_CLR_0 (*(volatile unsigned char *)0x4806280C)
#define ULPI_OTG_CTRL_CLR_0_OFFSET 0xC

#define ULPI_USB_INT_EN_RISE_0 (*(volatile unsigned char *)0x4806280D)
#define ULPI_USB_INT_EN_RISE_0_OFFSET 0xD

#define ULPI_USB_INT_EN_RISE_SET_0 (*(volatile unsigned char *)0x4806280E)
#define ULPI_USB_INT_EN_RISE_SET_0_OFFSET 0xE

#define ULPI_USB_INT_EN_RISE_CLR_0 (*(volatile unsigned char *)0x4806280F)
#define ULPI_USB_INT_EN_RISE_CLR_0_OFFSET 0xF

#define ULPI_USB_INT_EN_FALL_0 (*(volatile unsigned char *)0x48062810)
#define ULPI_USB_INT_EN_FALL_0_OFFSET 0x10

#define ULPI_USB_INT_EN_FALL_SET_0 (*(volatile unsigned char *)0x48062811)
#define ULPI_USB_INT_EN_FALL_SET_0_OFFSET 0x11

#define ULPI_USB_INT_EN_FALL_CLR_0 (*(volatile unsigned char *)0x48062812)
#define ULPI_USB_INT_EN_FALL_CLR_0_OFFSET 0x12

#define ULPI_USB_INT_STATUS_0 (*(volatile unsigned char *)0x48062813)
#define ULPI_USB_INT_STATUS_0_OFFSET 0x13

#define ULPI_USB_INT_LATCH_0 (*(volatile unsigned char *)0x48062814)
#define ULPI_USB_INT_LATCH_0_OFFSET 0x14

#define ULPI_DEBUG_0 (*(volatile unsigned char *)0x48062815)
#define ULPI_DEBUG_0_OFFSET 0x15

#define ULPI_SCRATCH_REGISTER_0 (*(volatile unsigned char *)0x48062816)
#define ULPI_SCRATCH_REGISTER_0_OFFSET 0x16

#define ULPI_SCRATCH_REGISTER_SET_0 (*(volatile unsigned char *)0x48062817)
#define ULPI_SCRATCH_REGISTER_SET_0_OFFSET 0x17

#define ULPI_SCRATCH_REGISTER_CLR_0 (*(volatile unsigned char *)0x48062818)
#define ULPI_SCRATCH_REGISTER_CLR_0_OFFSET 0x18

#define ULPI_EXTENDED_SET_ACCESS_0 (*(volatile unsigned char *)0x4806282F)
#define ULPI_EXTENDED_SET_ACCESS_0_OFFSET 0x2F

#define ULPI_UTMI_VCONTROL_EN_0 (*(volatile unsigned char *)0x48062830)
#define ULPI_UTMI_VCONTROL_EN_0_OFFSET 0x30

#define ULPI_UTMI_VCONTROL_EN_SET_0 (*(volatile unsigned char *)0x48062831)
#define ULPI_UTMI_VCONTROL_EN_SET_0_OFFSET 0x31

#define ULPI_UTMI_VCONTROL_EN_CLR_0 (*(volatile unsigned char *)0x48062832)
#define ULPI_UTMI_VCONTROL_EN_CLR_0_OFFSET 0x32

#define ULPI_UTMI_VCONTROL_STATUS_0 (*(volatile unsigned char *)0x48062833)
#define ULPI_UTMI_VCONTROL_STATUS_0_OFFSET 0x33

#define ULPI_UTMI_VCONTROL_LATCH_0 (*(volatile unsigned char *)0x48062834)
#define ULPI_UTMI_VCONTROL_LATCH_0_OFFSET 0x34

#define ULPI_UTMI_VSTATUS_0 (*(volatile unsigned char *)0x48062835)
#define ULPI_UTMI_VSTATUS_0_OFFSET 0x35

#define ULPI_UTMI_VSTATUS_SET_0 (*(volatile unsigned char *)0x48062836)
#define ULPI_UTMI_VSTATUS_SET_0_OFFSET 0x36

#define ULPI_UTMI_VSTATUS_CLR_0 (*(volatile unsigned char *)0x48062837)
#define ULPI_UTMI_VSTATUS_CLR_0_OFFSET 0x37

#define ULPI_USB_INT_LATCH_NOCLR_0 (*(volatile unsigned char *)0x48062838)
#define ULPI_USB_INT_LATCH_NOCLR_0_OFFSET 0x38

#define ULPI_VENDOR_INT_EN_0 (*(volatile unsigned char *)0x4806283B)
#define ULPI_VENDOR_INT_EN_0_OFFSET 0x3B

#define ULPI_VENDOR_INT_EN_SET_0 (*(volatile unsigned char *)0x4806283C)
#define ULPI_VENDOR_INT_EN_SET_0_OFFSET 0x3C

#define ULPI_VENDOR_INT_EN_CLR_0 (*(volatile unsigned char *)0x4806283D)
#define ULPI_VENDOR_INT_EN_CLR_0_OFFSET 0x3D

#define ULPI_VENDOR_INT_STATUS_0 (*(volatile unsigned char *)0x4806283E)
#define ULPI_VENDOR_INT_STATUS_0_OFFSET 0x3E

#define ULPI_VENDOR_INT_LATCH_0 (*(volatile unsigned char *)0x4806283F)
#define ULPI_VENDOR_INT_LATCH_0_OFFSET 0x3F

#ifndef HS_USB_USBTLL_ULPI1_BASE
#define HS_USB_USBTLL_ULPI1_BASE 0x48062900
#endif

#define ULPI_VENDOR_ID_LO_1 (*(volatile unsigned char *)0x48062900)
#define ULPI_VENDOR_ID_LO_1_OFFSET 0x0

#define ULPI_VENDOR_ID_HI_1 (*(volatile unsigned char *)0x48062901)
#define ULPI_VENDOR_ID_HI_1_OFFSET 0x1

#define ULPI_PRODUCT_ID_LO_1 (*(volatile unsigned char *)0x48062902)
#define ULPI_PRODUCT_ID_LO_1_OFFSET 0x2

#define ULPI_PRODUCT_ID_HI_1 (*(volatile unsigned char *)0x48062903)
#define ULPI_PRODUCT_ID_HI_1_OFFSET 0x3

#define ULPI_FUNCTION_CTRL_1 (*(volatile unsigned char *)0x48062904)
#define ULPI_FUNCTION_CTRL_1_OFFSET 0x4

#define ULPI_FUNCTION_CTRL_SET_1 (*(volatile unsigned char *)0x48062905)
#define ULPI_FUNCTION_CTRL_SET_1_OFFSET 0x5

#define ULPI_FUNCTION_CTRL_CLR_1 (*(volatile unsigned char *)0x48062906)
#define ULPI_FUNCTION_CTRL_CLR_1_OFFSET 0x6

#define ULPI_INTERFACE_CTRL_1 (*(volatile unsigned char *)0x48062907)
#define ULPI_INTERFACE_CTRL_1_OFFSET 0x7

#define ULPI_INTERFACE_CTRL_SET_1 (*(volatile unsigned char *)0x48062908)
#define ULPI_INTERFACE_CTRL_SET_1_OFFSET 0x8

#define ULPI_INTERFACE_CTRL_CLR_1 (*(volatile unsigned char *)0x48062909)
#define ULPI_INTERFACE_CTRL_CLR_1_OFFSET 0x9

#define ULPI_OTG_CTRL_1 (*(volatile unsigned char *)0x4806290A)
#define ULPI_OTG_CTRL_1_OFFSET 0xA

#define ULPI_OTG_CTRL_SET_1 (*(volatile unsigned char *)0x4806290B)
#define ULPI_OTG_CTRL_SET_1_OFFSET 0xB

#define ULPI_OTG_CTRL_CLR_1 (*(volatile unsigned char *)0x4806290C)
#define ULPI_OTG_CTRL_CLR_1_OFFSET 0xC

#define ULPI_USB_INT_EN_RISE_1 (*(volatile unsigned char *)0x4806290D)
#define ULPI_USB_INT_EN_RISE_1_OFFSET 0xD

#define ULPI_USB_INT_EN_RISE_SET_1 (*(volatile unsigned char *)0x4806290E)
#define ULPI_USB_INT_EN_RISE_SET_1_OFFSET 0xE

#define ULPI_USB_INT_EN_RISE_CLR_1 (*(volatile unsigned char *)0x4806290F)
#define ULPI_USB_INT_EN_RISE_CLR_1_OFFSET 0xF

#define ULPI_USB_INT_EN_FALL_1 (*(volatile unsigned char *)0x48062910)
#define ULPI_USB_INT_EN_FALL_1_OFFSET 0x10

#define ULPI_USB_INT_EN_FALL_SET_1 (*(volatile unsigned char *)0x48062911)
#define ULPI_USB_INT_EN_FALL_SET_1_OFFSET 0x11

#define ULPI_USB_INT_EN_FALL_CLR_1 (*(volatile unsigned char *)0x48062912)
#define ULPI_USB_INT_EN_FALL_CLR_1_OFFSET 0x12

#define ULPI_USB_INT_STATUS_1 (*(volatile unsigned char *)0x48062913)
#define ULPI_USB_INT_STATUS_1_OFFSET 0x13

#define ULPI_USB_INT_LATCH_1 (*(volatile unsigned char *)0x48062914)
#define ULPI_USB_INT_LATCH_1_OFFSET 0x14

#define ULPI_DEBUG_1 (*(volatile unsigned char *)0x48062915)
#define ULPI_DEBUG_1_OFFSET 0x15

#define ULPI_SCRATCH_REGISTER_1 (*(volatile unsigned char *)0x48062916)
#define ULPI_SCRATCH_REGISTER_1_OFFSET 0x16

#define ULPI_SCRATCH_REGISTER_SET_1 (*(volatile unsigned char *)0x48062917)
#define ULPI_SCRATCH_REGISTER_SET_1_OFFSET 0x17

#define ULPI_SCRATCH_REGISTER_CLR_1 (*(volatile unsigned char *)0x48062918)
#define ULPI_SCRATCH_REGISTER_CLR_1_OFFSET 0x18

#define ULPI_EXTENDED_SET_ACCESS_1 (*(volatile unsigned char *)0x4806292F)
#define ULPI_EXTENDED_SET_ACCESS_1_OFFSET 0x2F

#define ULPI_UTMI_VCONTROL_EN_1 (*(volatile unsigned char *)0x48062930)
#define ULPI_UTMI_VCONTROL_EN_1_OFFSET 0x30

#define ULPI_UTMI_VCONTROL_EN_SET_1 (*(volatile unsigned char *)0x48062931)
#define ULPI_UTMI_VCONTROL_EN_SET_1_OFFSET 0x31

#define ULPI_UTMI_VCONTROL_EN_CLR_1 (*(volatile unsigned char *)0x48062932)
#define ULPI_UTMI_VCONTROL_EN_CLR_1_OFFSET 0x32

#define ULPI_UTMI_VCONTROL_STATUS_1 (*(volatile unsigned char *)0x48062933)
#define ULPI_UTMI_VCONTROL_STATUS_1_OFFSET 0x33

#define ULPI_UTMI_VCONTROL_LATCH_1 (*(volatile unsigned char *)0x48062934)
#define ULPI_UTMI_VCONTROL_LATCH_1_OFFSET 0x34

#define ULPI_UTMI_VSTATUS_1 (*(volatile unsigned char *)0x48062935)
#define ULPI_UTMI_VSTATUS_1_OFFSET 0x35

#define ULPI_UTMI_VSTATUS_SET_1 (*(volatile unsigned char *)0x48062936)
#define ULPI_UTMI_VSTATUS_SET_1_OFFSET 0x36

#define ULPI_UTMI_VSTATUS_CLR_1 (*(volatile unsigned char *)0x48062937)
#define ULPI_UTMI_VSTATUS_CLR_1_OFFSET 0x37

#define ULPI_USB_INT_LATCH_NOCLR_1 (*(volatile unsigned char *)0x48062938)
#define ULPI_USB_INT_LATCH_NOCLR_1_OFFSET 0x38

#define ULPI_VENDOR_INT_EN_1 (*(volatile unsigned char *)0x4806293B)
#define ULPI_VENDOR_INT_EN_1_OFFSET 0x3B

#define ULPI_VENDOR_INT_EN_SET_1 (*(volatile unsigned char *)0x4806293C)
#define ULPI_VENDOR_INT_EN_SET_1_OFFSET 0x3C

#define ULPI_VENDOR_INT_EN_CLR_1 (*(volatile unsigned char *)0x4806293D)
#define ULPI_VENDOR_INT_EN_CLR_1_OFFSET 0x3D

#define ULPI_VENDOR_INT_STATUS_1 (*(volatile unsigned char *)0x4806293E)
#define ULPI_VENDOR_INT_STATUS_1_OFFSET 0x3E

#define ULPI_VENDOR_INT_LATCH_1 (*(volatile unsigned char *)0x4806293F)
#define ULPI_VENDOR_INT_LATCH_1_OFFSET 0x3F

#ifndef HS_USB_USBTLL_ULPI2_BASE
#define HS_USB_USBTLL_ULPI2_BASE 0x48062A00
#endif

#define ULPI_VENDOR_ID_LO_2 (*(volatile unsigned char *)0x48062A00)
#define ULPI_VENDOR_ID_LO_2_OFFSET 0x0

#define ULPI_VENDOR_ID_HI_2 (*(volatile unsigned char *)0x48062A01)
#define ULPI_VENDOR_ID_HI_2_OFFSET 0x1

#define ULPI_PRODUCT_ID_LO_2 (*(volatile unsigned char *)0x48062A02)
#define ULPI_PRODUCT_ID_LO_2_OFFSET 0x2

#define ULPI_PRODUCT_ID_HI_2 (*(volatile unsigned char *)0x48062A03)
#define ULPI_PRODUCT_ID_HI_2_OFFSET 0x3

#define ULPI_FUNCTION_CTRL_2 (*(volatile unsigned char *)0x48062A04)
#define ULPI_FUNCTION_CTRL_2_OFFSET 0x4

#define ULPI_FUNCTION_CTRL_SET_2 (*(volatile unsigned char *)0x48062A05)
#define ULPI_FUNCTION_CTRL_SET_2_OFFSET 0x5

#define ULPI_FUNCTION_CTRL_CLR_2 (*(volatile unsigned char *)0x48062A06)
#define ULPI_FUNCTION_CTRL_CLR_2_OFFSET 0x6

#define ULPI_INTERFACE_CTRL_2 (*(volatile unsigned char *)0x48062A07)
#define ULPI_INTERFACE_CTRL_2_OFFSET 0x7

#define ULPI_INTERFACE_CTRL_SET_2 (*(volatile unsigned char *)0x48062A08)
#define ULPI_INTERFACE_CTRL_SET_2_OFFSET 0x8

#define ULPI_INTERFACE_CTRL_CLR_2 (*(volatile unsigned char *)0x48062A09)
#define ULPI_INTERFACE_CTRL_CLR_2_OFFSET 0x9

#define ULPI_OTG_CTRL_2 (*(volatile unsigned char *)0x48062A0A)
#define ULPI_OTG_CTRL_2_OFFSET 0xA

#define ULPI_OTG_CTRL_SET_2 (*(volatile unsigned char *)0x48062A0B)
#define ULPI_OTG_CTRL_SET_2_OFFSET 0xB

#define ULPI_OTG_CTRL_CLR_2 (*(volatile unsigned char *)0x48062A0C)
#define ULPI_OTG_CTRL_CLR_2_OFFSET 0xC

#define ULPI_USB_INT_EN_RISE_2 (*(volatile unsigned char *)0x48062A0D)
#define ULPI_USB_INT_EN_RISE_2_OFFSET 0xD

#define ULPI_USB_INT_EN_RISE_SET_2 (*(volatile unsigned char *)0x48062A0E)
#define ULPI_USB_INT_EN_RISE_SET_2_OFFSET 0xE

#define ULPI_USB_INT_EN_RISE_CLR_2 (*(volatile unsigned char *)0x48062A0F)
#define ULPI_USB_INT_EN_RISE_CLR_2_OFFSET 0xF

#define ULPI_USB_INT_EN_FALL_2 (*(volatile unsigned char *)0x48062A10)
#define ULPI_USB_INT_EN_FALL_2_OFFSET 0x10

#define ULPI_USB_INT_EN_FALL_SET_2 (*(volatile unsigned char *)0x48062A11)
#define ULPI_USB_INT_EN_FALL_SET_2_OFFSET 0x11

#define ULPI_USB_INT_EN_FALL_CLR_2 (*(volatile unsigned char *)0x48062A12)
#define ULPI_USB_INT_EN_FALL_CLR_2_OFFSET 0x12

#define ULPI_USB_INT_STATUS_2 (*(volatile unsigned char *)0x48062A13)
#define ULPI_USB_INT_STATUS_2_OFFSET 0x13

#define ULPI_USB_INT_LATCH_2 (*(volatile unsigned char *)0x48062A14)
#define ULPI_USB_INT_LATCH_2_OFFSET 0x14

#define ULPI_DEBUG_2 (*(volatile unsigned char *)0x48062A15)
#define ULPI_DEBUG_2_OFFSET 0x15

#define ULPI_SCRATCH_REGISTER_2 (*(volatile unsigned char *)0x48062A16)
#define ULPI_SCRATCH_REGISTER_2_OFFSET 0x16

#define ULPI_SCRATCH_REGISTER_SET_2 (*(volatile unsigned char *)0x48062A17)
#define ULPI_SCRATCH_REGISTER_SET_2_OFFSET 0x17

#define ULPI_SCRATCH_REGISTER_CLR_2 (*(volatile unsigned char *)0x48062A18)
#define ULPI_SCRATCH_REGISTER_CLR_2_OFFSET 0x18

#define ULPI_EXTENDED_SET_ACCESS_2 (*(volatile unsigned char *)0x48062A2F)
#define ULPI_EXTENDED_SET_ACCESS_2_OFFSET 0x2F

#define ULPI_UTMI_VCONTROL_EN_2 (*(volatile unsigned char *)0x48062A30)
#define ULPI_UTMI_VCONTROL_EN_2_OFFSET 0x30

#define ULPI_UTMI_VCONTROL_EN_SET_2 (*(volatile unsigned char *)0x48062A31)
#define ULPI_UTMI_VCONTROL_EN_SET_2_OFFSET 0x31

#define ULPI_UTMI_VCONTROL_EN_CLR_2 (*(volatile unsigned char *)0x48062A32)
#define ULPI_UTMI_VCONTROL_EN_CLR_2_OFFSET 0x32

#define ULPI_UTMI_VCONTROL_STATUS_2 (*(volatile unsigned char *)0x48062A33)
#define ULPI_UTMI_VCONTROL_STATUS_2_OFFSET 0x33

#define ULPI_UTMI_VCONTROL_LATCH_2 (*(volatile unsigned char *)0x48062A34)
#define ULPI_UTMI_VCONTROL_LATCH_2_OFFSET 0x34

#define ULPI_UTMI_VSTATUS_2 (*(volatile unsigned char *)0x48062A35)
#define ULPI_UTMI_VSTATUS_2_OFFSET 0x35

#define ULPI_UTMI_VSTATUS_SET_2 (*(volatile unsigned char *)0x48062A36)
#define ULPI_UTMI_VSTATUS_SET_2_OFFSET 0x36

#define ULPI_UTMI_VSTATUS_CLR_2 (*(volatile unsigned char *)0x48062A37)
#define ULPI_UTMI_VSTATUS_CLR_2_OFFSET 0x37

#define ULPI_USB_INT_LATCH_NOCLR_2 (*(volatile unsigned char *)0x48062A38)
#define ULPI_USB_INT_LATCH_NOCLR_2_OFFSET 0x38

#define ULPI_VENDOR_INT_EN_2 (*(volatile unsigned char *)0x48062A3B)
#define ULPI_VENDOR_INT_EN_2_OFFSET 0x3B

#define ULPI_VENDOR_INT_EN_SET_2 (*(volatile unsigned char *)0x48062A3C)
#define ULPI_VENDOR_INT_EN_SET_2_OFFSET 0x3C

#define ULPI_VENDOR_INT_EN_CLR_2 (*(volatile unsigned char *)0x48062A3D)
#define ULPI_VENDOR_INT_EN_CLR_2_OFFSET 0x3D

#define ULPI_VENDOR_INT_STATUS_2 (*(volatile unsigned char *)0x48062A3E)
#define ULPI_VENDOR_INT_STATUS_2_OFFSET 0x3E

#define ULPI_VENDOR_INT_LATCH_2 (*(volatile unsigned char *)0x48062A3F)
#define ULPI_VENDOR_INT_LATCH_2_OFFSET 0x3F

#ifndef HS_USB_HOST_BASE
#define HS_USB_HOST_BASE 0x48064000
#endif

#define UHH_REVISION (*(volatile unsigned *)0x48064000)
#define UHH_REVISION_OFFSET 0x0

#define UHH_SYSCONFIG (*(volatile unsigned *)0x48064010)
#define UHH_SYSCONFIG_OFFSET 0x10

#define UHH_SYSSTATUS (*(volatile unsigned *)0x48064014)
#define UHH_SYSSTATUS_OFFSET 0x14

#define UHH_HOSTCONFIG (*(volatile unsigned *)0x48064040)
#define UHH_HOSTCONFIG_OFFSET 0x40

#define UHH_DEBUG_CSR (*(volatile unsigned *)0x48064044)
#define UHH_DEBUG_CSR_OFFSET 0x44

#ifndef HS_USB_OHCI_BASE
#define HS_USB_OHCI_BASE 0x48064400
#endif

#define HCREVISION (*(volatile unsigned *)0x48064400)
#define HCREVISION_OFFSET 0x0

#define HCCONTROL (*(volatile unsigned *)0x48064404)
#define HCCONTROL_OFFSET 0x4

#define HCCOMMANDSTATUS (*(volatile unsigned *)0x48064408)
#define HCCOMMANDSTATUS_OFFSET 0x8

#define HCINTERRUPTSTATUS (*(volatile unsigned *)0x4806440C)
#define HCINTERRUPTSTATUS_OFFSET 0xC

#define HCINTERRUPTENABLE (*(volatile unsigned *)0x48064410)
#define HCINTERRUPTENABLE_OFFSET 0x10

#define HCINTERRUPTDISABLE (*(volatile unsigned *)0x48064414)
#define HCINTERRUPTDISABLE_OFFSET 0x14

#define HCHCCA (*(volatile unsigned *)0x48064418)
#define HCHCCA_OFFSET 0x18

#define HCPERIODCURRENTED (*(volatile unsigned *)0x4806441C)
#define HCPERIODCURRENTED_OFFSET 0x1C

#define HCCONTROLHEADED (*(volatile unsigned *)0x48064420)
#define HCCONTROLHEADED_OFFSET 0x20

#define HCCONTROLCURRENTED (*(volatile unsigned *)0x48064424)
#define HCCONTROLCURRENTED_OFFSET 0x24

#define HCBULKHEADED (*(volatile unsigned *)0x48064428)
#define HCBULKHEADED_OFFSET 0x28

#define HCBULKCURRENTED (*(volatile unsigned *)0x4806442C)
#define HCBULKCURRENTED_OFFSET 0x2C

#define HCDONEHEAD (*(volatile unsigned *)0x48064430)
#define HCDONEHEAD_OFFSET 0x30

#define HCFMINTERVAL (*(volatile unsigned *)0x48064434)
#define HCFMINTERVAL_OFFSET 0x34

#define HCFMREMAINING (*(volatile unsigned *)0x48064438)
#define HCFMREMAINING_OFFSET 0x38

#define HCFMNUMBER (*(volatile unsigned *)0x4806443C)
#define HCFMNUMBER_OFFSET 0x3C

#define HCPERIODICSTART (*(volatile unsigned *)0x48064440)
#define HCPERIODICSTART_OFFSET 0x40

#define HCLSTHRESHOLD (*(volatile unsigned *)0x48064444)
#define HCLSTHRESHOLD_OFFSET 0x44

#define HCRHDESCRIPTORA (*(volatile unsigned *)0x48064448)
#define HCRHDESCRIPTORA_OFFSET 0x48

#define HCRHDESCRIPTORB (*(volatile unsigned *)0x4806444C)
#define HCRHDESCRIPTORB_OFFSET 0x4C

#define HCRHSTATUS (*(volatile unsigned *)0x48064450)
#define HCRHSTATUS_OFFSET 0x50

#define HCRHPORTSTATUS1 (*(volatile unsigned *)0x48064454)
#define HCRHPORTSTATUS1_OFFSET 0x54

#define HCRHPORTSTATUS2 (*(volatile unsigned *)0x48064458)
#define HCRHPORTSTATUS2_OFFSET 0x58

#define HCRHPORTSTATUS3 (*(volatile unsigned *)0x4806445C)
#define HCRHPORTSTATUS3_OFFSET 0x5C

#ifndef HS_USB_EHCI_BASE
#define HS_USB_EHCI_BASE 0x48064800
#endif

#define HCCAPBASE (*(volatile unsigned *)0x48064800)
#define HCCAPBASE_OFFSET 0x0

#define HCSPARAMS (*(volatile unsigned *)0x48064804)
#define HCSPARAMS_OFFSET 0x4

#define HCCPARAMS (*(volatile unsigned *)0x48064808)
#define HCCPARAMS_OFFSET 0x8

#define USBCMD (*(volatile unsigned *)0x48064810)
#define USBCMD_OFFSET 0x10

#define USBSTS (*(volatile unsigned *)0x48064814)
#define USBSTS_OFFSET 0x14

#define USBINTR (*(volatile unsigned *)0x48064818)
#define USBINTR_OFFSET 0x18

#define FRINDEX (*(volatile unsigned *)0x4806481C)
#define FRINDEX_OFFSET 0x1C

#define CTRLDSSEGMENT (*(volatile unsigned *)0x48064820)
#define CTRLDSSEGMENT_OFFSET 0x20

#define PERIODICLISTBASE (*(volatile unsigned *)0x48064824)
#define PERIODICLISTBASE_OFFSET 0x24

#define ASYNCLISTADDR (*(volatile unsigned *)0x48064828)
#define ASYNCLISTADDR_OFFSET 0x28

#define CONFIGFLAG (*(volatile unsigned *)0x48064850)
#define CONFIGFLAG_OFFSET 0x50

#define PORTSC_0 (*(volatile unsigned *)0x48064854)
#define PORTSC_0_OFFSET 0x54

#define PORTSC_1 (*(volatile unsigned *)0x48064858)
#define PORTSC_1_OFFSET 0x58

#define PORTSC_2 (*(volatile unsigned *)0x4806485C)
#define PORTSC_2_OFFSET 0x5C

#define INSNREG00 (*(volatile unsigned *)0x48064890)
#define INSNREG00_OFFSET 0x90

#define INSNREG01 (*(volatile unsigned *)0x48064894)
#define INSNREG01_OFFSET 0x94

#define INSNREG02 (*(volatile unsigned *)0x48064898)
#define INSNREG02_OFFSET 0x98

#define INSNREG03 (*(volatile unsigned *)0x4806489C)
#define INSNREG03_OFFSET 0x9C

#define INSNREG04 (*(volatile unsigned *)0x480648A0)
#define INSNREG04_OFFSET 0xA0

#define INSNREG05_UTMI (*(volatile unsigned *)0x480648A4)
#define INSNREG05_UTMI_OFFSET 0xA4

#define INSNREG05_ULPI (*(volatile unsigned *)0x480648A8)
#define INSNREG05_ULPI_OFFSET 0xA8

#ifndef UART1_BASE
#define UART1_BASE 0x4806A000
#endif

#define UART1_DLL_RHR_THR (*(volatile unsigned short *)0x4806A000)
#define UART1_DLL_RHR_THR_OFFSET 0x0

#define UART1_IER_DLH (*(volatile unsigned short *)0x4806A004)
#define UART1_IER_DLH_OFFSET 0x4

#define UART1_FCR_IIR_EFR (*(volatile unsigned short *)0x4806A008)
#define UART1_FCR_IIR_EFR_OFFSET 0x8

#define UART1_LCR (*(volatile unsigned short *)0x4806A00C)
#define UART1_LCR_OFFSET 0xC

#define UART1_MCR_ADDR1 (*(volatile unsigned short *)0x4806A010)
#define UART1_MCR_ADDR1_OFFSET 0x10

#define UART1_LSR_ADDR2 (*(volatile unsigned short *)0x4806A014)
#define UART1_LSR_ADDR2_OFFSET 0x14

#define UART1_MSR_TCR (*(volatile unsigned short *)0x4806A018)
#define UART1_MSR_TCR_OFFSET 0x18

#define UART1_SPR_TLR (*(volatile unsigned short *)0x4806A01C)
#define UART1_SPR_TLR_OFFSET 0x1C

#define UART1_MDR1 (*(volatile unsigned short *)0x4806A020)
#define UART1_MDR1_OFFSET 0x20

#define UART1_MDR2 (*(volatile unsigned short *)0x4806A024)
#define UART1_MDR2_OFFSET 0x24

#define UART1_BLR_UASR (*(volatile unsigned short *)0x4806A038)
#define UART1_BLR_UASR_OFFSET 0x38

#define UART1_SCR (*(volatile unsigned short *)0x4806A040)
#define UART1_SCR_OFFSET 0x40

#define UART1_SSR (*(volatile unsigned short *)0x4806A044)
#define UART1_SSR_OFFSET 0x44

#define UART1_SYSC (*(volatile unsigned short *)0x4806A054)
#define UART1_SYSC_OFFSET 0x54

#define UART1_SYSS (*(volatile unsigned short *)0x4806A058)
#define UART1_SYSS_OFFSET 0x58

#define UART1_WER (*(volatile unsigned short *)0x4806A05C)
#define UART1_WER_OFFSET 0x5C

#ifndef UART2_BASE
#define UART2_BASE 0x4806C000
#endif

#define UART2_DLL_RHR_THR (*(volatile unsigned short *)0x4806C000)
#define UART2_DLL_RHR_THR_OFFSET 0x0

#define UART2_IER_DLH (*(volatile unsigned short *)0x4806C004)
#define UART2_IER_DLH_OFFSET 0x4

#define UART2_FCR_IIR_EFR (*(volatile unsigned short *)0x4806C008)
#define UART2_FCR_IIR_EFR_OFFSET 0x8

#define UART2_LCR (*(volatile unsigned short *)0x4806C00C)
#define UART2_LCR_OFFSET 0xC

#define UART2_MCR_ADDR1 (*(volatile unsigned short *)0x4806C010)
#define UART2_MCR_ADDR1_OFFSET 0x10

#define UART2_LSR_ADDR2 (*(volatile unsigned short *)0x4806C014)
#define UART2_LSR_ADDR2_OFFSET 0x14

#define UART2_MSR_TCR (*(volatile unsigned short *)0x4806C018)
#define UART2_MSR_TCR_OFFSET 0x18

#define UART2_SPR_TLR (*(volatile unsigned short *)0x4806C01C)
#define UART2_SPR_TLR_OFFSET 0x1C

#define UART2_MDR1 (*(volatile unsigned short *)0x4806C020)
#define UART2_MDR1_OFFSET 0x20

#define UART2_MDR2 (*(volatile unsigned short *)0x4806C024)
#define UART2_MDR2_OFFSET 0x24

#define UART2_BLR_UASR (*(volatile unsigned short *)0x4806C038)
#define UART2_BLR_UASR_OFFSET 0x38

#define UART2_SCR (*(volatile unsigned short *)0x4806C040)
#define UART2_SCR_OFFSET 0x40

#define UART2_SSR (*(volatile unsigned short *)0x4806C044)
#define UART2_SSR_OFFSET 0x44

#define UART2_SYSC (*(volatile unsigned short *)0x4806C054)
#define UART2_SYSC_OFFSET 0x54

#define UART2_SYSS (*(volatile unsigned short *)0x4806C058)
#define UART2_SYSS_OFFSET 0x58

#define UART2_WER (*(volatile unsigned short *)0x4806C05C)
#define UART2_WER_OFFSET 0x5C

#ifndef I2C1_BASE
#define I2C1_BASE 0x48070000
#endif

#define I2C1_REV (*(volatile unsigned *)0x48070000)
#define I2C1_REV_OFFSET 0x0

#define I2C1_IE (*(volatile unsigned short *)0x48070004)
#define I2C1_IE_OFFSET 0x4

#define I2C1_STAT (*(volatile unsigned short *)0x48070008)
#define I2C1_STAT_OFFSET 0x8

#define I2C1_WE (*(volatile unsigned short *)0x4807000C)
#define I2C1_WE_OFFSET 0xC

#define I2C1_SYSS (*(volatile unsigned short *)0x48070010)
#define I2C1_SYSS_OFFSET 0x10

#define I2C1_BUF (*(volatile unsigned short *)0x48070014)
#define I2C1_BUF_OFFSET 0x14

#define I2C1_CNT (*(volatile unsigned short *)0x48070018)
#define I2C1_CNT_OFFSET 0x18

#define I2C1_DATA (*(volatile unsigned short *)0x4807001C)
#define I2C1_DATA_OFFSET 0x1C

#define I2C1_SYSC (*(volatile unsigned short *)0x48070020)
#define I2C1_SYSC_OFFSET 0x20

#define I2C1_CON (*(volatile unsigned short *)0x48070024)
#define I2C1_CON_OFFSET 0x24

#define I2C1_OA0 (*(volatile unsigned short *)0x48070028)
#define I2C1_OA0_OFFSET 0x28

#define I2C1_SA (*(volatile unsigned short *)0x4807002C)
#define I2C1_SA_OFFSET 0x2C

#define I2C1_PSC (*(volatile unsigned short *)0x48070030)
#define I2C1_PSC_OFFSET 0x30

#define I2C1_SCLL (*(volatile unsigned short *)0x48070034)
#define I2C1_SCLL_OFFSET 0x34

#define I2C1_SCLH (*(volatile unsigned short *)0x48070038)
#define I2C1_SCLH_OFFSET 0x38

#define I2C1_SYSTEST (*(volatile unsigned short *)0x4807003C)
#define I2C1_SYSTEST_OFFSET 0x3C

#define I2C1_BUFSTAT (*(volatile unsigned short *)0x48070040)
#define I2C1_BUFSTAT_OFFSET 0x40

#define I2C1_OA1 (*(volatile unsigned short *)0x48070044)
#define I2C1_OA1_OFFSET 0x44

#define I2C1_OA2 (*(volatile unsigned short *)0x48070048)
#define I2C1_OA2_OFFSET 0x48

#define I2C1_OA3 (*(volatile unsigned short *)0x4807004C)
#define I2C1_OA3_OFFSET 0x4C

#define I2C1_ACTOA (*(volatile unsigned short *)0x48070050)
#define I2C1_ACTOA_OFFSET 0x50

#define I2C1_SBLOCK (*(volatile unsigned short *)0x48070054)
#define I2C1_SBLOCK_OFFSET 0x54

#ifndef I2C2_BASE
#define I2C2_BASE 0x48072000
#endif

#define I2C2_REV (*(volatile unsigned *)0x48072000)
#define I2C2_REV_OFFSET 0x0

#define I2C2_IE (*(volatile unsigned short *)0x48072004)
#define I2C2_IE_OFFSET 0x4

#define I2C2_STAT (*(volatile unsigned short *)0x48072008)
#define I2C2_STAT_OFFSET 0x8

#define I2C2_WE (*(volatile unsigned short *)0x4807200C)
#define I2C2_WE_OFFSET 0xC

#define I2C2_SYSS (*(volatile unsigned short *)0x48072010)
#define I2C2_SYSS_OFFSET 0x10

#define I2C2_BUF (*(volatile unsigned short *)0x48072014)
#define I2C2_BUF_OFFSET 0x14

#define I2C2_CNT (*(volatile unsigned short *)0x48072018)
#define I2C2_CNT_OFFSET 0x18

#define I2C2_DATA (*(volatile unsigned short *)0x4807201C)
#define I2C2_DATA_OFFSET 0x1C

#define I2C2_SYSC (*(volatile unsigned short *)0x48072020)
#define I2C2_SYSC_OFFSET 0x20

#define I2C2_CON (*(volatile unsigned short *)0x48072024)
#define I2C2_CON_OFFSET 0x24

#define I2C2_OA0 (*(volatile unsigned short *)0x48072028)
#define I2C2_OA0_OFFSET 0x28

#define I2C2_SA (*(volatile unsigned short *)0x4807202C)
#define I2C2_SA_OFFSET 0x2C

#define I2C2_PSC (*(volatile unsigned short *)0x48072030)
#define I2C2_PSC_OFFSET 0x30

#define I2C2_SCLL (*(volatile unsigned short *)0x48072034)
#define I2C2_SCLL_OFFSET 0x34

#define I2C2_SCLH (*(volatile unsigned short *)0x48072038)
#define I2C2_SCLH_OFFSET 0x38

#define I2C2_SYSTEST (*(volatile unsigned short *)0x4807203C)
#define I2C2_SYSTEST_OFFSET 0x3C

#define I2C2_BUFSTAT (*(volatile unsigned short *)0x48072040)
#define I2C2_BUFSTAT_OFFSET 0x40

#define I2C2_OA1 (*(volatile unsigned short *)0x48072044)
#define I2C2_OA1_OFFSET 0x44

#define I2C2_OA2 (*(volatile unsigned short *)0x48072048)
#define I2C2_OA2_OFFSET 0x48

#define I2C2_OA3 (*(volatile unsigned short *)0x4807204C)
#define I2C2_OA3_OFFSET 0x4C

#define I2C2_ACTOA (*(volatile unsigned short *)0x48072050)
#define I2C2_ACTOA_OFFSET 0x50

#define I2C2_SBLOCK (*(volatile unsigned short *)0x48072054)
#define I2C2_SBLOCK_OFFSET 0x54

#ifndef McBSP1_BASE
#define McBSP1_BASE 0x48074000
#endif

#define McBSP1_MCBSPLP_DRR_REG (*(volatile unsigned long *)0x48074000)
#define McBSP1_MCBSPLP_DRR_REG_OFFSET 0x0

#define McBSP1_MCBSPLP_DXR_REG (*(volatile unsigned long *)0x48074008)
#define McBSP1_MCBSPLP_DXR_REG_OFFSET 0x8

#define McBSP1_MCBSPLP_SPCR2_REG (*(volatile unsigned long *)0x48074010)
#define McBSP1_MCBSPLP_SPCR2_REG_OFFSET 0x10

#define McBSP1_MCBSPLP_SPCR1_REG (*(volatile unsigned long *)0x48074014)
#define McBSP1_MCBSPLP_SPCR1_REG_OFFSET 0x14

#define McBSP1_MCBSPLP_RCR2_REG (*(volatile unsigned long *)0x48074018)
#define McBSP1_MCBSPLP_RCR2_REG_OFFSET 0x18

#define McBSP1_MCBSPLP_RCR1_REG (*(volatile unsigned long *)0x4807401C)
#define McBSP1_MCBSPLP_RCR1_REG_OFFSET 0x1C

#define McBSP1_MCBSPLP_XCR2_REG (*(volatile unsigned long *)0x48074020)
#define McBSP1_MCBSPLP_XCR2_REG_OFFSET 0x20

#define McBSP1_MCBSPLP_XCR1_REG (*(volatile unsigned long *)0x48074024)
#define McBSP1_MCBSPLP_XCR1_REG_OFFSET 0x24

#define McBSP1_MCBSPLP_SRGR2_REG (*(volatile unsigned long *)0x48074028)
#define McBSP1_MCBSPLP_SRGR2_REG_OFFSET 0x28

#define McBSP1_MCBSPLP_SRGR1_REG (*(volatile unsigned long *)0x4807402C)
#define McBSP1_MCBSPLP_SRGR1_REG_OFFSET 0x2C

#define McBSP1_MCBSPLP_MCR2_REG (*(volatile unsigned long *)0x48074030)
#define McBSP1_MCBSPLP_MCR2_REG_OFFSET 0x30

#define McBSP1_MCBSPLP_MCR1_REG (*(volatile unsigned long *)0x48074034)
#define McBSP1_MCBSPLP_MCR1_REG_OFFSET 0x34

#define McBSP1_MCBSPLP_RCERA_REG (*(volatile unsigned long *)0x48074038)
#define McBSP1_MCBSPLP_RCERA_REG_OFFSET 0x38

#define McBSP1_MCBSPLP_RCERREG (*(volatile unsigned long *)0x4807403C)
#define McBSP1_MCBSPLP_RCERREG_OFFSET 0x3C

#define McBSP1_MCBSPLP_XCERA_REG (*(volatile unsigned long *)0x48074040)
#define McBSP1_MCBSPLP_XCERA_REG_OFFSET 0x40

#define McBSP1_MCBSPLP_XCERREG (*(volatile unsigned long *)0x48074044)
#define McBSP1_MCBSPLP_XCERREG_OFFSET 0x44

#define McBSP1_MCBSPLP_PCR_REG (*(volatile unsigned long *)0x48074048)
#define McBSP1_MCBSPLP_PCR_REG_OFFSET 0x48

#define McBSP1_MCBSPLP_RCERC_REG (*(volatile unsigned long *)0x4807404C)
#define McBSP1_MCBSPLP_RCERC_REG_OFFSET 0x4C

#define McBSP1_MCBSPLP_RCERD_REG (*(volatile unsigned long *)0x48074050)
#define McBSP1_MCBSPLP_RCERD_REG_OFFSET 0x50

#define McBSP1_MCBSPLP_XCERC_REG (*(volatile unsigned long *)0x48074054)
#define McBSP1_MCBSPLP_XCERC_REG_OFFSET 0x54

#define McBSP1_MCBSPLP_XCERD_REG (*(volatile unsigned long *)0x48074058)
#define McBSP1_MCBSPLP_XCERD_REG_OFFSET 0x58

#define McBSP1_MCBSPLP_RCERE_REG (*(volatile unsigned long *)0x4807405C)
#define McBSP1_MCBSPLP_RCERE_REG_OFFSET 0x5C

#define McBSP1_MCBSPLP_RCERF_REG (*(volatile unsigned long *)0x48074060)
#define McBSP1_MCBSPLP_RCERF_REG_OFFSET 0x60

#define McBSP1_MCBSPLP_XCERE_REG (*(volatile unsigned long *)0x48074064)
#define McBSP1_MCBSPLP_XCERE_REG_OFFSET 0x64

#define McBSP1_MCBSPLP_XCERF_REG (*(volatile unsigned long *)0x48074068)
#define McBSP1_MCBSPLP_XCERF_REG_OFFSET 0x68

#define McBSP1_MCBSPLP_RCERG_REG (*(volatile unsigned long *)0x4807406C)
#define McBSP1_MCBSPLP_RCERG_REG_OFFSET 0x6C

#define McBSP1_MCBSPLP_RCERH_REG (*(volatile unsigned long *)0x48074070)
#define McBSP1_MCBSPLP_RCERH_REG_OFFSET 0x70

#define McBSP1_MCBSPLP_XCERG_REG (*(volatile unsigned long *)0x48074074)
#define McBSP1_MCBSPLP_XCERG_REG_OFFSET 0x74

#define McBSP1_MCBSPLP_XCERH_REG (*(volatile unsigned long *)0x48074078)
#define McBSP1_MCBSPLP_XCERH_REG_OFFSET 0x78

#define McBSP1_MCBSPLP_RINTCLR_REG (*(volatile unsigned long *)0x48074080)
#define McBSP1_MCBSPLP_RINTCLR_REG_OFFSET 0x80

#define McBSP1_MCBSPLP_XINTCLR_REG (*(volatile unsigned long *)0x48074084)
#define McBSP1_MCBSPLP_XINTCLR_REG_OFFSET 0x84

#define McBSP1_MCBSPLP_ROVFLCLR_REG (*(volatile unsigned long *)0x48074088)
#define McBSP1_MCBSPLP_ROVFLCLR_REG_OFFSET 0x88

#define McBSP1_MCBSPLP_SYSCONFIG_REG (*(volatile unsigned long *)0x4807408C)
#define McBSP1_MCBSPLP_SYSCONFIG_REG_OFFSET 0x8C

#define McBSP1_MCBSPLP_THRSH2_REG (*(volatile unsigned long *)0x48074090)
#define McBSP1_MCBSPLP_THRSH2_REG_OFFSET 0x90

#define McBSP1_MCBSPLP_THRSH1_REG (*(volatile unsigned long *)0x48074094)
#define McBSP1_MCBSPLP_THRSH1_REG_OFFSET 0x94

#define McBSP1_MCBSPLP_IRQSTATUS_REG (*(volatile unsigned long *)0x480740A0)
#define McBSP1_MCBSPLP_IRQSTATUS_REG_OFFSET 0xA0

#define McBSP1_MCBSPLP_IRQENABLE_REG (*(volatile unsigned long *)0x480740A4)
#define McBSP1_MCBSPLP_IRQENABLE_REG_OFFSET 0xA4

#define McBSP1_MCBSPLP_WAKEUPEN_REG (*(volatile unsigned long *)0x480740A8)
#define McBSP1_MCBSPLP_WAKEUPEN_REG_OFFSET 0xA8

#define McBSP1_MCBSPLP_XCCR_REG (*(volatile unsigned long *)0x480740AC)
#define McBSP1_MCBSPLP_XCCR_REG_OFFSET 0xAC

#define McBSP1_MCBSPLP_RCCR_REG (*(volatile unsigned long *)0x480740B0)
#define McBSP1_MCBSPLP_RCCR_REG_OFFSET 0xB0

#define McBSP1_MCBSPLP_XBUFFSTAT_REG (*(volatile unsigned long *)0x480740B4)
#define McBSP1_MCBSPLP_XBUFFSTAT_REG_OFFSET 0xB4

#define McBSP1_MCBSPLP_RBUFFSTAT_REG (*(volatile unsigned long *)0x480740B8)
#define McBSP1_MCBSPLP_RBUFFSTAT_REG_OFFSET 0xB8

#define McBSP1_MCBSPLP_SSELCR_REG (*(volatile unsigned long *)0x480740BC)
#define McBSP1_MCBSPLP_SSELCR_REG_OFFSET 0xBC

#define McBSP1_MCBSPLP_STATUS_REG (*(volatile unsigned long *)0x480740C0)
#define McBSP1_MCBSPLP_STATUS_REG_OFFSET 0xC0

#ifndef GPTIMER10_BASE
#define GPTIMER10_BASE 0x48086000
#endif

#define GPTIMER10_TIDR (*(volatile unsigned *)0x48086000)
#define GPTIMER10_TIDR_OFFSET 0x0

#define GPTIMER10_TIOCP_CFG (*(volatile unsigned long *)0x48086010)
#define GPTIMER10_TIOCP_CFG_OFFSET 0x10

#define GPTIMER10_TISTAT (*(volatile unsigned long *)0x48086014)
#define GPTIMER10_TISTAT_OFFSET 0x14

#define GPTIMER10_TISR (*(volatile unsigned long *)0x48086018)
#define GPTIMER10_TISR_OFFSET 0x18

#define GPTIMER10_TIER (*(volatile unsigned long *)0x4808601C)
#define GPTIMER10_TIER_OFFSET 0x1C

#define GPTIMER10_TWER (*(volatile unsigned long *)0x48086020)
#define GPTIMER10_TWER_OFFSET 0x20

#define GPTIMER10_TCLR (*(volatile unsigned long *)0x48086024)
#define GPTIMER10_TCLR_OFFSET 0x24

#define GPTIMER10_TCRR (*(volatile unsigned long *)0x48086028)
#define GPTIMER10_TCRR_OFFSET 0x28

#define GPTIMER10_TLDR (*(volatile unsigned long *)0x4808602C)
#define GPTIMER10_TLDR_OFFSET 0x2C

#define GPTIMER10_TTGR (*(volatile unsigned long *)0x48086030)
#define GPTIMER10_TTGR_OFFSET 0x30

#define GPTIMER10_TWPS (*(volatile unsigned long *)0x48086034)
#define GPTIMER10_TWPS_OFFSET 0x34

#define GPTIMER10_TMAR (*(volatile unsigned long *)0x48086038)
#define GPTIMER10_TMAR_OFFSET 0x38

#define GPTIMER10_TCAR1 (*(volatile unsigned long *)0x4808603C)
#define GPTIMER10_TCAR1_OFFSET 0x3C

#define GPTIMER10_TSICR (*(volatile unsigned long *)0x48086040)
#define GPTIMER10_TSICR_OFFSET 0x40

#define GPTIMER10_TCAR2 (*(volatile unsigned long *)0x48086044)
#define GPTIMER10_TCAR2_OFFSET 0x44

#define GPTIMER10_TPIR (*(volatile unsigned long *)0x48086048)
#define GPTIMER10_TPIR_OFFSET 0x48

#define GPTIMER10_TNIR (*(volatile unsigned long *)0x4808604C)
#define GPTIMER10_TNIR_OFFSET 0x4C

#define GPTIMER10_TCVR (*(volatile unsigned long *)0x48086050)
#define GPTIMER10_TCVR_OFFSET 0x50

#define GPTIMER10_TOCR (*(volatile unsigned long *)0x48086054)
#define GPTIMER10_TOCR_OFFSET 0x54

#define GPTIMER10_TOWR (*(volatile unsigned long *)0x48086058)
#define GPTIMER10_TOWR_OFFSET 0x58

#ifndef GPTIMER11_BASE
#define GPTIMER11_BASE 0x48088000
#endif

#define GPTIMER11_TIDR (*(volatile unsigned *)0x48088000)
#define GPTIMER11_TIDR_OFFSET 0x0

#define GPTIMER11_TIOCP_CFG (*(volatile unsigned long *)0x48088010)
#define GPTIMER11_TIOCP_CFG_OFFSET 0x10

#define GPTIMER11_TISTAT (*(volatile unsigned long *)0x48088014)
#define GPTIMER11_TISTAT_OFFSET 0x14

#define GPTIMER11_TISR (*(volatile unsigned long *)0x48088018)
#define GPTIMER11_TISR_OFFSET 0x18

#define GPTIMER11_TIER (*(volatile unsigned long *)0x4808801C)
#define GPTIMER11_TIER_OFFSET 0x1C

#define GPTIMER11_TWER (*(volatile unsigned long *)0x48088020)
#define GPTIMER11_TWER_OFFSET 0x20

#define GPTIMER11_TCLR (*(volatile unsigned long *)0x48088024)
#define GPTIMER11_TCLR_OFFSET 0x24

#define GPTIMER11_TCRR (*(volatile unsigned long *)0x48088028)
#define GPTIMER11_TCRR_OFFSET 0x28

#define GPTIMER11_TLDR (*(volatile unsigned long *)0x4808802C)
#define GPTIMER11_TLDR_OFFSET 0x2C

#define GPTIMER11_TTGR (*(volatile unsigned long *)0x48088030)
#define GPTIMER11_TTGR_OFFSET 0x30

#define GPTIMER11_TWPS (*(volatile unsigned long *)0x48088034)
#define GPTIMER11_TWPS_OFFSET 0x34

#define GPTIMER11_TMAR (*(volatile unsigned long *)0x48088038)
#define GPTIMER11_TMAR_OFFSET 0x38

#define GPTIMER11_TCAR1 (*(volatile unsigned long *)0x4808803C)
#define GPTIMER11_TCAR1_OFFSET 0x3C

#define GPTIMER11_TSICR (*(volatile unsigned long *)0x48088040)
#define GPTIMER11_TSICR_OFFSET 0x40

#define GPTIMER11_TCAR2 (*(volatile unsigned long *)0x48088044)
#define GPTIMER11_TCAR2_OFFSET 0x44

#ifndef IPC_BASE
#define IPC_BASE 0x48094010
#endif

#define MAILBOX_SYSCONFIG (*(volatile unsigned long *)0x48094010)
#define MAILBOX_SYSCONFIG_OFFSET 0x0

#define MAILBOX_SYSSTATUS (*(volatile unsigned long *)0x48094014)
#define MAILBOX_SYSSTATUS_OFFSET 0x4

#define MAILBOX_MESSAGE_0 (*(volatile unsigned long *)0x48094040)
#define MAILBOX_MESSAGE_0_OFFSET 0x30

#define MAILBOX_MESSAGE_1 (*(volatile unsigned long *)0x48094044)
#define MAILBOX_MESSAGE_1_OFFSET 0x34

#define MAILBOX_FIFOSTATUS_0 (*(volatile unsigned long *)0x48094080)
#define MAILBOX_FIFOSTATUS_0_OFFSET 0x70

#define MAILBOX_FIFOSTATUS_1 (*(volatile unsigned long *)0x48094084)
#define MAILBOX_FIFOSTATUS_1_OFFSET 0x74

#define MAILBOX_MSGSTATUS_0 (*(volatile unsigned long *)0x480940C0)
#define MAILBOX_MSGSTATUS_0_OFFSET 0xB0

#define MAILBOX_MSGSTATUS_1 (*(volatile unsigned long *)0x480940C4)
#define MAILBOX_MSGSTATUS_1_OFFSET 0xB4

#define MAILBOX_IRQ_STATUS_0 (*(volatile unsigned long *)0x48094100)
#define MAILBOX_IRQ_STATUS_0_OFFSET 0xF0

#define MAILBOX_IRQ_STATUS_1 (*(volatile unsigned long *)0x48094108)
#define MAILBOX_IRQ_STATUS_1_OFFSET 0xF8

#define MAILBOX_IRQ_ENABLE_0 (*(volatile unsigned long *)0x48094104)
#define MAILBOX_IRQ_ENABLE_0_OFFSET 0xF4

#define MAILBOX_IRQ_ENABLE_1 (*(volatile unsigned long *)0x4809410C)
#define MAILBOX_IRQ_ENABLE_1_OFFSET 0xFC

#ifndef McBSP5_BASE
#define McBSP5_BASE 0x48096000
#endif

#define McBSP5_MCBSPLP_DRR_REG (*(volatile unsigned long *)0x48096000)
#define McBSP5_MCBSPLP_DRR_REG_OFFSET 0x0

#define McBSP5_MCBSPLP_DXR_REG (*(volatile unsigned long *)0x48096008)
#define McBSP5_MCBSPLP_DXR_REG_OFFSET 0x8

#define McBSP5_MCBSPLP_SPCR2_REG (*(volatile unsigned long *)0x48096010)
#define McBSP5_MCBSPLP_SPCR2_REG_OFFSET 0x10

#define McBSP5_MCBSPLP_SPCR1_REG (*(volatile unsigned long *)0x48096014)
#define McBSP5_MCBSPLP_SPCR1_REG_OFFSET 0x14

#define McBSP5_MCBSPLP_RCR2_REG (*(volatile unsigned long *)0x48096018)
#define McBSP5_MCBSPLP_RCR2_REG_OFFSET 0x18

#define McBSP5_MCBSPLP_RCR1_REG (*(volatile unsigned long *)0x4809601C)
#define McBSP5_MCBSPLP_RCR1_REG_OFFSET 0x1C

#define McBSP5_MCBSPLP_XCR2_REG (*(volatile unsigned long *)0x48096020)
#define McBSP5_MCBSPLP_XCR2_REG_OFFSET 0x20

#define McBSP5_MCBSPLP_XCR1_REG (*(volatile unsigned long *)0x48096024)
#define McBSP5_MCBSPLP_XCR1_REG_OFFSET 0x24

#define McBSP5_MCBSPLP_SRGR2_REG (*(volatile unsigned long *)0x48096028)
#define McBSP5_MCBSPLP_SRGR2_REG_OFFSET 0x28

#define McBSP5_MCBSPLP_SRGR1_REG (*(volatile unsigned long *)0x4809602C)
#define McBSP5_MCBSPLP_SRGR1_REG_OFFSET 0x2C

#define McBSP5_MCBSPLP_MCR2_REG (*(volatile unsigned long *)0x48096030)
#define McBSP5_MCBSPLP_MCR2_REG_OFFSET 0x30

#define McBSP5_MCBSPLP_MCR1_REG (*(volatile unsigned long *)0x48096034)
#define McBSP5_MCBSPLP_MCR1_REG_OFFSET 0x34

#define McBSP5_MCBSPLP_RCERA_REG (*(volatile unsigned long *)0x48096038)
#define McBSP5_MCBSPLP_RCERA_REG_OFFSET 0x38

#define McBSP5_MCBSPLP_RCERREG (*(volatile unsigned long *)0x4809603C)
#define McBSP5_MCBSPLP_RCERREG_OFFSET 0x3C

#define McBSP5_MCBSPLP_XCERA_REG (*(volatile unsigned long *)0x48096040)
#define McBSP5_MCBSPLP_XCERA_REG_OFFSET 0x40

#define McBSP5_MCBSPLP_XCERREG (*(volatile unsigned long *)0x48096044)
#define McBSP5_MCBSPLP_XCERREG_OFFSET 0x44

#define McBSP5_MCBSPLP_PCR_REG (*(volatile unsigned long *)0x48096048)
#define McBSP5_MCBSPLP_PCR_REG_OFFSET 0x48

#define McBSP5_MCBSPLP_RCERC_REG (*(volatile unsigned long *)0x4809604C)
#define McBSP5_MCBSPLP_RCERC_REG_OFFSET 0x4C

#define McBSP5_MCBSPLP_RCERD_REG (*(volatile unsigned long *)0x48096050)
#define McBSP5_MCBSPLP_RCERD_REG_OFFSET 0x50

#define McBSP5_MCBSPLP_XCERC_REG (*(volatile unsigned long *)0x48096054)
#define McBSP5_MCBSPLP_XCERC_REG_OFFSET 0x54

#define McBSP5_MCBSPLP_XCERD_REG (*(volatile unsigned long *)0x48096058)
#define McBSP5_MCBSPLP_XCERD_REG_OFFSET 0x58

#define McBSP5_MCBSPLP_RCERE_REG (*(volatile unsigned long *)0x4809605C)
#define McBSP5_MCBSPLP_RCERE_REG_OFFSET 0x5C

#define McBSP5_MCBSPLP_RCERF_REG (*(volatile unsigned long *)0x48096060)
#define McBSP5_MCBSPLP_RCERF_REG_OFFSET 0x60

#define McBSP5_MCBSPLP_XCERE_REG (*(volatile unsigned long *)0x48096064)
#define McBSP5_MCBSPLP_XCERE_REG_OFFSET 0x64

#define McBSP5_MCBSPLP_XCERF_REG (*(volatile unsigned long *)0x48096068)
#define McBSP5_MCBSPLP_XCERF_REG_OFFSET 0x68

#define McBSP5_MCBSPLP_RCERG_REG (*(volatile unsigned long *)0x4809606C)
#define McBSP5_MCBSPLP_RCERG_REG_OFFSET 0x6C

#define McBSP5_MCBSPLP_RCERH_REG (*(volatile unsigned long *)0x48096070)
#define McBSP5_MCBSPLP_RCERH_REG_OFFSET 0x70

#define McBSP5_MCBSPLP_XCERG_REG (*(volatile unsigned long *)0x48096074)
#define McBSP5_MCBSPLP_XCERG_REG_OFFSET 0x74

#define McBSP5_MCBSPLP_XCERH_REG (*(volatile unsigned long *)0x48096078)
#define McBSP5_MCBSPLP_XCERH_REG_OFFSET 0x78

#define McBSP5_MCBSPLP_RINTCLR_REG (*(volatile unsigned long *)0x48096080)
#define McBSP5_MCBSPLP_RINTCLR_REG_OFFSET 0x80

#define McBSP5_MCBSPLP_XINTCLR_REG (*(volatile unsigned long *)0x48096084)
#define McBSP5_MCBSPLP_XINTCLR_REG_OFFSET 0x84

#define McBSP5_MCBSPLP_ROVFLCLR_REG (*(volatile unsigned long *)0x48096088)
#define McBSP5_MCBSPLP_ROVFLCLR_REG_OFFSET 0x88

#define McBSP5_MCBSPLP_SYSCONFIG_REG (*(volatile unsigned long *)0x4809608C)
#define McBSP5_MCBSPLP_SYSCONFIG_REG_OFFSET 0x8C

#define McBSP5_MCBSPLP_THRSH2_REG (*(volatile unsigned long *)0x48096090)
#define McBSP5_MCBSPLP_THRSH2_REG_OFFSET 0x90

#define McBSP5_MCBSPLP_THRSH1_REG (*(volatile unsigned long *)0x48096094)
#define McBSP5_MCBSPLP_THRSH1_REG_OFFSET 0x94

#define McBSP5_MCBSPLP_IRQSTATUS_REG (*(volatile unsigned long *)0x480960A0)
#define McBSP5_MCBSPLP_IRQSTATUS_REG_OFFSET 0xA0

#define McBSP5_MCBSPLP_IRQENABLE_REG (*(volatile unsigned long *)0x480960A4)
#define McBSP5_MCBSPLP_IRQENABLE_REG_OFFSET 0xA4

#define McBSP5_MCBSPLP_WAKEUPEN_REG (*(volatile unsigned long *)0x480960A8)
#define McBSP5_MCBSPLP_WAKEUPEN_REG_OFFSET 0xA8

#define McBSP5_MCBSPLP_XCCR_REG (*(volatile unsigned long *)0x480960AC)
#define McBSP5_MCBSPLP_XCCR_REG_OFFSET 0xAC

#define McBSP5_MCBSPLP_RCCR_REG (*(volatile unsigned long *)0x480960B0)
#define McBSP5_MCBSPLP_RCCR_REG_OFFSET 0xB0

#define McBSP5_MCBSPLP_XBUFFSTAT_REG (*(volatile unsigned long *)0x480960B4)
#define McBSP5_MCBSPLP_XBUFFSTAT_REG_OFFSET 0xB4

#define McBSP5_MCBSPLP_RBUFFSTAT_REG (*(volatile unsigned long *)0x480960B8)
#define McBSP5_MCBSPLP_RBUFFSTAT_REG_OFFSET 0xB8

#define McBSP5_MCBSPLP_SSELCR_REG (*(volatile unsigned long *)0x480960BC)
#define McBSP5_MCBSPLP_SSELCR_REG_OFFSET 0xBC

#define McBSP5_MCBSPLP_STATUS_REG (*(volatile unsigned long *)0x480960C0)
#define McBSP5_MCBSPLP_STATUS_REG_OFFSET 0xC0

#ifndef McSPI1_BASE
#define McSPI1_BASE 0x48098000
#endif

#define McSPI1_SYSCONFIG (*(volatile unsigned long *)0x48098010)
#define McSPI1_SYSCONFIG_OFFSET 0x10

#define McSPI1_SYSSTATUS (*(volatile unsigned long *)0x48098014)
#define McSPI1_SYSSTATUS_OFFSET 0x14

#define McSPI1_IRQSTATUS (*(volatile unsigned long *)0x48098018)
#define McSPI1_IRQSTATUS_OFFSET 0x18

#define McSPI1_IRQENABLE (*(volatile unsigned long *)0x4809801C)
#define McSPI1_IRQENABLE_OFFSET 0x1C

#define McSPI1_WAKEUPENABLE (*(volatile unsigned long *)0x48098020)
#define McSPI1_WAKEUPENABLE_OFFSET 0x20

#define McSPI1_SYST (*(volatile unsigned long *)0x48098024)
#define McSPI1_SYST_OFFSET 0x24

#define McSPI1_MODULCTRL (*(volatile unsigned long *)0x48098028)
#define McSPI1_MODULCTRL_OFFSET 0x28

#define McSPI1_CH0CONF (*(volatile unsigned long *)0x4809802C)
#define McSPI1_CH0CONF_OFFSET 0x2C

#define McSPI1_CH1CONF (*(volatile unsigned long *)0x48098040)
#define McSPI1_CH1CONF_OFFSET 0x40

#define McSPI1_CH2CONF (*(volatile unsigned long *)0x48098054)
#define McSPI1_CH2CONF_OFFSET 0x54

#define McSPI1_CH3CONF (*(volatile unsigned long *)0x48098068)
#define McSPI1_CH3CONF_OFFSET 0x68

#define McSPI1_CH0STAT (*(volatile unsigned long *)0x48098030)
#define McSPI1_CH0STAT_OFFSET 0x30

#define McSPI1_CH1STAT (*(volatile unsigned long *)0x48098044)
#define McSPI1_CH1STAT_OFFSET 0x44

#define McSPI1_CH2STAT (*(volatile unsigned long *)0x48098058)
#define McSPI1_CH2STAT_OFFSET 0x58

#define McSPI1_CH3STAT (*(volatile unsigned long *)0x4809806C)
#define McSPI1_CH3STAT_OFFSET 0x6C

#define McSPI1_CH0CTRL (*(volatile unsigned long *)0x48098034)
#define McSPI1_CH0CTRL_OFFSET 0x34

#define McSPI1_CH1CTRL (*(volatile unsigned long *)0x48098048)
#define McSPI1_CH1CTRL_OFFSET 0x48

#define McSPI1_CH2CTRL (*(volatile unsigned long *)0x4809805C)
#define McSPI1_CH2CTRL_OFFSET 0x5C

#define McSPI1_CH3CTRL (*(volatile unsigned long *)0x48098070)
#define McSPI1_CH3CTRL_OFFSET 0x70

#define McSPI1_TX0 (*(volatile unsigned long *)0x48098038)
#define McSPI1_TX0_OFFSET 0x38

#define McSPI1_TX1 (*(volatile unsigned long *)0x4809804C)
#define McSPI1_TX1_OFFSET 0x4C

#define McSPI1_TX2 (*(volatile unsigned long *)0x48098060)
#define McSPI1_TX2_OFFSET 0x60

#define McSPI1_TX3 (*(volatile unsigned long *)0x48098074)
#define McSPI1_TX3_OFFSET 0x74

#define McSPI1_RX0 (*(volatile unsigned long *)0x4809803C)
#define McSPI1_RX0_OFFSET 0x3C

#define McSPI1_RX1 (*(volatile unsigned long *)0x48098050)
#define McSPI1_RX1_OFFSET 0x50

#define McSPI1_RX2 (*(volatile unsigned long *)0x48098064)
#define McSPI1_RX2_OFFSET 0x64

#define McSPI1_RX3 (*(volatile unsigned long *)0x48098078)
#define McSPI1_RX3_OFFSET 0x78

#define McSPI1_XFERLEVEL (*(volatile unsigned long *)0x4809807C)
#define McSPI1_XFERLEVEL_OFFSET 0x7C

#ifndef McSPI2_BASE
#define McSPI2_BASE 0x4809A000
#endif

#define McSPI2_SYSCONFIG (*(volatile unsigned long *)0x4809A010)
#define McSPI2_SYSCONFIG_OFFSET 0x10

#define McSPI2_SYSSTATUS (*(volatile unsigned long *)0x4809A014)
#define McSPI2_SYSSTATUS_OFFSET 0x14

#define McSPI2_IRQSTATUS (*(volatile unsigned long *)0x4809A018)
#define McSPI2_IRQSTATUS_OFFSET 0x18

#define McSPI2_IRQENABLE (*(volatile unsigned long *)0x4809A01C)
#define McSPI2_IRQENABLE_OFFSET 0x1C

#define McSPI2_WAKEUPENABLE (*(volatile unsigned long *)0x4809A020)
#define McSPI2_WAKEUPENABLE_OFFSET 0x20

#define McSPI2_SYST (*(volatile unsigned long *)0x4809A024)
#define McSPI2_SYST_OFFSET 0x24

#define McSPI2_MODULCTRL (*(volatile unsigned long *)0x4809A028)
#define McSPI2_MODULCTRL_OFFSET 0x28

#define McSPI2_CH0CONF (*(volatile unsigned long *)0x4809A02C)
#define McSPI2_CH0CONF_OFFSET 0x2C

#define McSPI2_CH1CONF (*(volatile unsigned long *)0x4809A040)
#define McSPI2_CH1CONF_OFFSET 0x40

#define McSPI2_CH2CONF (*(volatile unsigned long *)0x4809A054)
#define McSPI2_CH2CONF_OFFSET 0x54

#define McSPI2_CH3CONF (*(volatile unsigned long *)0x4809A068)
#define McSPI2_CH3CONF_OFFSET 0x68

#define McSPI2_CH0STAT (*(volatile unsigned long *)0x4809A030)
#define McSPI2_CH0STAT_OFFSET 0x30

#define McSPI2_CH1STAT (*(volatile unsigned long *)0x4809A044)
#define McSPI2_CH1STAT_OFFSET 0x44

#define McSPI2_CH2STAT (*(volatile unsigned long *)0x4809A058)
#define McSPI2_CH2STAT_OFFSET 0x58

#define McSPI2_CH3STAT (*(volatile unsigned long *)0x4809A06C)
#define McSPI2_CH3STAT_OFFSET 0x6C

#define McSPI2_CH0CTRL (*(volatile unsigned long *)0x4809A034)
#define McSPI2_CH0CTRL_OFFSET 0x34

#define McSPI2_CH1CTRL (*(volatile unsigned long *)0x4809A048)
#define McSPI2_CH1CTRL_OFFSET 0x48

#define McSPI2_CH2CTRL (*(volatile unsigned long *)0x4809A05C)
#define McSPI2_CH2CTRL_OFFSET 0x5C

#define McSPI2_CH3CTRL (*(volatile unsigned long *)0x4809A070)
#define McSPI2_CH3CTRL_OFFSET 0x70

#define McSPI2_TX0 (*(volatile unsigned long *)0x4809A038)
#define McSPI2_TX0_OFFSET 0x38

#define McSPI2_TX1 (*(volatile unsigned long *)0x4809A04C)
#define McSPI2_TX1_OFFSET 0x4C

#define McSPI2_TX2 (*(volatile unsigned long *)0x4809A060)
#define McSPI2_TX2_OFFSET 0x60

#define McSPI2_TX3 (*(volatile unsigned long *)0x4809A074)
#define McSPI2_TX3_OFFSET 0x74

#define McSPI2_RX0 (*(volatile unsigned long *)0x4809A03C)
#define McSPI2_RX0_OFFSET 0x3C

#define McSPI2_RX1 (*(volatile unsigned long *)0x4809A050)
#define McSPI2_RX1_OFFSET 0x50

#define McSPI2_RX2 (*(volatile unsigned long *)0x4809A064)
#define McSPI2_RX2_OFFSET 0x64

#define McSPI2_RX3 (*(volatile unsigned long *)0x4809A078)
#define McSPI2_RX3_OFFSET 0x78

#define McSPI2_XFERLEVEL (*(volatile unsigned long *)0x4809A07C)
#define McSPI2_XFERLEVEL_OFFSET 0x7C

#ifndef MMCHS1_BASE
#define MMCHS1_BASE 0x4809C000
#endif

#define MMCHS1_SYSCONFIG (*(volatile unsigned long *)0x4809C010)
#define MMCHS1_SYSCONFIG_OFFSET 0x10

#define MMCHS1_SYSSTATUS (*(volatile unsigned long *)0x4809C014)
#define MMCHS1_SYSSTATUS_OFFSET 0x14

#define MMCHS1_CSRE (*(volatile unsigned long *)0x4809C024)
#define MMCHS1_CSRE_OFFSET 0x24

#define MMCHS1_SYSTEST (*(volatile unsigned long *)0x4809C028)
#define MMCHS1_SYSTEST_OFFSET 0x28

#define MMCHS1_CON (*(volatile unsigned long *)0x4809C02C)
#define MMCHS1_CON_OFFSET 0x2C

#define MMCHS1_PWCNT (*(volatile unsigned long *)0x4809C030)
#define MMCHS1_PWCNT_OFFSET 0x30

#define MMCHS1_BLK (*(volatile unsigned long *)0x4809C104)
#define MMCHS1_BLK_OFFSET 0x104

#define MMCHS1_ARG (*(volatile unsigned long *)0x4809C108)
#define MMCHS1_ARG_OFFSET 0x108

#define MMCHS1_CMD (*(volatile unsigned long *)0x4809C10C)
#define MMCHS1_CMD_OFFSET 0x10C

#define MMCHS1_RSP10 (*(volatile unsigned long *)0x4809C110)
#define MMCHS1_RSP10_OFFSET 0x110

#define MMCHS1_RSP32 (*(volatile unsigned long *)0x4809C114)
#define MMCHS1_RSP32_OFFSET 0x114

#define MMCHS1_RSP54 (*(volatile unsigned long *)0x4809C118)
#define MMCHS1_RSP54_OFFSET 0x118

#define MMCHS1_RSP76 (*(volatile unsigned long *)0x4809C11C)
#define MMCHS1_RSP76_OFFSET 0x11C

#define MMCHS1_DATA (*(volatile unsigned long *)0x4809C120)
#define MMCHS1_DATA_OFFSET 0x120

#define MMCHS1_PSTATE (*(volatile unsigned long *)0x4809C124)
#define MMCHS1_PSTATE_OFFSET 0x124

#define MMCHS1_HCTL (*(volatile unsigned long *)0x4809C128)
#define MMCHS1_HCTL_OFFSET 0x128

#define MMCHS1_SYSCTL (*(volatile unsigned long *)0x4809C12C)
#define MMCHS1_SYSCTL_OFFSET 0x12C

#define MMCHS1_STAT (*(volatile unsigned long *)0x4809C130)
#define MMCHS1_STAT_OFFSET 0x130

#define MMCHS1_IE (*(volatile unsigned long *)0x4809C134)
#define MMCHS1_IE_OFFSET 0x134

#define MMCHS1_ISE (*(volatile unsigned long *)0x4809C138)
#define MMCHS1_ISE_OFFSET 0x138

#define MMCHS1_AC12 (*(volatile unsigned long *)0x4809C13C)
#define MMCHS1_AC12_OFFSET 0x13C

#define MMCHS1_CAPA (*(volatile unsigned long *)0x4809C140)
#define MMCHS1_CAPA_OFFSET 0x140

#define MMCHS1_CUR_CAPA (*(volatile unsigned long *)0x4809C148)
#define MMCHS1_CUR_CAPA_OFFSET 0x148

#define MMCHS1_REV (*(volatile unsigned long *)0x4809C1FC)
#define MMCHS1_REV_OFFSET 0x1FC

#ifndef HS_USB_OTG_BASE
#define HS_USB_OTG_BASE 0x480AB400
#endif

#define OTG_REVISION (*(volatile unsigned long *)0x480AB400)
#define OTG_REVISION_OFFSET 0x0

#define OTG_SYSCONFIG (*(volatile unsigned long *)0x480AB404)
#define OTG_SYSCONFIG_OFFSET 0x4

#define OTG_SYSSTATUS (*(volatile unsigned long *)0x480AB408)
#define OTG_SYSSTATUS_OFFSET 0x8

#define OTG_INTERFSEL (*(volatile unsigned long *)0x480AB40C)
#define OTG_INTERFSEL_OFFSET 0xC

#define OTG_FORCESTDBY (*(volatile unsigned long *)0x480AB414)
#define OTG_FORCESTDBY_OFFSET 0x14

#ifndef MMCHS3_BASE
#define MMCHS3_BASE 0x480AD000
#endif

#define MMCHS3_SYSCONFIG (*(volatile unsigned long *)0x480AD010)
#define MMCHS3_SYSCONFIG_OFFSET 0x10

#define MMCHS3_SYSSTATUS (*(volatile unsigned long *)0x480AD014)
#define MMCHS3_SYSSTATUS_OFFSET 0x14

#define MMCHS3_CSRE (*(volatile unsigned long *)0x480AD024)
#define MMCHS3_CSRE_OFFSET 0x24

#define MMCHS3_SYSTEST (*(volatile unsigned long *)0x480AD028)
#define MMCHS3_SYSTEST_OFFSET 0x28

#define MMCHS3_CON (*(volatile unsigned long *)0x480AD02C)
#define MMCHS3_CON_OFFSET 0x2C

#define MMCHS3_PWCNT (*(volatile unsigned long *)0x480AD030)
#define MMCHS3_PWCNT_OFFSET 0x30

#define MMCHS3_BLK (*(volatile unsigned long *)0x480AD104)
#define MMCHS3_BLK_OFFSET 0x104

#define MMCHS3_ARG (*(volatile unsigned long *)0x480AD108)
#define MMCHS3_ARG_OFFSET 0x108

#define MMCHS3_CMD (*(volatile unsigned long *)0x480AD10C)
#define MMCHS3_CMD_OFFSET 0x10C

#define MMCHS3_RSP10 (*(volatile unsigned long *)0x480AD110)
#define MMCHS3_RSP10_OFFSET 0x110

#define MMCHS3_RSP32 (*(volatile unsigned long *)0x480AD114)
#define MMCHS3_RSP32_OFFSET 0x114

#define MMCHS3_RSP54 (*(volatile unsigned long *)0x480AD118)
#define MMCHS3_RSP54_OFFSET 0x118

#define MMCHS3_RSP76 (*(volatile unsigned long *)0x480AD11C)
#define MMCHS3_RSP76_OFFSET 0x11C

#define MMCHS3_DATA (*(volatile unsigned long *)0x480AD120)
#define MMCHS3_DATA_OFFSET 0x120

#define MMCHS3_PSTATE (*(volatile unsigned long *)0x480AD124)
#define MMCHS3_PSTATE_OFFSET 0x124

#define MMCHS3_HCTL (*(volatile unsigned long *)0x480AD128)
#define MMCHS3_HCTL_OFFSET 0x128

#define MMCHS3_SYSCTL (*(volatile unsigned long *)0x480AD12C)
#define MMCHS3_SYSCTL_OFFSET 0x12C

#define MMCHS3_STAT (*(volatile unsigned long *)0x480AD130)
#define MMCHS3_STAT_OFFSET 0x130

#define MMCHS3_IE (*(volatile unsigned long *)0x480AD134)
#define MMCHS3_IE_OFFSET 0x134

#define MMCHS3_ISE (*(volatile unsigned long *)0x480AD138)
#define MMCHS3_ISE_OFFSET 0x138

#define MMCHS3_AC12 (*(volatile unsigned long *)0x480AD13C)
#define MMCHS3_AC12_OFFSET 0x13C

#define MMCHS3_CAPA (*(volatile unsigned long *)0x480AD140)
#define MMCHS3_CAPA_OFFSET 0x140

#define MMCHS3_CUR_CAPA (*(volatile unsigned long *)0x480AD148)
#define MMCHS3_CUR_CAPA_OFFSET 0x148

#define MMCHS3_REV (*(volatile unsigned long *)0x480AD1FC)
#define MMCHS3_REV_OFFSET 0x1FC

#ifndef HDQ_1_WIRE_BASE
#define HDQ_1_WIRE_BASE 0x480B2004
#endif

#define HDQ_TX_DATA (*(volatile unsigned long *)0x480B2004)
#define HDQ_TX_DATA_OFFSET 0x0

#define HDQ_RX_DATA (*(volatile unsigned long *)0x480B2008)
#define HDQ_RX_DATA_OFFSET 0x4

#define HDQ_CTRL_STATUS (*(volatile unsigned long *)0x480B200C)
#define HDQ_CTRL_STATUS_OFFSET 0x8

#define HDQ_INT_STATUS (*(volatile unsigned long *)0x480B2010)
#define HDQ_INT_STATUS_OFFSET 0xC

#define HDQ_SYSCONFIG (*(volatile unsigned long *)0x480B2014)
#define HDQ_SYSCONFIG_OFFSET 0x10

#define HDQ_SYSSTATUS (*(volatile unsigned long *)0x480B2018)
#define HDQ_SYSSTATUS_OFFSET 0x14

#ifndef MMCHS2_BASE
#define MMCHS2_BASE 0x480B4000
#endif

#define MMCHS2_SYSCONFIG (*(volatile unsigned long *)0x480B4010)
#define MMCHS2_SYSCONFIG_OFFSET 0x10

#define MMCHS2_SYSSTATUS (*(volatile unsigned long *)0x480B4014)
#define MMCHS2_SYSSTATUS_OFFSET 0x14

#define MMCHS2_CSRE (*(volatile unsigned long *)0x480B4024)
#define MMCHS2_CSRE_OFFSET 0x24

#define MMCHS2_SYSTEST (*(volatile unsigned long *)0x480B4028)
#define MMCHS2_SYSTEST_OFFSET 0x28

#define MMCHS2_CON (*(volatile unsigned long *)0x480B402C)
#define MMCHS2_CON_OFFSET 0x2C

#define MMCHS2_PWCNT (*(volatile unsigned long *)0x480B4030)
#define MMCHS2_PWCNT_OFFSET 0x30

#define MMCHS2_BLK (*(volatile unsigned long *)0x480B4104)
#define MMCHS2_BLK_OFFSET 0x104

#define MMCHS2_ARG (*(volatile unsigned long *)0x480B4108)
#define MMCHS2_ARG_OFFSET 0x108

#define MMCHS2_CMD (*(volatile unsigned long *)0x480B410C)
#define MMCHS2_CMD_OFFSET 0x10C

#define MMCHS2_RSP10 (*(volatile unsigned long *)0x480B4110)
#define MMCHS2_RSP10_OFFSET 0x110

#define MMCHS2_RSP32 (*(volatile unsigned long *)0x480B4114)
#define MMCHS2_RSP32_OFFSET 0x114

#define MMCHS2_RSP54 (*(volatile unsigned long *)0x480B4118)
#define MMCHS2_RSP54_OFFSET 0x118

#define MMCHS2_RSP76 (*(volatile unsigned long *)0x480B411C)
#define MMCHS2_RSP76_OFFSET 0x11C

#define MMCHS2_DATA (*(volatile unsigned long *)0x480B4120)
#define MMCHS2_DATA_OFFSET 0x120

#define MMCHS2_PSTATE (*(volatile unsigned long *)0x480B4124)
#define MMCHS2_PSTATE_OFFSET 0x124

#define MMCHS2_HCTL (*(volatile unsigned long *)0x480B4128)
#define MMCHS2_HCTL_OFFSET 0x128

#define MMCHS2_SYSCTL (*(volatile unsigned long *)0x480B412C)
#define MMCHS2_SYSCTL_OFFSET 0x12C

#define MMCHS2_STAT (*(volatile unsigned long *)0x480B4130)
#define MMCHS2_STAT_OFFSET 0x130

#define MMCHS2_IE (*(volatile unsigned long *)0x480B4134)
#define MMCHS2_IE_OFFSET 0x134

#define MMCHS2_ISE (*(volatile unsigned long *)0x480B4138)
#define MMCHS2_ISE_OFFSET 0x138

#define MMCHS2_AC12 (*(volatile unsigned long *)0x480B413C)
#define MMCHS2_AC12_OFFSET 0x13C

#define MMCHS2_CAPA (*(volatile unsigned long *)0x480B4140)
#define MMCHS2_CAPA_OFFSET 0x140

#define MMCHS2_CUR_CAPA (*(volatile unsigned long *)0x480B4148)
#define MMCHS2_CUR_CAPA_OFFSET 0x148

#define MMCHS2_REV (*(volatile unsigned long *)0x480B41FC)
#define MMCHS2_REV_OFFSET 0x1FC

#ifndef McSPI3_BASE
#define McSPI3_BASE 0x480B8000
#endif

#define McSPI3_SYSCONFIG (*(volatile unsigned long *)0x480B8010)
#define McSPI3_SYSCONFIG_OFFSET 0x10

#define McSPI3_SYSSTATUS (*(volatile unsigned long *)0x480B8014)
#define McSPI3_SYSSTATUS_OFFSET 0x14

#define McSPI3_IRQSTATUS (*(volatile unsigned long *)0x480B8018)
#define McSPI3_IRQSTATUS_OFFSET 0x18

#define McSPI3_IRQENABLE (*(volatile unsigned long *)0x480B801C)
#define McSPI3_IRQENABLE_OFFSET 0x1C

#define McSPI3_WAKEUPENABLE (*(volatile unsigned long *)0x480B8020)
#define McSPI3_WAKEUPENABLE_OFFSET 0x20

#define McSPI3_SYST (*(volatile unsigned long *)0x480B8024)
#define McSPI3_SYST_OFFSET 0x24

#define McSPI3_MODULCTRL (*(volatile unsigned long *)0x480B8028)
#define McSPI3_MODULCTRL_OFFSET 0x28

#define McSPI3_CH0CONF (*(volatile unsigned long *)0x480B802C)
#define McSPI3_CH0CONF_OFFSET 0x2C

#define McSPI3_CH1CONF (*(volatile unsigned long *)0x480B8040)
#define McSPI3_CH1CONF_OFFSET 0x40

#define McSPI3_CH2CONF (*(volatile unsigned long *)0x480B8054)
#define McSPI3_CH2CONF_OFFSET 0x54

#define McSPI3_CH3CONF (*(volatile unsigned long *)0x480B8068)
#define McSPI3_CH3CONF_OFFSET 0x68

#define McSPI3_CH0STAT (*(volatile unsigned long *)0x480B8030)
#define McSPI3_CH0STAT_OFFSET 0x30

#define McSPI3_CH1STAT (*(volatile unsigned long *)0x480B8044)
#define McSPI3_CH1STAT_OFFSET 0x44

#define McSPI3_CH2STAT (*(volatile unsigned long *)0x480B8058)
#define McSPI3_CH2STAT_OFFSET 0x58

#define McSPI3_CH3STAT (*(volatile unsigned long *)0x480B806C)
#define McSPI3_CH3STAT_OFFSET 0x6C

#define McSPI3_CH0CTRL (*(volatile unsigned long *)0x480B8034)
#define McSPI3_CH0CTRL_OFFSET 0x34

#define McSPI3_CH1CTRL (*(volatile unsigned long *)0x480B8048)
#define McSPI3_CH1CTRL_OFFSET 0x48

#define McSPI3_CH2CTRL (*(volatile unsigned long *)0x480B805C)
#define McSPI3_CH2CTRL_OFFSET 0x5C

#define McSPI3_CH3CTRL (*(volatile unsigned long *)0x480B8070)
#define McSPI3_CH3CTRL_OFFSET 0x70

#define McSPI3_TX0 (*(volatile unsigned long *)0x480B8038)
#define McSPI3_TX0_OFFSET 0x38

#define McSPI3_TX1 (*(volatile unsigned long *)0x480B804C)
#define McSPI3_TX1_OFFSET 0x4C

#define McSPI3_TX2 (*(volatile unsigned long *)0x480B8060)
#define McSPI3_TX2_OFFSET 0x60

#define McSPI3_TX3 (*(volatile unsigned long *)0x480B8074)
#define McSPI3_TX3_OFFSET 0x74

#define McSPI3_RX0 (*(volatile unsigned long *)0x480B803C)
#define McSPI3_RX0_OFFSET 0x3C

#define McSPI3_RX1 (*(volatile unsigned long *)0x480B8050)
#define McSPI3_RX1_OFFSET 0x50

#define McSPI3_RX2 (*(volatile unsigned long *)0x480B8064)
#define McSPI3_RX2_OFFSET 0x64

#define McSPI3_RX3 (*(volatile unsigned long *)0x480B8078)
#define McSPI3_RX3_OFFSET 0x78

#define McSPI3_XFERLEVEL (*(volatile unsigned long *)0x480B807C)
#define McSPI3_XFERLEVEL_OFFSET 0x7C

#ifndef McSPI4_BASE
#define McSPI4_BASE 0x480BA000
#endif

#define McSPI4_SYSCONFIG (*(volatile unsigned long *)0x480BA010)
#define McSPI4_SYSCONFIG_OFFSET 0x10

#define McSPI4_SYSSTATUS (*(volatile unsigned long *)0x480BA014)
#define McSPI4_SYSSTATUS_OFFSET 0x14

#define McSPI4_IRQSTATUS (*(volatile unsigned long *)0x480BA018)
#define McSPI4_IRQSTATUS_OFFSET 0x18

#define McSPI4_IRQENABLE (*(volatile unsigned long *)0x480BA01C)
#define McSPI4_IRQENABLE_OFFSET 0x1C

#define McSPI4_WAKEUPENABLE (*(volatile unsigned long *)0x480BA020)
#define McSPI4_WAKEUPENABLE_OFFSET 0x20

#define McSPI4_SYST (*(volatile unsigned long *)0x480BA024)
#define McSPI4_SYST_OFFSET 0x24

#define McSPI4_MODULCTRL (*(volatile unsigned long *)0x480BA028)
#define McSPI4_MODULCTRL_OFFSET 0x28

#define McSPI4_CH0CONF (*(volatile unsigned long *)0x480BA02C)
#define McSPI4_CH0CONF_OFFSET 0x2C

#define McSPI4_CH1CONF (*(volatile unsigned long *)0x480BA040)
#define McSPI4_CH1CONF_OFFSET 0x40

#define McSPI4_CH2CONF (*(volatile unsigned long *)0x480BA054)
#define McSPI4_CH2CONF_OFFSET 0x54

#define McSPI4_CH3CONF (*(volatile unsigned long *)0x480BA068)
#define McSPI4_CH3CONF_OFFSET 0x68

#define McSPI4_CH0STAT (*(volatile unsigned long *)0x480BA030)
#define McSPI4_CH0STAT_OFFSET 0x30

#define McSPI4_CH1STAT (*(volatile unsigned long *)0x480BA044)
#define McSPI4_CH1STAT_OFFSET 0x44

#define McSPI4_CH2STAT (*(volatile unsigned long *)0x480BA058)
#define McSPI4_CH2STAT_OFFSET 0x58

#define McSPI4_CH3STAT (*(volatile unsigned long *)0x480BA06C)
#define McSPI4_CH3STAT_OFFSET 0x6C

#define McSPI4_CH0CTRL (*(volatile unsigned long *)0x480BA034)
#define McSPI4_CH0CTRL_OFFSET 0x34

#define McSPI4_CH1CTRL (*(volatile unsigned long *)0x480BA048)
#define McSPI4_CH1CTRL_OFFSET 0x48

#define McSPI4_CH2CTRL (*(volatile unsigned long *)0x480BA05C)
#define McSPI4_CH2CTRL_OFFSET 0x5C

#define McSPI4_CH3CTRL (*(volatile unsigned long *)0x480BA070)
#define McSPI4_CH3CTRL_OFFSET 0x70

#define McSPI4_TX0 (*(volatile unsigned long *)0x480BA038)
#define McSPI4_TX0_OFFSET 0x38

#define McSPI4_TX1 (*(volatile unsigned long *)0x480BA04C)
#define McSPI4_TX1_OFFSET 0x4C

#define McSPI4_TX2 (*(volatile unsigned long *)0x480BA060)
#define McSPI4_TX2_OFFSET 0x60

#define McSPI4_TX3 (*(volatile unsigned long *)0x480BA074)
#define McSPI4_TX3_OFFSET 0x74

#define McSPI4_RX0 (*(volatile unsigned long *)0x480BA03C)
#define McSPI4_RX0_OFFSET 0x3C

#define McSPI4_RX1 (*(volatile unsigned long *)0x480BA050)
#define McSPI4_RX1_OFFSET 0x50

#define McSPI4_RX2 (*(volatile unsigned long *)0x480BA064)
#define McSPI4_RX2_OFFSET 0x64

#define McSPI4_RX3 (*(volatile unsigned long *)0x480BA078)
#define McSPI4_RX3_OFFSET 0x78

#define McSPI4_XFERLEVEL (*(volatile unsigned long *)0x480BA07C)
#define McSPI4_XFERLEVEL_OFFSET 0x7C

#ifndef ISP_BASE
#define ISP_BASE 0x480BC004
#endif

#define ISP_SYSCONFIG (*(volatile unsigned long *)0x480BC004)
#define ISP_SYSCONFIG_OFFSET 0x0

#define ISP_SYSSTATUS (*(volatile unsigned long *)0x480BC008)
#define ISP_SYSSTATUS_OFFSET 0x4

#define ISP_IRQ0ENABLE (*(volatile unsigned long *)0x480BC00C)
#define ISP_IRQ0ENABLE_OFFSET 0x8

#define ISP_IRQ0STATUS (*(volatile unsigned long *)0x480BC010)
#define ISP_IRQ0STATUS_OFFSET 0xC

#define ISP_IRQ1ENABLE (*(volatile unsigned long *)0x480BC014)
#define ISP_IRQ1ENABLE_OFFSET 0x10

#define ISP_IRQ1STATUS (*(volatile unsigned long *)0x480BC018)
#define ISP_IRQ1STATUS_OFFSET 0x14

#define TCTRL_GRESET_LENGTH (*(volatile unsigned long *)0x480BC030)
#define TCTRL_GRESET_LENGTH_OFFSET 0x2C

#define TCTRL_PSTRB_REPLAY (*(volatile unsigned long *)0x480BC034)
#define TCTRL_PSTRB_REPLAY_OFFSET 0x30

#define ISP_CTRL (*(volatile unsigned long *)0x480BC040)
#define ISP_CTRL_OFFSET 0x3C

#define ISP_SECURE (*(volatile unsigned long *)0x480BC044)
#define ISP_SECURE_OFFSET 0x40

#define TCTRL_CTRL (*(volatile unsigned long *)0x480BC050)
#define TCTRL_CTRL_OFFSET 0x4C

#define TCTRL_FRAME (*(volatile unsigned long *)0x480BC054)
#define TCTRL_FRAME_OFFSET 0x50

#define TCTRL_PSTRB_DELAY (*(volatile unsigned long *)0x480BC058)
#define TCTRL_PSTRB_DELAY_OFFSET 0x54

#define TCTRL_STRB_DELAY (*(volatile unsigned long *)0x480BC05C)
#define TCTRL_STRB_DELAY_OFFSET 0x58

#define TCTRL_SHUT_DELAY (*(volatile unsigned long *)0x480BC060)
#define TCTRL_SHUT_DELAY_OFFSET 0x5C

#define TCTRL_PSTRB_LENGTH (*(volatile unsigned long *)0x480BC064)
#define TCTRL_PSTRB_LENGTH_OFFSET 0x60

#define TCTRL_STRB_LENGTH (*(volatile unsigned long *)0x480BC068)
#define TCTRL_STRB_LENGTH_OFFSET 0x64

#define TCTRL_SHUT_LENGTH (*(volatile unsigned long *)0x480BC06C)
#define TCTRL_SHUT_LENGTH_OFFSET 0x68

#ifndef ISP_CBUFF_BASE
#define ISP_CBUFF_BASE 0x480BC110
#endif

#define CBUFF_SYSCONFIG (*(volatile unsigned long *)0x480BC110)
#define CBUFF_SYSCONFIG_OFFSET 0x0

#define CBUFF_SYSSTATUS (*(volatile unsigned long *)0x480BC114)
#define CBUFF_SYSSTATUS_OFFSET 0x4

#define CBUFF_IRQSTATUS (*(volatile unsigned long *)0x480BC118)
#define CBUFF_IRQSTATUS_OFFSET 0x8

#define CBUFF_IRQENABLE (*(volatile unsigned long *)0x480BC11C)
#define CBUFF_IRQENABLE_OFFSET 0xC

#define CBUFF0_CTRL (*(volatile unsigned long *)0x480BC120)
#define CBUFF0_CTRL_OFFSET 0x10

#define CBUFF1_CTRL (*(volatile unsigned long *)0x480BC124)
#define CBUFF1_CTRL_OFFSET 0x14

#define CBUFF0_STATUS (*(volatile unsigned long *)0x480BC130)
#define CBUFF0_STATUS_OFFSET 0x20

#define CBUFF1_STATUS (*(volatile unsigned long *)0x480BC134)
#define CBUFF1_STATUS_OFFSET 0x24

#define CBUFF0_START (*(volatile unsigned long *)0x480BC140)
#define CBUFF0_START_OFFSET 0x30

#define CBUFF1_START (*(volatile unsigned long *)0x480BC144)
#define CBUFF1_START_OFFSET 0x34

#define CBUFF0_END (*(volatile unsigned long *)0x480BC150)
#define CBUFF0_END_OFFSET 0x40

#define CBUFF1_END (*(volatile unsigned long *)0x480BC154)
#define CBUFF1_END_OFFSET 0x44

#define CBUFF0_WINDOWSIZE (*(volatile unsigned long *)0x480BC160)
#define CBUFF0_WINDOWSIZE_OFFSET 0x50

#define CBUFF1_WINDOWSIZE (*(volatile unsigned long *)0x480BC164)
#define CBUFF1_WINDOWSIZE_OFFSET 0x54

#define CBUFF0_THRESHOLD (*(volatile unsigned long *)0x480BC170)
#define CBUFF0_THRESHOLD_OFFSET 0x60

#define CBUFF1_THRESHOLD (*(volatile unsigned long *)0x480BC174)
#define CBUFF1_THRESHOLD_OFFSET 0x64

#ifndef ISP_CCDC_BASE
#define ISP_CCDC_BASE 0x480BC600
#endif

#define CCDC_PID (*(volatile unsigned long *)0x480BC600)
#define CCDC_PID_OFFSET 0x0

#define CCDC_PCR (*(volatile unsigned long *)0x480BC604)
#define CCDC_PCR_OFFSET 0x4

#define CCDC_SYN_MODE (*(volatile unsigned long *)0x480BC608)
#define CCDC_SYN_MODE_OFFSET 0x8

#define CCDC_HD_VD_WID (*(volatile unsigned long *)0x480BC60C)
#define CCDC_HD_VD_WID_OFFSET 0xC

#define CCDC_PIX_LINES (*(volatile unsigned long *)0x480BC610)
#define CCDC_PIX_LINES_OFFSET 0x10

#define CCDC_HORZ_INFO (*(volatile unsigned long *)0x480BC614)
#define CCDC_HORZ_INFO_OFFSET 0x14

#define CCDC_VERT_START (*(volatile unsigned long *)0x480BC618)
#define CCDC_VERT_START_OFFSET 0x18

#define CCDC_VERT_LINES (*(volatile unsigned long *)0x480BC61C)
#define CCDC_VERT_LINES_OFFSET 0x1C

#define CCDC_CULLING (*(volatile unsigned long *)0x480BC620)
#define CCDC_CULLING_OFFSET 0x20

#define CCDC_HSIZE_OFF (*(volatile unsigned long *)0x480BC624)
#define CCDC_HSIZE_OFF_OFFSET 0x24

#define CCDC_SDOFST (*(volatile unsigned long *)0x480BC628)
#define CCDC_SDOFST_OFFSET 0x28

#define CCDC_SDR_ADDR (*(volatile unsigned long *)0x480BC62C)
#define CCDC_SDR_ADDR_OFFSET 0x2C

#define CCDC_CLAMP (*(volatile unsigned long *)0x480BC630)
#define CCDC_CLAMP_OFFSET 0x30

#define CCDC_DCSUB (*(volatile unsigned long *)0x480BC634)
#define CCDC_DCSUB_OFFSET 0x34

#define CCDC_COLPTN (*(volatile unsigned long *)0x480BC638)
#define CCDC_COLPTN_OFFSET 0x38

#define CCDC_BLKCMP (*(volatile unsigned long *)0x480BC63C)
#define CCDC_BLKCMP_OFFSET 0x3C

#define CCDC_FPC (*(volatile unsigned long *)0x480BC640)
#define CCDC_FPC_OFFSET 0x40

#define CCDC_FPC_ADDR (*(volatile unsigned long *)0x480BC644)
#define CCDC_FPC_ADDR_OFFSET 0x44

#define CCDC_VDINT (*(volatile unsigned long *)0x480BC648)
#define CCDC_VDINT_OFFSET 0x48

#define CCDC_ALAW (*(volatile unsigned long *)0x480BC64C)
#define CCDC_ALAW_OFFSET 0x4C

#define CCDC_REC656IF (*(volatile unsigned long *)0x480BC650)
#define CCDC_REC656IF_OFFSET 0x50

#define CCDC_CFG (*(volatile unsigned long *)0x480BC654)
#define CCDC_CFG_OFFSET 0x54

#define CCDC_FMTCFG (*(volatile unsigned long *)0x480BC658)
#define CCDC_FMTCFG_OFFSET 0x58

#define CCDC_FMT_HORZ (*(volatile unsigned long *)0x480BC65C)
#define CCDC_FMT_HORZ_OFFSET 0x5C

#define CCDC_FMT_VERT (*(volatile unsigned long *)0x480BC660)
#define CCDC_FMT_VERT_OFFSET 0x60

#define CCDC_FMT_ADDR0 (*(volatile unsigned long *)0x480BC664)
#define CCDC_FMT_ADDR0_OFFSET 0x64

#define CCDC_FMT_ADDR1 (*(volatile unsigned long *)0x480BC668)
#define CCDC_FMT_ADDR1_OFFSET 0x68

#define CCDC_FMT_ADDR2 (*(volatile unsigned long *)0x480BC66C)
#define CCDC_FMT_ADDR2_OFFSET 0x6C

#define CCDC_FMT_ADDR3 (*(volatile unsigned long *)0x480BC670)
#define CCDC_FMT_ADDR3_OFFSET 0x70

#define CCDC_FMT_ADDR4 (*(volatile unsigned long *)0x480BC674)
#define CCDC_FMT_ADDR4_OFFSET 0x74

#define CCDC_FMT_ADDR5 (*(volatile unsigned long *)0x480BC678)
#define CCDC_FMT_ADDR5_OFFSET 0x78

#define CCDC_FMT_ADDR6 (*(volatile unsigned long *)0x480BC67C)
#define CCDC_FMT_ADDR6_OFFSET 0x7C

#define CCDC_FMT_ADDR7 (*(volatile unsigned long *)0x480BC680)
#define CCDC_FMT_ADDR7_OFFSET 0x80

#define CCDC_PRGEVEN0 (*(volatile unsigned long *)0x480BC684)
#define CCDC_PRGEVEN0_OFFSET 0x84

#define CCDC_PRGEVEN1 (*(volatile unsigned long *)0x480BC688)
#define CCDC_PRGEVEN1_OFFSET 0x88

#define CCDC_PRGODD0 (*(volatile unsigned long *)0x480BC68C)
#define CCDC_PRGODD0_OFFSET 0x8C

#define CCDC_PRGODD1 (*(volatile unsigned long *)0x480BC690)
#define CCDC_PRGODD1_OFFSET 0x90

#define CCDC_VP_OUT (*(volatile unsigned long *)0x480BC694)
#define CCDC_VP_OUT_OFFSET 0x94

#define CCDC_LSC_CONFIG (*(volatile unsigned long *)0x480BC698)
#define CCDC_LSC_CONFIG_OFFSET 0x98

#define CCDC_LSC_INITIAL (*(volatile unsigned long *)0x480BC69C)
#define CCDC_LSC_INITIAL_OFFSET 0x9C

#define CCDC_LSC_TABLE_BASE (*(volatile unsigned long *)0x480BC6A0)
#define CCDC_LSC_TABLE_BASE_OFFSET 0xA0

#define CCDC_LSC_TABLE_OFFSET (*(volatile unsigned long *)0x480BC6A4)
#define CCDC_LSC_TABLE_OFFSET_OFFSET 0xA4

#ifndef ISP_HIST_BASE
#define ISP_HIST_BASE 0x480BCA00
#endif

#define HIST_PID (*(volatile unsigned long *)0x480BCA00)
#define HIST_PID_OFFSET 0x0

#define HIST_PCR (*(volatile unsigned long *)0x480BCA04)
#define HIST_PCR_OFFSET 0x4

#define HIST_CNT (*(volatile unsigned long *)0x480BCA08)
#define HIST_CNT_OFFSET 0x8

#define HIST_WB_GAIN (*(volatile unsigned long *)0x480BCA0C)
#define HIST_WB_GAIN_OFFSET 0xC

#define HIST_R0_HORZ (*(volatile unsigned long *)0x480BCA10)
#define HIST_R0_HORZ_OFFSET 0x10

#define HIST_R1_HORZ (*(volatile unsigned long *)0x480BCA18)
#define HIST_R1_HORZ_OFFSET 0x18

#define HIST_R2_HORZ (*(volatile unsigned long *)0x480BCA20)
#define HIST_R2_HORZ_OFFSET 0x20

#define HIST_R3_HORZ (*(volatile unsigned long *)0x480BCA28)
#define HIST_R3_HORZ_OFFSET 0x28

#define HIST_R0_VERT (*(volatile unsigned long *)0x480BCA14)
#define HIST_R0_VERT_OFFSET 0x14

#define HIST_R1_VERT (*(volatile unsigned long *)0x480BCA1C)
#define HIST_R1_VERT_OFFSET 0x1C

#define HIST_R2_VERT (*(volatile unsigned long *)0x480BCA24)
#define HIST_R2_VERT_OFFSET 0x24

#define HIST_R3_VERT (*(volatile unsigned long *)0x480BCA2C)
#define HIST_R3_VERT_OFFSET 0x2C

#define HIST_ADDR (*(volatile unsigned long *)0x480BCA30)
#define HIST_ADDR_OFFSET 0x30

#define HIST_DATA (*(volatile unsigned long *)0x480BCA34)
#define HIST_DATA_OFFSET 0x34

#define HIST_RADD (*(volatile unsigned long *)0x480BCA38)
#define HIST_RADD_OFFSET 0x38

#define HIST_RADD_OFF (*(volatile unsigned long *)0x480BCA3C)
#define HIST_RADD_OFF_OFFSET 0x3C

#define HIST_H_V_INFO (*(volatile unsigned long *)0x480BCA40)
#define HIST_H_V_INFO_OFFSET 0x40

#ifndef ISP_H3A_BASE
#define ISP_H3A_BASE 0x480BCC00
#endif

#define H3A_PID (*(volatile unsigned long *)0x480BCC00)
#define H3A_PID_OFFSET 0x0

#define H3A_PCR (*(volatile unsigned long *)0x480BCC04)
#define H3A_PCR_OFFSET 0x4

#define H3A_AFPAX1 (*(volatile unsigned long *)0x480BCC08)
#define H3A_AFPAX1_OFFSET 0x8

#define H3A_AFPAX2 (*(volatile unsigned long *)0x480BCC0C)
#define H3A_AFPAX2_OFFSET 0xC

#define H3A_AFPAXSTART (*(volatile unsigned long *)0x480BCC10)
#define H3A_AFPAXSTART_OFFSET 0x10

#define H3A_AFIIRSH (*(volatile unsigned long *)0x480BCC14)
#define H3A_AFIIRSH_OFFSET 0x14

#define H3A_AFBUFST (*(volatile unsigned long *)0x480BCC18)
#define H3A_AFBUFST_OFFSET 0x18

#define H3A_AFCOEF010 (*(volatile unsigned long *)0x480BCC1C)
#define H3A_AFCOEF010_OFFSET 0x1C

#define H3A_AFCOEF032 (*(volatile unsigned long *)0x480BCC20)
#define H3A_AFCOEF032_OFFSET 0x20

#define H3A_AFCOEF054 (*(volatile unsigned long *)0x480BCC24)
#define H3A_AFCOEF054_OFFSET 0x24

#define H3A_AFCOEF076 (*(volatile unsigned long *)0x480BCC28)
#define H3A_AFCOEF076_OFFSET 0x28

#define H3A_AFCOEF098 (*(volatile unsigned long *)0x480BCC2C)
#define H3A_AFCOEF098_OFFSET 0x2C

#define H3A_AFCOEF0010 (*(volatile unsigned long *)0x480BCC30)
#define H3A_AFCOEF0010_OFFSET 0x30

#define H3A_AFCOEF110 (*(volatile unsigned long *)0x480BCC34)
#define H3A_AFCOEF110_OFFSET 0x34

#define H3A_AFCOEF132 (*(volatile unsigned long *)0x480BCC38)
#define H3A_AFCOEF132_OFFSET 0x38

#define H3A_AFCOEF154 (*(volatile unsigned long *)0x480BCC3C)
#define H3A_AFCOEF154_OFFSET 0x3C

#define H3A_AFCOEF176 (*(volatile unsigned long *)0x480BCC40)
#define H3A_AFCOEF176_OFFSET 0x40

#define H3A_AFCOEF198 (*(volatile unsigned long *)0x480BCC44)
#define H3A_AFCOEF198_OFFSET 0x44

#define H3A_AFCOEF1010 (*(volatile unsigned long *)0x480BCC48)
#define H3A_AFCOEF1010_OFFSET 0x48

#define H3A_AEWWIN1 (*(volatile unsigned long *)0x480BCC4C)
#define H3A_AEWWIN1_OFFSET 0x4C

#define H3A_AEWINSTART (*(volatile unsigned long *)0x480BCC50)
#define H3A_AEWINSTART_OFFSET 0x50

#define H3A_AEWINBLK (*(volatile unsigned long *)0x480BCC54)
#define H3A_AEWINBLK_OFFSET 0x54

#define H3A_AEWSUBWIN (*(volatile unsigned long *)0x480BCC58)
#define H3A_AEWSUBWIN_OFFSET 0x58

#define H3A_AEWBUFST (*(volatile unsigned long *)0x480BCC5C)
#define H3A_AEWBUFST_OFFSET 0x5C

#ifndef ISP_PREVIEW_BASE
#define ISP_PREVIEW_BASE 0x480BCE00
#endif

#define PRV_PID (*(volatile unsigned long *)0x480BCE00)
#define PRV_PID_OFFSET 0x0

#define PRV_PCR (*(volatile unsigned long *)0x480BCE04)
#define PRV_PCR_OFFSET 0x4

#define PRV_HORZ_INFO (*(volatile unsigned long *)0x480BCE08)
#define PRV_HORZ_INFO_OFFSET 0x8

#define PRV_VERT_INFO (*(volatile unsigned long *)0x480BCE0C)
#define PRV_VERT_INFO_OFFSET 0xC

#define PRV_RSDR_ADDR (*(volatile unsigned long *)0x480BCE10)
#define PRV_RSDR_ADDR_OFFSET 0x10

#define PRV_RADR_OFFSET (*(volatile unsigned long *)0x480BCE14)
#define PRV_RADR_OFFSET_OFFSET 0x14

#define PRV_DSDR_ADDR (*(volatile unsigned long *)0x480BCE18)
#define PRV_DSDR_ADDR_OFFSET 0x18

#define PRV_DRKF_OFFSET (*(volatile unsigned long *)0x480BCE1C)
#define PRV_DRKF_OFFSET_OFFSET 0x1C

#define PRV_WSDR_ADDR (*(volatile unsigned long *)0x480BCE20)
#define PRV_WSDR_ADDR_OFFSET 0x20

#define PRV_WADD_OFFSET (*(volatile unsigned long *)0x480BCE24)
#define PRV_WADD_OFFSET_OFFSET 0x24

#define PRV_AVE (*(volatile unsigned long *)0x480BCE28)
#define PRV_AVE_OFFSET 0x28

#define PRV_HMED (*(volatile unsigned long *)0x480BCE2C)
#define PRV_HMED_OFFSET 0x2C

#define PRV_NF (*(volatile unsigned long *)0x480BCE30)
#define PRV_NF_OFFSET 0x30

#define PRV_WB_DGAIN (*(volatile unsigned long *)0x480BCE34)
#define PRV_WB_DGAIN_OFFSET 0x34

#define PRV_WBGAIN (*(volatile unsigned long *)0x480BCE38)
#define PRV_WBGAIN_OFFSET 0x38

#define PRV_WBSEL (*(volatile unsigned long *)0x480BCE3C)
#define PRV_WBSEL_OFFSET 0x3C

#define PRV_CFA (*(volatile unsigned long *)0x480BCE40)
#define PRV_CFA_OFFSET 0x40

#define PRV_BLKADJOFF (*(volatile unsigned long *)0x480BCE44)
#define PRV_BLKADJOFF_OFFSET 0x44

#define PRV_RGB_MAT1 (*(volatile unsigned long *)0x480BCE48)
#define PRV_RGB_MAT1_OFFSET 0x48

#define PRV_RGB_MAT2 (*(volatile unsigned long *)0x480BCE4C)
#define PRV_RGB_MAT2_OFFSET 0x4C

#define PRV_RGB_MAT3 (*(volatile unsigned long *)0x480BCE50)
#define PRV_RGB_MAT3_OFFSET 0x50

#define PRV_RGB_MAT4 (*(volatile unsigned long *)0x480BCE54)
#define PRV_RGB_MAT4_OFFSET 0x54

#define PRV_RGB_MAT5 (*(volatile unsigned long *)0x480BCE58)
#define PRV_RGB_MAT5_OFFSET 0x58

#define PRV_RGB_OFF1 (*(volatile unsigned long *)0x480BCE5C)
#define PRV_RGB_OFF1_OFFSET 0x5C

#define PRV_RGB_OFF2 (*(volatile unsigned long *)0x480BCE60)
#define PRV_RGB_OFF2_OFFSET 0x60

#define PRV_CSC0 (*(volatile unsigned long *)0x480BCE64)
#define PRV_CSC0_OFFSET 0x64

#define PRV_CSC1 (*(volatile unsigned long *)0x480BCE68)
#define PRV_CSC1_OFFSET 0x68

#define PRV_CSC2 (*(volatile unsigned long *)0x480BCE6C)
#define PRV_CSC2_OFFSET 0x6C

#define PRV_CSC_OFFSET (*(volatile unsigned long *)0x480BCE70)
#define PRV_CSC_OFFSET_OFFSET 0x70

#define PRV_CNT_BRT (*(volatile unsigned long *)0x480BCE74)
#define PRV_CNT_BRT_OFFSET 0x74

#define PRV_CSUP (*(volatile unsigned long *)0x480BCE78)
#define PRV_CSUP_OFFSET 0x78

#define PRV_SETUP_YC (*(volatile unsigned long *)0x480BCE7C)
#define PRV_SETUP_YC_OFFSET 0x7C

#define PRV_SET_TBL_ADDR (*(volatile unsigned long *)0x480BCE80)
#define PRV_SET_TBL_ADDR_OFFSET 0x80

#define PRV_SET_TBL_DATA (*(volatile unsigned long *)0x480BCE84)
#define PRV_SET_TBL_DATA_OFFSET 0x84

#define PRV_CDC_THR0 (*(volatile unsigned long *)0x480BCE90)
#define PRV_CDC_THR0_OFFSET 0x90

#define PRV_CDC_THR1 (*(volatile unsigned long *)0x480BCE94)
#define PRV_CDC_THR1_OFFSET 0x94

#define PRV_CDC_THR2 (*(volatile unsigned long *)0x480BCE98)
#define PRV_CDC_THR2_OFFSET 0x98

#define PRV_CDC_THR3 (*(volatile unsigned long *)0x480BCE9C)
#define PRV_CDC_THR3_OFFSET 0x9C

#define PRV_CDC_THR4 (*(volatile unsigned long *)0x480BCEA0)
#define PRV_CDC_THR4_OFFSET 0xA0

#ifndef ISP_RESIZER_BASE
#define ISP_RESIZER_BASE 0x480BD000
#endif

#define RSZ_PID (*(volatile unsigned long *)0x480BD000)
#define RSZ_PID_OFFSET 0x0

#define RSZ_PCR (*(volatile unsigned long *)0x480BD004)
#define RSZ_PCR_OFFSET 0x4

#define RSZ_CNT (*(volatile unsigned long *)0x480BD008)
#define RSZ_CNT_OFFSET 0x8

#define RSZ_OUT_SIZE (*(volatile unsigned long *)0x480BD00C)
#define RSZ_OUT_SIZE_OFFSET 0xC

#define RSZ_IN_START (*(volatile unsigned long *)0x480BD010)
#define RSZ_IN_START_OFFSET 0x10

#define RSZ_IN_SIZE (*(volatile unsigned long *)0x480BD014)
#define RSZ_IN_SIZE_OFFSET 0x14

#define RSZ_SDR_INADD (*(volatile unsigned long *)0x480BD018)
#define RSZ_SDR_INADD_OFFSET 0x18

#define RSZ_SDR_INOFF (*(volatile unsigned long *)0x480BD01C)
#define RSZ_SDR_INOFF_OFFSET 0x1C

#define RSZ_SDR_OUTADD (*(volatile unsigned long *)0x480BD020)
#define RSZ_SDR_OUTADD_OFFSET 0x20

#define RSZ_SDR_OUTOFF (*(volatile unsigned long *)0x480BD024)
#define RSZ_SDR_OUTOFF_OFFSET 0x24

#define RSZ_HFILT10 (*(volatile unsigned long *)0x480BD028)
#define RSZ_HFILT10_OFFSET 0x28

#define RSZ_HFILT32 (*(volatile unsigned long *)0x480BD02C)
#define RSZ_HFILT32_OFFSET 0x2C

#define RSZ_HFILT54 (*(volatile unsigned long *)0x480BD030)
#define RSZ_HFILT54_OFFSET 0x30

#define RSZ_HFILT76 (*(volatile unsigned long *)0x480BD034)
#define RSZ_HFILT76_OFFSET 0x34

#define RSZ_HFILT98 (*(volatile unsigned long *)0x480BD038)
#define RSZ_HFILT98_OFFSET 0x38

#define RSZ_HFILT1110 (*(volatile unsigned long *)0x480BD03C)
#define RSZ_HFILT1110_OFFSET 0x3C

#define RSZ_HFILT1312 (*(volatile unsigned long *)0x480BD040)
#define RSZ_HFILT1312_OFFSET 0x40

#define RSZ_HFILT1514 (*(volatile unsigned long *)0x480BD044)
#define RSZ_HFILT1514_OFFSET 0x44

#define RSZ_HFILT1716 (*(volatile unsigned long *)0x480BD048)
#define RSZ_HFILT1716_OFFSET 0x48

#define RSZ_HFILT1918 (*(volatile unsigned long *)0x480BD04C)
#define RSZ_HFILT1918_OFFSET 0x4C

#define RSZ_HFILT2120 (*(volatile unsigned long *)0x480BD050)
#define RSZ_HFILT2120_OFFSET 0x50

#define RSZ_HFILT2322 (*(volatile unsigned long *)0x480BD054)
#define RSZ_HFILT2322_OFFSET 0x54

#define RSZ_HFILT2524 (*(volatile unsigned long *)0x480BD058)
#define RSZ_HFILT2524_OFFSET 0x58

#define RSZ_HFILT2726 (*(volatile unsigned long *)0x480BD05C)
#define RSZ_HFILT2726_OFFSET 0x5C

#define RSZ_HFILT2928 (*(volatile unsigned long *)0x480BD060)
#define RSZ_HFILT2928_OFFSET 0x60

#define RSZ_HFILT3130 (*(volatile unsigned long *)0x480BD064)
#define RSZ_HFILT3130_OFFSET 0x64

#define RSZ_VFILT10 (*(volatile unsigned long *)0x480BD068)
#define RSZ_VFILT10_OFFSET 0x68

#define RSZ_VFILT32 (*(volatile unsigned long *)0x480BD06C)
#define RSZ_VFILT32_OFFSET 0x6C

#define RSZ_VFILT54 (*(volatile unsigned long *)0x480BD070)
#define RSZ_VFILT54_OFFSET 0x70

#define RSZ_VFILT76 (*(volatile unsigned long *)0x480BD074)
#define RSZ_VFILT76_OFFSET 0x74

#define RSZ_VFILT98 (*(volatile unsigned long *)0x480BD078)
#define RSZ_VFILT98_OFFSET 0x78

#define RSZ_VFILT1110 (*(volatile unsigned long *)0x480BD07C)
#define RSZ_VFILT1110_OFFSET 0x7C

#define RSZ_VFILT1312 (*(volatile unsigned long *)0x480BD080)
#define RSZ_VFILT1312_OFFSET 0x80

#define RSZ_VFILT1514 (*(volatile unsigned long *)0x480BD084)
#define RSZ_VFILT1514_OFFSET 0x84

#define RSZ_VFILT1716 (*(volatile unsigned long *)0x480BD088)
#define RSZ_VFILT1716_OFFSET 0x88

#define RSZ_VFILT1918 (*(volatile unsigned long *)0x480BD08C)
#define RSZ_VFILT1918_OFFSET 0x8C

#define RSZ_VFILT2120 (*(volatile unsigned long *)0x480BD090)
#define RSZ_VFILT2120_OFFSET 0x90

#define RSZ_VFILT2322 (*(volatile unsigned long *)0x480BD094)
#define RSZ_VFILT2322_OFFSET 0x94

#define RSZ_VFILT2524 (*(volatile unsigned long *)0x480BD098)
#define RSZ_VFILT2524_OFFSET 0x98

#define RSZ_VFILT2726 (*(volatile unsigned long *)0x480BD09C)
#define RSZ_VFILT2726_OFFSET 0x9C

#define RSZ_VFILT2928 (*(volatile unsigned long *)0x480BD0A0)
#define RSZ_VFILT2928_OFFSET 0xA0

#define RSZ_VFILT3130 (*(volatile unsigned long *)0x480BD0A4)
#define RSZ_VFILT3130_OFFSET 0xA4

#define RSZ_YENH (*(volatile unsigned long *)0x480BD0A8)
#define RSZ_YENH_OFFSET 0xA8

#ifndef ISP_SBL_BASE
#define ISP_SBL_BASE 0x480BD200
#endif

#define SBL_PID (*(volatile unsigned long *)0x480BD200)
#define SBL_PID_OFFSET 0x0

#define SBL_PCR (*(volatile unsigned long *)0x480BD204)
#define SBL_PCR_OFFSET 0x4

#define SBL_GLB_REG_0 (*(volatile unsigned long *)0x480BD208)
#define SBL_GLB_REG_0_OFFSET 0x8

#define SBL_GLB_REG_1 (*(volatile unsigned long *)0x480BD20C)
#define SBL_GLB_REG_1_OFFSET 0xC

#define SBL_GLB_REG_2 (*(volatile unsigned long *)0x480BD210)
#define SBL_GLB_REG_2_OFFSET 0x10

#define SBL_GLB_REG_3 (*(volatile unsigned long *)0x480BD214)
#define SBL_GLB_REG_3_OFFSET 0x14

#define SBL_GLB_REG_4 (*(volatile unsigned long *)0x480BD218)
#define SBL_GLB_REG_4_OFFSET 0x18

#define SBL_GLB_REG_5 (*(volatile unsigned long *)0x480BD21C)
#define SBL_GLB_REG_5_OFFSET 0x1C

#define SBL_GLB_REG_6 (*(volatile unsigned long *)0x480BD220)
#define SBL_GLB_REG_6_OFFSET 0x20

#define SBL_GLB_REG_7 (*(volatile unsigned long *)0x480BD224)
#define SBL_GLB_REG_7_OFFSET 0x24

#define SBL_CCDC_WR_0 (*(volatile unsigned long *)0x480BD248)
#define SBL_CCDC_WR_0_OFFSET 0x48

#define SBL_CCDC_WR_1 (*(volatile unsigned long *)0x480BD24C)
#define SBL_CCDC_WR_1_OFFSET 0x4C

#define SBL_CCDC_WR_2 (*(volatile unsigned long *)0x480BD250)
#define SBL_CCDC_WR_2_OFFSET 0x50

#define SBL_CCDC_WR_3 (*(volatile unsigned long *)0x480BD254)
#define SBL_CCDC_WR_3_OFFSET 0x54

#define SBL_CCDC_FP_RD_0 (*(volatile unsigned long *)0x480BD258)
#define SBL_CCDC_FP_RD_0_OFFSET 0x58

#define SBL_CCDC_FP_RD_1 (*(volatile unsigned long *)0x480BD25C)
#define SBL_CCDC_FP_RD_1_OFFSET 0x5C

#define SBL_PRV_RD_0 (*(volatile unsigned long *)0x480BD260)
#define SBL_PRV_RD_0_OFFSET 0x60

#define SBL_PRV_RD_1 (*(volatile unsigned long *)0x480BD264)
#define SBL_PRV_RD_1_OFFSET 0x64

#define SBL_PRV_RD_2 (*(volatile unsigned long *)0x480BD268)
#define SBL_PRV_RD_2_OFFSET 0x68

#define SBL_PRV_RD_3 (*(volatile unsigned long *)0x480BD26C)
#define SBL_PRV_RD_3_OFFSET 0x6C

#define SBL_PRV_WR_0 (*(volatile unsigned long *)0x480BD270)
#define SBL_PRV_WR_0_OFFSET 0x70

#define SBL_PRV_WR_1 (*(volatile unsigned long *)0x480BD274)
#define SBL_PRV_WR_1_OFFSET 0x74

#define SBL_PRV_WR_2 (*(volatile unsigned long *)0x480BD278)
#define SBL_PRV_WR_2_OFFSET 0x78

#define SBL_PRV_WR_3 (*(volatile unsigned long *)0x480BD27C)
#define SBL_PRV_WR_3_OFFSET 0x7C

#define SBL_PRV_DK_RD_0 (*(volatile unsigned long *)0x480BD280)
#define SBL_PRV_DK_RD_0_OFFSET 0x80

#define SBL_PRV_DK_RD_1 (*(volatile unsigned long *)0x480BD284)
#define SBL_PRV_DK_RD_1_OFFSET 0x84

#define SBL_PRV_DK_RD_2 (*(volatile unsigned long *)0x480BD288)
#define SBL_PRV_DK_RD_2_OFFSET 0x88

#define SBL_PRV_DK_RD_3 (*(volatile unsigned long *)0x480BD28C)
#define SBL_PRV_DK_RD_3_OFFSET 0x8C

#define SBL_RSZ_RD_0 (*(volatile unsigned long *)0x480BD290)
#define SBL_RSZ_RD_0_OFFSET 0x90

#define SBL_RSZ_RD_1 (*(volatile unsigned long *)0x480BD294)
#define SBL_RSZ_RD_1_OFFSET 0x94

#define SBL_RSZ_RD_2 (*(volatile unsigned long *)0x480BD298)
#define SBL_RSZ_RD_2_OFFSET 0x98

#define SBL_RSZ_RD_3 (*(volatile unsigned long *)0x480BD29C)
#define SBL_RSZ_RD_3_OFFSET 0x9C

#define SBL_RSZ1_WR_0 (*(volatile unsigned long *)0x480BD2A0)
#define SBL_RSZ1_WR_0_OFFSET 0xA0

#define SBL_RSZ1_WR_1 (*(volatile unsigned long *)0x480BD2A4)
#define SBL_RSZ1_WR_1_OFFSET 0xA4

#define SBL_RSZ1_WR_2 (*(volatile unsigned long *)0x480BD2A8)
#define SBL_RSZ1_WR_2_OFFSET 0xA8

#define SBL_RSZ1_WR_3 (*(volatile unsigned long *)0x480BD2AC)
#define SBL_RSZ1_WR_3_OFFSET 0xAC

#define SBL_RSZ2_WR_0 (*(volatile unsigned long *)0x480BD2B0)
#define SBL_RSZ2_WR_0_OFFSET 0xB0

#define SBL_RSZ2_WR_1 (*(volatile unsigned long *)0x480BD2B4)
#define SBL_RSZ2_WR_1_OFFSET 0xB4

#define SBL_RSZ2_WR_2 (*(volatile unsigned long *)0x480BD2B8)
#define SBL_RSZ2_WR_2_OFFSET 0xB8

#define SBL_RSZ2_WR_3 (*(volatile unsigned long *)0x480BD2BC)
#define SBL_RSZ2_WR_3_OFFSET 0xBC

#define SBL_RSZ3_WR_0 (*(volatile unsigned long *)0x480BD2C0)
#define SBL_RSZ3_WR_0_OFFSET 0xC0

#define SBL_RSZ3_WR_1 (*(volatile unsigned long *)0x480BD2C4)
#define SBL_RSZ3_WR_1_OFFSET 0xC4

#define SBL_RSZ3_WR_2 (*(volatile unsigned long *)0x480BD2C8)
#define SBL_RSZ3_WR_2_OFFSET 0xC8

#define SBL_RSZ3_WR_3 (*(volatile unsigned long *)0x480BD2CC)
#define SBL_RSZ3_WR_3_OFFSET 0xCC

#define SBL_RSZ4_WR_0 (*(volatile unsigned long *)0x480BD2D0)
#define SBL_RSZ4_WR_0_OFFSET 0xD0

#define SBL_RSZ4_WR_1 (*(volatile unsigned long *)0x480BD2D4)
#define SBL_RSZ4_WR_1_OFFSET 0xD4

#define SBL_RSZ4_WR_2 (*(volatile unsigned long *)0x480BD2D8)
#define SBL_RSZ4_WR_2_OFFSET 0xD8

#define SBL_RSZ4_WR_3 (*(volatile unsigned long *)0x480BD2DC)
#define SBL_RSZ4_WR_3_OFFSET 0xDC

#define SBL_HIST_RD_0 (*(volatile unsigned long *)0x480BD2E0)
#define SBL_HIST_RD_0_OFFSET 0xE0

#define SBL_HIST_RD_1 (*(volatile unsigned long *)0x480BD2E4)
#define SBL_HIST_RD_1_OFFSET 0xE4

#define SBL_H3A_AF_WR_0 (*(volatile unsigned long *)0x480BD2E8)
#define SBL_H3A_AF_WR_0_OFFSET 0xE8

#define SBL_H3A_AF_WR_1 (*(volatile unsigned long *)0x480BD2EC)
#define SBL_H3A_AF_WR_1_OFFSET 0xEC

#define SBL_H3A_AEAWB_WR_0 (*(volatile unsigned long *)0x480BD2F0)
#define SBL_H3A_AEAWB_WR_0_OFFSET 0xF0

#define SBL_H3A_AEAWB_WR_1 (*(volatile unsigned long *)0x480BD2F4)
#define SBL_H3A_AEAWB_WR_1_OFFSET 0xF4

#define SBL_SDR_REQ_EXP (*(volatile unsigned long *)0x480BD2F8)
#define SBL_SDR_REQ_EXP_OFFSET 0xF8

#ifndef INTC_BASE
#define INTC_BASE 0x48200000
#endif

#define INTC_REVISION (*(volatile unsigned *)0x48200000)
#define INTC_REVISION_OFFSET 0x0

#define INTC_SYSCONFIG (*(volatile unsigned *)0x48200010)
#define INTC_SYSCONFIG_OFFSET 0x10

#define INTC_SYSSTATUS (*(volatile unsigned *)0x48200014)
#define INTC_SYSSTATUS_OFFSET 0x14

#define INTC_SIR_IRQ (*(volatile unsigned *)0x48200040)
#define INTC_SIR_IRQ_OFFSET 0x40

#define INTC_SIR_FIQ (*(volatile unsigned *)0x48200044)
#define INTC_SIR_FIQ_OFFSET 0x44

#define INTC_CONTROL (*(volatile unsigned *)0x48200048)
#define INTC_CONTROL_OFFSET 0x48

#define INTC_PROTECTION (*(volatile unsigned *)0x4820004C)
#define INTC_PROTECTION_OFFSET 0x4C

#define INTC_IDLE (*(volatile unsigned *)0x48200050)
#define INTC_IDLE_OFFSET 0x50

#define INTC_IRQ_PRIORITY (*(volatile unsigned *)0x48200060)
#define INTC_IRQ_PRIORITY_OFFSET 0x60

#define INTC_FIQ_PRIORITY (*(volatile unsigned *)0x48200064)
#define INTC_FIQ_PRIORITY_OFFSET 0x64

#define INTC_THRESHOLD (*(volatile unsigned *)0x48200068)
#define INTC_THRESHOLD_OFFSET 0x68

#define INTC_ITR0 (*(volatile unsigned *)0x48200080)
#define INTC_ITR0_OFFSET 0x80

#define INTC_MIR0 (*(volatile unsigned *)0x48200084)
#define INTC_MIR0_OFFSET 0x84

#define INTC_MIR_CLEAR0 (*(volatile unsigned *)0x48200088)
#define INTC_MIR_CLEAR0_OFFSET 0x88

#define INTC_MIR_SET0 (*(volatile unsigned *)0x4820008C)
#define INTC_MIR_SET0_OFFSET 0x8C

#define INTC_ISR_SET0 (*(volatile unsigned *)0x48200090)
#define INTC_ISR_SET0_OFFSET 0x90

#define INTC_ISR_CLEAR0 (*(volatile unsigned *)0x48200094)
#define INTC_ISR_CLEAR0_OFFSET 0x94

#define INTC_PENDING_IRQ0 (*(volatile unsigned *)0x48200098)
#define INTC_PENDING_IRQ0_OFFSET 0x98

#define INTC_PENDING_FIQ0 (*(volatile unsigned *)0x4820009C)
#define INTC_PENDING_FIQ0_OFFSET 0x9C

#define INTC_ITR1 (*(volatile unsigned *)0x482000A0)
#define INTC_ITR1_OFFSET 0xA0

#define INTC_MIR1 (*(volatile unsigned *)0x482000A4)
#define INTC_MIR1_OFFSET 0xA4

#define INTC_MIR_CLEAR1 (*(volatile unsigned *)0x482000A8)
#define INTC_MIR_CLEAR1_OFFSET 0xA8

#define INTC_MIR_SET1 (*(volatile unsigned *)0x482000AC)
#define INTC_MIR_SET1_OFFSET 0xAC

#define INTC_ISR_SET1 (*(volatile unsigned *)0x482000B0)
#define INTC_ISR_SET1_OFFSET 0xB0

#define INTC_ISR_CLEAR1 (*(volatile unsigned *)0x482000B4)
#define INTC_ISR_CLEAR1_OFFSET 0xB4

#define INTC_PENDING_IRQ1 (*(volatile unsigned *)0x482000B8)
#define INTC_PENDING_IRQ1_OFFSET 0xB8

#define INTC_PENDING_FIQ1 (*(volatile unsigned *)0x482000BC)
#define INTC_PENDING_FIQ1_OFFSET 0xBC

#define INTC_ITR2 (*(volatile unsigned *)0x482000C0)
#define INTC_ITR2_OFFSET 0xC0

#define INTC_MIR2 (*(volatile unsigned *)0x482000C4)
#define INTC_MIR2_OFFSET 0xC4

#define INTC_MIR_CLEAR2 (*(volatile unsigned *)0x482000C8)
#define INTC_MIR_CLEAR2_OFFSET 0xC8

#define INTC_MIR_SET2 (*(volatile unsigned *)0x482000CC)
#define INTC_MIR_SET2_OFFSET 0xCC

#define INTC_ISR_SET2 (*(volatile unsigned *)0x482000D0)
#define INTC_ISR_SET2_OFFSET 0xD0

#define INTC_ISR_CLEAR2 (*(volatile unsigned *)0x482000D4)
#define INTC_ISR_CLEAR2_OFFSET 0xD4

#define INTC_PENDING_IRQ2 (*(volatile unsigned *)0x482000D8)
#define INTC_PENDING_IRQ2_OFFSET 0xD8

#define INTC_PENDING_FIQ2 (*(volatile unsigned *)0x482000DC)
#define INTC_PENDING_FIQ2_OFFSET 0xDC

#define INTC_ILR0 (*(volatile unsigned *)0x48200100)
#define INTC_ILR0_OFFSET 0x100

#define INTC_ILR1 (*(volatile unsigned *)0x48200104)
#define INTC_ILR1_OFFSET 0x104

#define INTC_ILR2 (*(volatile unsigned *)0x48200108)
#define INTC_ILR2_OFFSET 0x108

#define INTC_ILR3 (*(volatile unsigned *)0x4820010C)
#define INTC_ILR3_OFFSET 0x10C

#define INTC_ILR4 (*(volatile unsigned *)0x48200110)
#define INTC_ILR4_OFFSET 0x110

#define INTC_ILR5 (*(volatile unsigned *)0x48200114)
#define INTC_ILR5_OFFSET 0x114

#define INTC_ILR6 (*(volatile unsigned *)0x48200118)
#define INTC_ILR6_OFFSET 0x118

#define INTC_ILR7 (*(volatile unsigned *)0x4820011C)
#define INTC_ILR7_OFFSET 0x11C

#define INTC_ILR8 (*(volatile unsigned *)0x48200120)
#define INTC_ILR8_OFFSET 0x120

#define INTC_ILR9 (*(volatile unsigned *)0x48200124)
#define INTC_ILR9_OFFSET 0x124

#define INTC_ILR10 (*(volatile unsigned *)0x48200128)
#define INTC_ILR10_OFFSET 0x128

#define INTC_ILR11 (*(volatile unsigned *)0x4820012C)
#define INTC_ILR11_OFFSET 0x12C

#define INTC_ILR12 (*(volatile unsigned *)0x48200130)
#define INTC_ILR12_OFFSET 0x130

#define INTC_ILR13 (*(volatile unsigned *)0x48200134)
#define INTC_ILR13_OFFSET 0x134

#define INTC_ILR14 (*(volatile unsigned *)0x48200138)
#define INTC_ILR14_OFFSET 0x138

#define INTC_ILR15 (*(volatile unsigned *)0x4820013C)
#define INTC_ILR15_OFFSET 0x13C

#define INTC_ILR16 (*(volatile unsigned *)0x48200140)
#define INTC_ILR16_OFFSET 0x140

#define INTC_ILR17 (*(volatile unsigned *)0x48200144)
#define INTC_ILR17_OFFSET 0x144

#define INTC_ILR18 (*(volatile unsigned *)0x48200148)
#define INTC_ILR18_OFFSET 0x148

#define INTC_ILR19 (*(volatile unsigned *)0x4820014C)
#define INTC_ILR19_OFFSET 0x14C

#define INTC_ILR20 (*(volatile unsigned *)0x48200150)
#define INTC_ILR20_OFFSET 0x150

#define INTC_ILR21 (*(volatile unsigned *)0x48200154)
#define INTC_ILR21_OFFSET 0x154

#define INTC_ILR22 (*(volatile unsigned *)0x48200158)
#define INTC_ILR22_OFFSET 0x158

#define INTC_ILR23 (*(volatile unsigned *)0x4820015C)
#define INTC_ILR23_OFFSET 0x15C

#define INTC_ILR24 (*(volatile unsigned *)0x48200160)
#define INTC_ILR24_OFFSET 0x160

#define INTC_ILR25 (*(volatile unsigned *)0x48200164)
#define INTC_ILR25_OFFSET 0x164

#define INTC_ILR26 (*(volatile unsigned *)0x48200168)
#define INTC_ILR26_OFFSET 0x168

#define INTC_ILR27 (*(volatile unsigned *)0x4820016C)
#define INTC_ILR27_OFFSET 0x16C

#define INTC_ILR28 (*(volatile unsigned *)0x48200170)
#define INTC_ILR28_OFFSET 0x170

#define INTC_ILR29 (*(volatile unsigned *)0x48200174)
#define INTC_ILR29_OFFSET 0x174

#define INTC_ILR30 (*(volatile unsigned *)0x48200178)
#define INTC_ILR30_OFFSET 0x178

#define INTC_ILR31 (*(volatile unsigned *)0x4820017C)
#define INTC_ILR31_OFFSET 0x17C

#define INTC_ILR32 (*(volatile unsigned *)0x48200180)
#define INTC_ILR32_OFFSET 0x180

#define INTC_ILR33 (*(volatile unsigned *)0x48200184)
#define INTC_ILR33_OFFSET 0x184

#define INTC_ILR34 (*(volatile unsigned *)0x48200188)
#define INTC_ILR34_OFFSET 0x188

#define INTC_ILR35 (*(volatile unsigned *)0x4820018C)
#define INTC_ILR35_OFFSET 0x18C

#define INTC_ILR36 (*(volatile unsigned *)0x48200190)
#define INTC_ILR36_OFFSET 0x190

#define INTC_ILR37 (*(volatile unsigned *)0x48200194)
#define INTC_ILR37_OFFSET 0x194

#define INTC_ILR38 (*(volatile unsigned *)0x48200198)
#define INTC_ILR38_OFFSET 0x198

#define INTC_ILR39 (*(volatile unsigned *)0x4820019C)
#define INTC_ILR39_OFFSET 0x19C

#define INTC_ILR40 (*(volatile unsigned *)0x482001A0)
#define INTC_ILR40_OFFSET 0x1A0

#define INTC_ILR41 (*(volatile unsigned *)0x482001A4)
#define INTC_ILR41_OFFSET 0x1A4

#define INTC_ILR42 (*(volatile unsigned *)0x482001A8)
#define INTC_ILR42_OFFSET 0x1A8

#define INTC_ILR43 (*(volatile unsigned *)0x482001AC)
#define INTC_ILR43_OFFSET 0x1AC

#define INTC_ILR44 (*(volatile unsigned *)0x482001B0)
#define INTC_ILR44_OFFSET 0x1B0

#define INTC_ILR45 (*(volatile unsigned *)0x482001B4)
#define INTC_ILR45_OFFSET 0x1B4

#define INTC_ILR46 (*(volatile unsigned *)0x482001B8)
#define INTC_ILR46_OFFSET 0x1B8

#define INTC_ILR47 (*(volatile unsigned *)0x482001BC)
#define INTC_ILR47_OFFSET 0x1BC

#define INTC_ILR48 (*(volatile unsigned *)0x482001C0)
#define INTC_ILR48_OFFSET 0x1C0

#define INTC_ILR49 (*(volatile unsigned *)0x482001C4)
#define INTC_ILR49_OFFSET 0x1C4

#define INTC_ILR50 (*(volatile unsigned *)0x482001C8)
#define INTC_ILR50_OFFSET 0x1C8

#define INTC_ILR51 (*(volatile unsigned *)0x482001CC)
#define INTC_ILR51_OFFSET 0x1CC

#define INTC_ILR52 (*(volatile unsigned *)0x482001D0)
#define INTC_ILR52_OFFSET 0x1D0

#define INTC_ILR53 (*(volatile unsigned *)0x482001D4)
#define INTC_ILR53_OFFSET 0x1D4

#define INTC_ILR54 (*(volatile unsigned *)0x482001D8)
#define INTC_ILR54_OFFSET 0x1D8

#define INTC_ILR55 (*(volatile unsigned *)0x482001DC)
#define INTC_ILR55_OFFSET 0x1DC

#define INTC_ILR56 (*(volatile unsigned *)0x482001E0)
#define INTC_ILR56_OFFSET 0x1E0

#define INTC_ILR57 (*(volatile unsigned *)0x482001E4)
#define INTC_ILR57_OFFSET 0x1E4

#define INTC_ILR58 (*(volatile unsigned *)0x482001E8)
#define INTC_ILR58_OFFSET 0x1E8

#define INTC_ILR59 (*(volatile unsigned *)0x482001EC)
#define INTC_ILR59_OFFSET 0x1EC

#define INTC_ILR60 (*(volatile unsigned *)0x482001F0)
#define INTC_ILR60_OFFSET 0x1F0

#define INTC_ILR61 (*(volatile unsigned *)0x482001F4)
#define INTC_ILR61_OFFSET 0x1F4

#define INTC_ILR62 (*(volatile unsigned *)0x482001F8)
#define INTC_ILR62_OFFSET 0x1F8

#define INTC_ILR63 (*(volatile unsigned *)0x482001FC)
#define INTC_ILR63_OFFSET 0x1FC

#define INTC_ILR64 (*(volatile unsigned *)0x48200200)
#define INTC_ILR64_OFFSET 0x200

#define INTC_ILR65 (*(volatile unsigned *)0x48200204)
#define INTC_ILR65_OFFSET 0x204

#define INTC_ILR66 (*(volatile unsigned *)0x48200208)
#define INTC_ILR66_OFFSET 0x208

#define INTC_ILR67 (*(volatile unsigned *)0x4820020C)
#define INTC_ILR67_OFFSET 0x20C

#define INTC_ILR68 (*(volatile unsigned *)0x48200210)
#define INTC_ILR68_OFFSET 0x210

#define INTC_ILR69 (*(volatile unsigned *)0x48200214)
#define INTC_ILR69_OFFSET 0x214

#define INTC_ILR70 (*(volatile unsigned *)0x48200218)
#define INTC_ILR70_OFFSET 0x218

#define INTC_ILR71 (*(volatile unsigned *)0x4820021C)
#define INTC_ILR71_OFFSET 0x21C

#define INTC_ILR72 (*(volatile unsigned *)0x48200220)
#define INTC_ILR72_OFFSET 0x220

#define INTC_ILR73 (*(volatile unsigned *)0x48200224)
#define INTC_ILR73_OFFSET 0x224

#define INTC_ILR74 (*(volatile unsigned *)0x48200228)
#define INTC_ILR74_OFFSET 0x228

#define INTC_ILR75 (*(volatile unsigned *)0x4820022C)
#define INTC_ILR75_OFFSET 0x22C

#define INTC_ILR76 (*(volatile unsigned *)0x48200230)
#define INTC_ILR76_OFFSET 0x230

#define INTC_ILR77 (*(volatile unsigned *)0x48200234)
#define INTC_ILR77_OFFSET 0x234

#define INTC_ILR78 (*(volatile unsigned *)0x48200238)
#define INTC_ILR78_OFFSET 0x238

#define INTC_ILR79 (*(volatile unsigned *)0x4820023C)
#define INTC_ILR79_OFFSET 0x23C

#define INTC_ILR80 (*(volatile unsigned *)0x48200240)
#define INTC_ILR80_OFFSET 0x240

#define INTC_ILR81 (*(volatile unsigned *)0x48200244)
#define INTC_ILR81_OFFSET 0x244

#define INTC_ILR82 (*(volatile unsigned *)0x48200248)
#define INTC_ILR82_OFFSET 0x248

#define INTC_ILR83 (*(volatile unsigned *)0x4820024C)
#define INTC_ILR83_OFFSET 0x24C

#define INTC_ILR84 (*(volatile unsigned *)0x48200250)
#define INTC_ILR84_OFFSET 0x250

#define INTC_ILR85 (*(volatile unsigned *)0x48200254)
#define INTC_ILR85_OFFSET 0x254

#define INTC_ILR86 (*(volatile unsigned *)0x48200258)
#define INTC_ILR86_OFFSET 0x258

#define INTC_ILR87 (*(volatile unsigned *)0x4820025C)
#define INTC_ILR87_OFFSET 0x25C

#define INTC_ILR88 (*(volatile unsigned *)0x48200260)
#define INTC_ILR88_OFFSET 0x260

#define INTC_ILR89 (*(volatile unsigned *)0x48200264)
#define INTC_ILR89_OFFSET 0x264

#define INTC_ILR90 (*(volatile unsigned *)0x48200268)
#define INTC_ILR90_OFFSET 0x268

#define INTC_ILR91 (*(volatile unsigned *)0x4820026C)
#define INTC_ILR91_OFFSET 0x26C

#define INTC_ILR92 (*(volatile unsigned *)0x48200270)
#define INTC_ILR92_OFFSET 0x270

#define INTC_ILR93 (*(volatile unsigned *)0x48200274)
#define INTC_ILR93_OFFSET 0x274

#define INTC_ILR94 (*(volatile unsigned *)0x48200278)
#define INTC_ILR94_OFFSET 0x278

#define INTC_ILR95 (*(volatile unsigned *)0x4820027C)
#define INTC_ILR95_OFFSET 0x27C

#ifndef WDT1_BASE
#define WDT1_BASE 0x4830C000
#endif

#define WDT1_WIDR (*(volatile unsigned *)0x4830C000)
#define WDT1_WIDR_OFFSET 0x0

#define WDT1_WD_SYSCONFIG (*(volatile unsigned *)0x4830C010)
#define WDT1_WD_SYSCONFIG_OFFSET 0x10

#define WDT1_WD_SYSSTATUS (*(volatile unsigned *)0x4830C014)
#define WDT1_WD_SYSSTATUS_OFFSET 0x14

#define WDT1_WISR (*(volatile unsigned *)0x4830C018)
#define WDT1_WISR_OFFSET 0x18

#define WDT1_WIER (*(volatile unsigned *)0x4830C01C)
#define WDT1_WIER_OFFSET 0x1C

#define WDT1_WWER (*(volatile unsigned *)0x4830C020)
#define WDT1_WWER_OFFSET 0x20

#define WDT1_WCLR (*(volatile unsigned *)0x4830C024)
#define WDT1_WCLR_OFFSET 0x24

#define WDT1_WCRR (*(volatile unsigned *)0x4830C028)
#define WDT1_WCRR_OFFSET 0x28

#define WDT1_WLDR (*(volatile unsigned *)0x4830C02C)
#define WDT1_WLDR_OFFSET 0x2C

#define WDT1_WTGR (*(volatile unsigned *)0x4830C030)
#define WDT1_WTGR_OFFSET 0x30

#define WDT1_WWPS (*(volatile unsigned *)0x4830C034)
#define WDT1_WWPS_OFFSET 0x34

#define WDT1_WSPR (*(volatile unsigned *)0x4830C048)
#define WDT1_WSPR_OFFSET 0x48

#ifndef GPIO1_BASE
#define GPIO1_BASE 0x48310000
#endif

#define GPIO1_REVISION (*(volatile unsigned long *)0x48310000)
#define GPIO1_REVISION_OFFSET 0x0

#define GPIO1_SYSCONFIG (*(volatile unsigned long *)0x48310010)
#define GPIO1_SYSCONFIG_OFFSET 0x10

#define GPIO1_SYSSTATUS (*(volatile unsigned long *)0x48310014)
#define GPIO1_SYSSTATUS_OFFSET 0x14

#define GPIO1_IRQSTATUS1 (*(volatile unsigned long *)0x48310018)
#define GPIO1_IRQSTATUS1_OFFSET 0x18

#define GPIO1_IRQENABLE1 (*(volatile unsigned long *)0x4831001C)
#define GPIO1_IRQENABLE1_OFFSET 0x1C

#define GPIO1_WAKEUPENABLE (*(volatile unsigned long *)0x48310020)
#define GPIO1_WAKEUPENABLE_OFFSET 0x20

#define GPIO1_IRQSTATUS2 (*(volatile unsigned long *)0x48310028)
#define GPIO1_IRQSTATUS2_OFFSET 0x28

#define GPIO1_IRQENABLE2 (*(volatile unsigned long *)0x4831002C)
#define GPIO1_IRQENABLE2_OFFSET 0x2C

#define GPIO1_CTRL (*(volatile unsigned long *)0x48310030)
#define GPIO1_CTRL_OFFSET 0x30

#define GPIO1_OE (*(volatile unsigned long *)0x48310034)
#define GPIO1_OE_OFFSET 0x34

#define GPIO1_DATAIN (*(volatile unsigned long *)0x48310038)
#define GPIO1_DATAIN_OFFSET 0x38

#define GPIO1_DATAOUT (*(volatile unsigned long *)0x4831003C)
#define GPIO1_DATAOUT_OFFSET 0x3C

#define GPIO1_LEVELDETECT0 (*(volatile unsigned long *)0x48310040)
#define GPIO1_LEVELDETECT0_OFFSET 0x40

#define GPIO1_LEVELDETECT1 (*(volatile unsigned long *)0x48310044)
#define GPIO1_LEVELDETECT1_OFFSET 0x44

#define GPIO1_RISINGDETECT (*(volatile unsigned long *)0x48310048)
#define GPIO1_RISINGDETECT_OFFSET 0x48

#define GPIO1_FALLINGDETECT (*(volatile unsigned long *)0x4831004C)
#define GPIO1_FALLINGDETECT_OFFSET 0x4C

#define GPIO1_DEBOUNCENABLE (*(volatile unsigned long *)0x48310050)
#define GPIO1_DEBOUNCENABLE_OFFSET 0x50

#define GPIO1_DEBOUNCINGTIME (*(volatile unsigned long *)0x48310054)
#define GPIO1_DEBOUNCINGTIME_OFFSET 0x54

#define GPIO1_CLEARIRQENABLE1 (*(volatile unsigned long *)0x48310060)
#define GPIO1_CLEARIRQENABLE1_OFFSET 0x60

#define GPIO1_SETIRQENABLE1 (*(volatile unsigned long *)0x48310064)
#define GPIO1_SETIRQENABLE1_OFFSET 0x64

#define GPIO1_CLEARIRQENABLE2 (*(volatile unsigned long *)0x48310070)
#define GPIO1_CLEARIRQENABLE2_OFFSET 0x70

#define GPIO1_SETIRQENABLE2 (*(volatile unsigned long *)0x48310074)
#define GPIO1_SETIRQENABLE2_OFFSET 0x74

#define GPIO1_CLEARWKUENA (*(volatile unsigned long *)0x48310080)
#define GPIO1_CLEARWKUENA_OFFSET 0x80

#define GPIO1_SETWKUENA (*(volatile unsigned long *)0x48310084)
#define GPIO1_SETWKUENA_OFFSET 0x84

#define GPIO1_CLEARDATAOUT (*(volatile unsigned long *)0x48310090)
#define GPIO1_CLEARDATAOUT_OFFSET 0x90

#define GPIO1_SETDATAOUT (*(volatile unsigned long *)0x48310094)
#define GPIO1_SETDATAOUT_OFFSET 0x94

#ifndef WDT2_BASE
#define WDT2_BASE 0x48314000
#endif

#define WDT2_WIDR (*(volatile unsigned *)0x48314000)
#define WDT2_WIDR_OFFSET 0x0

#define WDT2_WD_SYSCONFIG (*(volatile unsigned *)0x48314010)
#define WDT2_WD_SYSCONFIG_OFFSET 0x10

#define WDT2_WD_SYSSTATUS (*(volatile unsigned *)0x48314014)
#define WDT2_WD_SYSSTATUS_OFFSET 0x14

#define WDT2_WISR (*(volatile unsigned *)0x48314018)
#define WDT2_WISR_OFFSET 0x18

#define WDT2_WIER (*(volatile unsigned *)0x4831401C)
#define WDT2_WIER_OFFSET 0x1C

#define WDT2_WWER (*(volatile unsigned *)0x48314020)
#define WDT2_WWER_OFFSET 0x20

#define WDT2_WCLR (*(volatile unsigned *)0x48314024)
#define WDT2_WCLR_OFFSET 0x24

#define WDT2_WCRR (*(volatile unsigned *)0x48314028)
#define WDT2_WCRR_OFFSET 0x28

#define WDT2_WLDR (*(volatile unsigned *)0x4831402C)
#define WDT2_WLDR_OFFSET 0x2C

#define WDT2_WTGR (*(volatile unsigned *)0x48314030)
#define WDT2_WTGR_OFFSET 0x30

#define WDT2_WWPS (*(volatile unsigned *)0x48314034)
#define WDT2_WWPS_OFFSET 0x34

#define WDT2_WSPR (*(volatile unsigned *)0x48314048)
#define WDT2_WSPR_OFFSET 0x48

#ifndef GPTIMER1_BASE
#define GPTIMER1_BASE 0x48318000
#endif

#define GPTIMER1_TIDR (*(volatile unsigned *)0x48318000)
#define GPTIMER1_TIDR_OFFSET 0x0

#define GPTIMER1_TIOCP_CFG (*(volatile unsigned long *)0x48318010)
#define GPTIMER1_TIOCP_CFG_OFFSET 0x10

#define GPTIMER1_TISTAT (*(volatile unsigned long *)0x48318014)
#define GPTIMER1_TISTAT_OFFSET 0x14

#define GPTIMER1_TISR (*(volatile unsigned long *)0x48318018)
#define GPTIMER1_TISR_OFFSET 0x18

#define GPTIMER1_TIER (*(volatile unsigned long *)0x4831801C)
#define GPTIMER1_TIER_OFFSET 0x1C

#define GPTIMER1_TWER (*(volatile unsigned long *)0x48318020)
#define GPTIMER1_TWER_OFFSET 0x20

#define GPTIMER1_TCLR (*(volatile unsigned long *)0x48318024)
#define GPTIMER1_TCLR_OFFSET 0x24

#define GPTIMER1_TCRR (*(volatile unsigned long *)0x48318028)
#define GPTIMER1_TCRR_OFFSET 0x28

#define GPTIMER1_TLDR (*(volatile unsigned long *)0x4831802C)
#define GPTIMER1_TLDR_OFFSET 0x2C

#define GPTIMER1_TTGR (*(volatile unsigned long *)0x48318030)
#define GPTIMER1_TTGR_OFFSET 0x30

#define GPTIMER1_TWPS (*(volatile unsigned long *)0x48318034)
#define GPTIMER1_TWPS_OFFSET 0x34

#define GPTIMER1_TMAR (*(volatile unsigned long *)0x48318038)
#define GPTIMER1_TMAR_OFFSET 0x38

#define GPTIMER1_TCAR1 (*(volatile unsigned long *)0x4831803C)
#define GPTIMER1_TCAR1_OFFSET 0x3C

#define GPTIMER1_TSICR (*(volatile unsigned long *)0x48318040)
#define GPTIMER1_TSICR_OFFSET 0x40

#define GPTIMER1_TCAR2 (*(volatile unsigned long *)0x48318044)
#define GPTIMER1_TCAR2_OFFSET 0x44

#define GPTIMER1_TPIR (*(volatile unsigned long *)0x48318048)
#define GPTIMER1_TPIR_OFFSET 0x48

#define GPTIMER1_TNIR (*(volatile unsigned long *)0x4831804C)
#define GPTIMER1_TNIR_OFFSET 0x4C

#define GPTIMER1_TCVR (*(volatile unsigned long *)0x48318050)
#define GPTIMER1_TCVR_OFFSET 0x50

#define GPTIMER1_TOCR (*(volatile unsigned long *)0x48318054)
#define GPTIMER1_TOCR_OFFSET 0x54

#define GPTIMER1_TOWR (*(volatile unsigned long *)0x48318058)
#define GPTIMER1_TOWR_OFFSET 0x58

#ifndef REG_32K_Timer_BASE
#define REG_32K_Timer_BASE 0x48320004
#endif

#define REG_32KSYNCNT_SYSCONFIG (*(volatile unsigned long *)0x48320004)
#define REG_32KSYNCNT_SYSCONFIG_OFFSET 0x0

#define REG_32KSYNCNT_CR (*(volatile unsigned long *)0x48320010)
#define REG_32KSYNCNT_CR_OFFSET 0xC

#ifndef UART3_BASE
#define UART3_BASE 0x49020000
#endif

#define UART3_DLL_RHR_THR (*(volatile unsigned short *)0x49020000)
#define UART3_DLL_RHR_THR_OFFSET 0x0

#define UART3_IER_DLH (*(volatile unsigned short *)0x49020004)
#define UART3_IER_DLH_OFFSET 0x4

#define UART3_FCR_IIR_EFR (*(volatile unsigned short *)0x49020008)
#define UART3_FCR_IIR_EFR_OFFSET 0x8

#define UART3_LCR (*(volatile unsigned short *)0x4902000C)
#define UART3_LCR_OFFSET 0xC

#define UART3_MCR_ADDR1 (*(volatile unsigned short *)0x49020010)
#define UART3_MCR_ADDR1_OFFSET 0x10

#define UART3_LSR_ADDR2 (*(volatile unsigned short *)0x49020014)
#define UART3_LSR_ADDR2_OFFSET 0x14

#define UART3_MSR_TCR (*(volatile unsigned short *)0x49020018)
#define UART3_MSR_TCR_OFFSET 0x18

#define UART3_SPR_TLR (*(volatile unsigned short *)0x4902001C)
#define UART3_SPR_TLR_OFFSET 0x1C

#define UART3_MDR1 (*(volatile unsigned short *)0x49020020)
#define UART3_MDR1_OFFSET 0x20

#define UART3_MDR2 (*(volatile unsigned short *)0x49020024)
#define UART3_MDR2_OFFSET 0x24

#define UART3_TXFLL_SFLSR (*(volatile unsigned short *)0x49020028)
#define UART3_TXFLL_SFLSR_OFFSET 0x28

#define UART3_RESUME_TXFLH (*(volatile unsigned short *)0x4902002C)
#define UART3_RESUME_TXFLH_OFFSET 0x2C

#define UART3_RXFLL_SFREGL (*(volatile unsigned short *)0x49020030)
#define UART3_RXFLL_SFREGL_OFFSET 0x30

#define UART3_SFREGH_RXFLH (*(volatile unsigned short *)0x49020034)
#define UART3_SFREGH_RXFLH_OFFSET 0x34

#define UART3_BLR_UASR (*(volatile unsigned short *)0x49020038)
#define UART3_BLR_UASR_OFFSET 0x38

#define UART3_ACREG (*(volatile unsigned short *)0x4902003C)
#define UART3_ACREG_OFFSET 0x3C

#define UART3_SCR (*(volatile unsigned short *)0x49020040)
#define UART3_SCR_OFFSET 0x40

#define UART3_SSR (*(volatile unsigned short *)0x49020044)
#define UART3_SSR_OFFSET 0x44

#define UART3_EBLR (*(volatile unsigned short *)0x49020048)
#define UART3_EBLR_OFFSET 0x48

#define UART3_SYSC (*(volatile unsigned short *)0x49020054)
#define UART3_SYSC_OFFSET 0x54

#define UART3_SYSS (*(volatile unsigned short *)0x49020058)
#define UART3_SYSS_OFFSET 0x58

#define UART3_WER (*(volatile unsigned short *)0x4902005C)
#define UART3_WER_OFFSET 0x5C

#define UART3_CFPS (*(volatile unsigned short *)0x49020060)
#define UART3_CFPS_OFFSET 0x60

#ifndef McBSP2_BASE
#define McBSP2_BASE 0x49022000
#endif

#define McBSP2_MCBSPLP_DRR_REG (*(volatile unsigned long *)0x49022000)
#define McBSP2_MCBSPLP_DRR_REG_OFFSET 0x0

#define McBSP2_MCBSPLP_DXR_REG (*(volatile unsigned long *)0x49022008)
#define McBSP2_MCBSPLP_DXR_REG_OFFSET 0x8

#define McBSP2_MCBSPLP_SPCR2_REG (*(volatile unsigned long *)0x49022010)
#define McBSP2_MCBSPLP_SPCR2_REG_OFFSET 0x10

#define McBSP2_MCBSPLP_SPCR1_REG (*(volatile unsigned long *)0x49022014)
#define McBSP2_MCBSPLP_SPCR1_REG_OFFSET 0x14

#define McBSP2_MCBSPLP_RCR2_REG (*(volatile unsigned long *)0x49022018)
#define McBSP2_MCBSPLP_RCR2_REG_OFFSET 0x18

#define McBSP2_MCBSPLP_RCR1_REG (*(volatile unsigned long *)0x4902201C)
#define McBSP2_MCBSPLP_RCR1_REG_OFFSET 0x1C

#define McBSP2_MCBSPLP_XCR2_REG (*(volatile unsigned long *)0x49022020)
#define McBSP2_MCBSPLP_XCR2_REG_OFFSET 0x20

#define McBSP2_MCBSPLP_XCR1_REG (*(volatile unsigned long *)0x49022024)
#define McBSP2_MCBSPLP_XCR1_REG_OFFSET 0x24

#define McBSP2_MCBSPLP_SRGR2_REG (*(volatile unsigned long *)0x49022028)
#define McBSP2_MCBSPLP_SRGR2_REG_OFFSET 0x28

#define McBSP2_MCBSPLP_SRGR1_REG (*(volatile unsigned long *)0x4902202C)
#define McBSP2_MCBSPLP_SRGR1_REG_OFFSET 0x2C

#define McBSP2_MCBSPLP_MCR2_REG (*(volatile unsigned long *)0x49022030)
#define McBSP2_MCBSPLP_MCR2_REG_OFFSET 0x30

#define McBSP2_MCBSPLP_MCR1_REG (*(volatile unsigned long *)0x49022034)
#define McBSP2_MCBSPLP_MCR1_REG_OFFSET 0x34

#define McBSP2_MCBSPLP_RCERA_REG (*(volatile unsigned long *)0x49022038)
#define McBSP2_MCBSPLP_RCERA_REG_OFFSET 0x38

#define McBSP2_MCBSPLP_RCERREG (*(volatile unsigned long *)0x4902203C)
#define McBSP2_MCBSPLP_RCERREG_OFFSET 0x3C

#define McBSP2_MCBSPLP_XCERA_REG (*(volatile unsigned long *)0x49022040)
#define McBSP2_MCBSPLP_XCERA_REG_OFFSET 0x40

#define McBSP2_MCBSPLP_XCERREG (*(volatile unsigned long *)0x49022044)
#define McBSP2_MCBSPLP_XCERREG_OFFSET 0x44

#define McBSP2_MCBSPLP_PCR_REG (*(volatile unsigned long *)0x49022048)
#define McBSP2_MCBSPLP_PCR_REG_OFFSET 0x48

#define McBSP2_MCBSPLP_RCERC_REG (*(volatile unsigned long *)0x4902204C)
#define McBSP2_MCBSPLP_RCERC_REG_OFFSET 0x4C

#define McBSP2_MCBSPLP_RCERD_REG (*(volatile unsigned long *)0x49022050)
#define McBSP2_MCBSPLP_RCERD_REG_OFFSET 0x50

#define McBSP2_MCBSPLP_XCERC_REG (*(volatile unsigned long *)0x49022054)
#define McBSP2_MCBSPLP_XCERC_REG_OFFSET 0x54

#define McBSP2_MCBSPLP_XCERD_REG (*(volatile unsigned long *)0x49022058)
#define McBSP2_MCBSPLP_XCERD_REG_OFFSET 0x58

#define McBSP2_MCBSPLP_RCERE_REG (*(volatile unsigned long *)0x4902205C)
#define McBSP2_MCBSPLP_RCERE_REG_OFFSET 0x5C

#define McBSP2_MCBSPLP_RCERF_REG (*(volatile unsigned long *)0x49022060)
#define McBSP2_MCBSPLP_RCERF_REG_OFFSET 0x60

#define McBSP2_MCBSPLP_XCERE_REG (*(volatile unsigned long *)0x49022064)
#define McBSP2_MCBSPLP_XCERE_REG_OFFSET 0x64

#define McBSP2_MCBSPLP_XCERF_REG (*(volatile unsigned long *)0x49022068)
#define McBSP2_MCBSPLP_XCERF_REG_OFFSET 0x68

#define McBSP2_MCBSPLP_RCERG_REG (*(volatile unsigned long *)0x4902206C)
#define McBSP2_MCBSPLP_RCERG_REG_OFFSET 0x6C

#define McBSP2_MCBSPLP_RCERH_REG (*(volatile unsigned long *)0x49022070)
#define McBSP2_MCBSPLP_RCERH_REG_OFFSET 0x70

#define McBSP2_MCBSPLP_XCERG_REG (*(volatile unsigned long *)0x49022074)
#define McBSP2_MCBSPLP_XCERG_REG_OFFSET 0x74

#define McBSP2_MCBSPLP_XCERH_REG (*(volatile unsigned long *)0x49022078)
#define McBSP2_MCBSPLP_XCERH_REG_OFFSET 0x78

#define McBSP2_MCBSPLP_RINTCLR_REG (*(volatile unsigned long *)0x49022080)
#define McBSP2_MCBSPLP_RINTCLR_REG_OFFSET 0x80

#define McBSP2_MCBSPLP_XINTCLR_REG (*(volatile unsigned long *)0x49022084)
#define McBSP2_MCBSPLP_XINTCLR_REG_OFFSET 0x84

#define McBSP2_MCBSPLP_ROVFLCLR_REG (*(volatile unsigned long *)0x49022088)
#define McBSP2_MCBSPLP_ROVFLCLR_REG_OFFSET 0x88

#define McBSP2_MCBSPLP_SYSCONFIG_REG (*(volatile unsigned long *)0x4902208C)
#define McBSP2_MCBSPLP_SYSCONFIG_REG_OFFSET 0x8C

#define McBSP2_MCBSPLP_THRSH2_REG (*(volatile unsigned long *)0x49022090)
#define McBSP2_MCBSPLP_THRSH2_REG_OFFSET 0x90

#define McBSP2_MCBSPLP_THRSH1_REG (*(volatile unsigned long *)0x49022094)
#define McBSP2_MCBSPLP_THRSH1_REG_OFFSET 0x94

#define McBSP2_MCBSPLP_IRQSTATUS_REG (*(volatile unsigned long *)0x490220A0)
#define McBSP2_MCBSPLP_IRQSTATUS_REG_OFFSET 0xA0

#define McBSP2_MCBSPLP_IRQENABLE_REG (*(volatile unsigned long *)0x490220A4)
#define McBSP2_MCBSPLP_IRQENABLE_REG_OFFSET 0xA4

#define McBSP2_MCBSPLP_WAKEUPEN_REG (*(volatile unsigned long *)0x490220A8)
#define McBSP2_MCBSPLP_WAKEUPEN_REG_OFFSET 0xA8

#define McBSP2_MCBSPLP_XCCR_REG (*(volatile unsigned long *)0x490220AC)
#define McBSP2_MCBSPLP_XCCR_REG_OFFSET 0xAC

#define McBSP2_MCBSPLP_RCCR_REG (*(volatile unsigned long *)0x490220B0)
#define McBSP2_MCBSPLP_RCCR_REG_OFFSET 0xB0

#define McBSP2_MCBSPLP_XBUFFSTAT_REG (*(volatile unsigned long *)0x490220B4)
#define McBSP2_MCBSPLP_XBUFFSTAT_REG_OFFSET 0xB4

#define McBSP2_MCBSPLP_RBUFFSTAT_REG (*(volatile unsigned long *)0x490220B8)
#define McBSP2_MCBSPLP_RBUFFSTAT_REG_OFFSET 0xB8

#define McBSP2_MCBSPLP_SSELCR_REG (*(volatile unsigned long *)0x490220BC)
#define McBSP2_MCBSPLP_SSELCR_REG_OFFSET 0xBC

#define McBSP2_MCBSPLP_STATUS_REG (*(volatile unsigned long *)0x490220C0)
#define McBSP2_MCBSPLP_STATUS_REG_OFFSET 0xC0

#ifndef SIDETONE_McBSP2_BASE
#define SIDETONE_McBSP2_BASE 0x49028000
#endif

#define SIDETONE_McBSP2_ST_REV_REG (*(volatile unsigned long *)0x49028000)
#define SIDETONE_McBSP2_ST_REV_REG_OFFSET 0x0

#define SIDETONE_McBSP2_ST_SYSCONFIG_REG (*(volatile unsigned long *)0x49028010)
#define SIDETONE_McBSP2_ST_SYSCONFIG_REG_OFFSET 0x10

#define SIDETONE_McBSP2_ST_IRQSTATUS_REG (*(volatile unsigned long *)0x49028018)
#define SIDETONE_McBSP2_ST_IRQSTATUS_REG_OFFSET 0x18

#define SIDETONE_McBSP2_ST_IRQENABLE_REG (*(volatile unsigned long *)0x4902801C)
#define SIDETONE_McBSP2_ST_IRQENABLE_REG_OFFSET 0x1C

#define SIDETONE_McBSP2_ST_SGAINCR_REG (*(volatile unsigned long *)0x49028024)
#define SIDETONE_McBSP2_ST_SGAINCR_REG_OFFSET 0x24

#define SIDETONE_McBSP2_ST_SFIRCR_REG (*(volatile unsigned long *)0x49028028)
#define SIDETONE_McBSP2_ST_SFIRCR_REG_OFFSET 0x28

#define SIDETONE_McBSP2_ST_SSELCR_REG (*(volatile unsigned long *)0x4902802C)
#define SIDETONE_McBSP2_ST_SSELCR_REG_OFFSET 0x2C

#ifndef McBSP3_BASE
#define McBSP3_BASE 0x49024000
#endif

#define McBSP3_MCBSPLP_DRR_REG (*(volatile unsigned long *)0x49024000)
#define McBSP3_MCBSPLP_DRR_REG_OFFSET 0x0

#define McBSP3_MCBSPLP_DXR_REG (*(volatile unsigned long *)0x49024008)
#define McBSP3_MCBSPLP_DXR_REG_OFFSET 0x8

#define McBSP3_MCBSPLP_SPCR2_REG (*(volatile unsigned long *)0x49024010)
#define McBSP3_MCBSPLP_SPCR2_REG_OFFSET 0x10

#define McBSP3_MCBSPLP_SPCR1_REG (*(volatile unsigned long *)0x49024014)
#define McBSP3_MCBSPLP_SPCR1_REG_OFFSET 0x14

#define McBSP3_MCBSPLP_RCR2_REG (*(volatile unsigned long *)0x49024018)
#define McBSP3_MCBSPLP_RCR2_REG_OFFSET 0x18

#define McBSP3_MCBSPLP_RCR1_REG (*(volatile unsigned long *)0x4902401C)
#define McBSP3_MCBSPLP_RCR1_REG_OFFSET 0x1C

#define McBSP3_MCBSPLP_XCR2_REG (*(volatile unsigned long *)0x49024020)
#define McBSP3_MCBSPLP_XCR2_REG_OFFSET 0x20

#define McBSP3_MCBSPLP_XCR1_REG (*(volatile unsigned long *)0x49024024)
#define McBSP3_MCBSPLP_XCR1_REG_OFFSET 0x24

#define McBSP3_MCBSPLP_SRGR2_REG (*(volatile unsigned long *)0x49024028)
#define McBSP3_MCBSPLP_SRGR2_REG_OFFSET 0x28

#define McBSP3_MCBSPLP_SRGR1_REG (*(volatile unsigned long *)0x4902402C)
#define McBSP3_MCBSPLP_SRGR1_REG_OFFSET 0x2C

#define McBSP3_MCBSPLP_MCR2_REG (*(volatile unsigned long *)0x49024030)
#define McBSP3_MCBSPLP_MCR2_REG_OFFSET 0x30

#define McBSP3_MCBSPLP_MCR1_REG (*(volatile unsigned long *)0x49024034)
#define McBSP3_MCBSPLP_MCR1_REG_OFFSET 0x34

#define McBSP3_MCBSPLP_RCERA_REG (*(volatile unsigned long *)0x49024038)
#define McBSP3_MCBSPLP_RCERA_REG_OFFSET 0x38

#define McBSP3_MCBSPLP_RCERREG (*(volatile unsigned long *)0x4902403C)
#define McBSP3_MCBSPLP_RCERREG_OFFSET 0x3C

#define McBSP3_MCBSPLP_XCERA_REG (*(volatile unsigned long *)0x49024040)
#define McBSP3_MCBSPLP_XCERA_REG_OFFSET 0x40

#define McBSP3_MCBSPLP_XCERREG (*(volatile unsigned long *)0x49024044)
#define McBSP3_MCBSPLP_XCERREG_OFFSET 0x44

#define McBSP3_MCBSPLP_PCR_REG (*(volatile unsigned long *)0x49024048)
#define McBSP3_MCBSPLP_PCR_REG_OFFSET 0x48

#define McBSP3_MCBSPLP_RCERC_REG (*(volatile unsigned long *)0x4902404C)
#define McBSP3_MCBSPLP_RCERC_REG_OFFSET 0x4C

#define McBSP3_MCBSPLP_RCERD_REG (*(volatile unsigned long *)0x49024050)
#define McBSP3_MCBSPLP_RCERD_REG_OFFSET 0x50

#define McBSP3_MCBSPLP_XCERC_REG (*(volatile unsigned long *)0x49024054)
#define McBSP3_MCBSPLP_XCERC_REG_OFFSET 0x54

#define McBSP3_MCBSPLP_XCERD_REG (*(volatile unsigned long *)0x49024058)
#define McBSP3_MCBSPLP_XCERD_REG_OFFSET 0x58

#define McBSP3_MCBSPLP_RCERE_REG (*(volatile unsigned long *)0x4902405C)
#define McBSP3_MCBSPLP_RCERE_REG_OFFSET 0x5C

#define McBSP3_MCBSPLP_RCERF_REG (*(volatile unsigned long *)0x49024060)
#define McBSP3_MCBSPLP_RCERF_REG_OFFSET 0x60

#define McBSP3_MCBSPLP_XCERE_REG (*(volatile unsigned long *)0x49024064)
#define McBSP3_MCBSPLP_XCERE_REG_OFFSET 0x64

#define McBSP3_MCBSPLP_XCERF_REG (*(volatile unsigned long *)0x49024068)
#define McBSP3_MCBSPLP_XCERF_REG_OFFSET 0x68

#define McBSP3_MCBSPLP_RCERG_REG (*(volatile unsigned long *)0x4902406C)
#define McBSP3_MCBSPLP_RCERG_REG_OFFSET 0x6C

#define McBSP3_MCBSPLP_RCERH_REG (*(volatile unsigned long *)0x49024070)
#define McBSP3_MCBSPLP_RCERH_REG_OFFSET 0x70

#define McBSP3_MCBSPLP_XCERG_REG (*(volatile unsigned long *)0x49024074)
#define McBSP3_MCBSPLP_XCERG_REG_OFFSET 0x74

#define McBSP3_MCBSPLP_XCERH_REG (*(volatile unsigned long *)0x49024078)
#define McBSP3_MCBSPLP_XCERH_REG_OFFSET 0x78

#define McBSP3_MCBSPLP_RINTCLR_REG (*(volatile unsigned long *)0x49024080)
#define McBSP3_MCBSPLP_RINTCLR_REG_OFFSET 0x80

#define McBSP3_MCBSPLP_XINTCLR_REG (*(volatile unsigned long *)0x49024084)
#define McBSP3_MCBSPLP_XINTCLR_REG_OFFSET 0x84

#define McBSP3_MCBSPLP_ROVFLCLR_REG (*(volatile unsigned long *)0x49024088)
#define McBSP3_MCBSPLP_ROVFLCLR_REG_OFFSET 0x88

#define McBSP3_MCBSPLP_SYSCONFIG_REG (*(volatile unsigned long *)0x4902408C)
#define McBSP3_MCBSPLP_SYSCONFIG_REG_OFFSET 0x8C

#define McBSP3_MCBSPLP_THRSH2_REG (*(volatile unsigned long *)0x49024090)
#define McBSP3_MCBSPLP_THRSH2_REG_OFFSET 0x90

#define McBSP3_MCBSPLP_THRSH1_REG (*(volatile unsigned long *)0x49024094)
#define McBSP3_MCBSPLP_THRSH1_REG_OFFSET 0x94

#define McBSP3_MCBSPLP_IRQSTATUS_REG (*(volatile unsigned long *)0x490240A0)
#define McBSP3_MCBSPLP_IRQSTATUS_REG_OFFSET 0xA0

#define McBSP3_MCBSPLP_IRQENABLE_REG (*(volatile unsigned long *)0x490240A4)
#define McBSP3_MCBSPLP_IRQENABLE_REG_OFFSET 0xA4

#define McBSP3_MCBSPLP_WAKEUPEN_REG (*(volatile unsigned long *)0x490240A8)
#define McBSP3_MCBSPLP_WAKEUPEN_REG_OFFSET 0xA8

#define McBSP3_MCBSPLP_XCCR_REG (*(volatile unsigned long *)0x490240AC)
#define McBSP3_MCBSPLP_XCCR_REG_OFFSET 0xAC

#define McBSP3_MCBSPLP_RCCR_REG (*(volatile unsigned long *)0x490240B0)
#define McBSP3_MCBSPLP_RCCR_REG_OFFSET 0xB0

#define McBSP3_MCBSPLP_XBUFFSTAT_REG (*(volatile unsigned long *)0x490240B4)
#define McBSP3_MCBSPLP_XBUFFSTAT_REG_OFFSET 0xB4

#define McBSP3_MCBSPLP_RBUFFSTAT_REG (*(volatile unsigned long *)0x490240B8)
#define McBSP3_MCBSPLP_RBUFFSTAT_REG_OFFSET 0xB8

#define McBSP3_MCBSPLP_SSELCR_REG (*(volatile unsigned long *)0x490240BC)
#define McBSP3_MCBSPLP_SSELCR_REG_OFFSET 0xBC

#define McBSP3_MCBSPLP_STATUS_REG (*(volatile unsigned long *)0x490240C0)
#define McBSP3_MCBSPLP_STATUS_REG_OFFSET 0xC0

#ifndef SIDETONE_McBSP3_BASE
#define SIDETONE_McBSP3_BASE 0x4902A000
#endif

#define SIDETONE_McBSP3_ST_REV_REG (*(volatile unsigned long *)0x4902A000)
#define SIDETONE_McBSP3_ST_REV_REG_OFFSET 0x0

#define SIDETONE_McBSP3_ST_SYSCONFIG_REG (*(volatile unsigned long *)0x4902A010)
#define SIDETONE_McBSP3_ST_SYSCONFIG_REG_OFFSET 0x10

#define SIDETONE_McBSP3_ST_IRQSTATUS_REG (*(volatile unsigned long *)0x4902A018)
#define SIDETONE_McBSP3_ST_IRQSTATUS_REG_OFFSET 0x18

#define SIDETONE_McBSP3_ST_IRQENABLE_REG (*(volatile unsigned long *)0x4902A01C)
#define SIDETONE_McBSP3_ST_IRQENABLE_REG_OFFSET 0x1C

#define SIDETONE_McBSP3_ST_SGAINCR_REG (*(volatile unsigned long *)0x4902A024)
#define SIDETONE_McBSP3_ST_SGAINCR_REG_OFFSET 0x24

#define SIDETONE_McBSP3_ST_SFIRCR_REG (*(volatile unsigned long *)0x4902A028)
#define SIDETONE_McBSP3_ST_SFIRCR_REG_OFFSET 0x28

#define SIDETONE_McBSP3_ST_SSELCR_REG (*(volatile unsigned long *)0x4902A02C)
#define SIDETONE_McBSP3_ST_SSELCR_REG_OFFSET 0x2C

#ifndef McBSP4_BASE
#define McBSP4_BASE 0x49026000
#endif

#define McBSP4_MCBSPLP_DRR_REG (*(volatile unsigned long *)0x49026000)
#define McBSP4_MCBSPLP_DRR_REG_OFFSET 0x0

#define McBSP4_MCBSPLP_DXR_REG (*(volatile unsigned long *)0x49026008)
#define McBSP4_MCBSPLP_DXR_REG_OFFSET 0x8

#define McBSP4_MCBSPLP_SPCR2_REG (*(volatile unsigned long *)0x49026010)
#define McBSP4_MCBSPLP_SPCR2_REG_OFFSET 0x10

#define McBSP4_MCBSPLP_SPCR1_REG (*(volatile unsigned long *)0x49026014)
#define McBSP4_MCBSPLP_SPCR1_REG_OFFSET 0x14

#define McBSP4_MCBSPLP_RCR2_REG (*(volatile unsigned long *)0x49026018)
#define McBSP4_MCBSPLP_RCR2_REG_OFFSET 0x18

#define McBSP4_MCBSPLP_RCR1_REG (*(volatile unsigned long *)0x4902601C)
#define McBSP4_MCBSPLP_RCR1_REG_OFFSET 0x1C

#define McBSP4_MCBSPLP_XCR2_REG (*(volatile unsigned long *)0x49026020)
#define McBSP4_MCBSPLP_XCR2_REG_OFFSET 0x20

#define McBSP4_MCBSPLP_XCR1_REG (*(volatile unsigned long *)0x49026024)
#define McBSP4_MCBSPLP_XCR1_REG_OFFSET 0x24

#define McBSP4_MCBSPLP_SRGR2_REG (*(volatile unsigned long *)0x49026028)
#define McBSP4_MCBSPLP_SRGR2_REG_OFFSET 0x28

#define McBSP4_MCBSPLP_SRGR1_REG (*(volatile unsigned long *)0x4902602C)
#define McBSP4_MCBSPLP_SRGR1_REG_OFFSET 0x2C

#define McBSP4_MCBSPLP_MCR2_REG (*(volatile unsigned long *)0x49026030)
#define McBSP4_MCBSPLP_MCR2_REG_OFFSET 0x30

#define McBSP4_MCBSPLP_MCR1_REG (*(volatile unsigned long *)0x49026034)
#define McBSP4_MCBSPLP_MCR1_REG_OFFSET 0x34

#define McBSP4_MCBSPLP_RCERA_REG (*(volatile unsigned long *)0x49026038)
#define McBSP4_MCBSPLP_RCERA_REG_OFFSET 0x38

#define McBSP4_MCBSPLP_RCERREG (*(volatile unsigned long *)0x4902603C)
#define McBSP4_MCBSPLP_RCERREG_OFFSET 0x3C

#define McBSP4_MCBSPLP_XCERA_REG (*(volatile unsigned long *)0x49026040)
#define McBSP4_MCBSPLP_XCERA_REG_OFFSET 0x40

#define McBSP4_MCBSPLP_XCERREG (*(volatile unsigned long *)0x49026044)
#define McBSP4_MCBSPLP_XCERREG_OFFSET 0x44

#define McBSP4_MCBSPLP_PCR_REG (*(volatile unsigned long *)0x49026048)
#define McBSP4_MCBSPLP_PCR_REG_OFFSET 0x48

#define McBSP4_MCBSPLP_RCERC_REG (*(volatile unsigned long *)0x4902604C)
#define McBSP4_MCBSPLP_RCERC_REG_OFFSET 0x4C

#define McBSP4_MCBSPLP_RCERD_REG (*(volatile unsigned long *)0x49026050)
#define McBSP4_MCBSPLP_RCERD_REG_OFFSET 0x50

#define McBSP4_MCBSPLP_XCERC_REG (*(volatile unsigned long *)0x49026054)
#define McBSP4_MCBSPLP_XCERC_REG_OFFSET 0x54

#define McBSP4_MCBSPLP_XCERD_REG (*(volatile unsigned long *)0x49026058)
#define McBSP4_MCBSPLP_XCERD_REG_OFFSET 0x58

#define McBSP4_MCBSPLP_RCERE_REG (*(volatile unsigned long *)0x4902605C)
#define McBSP4_MCBSPLP_RCERE_REG_OFFSET 0x5C

#define McBSP4_MCBSPLP_RCERF_REG (*(volatile unsigned long *)0x49026060)
#define McBSP4_MCBSPLP_RCERF_REG_OFFSET 0x60

#define McBSP4_MCBSPLP_XCERE_REG (*(volatile unsigned long *)0x49026064)
#define McBSP4_MCBSPLP_XCERE_REG_OFFSET 0x64

#define McBSP4_MCBSPLP_XCERF_REG (*(volatile unsigned long *)0x49026068)
#define McBSP4_MCBSPLP_XCERF_REG_OFFSET 0x68

#define McBSP4_MCBSPLP_RCERG_REG (*(volatile unsigned long *)0x4902606C)
#define McBSP4_MCBSPLP_RCERG_REG_OFFSET 0x6C

#define McBSP4_MCBSPLP_RCERH_REG (*(volatile unsigned long *)0x49026070)
#define McBSP4_MCBSPLP_RCERH_REG_OFFSET 0x70

#define McBSP4_MCBSPLP_XCERG_REG (*(volatile unsigned long *)0x49026074)
#define McBSP4_MCBSPLP_XCERG_REG_OFFSET 0x74

#define McBSP4_MCBSPLP_XCERH_REG (*(volatile unsigned long *)0x49026078)
#define McBSP4_MCBSPLP_XCERH_REG_OFFSET 0x78

#define McBSP4_MCBSPLP_RINTCLR_REG (*(volatile unsigned long *)0x49026080)
#define McBSP4_MCBSPLP_RINTCLR_REG_OFFSET 0x80

#define McBSP4_MCBSPLP_XINTCLR_REG (*(volatile unsigned long *)0x49026084)
#define McBSP4_MCBSPLP_XINTCLR_REG_OFFSET 0x84

#define McBSP4_MCBSPLP_ROVFLCLR_REG (*(volatile unsigned long *)0x49026088)
#define McBSP4_MCBSPLP_ROVFLCLR_REG_OFFSET 0x88

#define McBSP4_MCBSPLP_SYSCONFIG_REG (*(volatile unsigned long *)0x4902608C)
#define McBSP4_MCBSPLP_SYSCONFIG_REG_OFFSET 0x8C

#define McBSP4_MCBSPLP_THRSH2_REG (*(volatile unsigned long *)0x49026090)
#define McBSP4_MCBSPLP_THRSH2_REG_OFFSET 0x90

#define McBSP4_MCBSPLP_THRSH1_REG (*(volatile unsigned long *)0x49026094)
#define McBSP4_MCBSPLP_THRSH1_REG_OFFSET 0x94

#define McBSP4_MCBSPLP_IRQSTATUS_REG (*(volatile unsigned long *)0x490260A0)
#define McBSP4_MCBSPLP_IRQSTATUS_REG_OFFSET 0xA0

#define McBSP4_MCBSPLP_IRQENABLE_REG (*(volatile unsigned long *)0x490260A4)
#define McBSP4_MCBSPLP_IRQENABLE_REG_OFFSET 0xA4

#define McBSP4_MCBSPLP_WAKEUPEN_REG (*(volatile unsigned long *)0x490260A8)
#define McBSP4_MCBSPLP_WAKEUPEN_REG_OFFSET 0xA8

#define McBSP4_MCBSPLP_XCCR_REG (*(volatile unsigned long *)0x490260AC)
#define McBSP4_MCBSPLP_XCCR_REG_OFFSET 0xAC

#define McBSP4_MCBSPLP_RCCR_REG (*(volatile unsigned long *)0x490260B0)
#define McBSP4_MCBSPLP_RCCR_REG_OFFSET 0xB0

#define McBSP4_MCBSPLP_XBUFFSTAT_REG (*(volatile unsigned long *)0x490260B4)
#define McBSP4_MCBSPLP_XBUFFSTAT_REG_OFFSET 0xB4

#define McBSP4_MCBSPLP_RBUFFSTAT_REG (*(volatile unsigned long *)0x490260B8)
#define McBSP4_MCBSPLP_RBUFFSTAT_REG_OFFSET 0xB8

#define McBSP4_MCBSPLP_SSELCR_REG (*(volatile unsigned long *)0x490260BC)
#define McBSP4_MCBSPLP_SSELCR_REG_OFFSET 0xBC

#define McBSP4_MCBSPLP_STATUS_REG (*(volatile unsigned long *)0x490260C0)
#define McBSP4_MCBSPLP_STATUS_REG_OFFSET 0xC0

#ifndef WDT3_BASE
#define WDT3_BASE 0x49030000
#endif

#define WDT3_WIDR (*(volatile unsigned *)0x49030000)
#define WDT3_WIDR_OFFSET 0x0

#define WDT3_WD_SYSCONFIG (*(volatile unsigned *)0x49030010)
#define WDT3_WD_SYSCONFIG_OFFSET 0x10

#define WDT3_WD_SYSSTATUS (*(volatile unsigned *)0x49030014)
#define WDT3_WD_SYSSTATUS_OFFSET 0x14

#define WDT3_WISR (*(volatile unsigned *)0x49030018)
#define WDT3_WISR_OFFSET 0x18

#define WDT3_WIER (*(volatile unsigned *)0x4903001C)
#define WDT3_WIER_OFFSET 0x1C

#define WDT3_WWER (*(volatile unsigned *)0x49030020)
#define WDT3_WWER_OFFSET 0x20

#define WDT3_WCLR (*(volatile unsigned *)0x49030024)
#define WDT3_WCLR_OFFSET 0x24

#define WDT3_WCRR (*(volatile unsigned *)0x49030028)
#define WDT3_WCRR_OFFSET 0x28

#define WDT3_WLDR (*(volatile unsigned *)0x4903002C)
#define WDT3_WLDR_OFFSET 0x2C

#define WDT3_WTGR (*(volatile unsigned *)0x49030030)
#define WDT3_WTGR_OFFSET 0x30

#define WDT3_WWPS (*(volatile unsigned *)0x49030034)
#define WDT3_WWPS_OFFSET 0x34

#define WDT3_WSPR (*(volatile unsigned *)0x49030048)
#define WDT3_WSPR_OFFSET 0x48

#ifndef GPTIMER2_BASE
#define GPTIMER2_BASE 0x49032000
#endif

#define GPTIMER2_TIDR (*(volatile unsigned *)0x49032000)
#define GPTIMER2_TIDR_OFFSET 0x0

#define GPTIMER2_TIOCP_CFG (*(volatile unsigned long *)0x49032010)
#define GPTIMER2_TIOCP_CFG_OFFSET 0x10

#define GPTIMER2_TISTAT (*(volatile unsigned long *)0x49032014)
#define GPTIMER2_TISTAT_OFFSET 0x14

#define GPTIMER2_TISR (*(volatile unsigned long *)0x49032018)
#define GPTIMER2_TISR_OFFSET 0x18

#define GPTIMER2_TIER (*(volatile unsigned long *)0x4903201C)
#define GPTIMER2_TIER_OFFSET 0x1C

#define GPTIMER2_TWER (*(volatile unsigned long *)0x49032020)
#define GPTIMER2_TWER_OFFSET 0x20

#define GPTIMER2_TCLR (*(volatile unsigned long *)0x49032024)
#define GPTIMER2_TCLR_OFFSET 0x24

#define GPTIMER2_TCRR (*(volatile unsigned long *)0x49032028)
#define GPTIMER2_TCRR_OFFSET 0x28

#define GPTIMER2_TLDR (*(volatile unsigned long *)0x4903202C)
#define GPTIMER2_TLDR_OFFSET 0x2C

#define GPTIMER2_TTGR (*(volatile unsigned long *)0x49032030)
#define GPTIMER2_TTGR_OFFSET 0x30

#define GPTIMER2_TWPS (*(volatile unsigned long *)0x49032034)
#define GPTIMER2_TWPS_OFFSET 0x34

#define GPTIMER2_TMAR (*(volatile unsigned long *)0x49032038)
#define GPTIMER2_TMAR_OFFSET 0x38

#define GPTIMER2_TCAR1 (*(volatile unsigned long *)0x4903203C)
#define GPTIMER2_TCAR1_OFFSET 0x3C

#define GPTIMER2_TSICR (*(volatile unsigned long *)0x49032040)
#define GPTIMER2_TSICR_OFFSET 0x40

#define GPTIMER2_TCAR2 (*(volatile unsigned long *)0x49032044)
#define GPTIMER2_TCAR2_OFFSET 0x44

#define GPTIMER2_TPIR (*(volatile unsigned long *)0x49032048)
#define GPTIMER2_TPIR_OFFSET 0x48

#define GPTIMER2_TNIR (*(volatile unsigned long *)0x4903204C)
#define GPTIMER2_TNIR_OFFSET 0x4C

#define GPTIMER2_TCVR (*(volatile unsigned long *)0x49032050)
#define GPTIMER2_TCVR_OFFSET 0x50

#define GPTIMER2_TOCR (*(volatile unsigned long *)0x49032054)
#define GPTIMER2_TOCR_OFFSET 0x54

#define GPTIMER2_TOWR (*(volatile unsigned long *)0x49032058)
#define GPTIMER2_TOWR_OFFSET 0x58

#ifndef GPTIMER3_BASE
#define GPTIMER3_BASE 0x49034000
#endif

#define GPTIMER3_TIDR (*(volatile unsigned *)0x49034000)
#define GPTIMER3_TIDR_OFFSET 0x0

#define GPTIMER3_TIOCP_CFG (*(volatile unsigned long *)0x49034010)
#define GPTIMER3_TIOCP_CFG_OFFSET 0x10

#define GPTIMER3_TISTAT (*(volatile unsigned long *)0x49034014)
#define GPTIMER3_TISTAT_OFFSET 0x14

#define GPTIMER3_TISR (*(volatile unsigned long *)0x49034018)
#define GPTIMER3_TISR_OFFSET 0x18

#define GPTIMER3_TIER (*(volatile unsigned long *)0x4903401C)
#define GPTIMER3_TIER_OFFSET 0x1C

#define GPTIMER3_TWER (*(volatile unsigned long *)0x49034020)
#define GPTIMER3_TWER_OFFSET 0x20

#define GPTIMER3_TCLR (*(volatile unsigned long *)0x49034024)
#define GPTIMER3_TCLR_OFFSET 0x24

#define GPTIMER3_TCRR (*(volatile unsigned long *)0x49034028)
#define GPTIMER3_TCRR_OFFSET 0x28

#define GPTIMER3_TLDR (*(volatile unsigned long *)0x4903402C)
#define GPTIMER3_TLDR_OFFSET 0x2C

#define GPTIMER3_TTGR (*(volatile unsigned long *)0x49034030)
#define GPTIMER3_TTGR_OFFSET 0x30

#define GPTIMER3_TWPS (*(volatile unsigned long *)0x49034034)
#define GPTIMER3_TWPS_OFFSET 0x34

#define GPTIMER3_TMAR (*(volatile unsigned long *)0x49034038)
#define GPTIMER3_TMAR_OFFSET 0x38

#define GPTIMER3_TCAR1 (*(volatile unsigned long *)0x4903403C)
#define GPTIMER3_TCAR1_OFFSET 0x3C

#define GPTIMER3_TSICR (*(volatile unsigned long *)0x49034040)
#define GPTIMER3_TSICR_OFFSET 0x40

#define GPTIMER3_TCAR2 (*(volatile unsigned long *)0x49034044)
#define GPTIMER3_TCAR2_OFFSET 0x44

#ifndef GPTIMER4_BASE
#define GPTIMER4_BASE 0x49036000
#endif

#define GPTIMER4_TIDR (*(volatile unsigned *)0x49036000)
#define GPTIMER4_TIDR_OFFSET 0x0

#define GPTIMER4_TIOCP_CFG (*(volatile unsigned long *)0x49036010)
#define GPTIMER4_TIOCP_CFG_OFFSET 0x10

#define GPTIMER4_TISTAT (*(volatile unsigned long *)0x49036014)
#define GPTIMER4_TISTAT_OFFSET 0x14

#define GPTIMER4_TISR (*(volatile unsigned long *)0x49036018)
#define GPTIMER4_TISR_OFFSET 0x18

#define GPTIMER4_TIER (*(volatile unsigned long *)0x4903601C)
#define GPTIMER4_TIER_OFFSET 0x1C

#define GPTIMER4_TWER (*(volatile unsigned long *)0x49036020)
#define GPTIMER4_TWER_OFFSET 0x20

#define GPTIMER4_TCLR (*(volatile unsigned long *)0x49036024)
#define GPTIMER4_TCLR_OFFSET 0x24

#define GPTIMER4_TCRR (*(volatile unsigned long *)0x49036028)
#define GPTIMER4_TCRR_OFFSET 0x28

#define GPTIMER4_TLDR (*(volatile unsigned long *)0x4903602C)
#define GPTIMER4_TLDR_OFFSET 0x2C

#define GPTIMER4_TTGR (*(volatile unsigned long *)0x49036030)
#define GPTIMER4_TTGR_OFFSET 0x30

#define GPTIMER4_TWPS (*(volatile unsigned long *)0x49036034)
#define GPTIMER4_TWPS_OFFSET 0x34

#define GPTIMER4_TMAR (*(volatile unsigned long *)0x49036038)
#define GPTIMER4_TMAR_OFFSET 0x38

#define GPTIMER4_TCAR1 (*(volatile unsigned long *)0x4903603C)
#define GPTIMER4_TCAR1_OFFSET 0x3C

#define GPTIMER4_TSICR (*(volatile unsigned long *)0x49036040)
#define GPTIMER4_TSICR_OFFSET 0x40

#define GPTIMER4_TCAR2 (*(volatile unsigned long *)0x49036044)
#define GPTIMER4_TCAR2_OFFSET 0x44

#ifndef GPTIMER5_BASE
#define GPTIMER5_BASE 0x49038000
#endif

#define GPTIMER5_TIDR (*(volatile unsigned *)0x49038000)
#define GPTIMER5_TIDR_OFFSET 0x0

#define GPTIMER5_TIOCP_CFG (*(volatile unsigned long *)0x49038010)
#define GPTIMER5_TIOCP_CFG_OFFSET 0x10

#define GPTIMER5_TISTAT (*(volatile unsigned long *)0x49038014)
#define GPTIMER5_TISTAT_OFFSET 0x14

#define GPTIMER5_TISR (*(volatile unsigned long *)0x49038018)
#define GPTIMER5_TISR_OFFSET 0x18

#define GPTIMER5_TIER (*(volatile unsigned long *)0x4903801C)
#define GPTIMER5_TIER_OFFSET 0x1C

#define GPTIMER5_TWER (*(volatile unsigned long *)0x49038020)
#define GPTIMER5_TWER_OFFSET 0x20

#define GPTIMER5_TCLR (*(volatile unsigned long *)0x49038024)
#define GPTIMER5_TCLR_OFFSET 0x24

#define GPTIMER5_TCRR (*(volatile unsigned long *)0x49038028)
#define GPTIMER5_TCRR_OFFSET 0x28

#define GPTIMER5_TLDR (*(volatile unsigned long *)0x4903802C)
#define GPTIMER5_TLDR_OFFSET 0x2C

#define GPTIMER5_TTGR (*(volatile unsigned long *)0x49038030)
#define GPTIMER5_TTGR_OFFSET 0x30

#define GPTIMER5_TWPS (*(volatile unsigned long *)0x49038034)
#define GPTIMER5_TWPS_OFFSET 0x34

#define GPTIMER5_TMAR (*(volatile unsigned long *)0x49038038)
#define GPTIMER5_TMAR_OFFSET 0x38

#define GPTIMER5_TCAR1 (*(volatile unsigned long *)0x4903803C)
#define GPTIMER5_TCAR1_OFFSET 0x3C

#define GPTIMER5_TSICR (*(volatile unsigned long *)0x49038040)
#define GPTIMER5_TSICR_OFFSET 0x40

#define GPTIMER5_TCAR2 (*(volatile unsigned long *)0x49038044)
#define GPTIMER5_TCAR2_OFFSET 0x44

#ifndef GPTIMER6_BASE
#define GPTIMER6_BASE 0x4903A000
#endif

#define GPTIMER6_TIDR (*(volatile unsigned *)0x4903A000)
#define GPTIMER6_TIDR_OFFSET 0x0

#define GPTIMER6_TIOCP_CFG (*(volatile unsigned long *)0x4903A010)
#define GPTIMER6_TIOCP_CFG_OFFSET 0x10

#define GPTIMER6_TISTAT (*(volatile unsigned long *)0x4903A014)
#define GPTIMER6_TISTAT_OFFSET 0x14

#define GPTIMER6_TISR (*(volatile unsigned long *)0x4903A018)
#define GPTIMER6_TISR_OFFSET 0x18

#define GPTIMER6_TIER (*(volatile unsigned long *)0x4903A01C)
#define GPTIMER6_TIER_OFFSET 0x1C

#define GPTIMER6_TWER (*(volatile unsigned long *)0x4903A020)
#define GPTIMER6_TWER_OFFSET 0x20

#define GPTIMER6_TCLR (*(volatile unsigned long *)0x4903A024)
#define GPTIMER6_TCLR_OFFSET 0x24

#define GPTIMER6_TCRR (*(volatile unsigned long *)0x4903A028)
#define GPTIMER6_TCRR_OFFSET 0x28

#define GPTIMER6_TLDR (*(volatile unsigned long *)0x4903A02C)
#define GPTIMER6_TLDR_OFFSET 0x2C

#define GPTIMER6_TTGR (*(volatile unsigned long *)0x4903A030)
#define GPTIMER6_TTGR_OFFSET 0x30

#define GPTIMER6_TWPS (*(volatile unsigned long *)0x4903A034)
#define GPTIMER6_TWPS_OFFSET 0x34

#define GPTIMER6_TMAR (*(volatile unsigned long *)0x4903A038)
#define GPTIMER6_TMAR_OFFSET 0x38

#define GPTIMER6_TCAR1 (*(volatile unsigned long *)0x4903A03C)
#define GPTIMER6_TCAR1_OFFSET 0x3C

#define GPTIMER6_TSICR (*(volatile unsigned long *)0x4903A040)
#define GPTIMER6_TSICR_OFFSET 0x40

#define GPTIMER6_TCAR2 (*(volatile unsigned long *)0x4903A044)
#define GPTIMER6_TCAR2_OFFSET 0x44

#ifndef GPTIMER7_BASE
#define GPTIMER7_BASE 0x4903C000
#endif

#define GPTIMER7_TIDR (*(volatile unsigned *)0x4903C000)
#define GPTIMER7_TIDR_OFFSET 0x0

#define GPTIMER7_TIOCP_CFG (*(volatile unsigned long *)0x4903C010)
#define GPTIMER7_TIOCP_CFG_OFFSET 0x10

#define GPTIMER7_TISTAT (*(volatile unsigned long *)0x4903C014)
#define GPTIMER7_TISTAT_OFFSET 0x14

#define GPTIMER7_TISR (*(volatile unsigned long *)0x4903C018)
#define GPTIMER7_TISR_OFFSET 0x18

#define GPTIMER7_TIER (*(volatile unsigned long *)0x4903C01C)
#define GPTIMER7_TIER_OFFSET 0x1C

#define GPTIMER7_TWER (*(volatile unsigned long *)0x4903C020)
#define GPTIMER7_TWER_OFFSET 0x20

#define GPTIMER7_TCLR (*(volatile unsigned long *)0x4903C024)
#define GPTIMER7_TCLR_OFFSET 0x24

#define GPTIMER7_TCRR (*(volatile unsigned long *)0x4903C028)
#define GPTIMER7_TCRR_OFFSET 0x28

#define GPTIMER7_TLDR (*(volatile unsigned long *)0x4903C02C)
#define GPTIMER7_TLDR_OFFSET 0x2C

#define GPTIMER7_TTGR (*(volatile unsigned long *)0x4903C030)
#define GPTIMER7_TTGR_OFFSET 0x30

#define GPTIMER7_TWPS (*(volatile unsigned long *)0x4903C034)
#define GPTIMER7_TWPS_OFFSET 0x34

#define GPTIMER7_TMAR (*(volatile unsigned long *)0x4903C038)
#define GPTIMER7_TMAR_OFFSET 0x38

#define GPTIMER7_TCAR1 (*(volatile unsigned long *)0x4903C03C)
#define GPTIMER7_TCAR1_OFFSET 0x3C

#define GPTIMER7_TSICR (*(volatile unsigned long *)0x4903C040)
#define GPTIMER7_TSICR_OFFSET 0x40

#define GPTIMER7_TCAR2 (*(volatile unsigned long *)0x4903C044)
#define GPTIMER7_TCAR2_OFFSET 0x44

#ifndef GPTIMER8_BASE
#define GPTIMER8_BASE 0x4903E000
#endif

#define GPTIMER8_TIDR (*(volatile unsigned *)0x4903E000)
#define GPTIMER8_TIDR_OFFSET 0x0

#define GPTIMER8_TIOCP_CFG (*(volatile unsigned long *)0x4903E010)
#define GPTIMER8_TIOCP_CFG_OFFSET 0x10

#define GPTIMER8_TISTAT (*(volatile unsigned long *)0x4903E014)
#define GPTIMER8_TISTAT_OFFSET 0x14

#define GPTIMER8_TISR (*(volatile unsigned long *)0x4903E018)
#define GPTIMER8_TISR_OFFSET 0x18

#define GPTIMER8_TIER (*(volatile unsigned long *)0x4903E01C)
#define GPTIMER8_TIER_OFFSET 0x1C

#define GPTIMER8_TWER (*(volatile unsigned long *)0x4903E020)
#define GPTIMER8_TWER_OFFSET 0x20

#define GPTIMER8_TCLR (*(volatile unsigned long *)0x4903E024)
#define GPTIMER8_TCLR_OFFSET 0x24

#define GPTIMER8_TCRR (*(volatile unsigned long *)0x4903E028)
#define GPTIMER8_TCRR_OFFSET 0x28

#define GPTIMER8_TLDR (*(volatile unsigned long *)0x4903E02C)
#define GPTIMER8_TLDR_OFFSET 0x2C

#define GPTIMER8_TTGR (*(volatile unsigned long *)0x4903E030)
#define GPTIMER8_TTGR_OFFSET 0x30

#define GPTIMER8_TWPS (*(volatile unsigned long *)0x4903E034)
#define GPTIMER8_TWPS_OFFSET 0x34

#define GPTIMER8_TMAR (*(volatile unsigned long *)0x4903E038)
#define GPTIMER8_TMAR_OFFSET 0x38

#define GPTIMER8_TCAR1 (*(volatile unsigned long *)0x4903E03C)
#define GPTIMER8_TCAR1_OFFSET 0x3C

#define GPTIMER8_TSICR (*(volatile unsigned long *)0x4903E040)
#define GPTIMER8_TSICR_OFFSET 0x40

#define GPTIMER8_TCAR2 (*(volatile unsigned long *)0x4903E044)
#define GPTIMER8_TCAR2_OFFSET 0x44

#ifndef GPTIMER9_BASE
#define GPTIMER9_BASE 0x49040000
#endif

#define GPTIMER9_TIDR (*(volatile unsigned *)0x49040000)
#define GPTIMER9_TIDR_OFFSET 0x0

#define GPTIMER9_TIOCP_CFG (*(volatile unsigned long *)0x49040010)
#define GPTIMER9_TIOCP_CFG_OFFSET 0x10

#define GPTIMER9_TISTAT (*(volatile unsigned long *)0x49040014)
#define GPTIMER9_TISTAT_OFFSET 0x14

#define GPTIMER9_TISR (*(volatile unsigned long *)0x49040018)
#define GPTIMER9_TISR_OFFSET 0x18

#define GPTIMER9_TIER (*(volatile unsigned long *)0x4904001C)
#define GPTIMER9_TIER_OFFSET 0x1C

#define GPTIMER9_TWER (*(volatile unsigned long *)0x49040020)
#define GPTIMER9_TWER_OFFSET 0x20

#define GPTIMER9_TCLR (*(volatile unsigned long *)0x49040024)
#define GPTIMER9_TCLR_OFFSET 0x24

#define GPTIMER9_TCRR (*(volatile unsigned long *)0x49040028)
#define GPTIMER9_TCRR_OFFSET 0x28

#define GPTIMER9_TLDR (*(volatile unsigned long *)0x4904002C)
#define GPTIMER9_TLDR_OFFSET 0x2C

#define GPTIMER9_TTGR (*(volatile unsigned long *)0x49040030)
#define GPTIMER9_TTGR_OFFSET 0x30

#define GPTIMER9_TWPS (*(volatile unsigned long *)0x49040034)
#define GPTIMER9_TWPS_OFFSET 0x34

#define GPTIMER9_TMAR (*(volatile unsigned long *)0x49040038)
#define GPTIMER9_TMAR_OFFSET 0x38

#define GPTIMER9_TCAR1 (*(volatile unsigned long *)0x4904003C)
#define GPTIMER9_TCAR1_OFFSET 0x3C

#define GPTIMER9_TSICR (*(volatile unsigned long *)0x49040040)
#define GPTIMER9_TSICR_OFFSET 0x40

#define GPTIMER9_TCAR2 (*(volatile unsigned long *)0x49040044)
#define GPTIMER9_TCAR2_OFFSET 0x44

#ifndef GPIO2_BASE
#define GPIO2_BASE 0x49050000
#endif

#define GPIO2_REVISION (*(volatile unsigned long *)0x49050000)
#define GPIO2_REVISION_OFFSET 0x0

#define GPIO2_SYSCONFIG (*(volatile unsigned long *)0x49050010)
#define GPIO2_SYSCONFIG_OFFSET 0x10

#define GPIO2_SYSSTATUS (*(volatile unsigned long *)0x49050014)
#define GPIO2_SYSSTATUS_OFFSET 0x14

#define GPIO2_IRQSTATUS1 (*(volatile unsigned long *)0x49050018)
#define GPIO2_IRQSTATUS1_OFFSET 0x18

#define GPIO2_IRQENABLE1 (*(volatile unsigned long *)0x4905001C)
#define GPIO2_IRQENABLE1_OFFSET 0x1C

#define GPIO2_WAKEUPENABLE (*(volatile unsigned long *)0x49050020)
#define GPIO2_WAKEUPENABLE_OFFSET 0x20

#define GPIO2_IRQSTATUS2 (*(volatile unsigned long *)0x49050028)
#define GPIO2_IRQSTATUS2_OFFSET 0x28

#define GPIO2_IRQENABLE2 (*(volatile unsigned long *)0x4905002C)
#define GPIO2_IRQENABLE2_OFFSET 0x2C

#define GPIO2_CTRL (*(volatile unsigned long *)0x49050030)
#define GPIO2_CTRL_OFFSET 0x30

#define GPIO2_OE (*(volatile unsigned long *)0x49050034)
#define GPIO2_OE_OFFSET 0x34

#define GPIO2_DATAIN (*(volatile unsigned long *)0x49050038)
#define GPIO2_DATAIN_OFFSET 0x38

#define GPIO2_DATAOUT (*(volatile unsigned long *)0x4905003C)
#define GPIO2_DATAOUT_OFFSET 0x3C

#define GPIO2_LEVELDETECT0 (*(volatile unsigned long *)0x49050040)
#define GPIO2_LEVELDETECT0_OFFSET 0x40

#define GPIO2_LEVELDETECT1 (*(volatile unsigned long *)0x49050044)
#define GPIO2_LEVELDETECT1_OFFSET 0x44

#define GPIO2_RISINGDETECT (*(volatile unsigned long *)0x49050048)
#define GPIO2_RISINGDETECT_OFFSET 0x48

#define GPIO2_FALLINGDETECT (*(volatile unsigned long *)0x4905004C)
#define GPIO2_FALLINGDETECT_OFFSET 0x4C

#define GPIO2_DEBOUNCENABLE (*(volatile unsigned long *)0x49050050)
#define GPIO2_DEBOUNCENABLE_OFFSET 0x50

#define GPIO2_DEBOUNCINGTIME (*(volatile unsigned long *)0x49050054)
#define GPIO2_DEBOUNCINGTIME_OFFSET 0x54

#define GPIO2_CLEARIRQENABLE1 (*(volatile unsigned long *)0x49050060)
#define GPIO2_CLEARIRQENABLE1_OFFSET 0x60

#define GPIO2_SETIRQENABLE1 (*(volatile unsigned long *)0x49050064)
#define GPIO2_SETIRQENABLE1_OFFSET 0x64

#define GPIO2_CLEARIRQENABLE2 (*(volatile unsigned long *)0x49050070)
#define GPIO2_CLEARIRQENABLE2_OFFSET 0x70

#define GPIO2_SETIRQENABLE2 (*(volatile unsigned long *)0x49050074)
#define GPIO2_SETIRQENABLE2_OFFSET 0x74

#define GPIO2_CLEARWKUENA (*(volatile unsigned long *)0x49050080)
#define GPIO2_CLEARWKUENA_OFFSET 0x80

#define GPIO2_SETWKUENA (*(volatile unsigned long *)0x49050084)
#define GPIO2_SETWKUENA_OFFSET 0x84

#define GPIO2_CLEARDATAOUT (*(volatile unsigned long *)0x49050090)
#define GPIO2_CLEARDATAOUT_OFFSET 0x90

#define GPIO2_SETDATAOUT (*(volatile unsigned long *)0x49050094)
#define GPIO2_SETDATAOUT_OFFSET 0x94

#ifndef GPIO3_BASE
#define GPIO3_BASE 0x49052000
#endif

#define GPIO3_REVISION (*(volatile unsigned long *)0x49052000)
#define GPIO3_REVISION_OFFSET 0x0

#define GPIO3_SYSCONFIG (*(volatile unsigned long *)0x49052010)
#define GPIO3_SYSCONFIG_OFFSET 0x10

#define GPIO3_SYSSTATUS (*(volatile unsigned long *)0x49052014)
#define GPIO3_SYSSTATUS_OFFSET 0x14

#define GPIO3_IRQSTATUS1 (*(volatile unsigned long *)0x49052018)
#define GPIO3_IRQSTATUS1_OFFSET 0x18

#define GPIO3_IRQENABLE1 (*(volatile unsigned long *)0x4905201C)
#define GPIO3_IRQENABLE1_OFFSET 0x1C

#define GPIO3_WAKEUPENABLE (*(volatile unsigned long *)0x49052020)
#define GPIO3_WAKEUPENABLE_OFFSET 0x20

#define GPIO3_IRQSTATUS2 (*(volatile unsigned long *)0x49052028)
#define GPIO3_IRQSTATUS2_OFFSET 0x28

#define GPIO3_IRQENABLE2 (*(volatile unsigned long *)0x4905202C)
#define GPIO3_IRQENABLE2_OFFSET 0x2C

#define GPIO3_CTRL (*(volatile unsigned long *)0x49052030)
#define GPIO3_CTRL_OFFSET 0x30

#define GPIO3_OE (*(volatile unsigned long *)0x49052034)
#define GPIO3_OE_OFFSET 0x34

#define GPIO3_DATAIN (*(volatile unsigned long *)0x49052038)
#define GPIO3_DATAIN_OFFSET 0x38

#define GPIO3_DATAOUT (*(volatile unsigned long *)0x4905203C)
#define GPIO3_DATAOUT_OFFSET 0x3C

#define GPIO3_LEVELDETECT0 (*(volatile unsigned long *)0x49052040)
#define GPIO3_LEVELDETECT0_OFFSET 0x40

#define GPIO3_LEVELDETECT1 (*(volatile unsigned long *)0x49052044)
#define GPIO3_LEVELDETECT1_OFFSET 0x44

#define GPIO3_RISINGDETECT (*(volatile unsigned long *)0x49052048)
#define GPIO3_RISINGDETECT_OFFSET 0x48

#define GPIO3_FALLINGDETECT (*(volatile unsigned long *)0x4905204C)
#define GPIO3_FALLINGDETECT_OFFSET 0x4C

#define GPIO3_DEBOUNCENABLE (*(volatile unsigned long *)0x49052050)
#define GPIO3_DEBOUNCENABLE_OFFSET 0x50

#define GPIO3_DEBOUNCINGTIME (*(volatile unsigned long *)0x49052054)
#define GPIO3_DEBOUNCINGTIME_OFFSET 0x54

#define GPIO3_CLEARIRQENABLE1 (*(volatile unsigned long *)0x49052060)
#define GPIO3_CLEARIRQENABLE1_OFFSET 0x60

#define GPIO3_SETIRQENABLE1 (*(volatile unsigned long *)0x49052064)
#define GPIO3_SETIRQENABLE1_OFFSET 0x64

#define GPIO3_CLEARIRQENABLE2 (*(volatile unsigned long *)0x49052070)
#define GPIO3_CLEARIRQENABLE2_OFFSET 0x70

#define GPIO3_SETIRQENABLE2 (*(volatile unsigned long *)0x49052074)
#define GPIO3_SETIRQENABLE2_OFFSET 0x74

#define GPIO3_CLEARWKUENA (*(volatile unsigned long *)0x49052080)
#define GPIO3_CLEARWKUENA_OFFSET 0x80

#define GPIO3_SETWKUENA (*(volatile unsigned long *)0x49052084)
#define GPIO3_SETWKUENA_OFFSET 0x84

#define GPIO3_CLEARDATAOUT (*(volatile unsigned long *)0x49052090)
#define GPIO3_CLEARDATAOUT_OFFSET 0x90

#define GPIO3_SETDATAOUT (*(volatile unsigned long *)0x49052094)
#define GPIO3_SETDATAOUT_OFFSET 0x94

#ifndef GPIO4_BASE
#define GPIO4_BASE 0x49054000
#endif

#define GPIO4_REVISION (*(volatile unsigned long *)0x49054000)
#define GPIO4_REVISION_OFFSET 0x0

#define GPIO4_SYSCONFIG (*(volatile unsigned long *)0x49054010)
#define GPIO4_SYSCONFIG_OFFSET 0x10

#define GPIO4_SYSSTATUS (*(volatile unsigned long *)0x49054014)
#define GPIO4_SYSSTATUS_OFFSET 0x14

#define GPIO4_IRQSTATUS1 (*(volatile unsigned long *)0x49054018)
#define GPIO4_IRQSTATUS1_OFFSET 0x18

#define GPIO4_IRQENABLE1 (*(volatile unsigned long *)0x4905401C)
#define GPIO4_IRQENABLE1_OFFSET 0x1C

#define GPIO4_WAKEUPENABLE (*(volatile unsigned long *)0x49054020)
#define GPIO4_WAKEUPENABLE_OFFSET 0x20

#define GPIO4_IRQSTATUS2 (*(volatile unsigned long *)0x49054028)
#define GPIO4_IRQSTATUS2_OFFSET 0x28

#define GPIO4_IRQENABLE2 (*(volatile unsigned long *)0x4905402C)
#define GPIO4_IRQENABLE2_OFFSET 0x2C

#define GPIO4_CTRL (*(volatile unsigned long *)0x49054030)
#define GPIO4_CTRL_OFFSET 0x30

#define GPIO4_OE (*(volatile unsigned long *)0x49054034)
#define GPIO4_OE_OFFSET 0x34

#define GPIO4_DATAIN (*(volatile unsigned long *)0x49054038)
#define GPIO4_DATAIN_OFFSET 0x38

#define GPIO4_DATAOUT (*(volatile unsigned long *)0x4905403C)
#define GPIO4_DATAOUT_OFFSET 0x3C

#define GPIO4_LEVELDETECT0 (*(volatile unsigned long *)0x49054040)
#define GPIO4_LEVELDETECT0_OFFSET 0x40

#define GPIO4_LEVELDETECT1 (*(volatile unsigned long *)0x49054044)
#define GPIO4_LEVELDETECT1_OFFSET 0x44

#define GPIO4_RISINGDETECT (*(volatile unsigned long *)0x49054048)
#define GPIO4_RISINGDETECT_OFFSET 0x48

#define GPIO4_FALLINGDETECT (*(volatile unsigned long *)0x4905404C)
#define GPIO4_FALLINGDETECT_OFFSET 0x4C

#define GPIO4_DEBOUNCENABLE (*(volatile unsigned long *)0x49054050)
#define GPIO4_DEBOUNCENABLE_OFFSET 0x50

#define GPIO4_DEBOUNCINGTIME (*(volatile unsigned long *)0x49054054)
#define GPIO4_DEBOUNCINGTIME_OFFSET 0x54

#define GPIO4_CLEARIRQENABLE1 (*(volatile unsigned long *)0x49054060)
#define GPIO4_CLEARIRQENABLE1_OFFSET 0x60

#define GPIO4_SETIRQENABLE1 (*(volatile unsigned long *)0x49054064)
#define GPIO4_SETIRQENABLE1_OFFSET 0x64

#define GPIO4_CLEARIRQENABLE2 (*(volatile unsigned long *)0x49054070)
#define GPIO4_CLEARIRQENABLE2_OFFSET 0x70

#define GPIO4_SETIRQENABLE2 (*(volatile unsigned long *)0x49054074)
#define GPIO4_SETIRQENABLE2_OFFSET 0x74

#define GPIO4_CLEARWKUENA (*(volatile unsigned long *)0x49054080)
#define GPIO4_CLEARWKUENA_OFFSET 0x80

#define GPIO4_SETWKUENA (*(volatile unsigned long *)0x49054084)
#define GPIO4_SETWKUENA_OFFSET 0x84

#define GPIO4_CLEARDATAOUT (*(volatile unsigned long *)0x49054090)
#define GPIO4_CLEARDATAOUT_OFFSET 0x90

#define GPIO4_SETDATAOUT (*(volatile unsigned long *)0x49054094)
#define GPIO4_SETDATAOUT_OFFSET 0x94

#ifndef GPIO5_BASE
#define GPIO5_BASE 0x49056000
#endif

#define GPIO5_REVISION (*(volatile unsigned long *)0x49056000)
#define GPIO5_REVISION_OFFSET 0x0

#define GPIO5_SYSCONFIG (*(volatile unsigned long *)0x49056010)
#define GPIO5_SYSCONFIG_OFFSET 0x10

#define GPIO5_SYSSTATUS (*(volatile unsigned long *)0x49056014)
#define GPIO5_SYSSTATUS_OFFSET 0x14

#define GPIO5_IRQSTATUS1 (*(volatile unsigned long *)0x49056018)
#define GPIO5_IRQSTATUS1_OFFSET 0x18

#define GPIO5_IRQENABLE1 (*(volatile unsigned long *)0x4905601C)
#define GPIO5_IRQENABLE1_OFFSET 0x1C

#define GPIO5_WAKEUPENABLE (*(volatile unsigned long *)0x49056020)
#define GPIO5_WAKEUPENABLE_OFFSET 0x20

#define GPIO5_IRQSTATUS2 (*(volatile unsigned long *)0x49056028)
#define GPIO5_IRQSTATUS2_OFFSET 0x28

#define GPIO5_IRQENABLE2 (*(volatile unsigned long *)0x4905602C)
#define GPIO5_IRQENABLE2_OFFSET 0x2C

#define GPIO5_CTRL (*(volatile unsigned long *)0x49056030)
#define GPIO5_CTRL_OFFSET 0x30

#define GPIO5_OE (*(volatile unsigned long *)0x49056034)
#define GPIO5_OE_OFFSET 0x34

#define GPIO5_DATAIN (*(volatile unsigned long *)0x49056038)
#define GPIO5_DATAIN_OFFSET 0x38

#define GPIO5_DATAOUT (*(volatile unsigned long *)0x4905603C)
#define GPIO5_DATAOUT_OFFSET 0x3C

#define GPIO5_LEVELDETECT0 (*(volatile unsigned long *)0x49056040)
#define GPIO5_LEVELDETECT0_OFFSET 0x40

#define GPIO5_LEVELDETECT1 (*(volatile unsigned long *)0x49056044)
#define GPIO5_LEVELDETECT1_OFFSET 0x44

#define GPIO5_RISINGDETECT (*(volatile unsigned long *)0x49056048)
#define GPIO5_RISINGDETECT_OFFSET 0x48

#define GPIO5_FALLINGDETECT (*(volatile unsigned long *)0x4905604C)
#define GPIO5_FALLINGDETECT_OFFSET 0x4C

#define GPIO5_DEBOUNCENABLE (*(volatile unsigned long *)0x49056050)
#define GPIO5_DEBOUNCENABLE_OFFSET 0x50

#define GPIO5_DEBOUNCINGTIME (*(volatile unsigned long *)0x49056054)
#define GPIO5_DEBOUNCINGTIME_OFFSET 0x54

#define GPIO5_CLEARIRQENABLE1 (*(volatile unsigned long *)0x49056060)
#define GPIO5_CLEARIRQENABLE1_OFFSET 0x60

#define GPIO5_SETIRQENABLE1 (*(volatile unsigned long *)0x49056064)
#define GPIO5_SETIRQENABLE1_OFFSET 0x64

#define GPIO5_CLEARIRQENABLE2 (*(volatile unsigned long *)0x49056070)
#define GPIO5_CLEARIRQENABLE2_OFFSET 0x70

#define GPIO5_SETIRQENABLE2 (*(volatile unsigned long *)0x49056074)
#define GPIO5_SETIRQENABLE2_OFFSET 0x74

#define GPIO5_CLEARWKUENA (*(volatile unsigned long *)0x49056080)
#define GPIO5_CLEARWKUENA_OFFSET 0x80

#define GPIO5_SETWKUENA (*(volatile unsigned long *)0x49056084)
#define GPIO5_SETWKUENA_OFFSET 0x84

#define GPIO5_CLEARDATAOUT (*(volatile unsigned long *)0x49056090)
#define GPIO5_CLEARDATAOUT_OFFSET 0x90

#define GPIO5_SETDATAOUT (*(volatile unsigned long *)0x49056094)
#define GPIO5_SETDATAOUT_OFFSET 0x94

#ifndef GPIO6_BASE
#define GPIO6_BASE 0x49058000
#endif

#define GPIO6_REVISION (*(volatile unsigned long *)0x49058000)
#define GPIO6_REVISION_OFFSET 0x0

#define GPIO6_SYSCONFIG (*(volatile unsigned long *)0x49058010)
#define GPIO6_SYSCONFIG_OFFSET 0x10

#define GPIO6_SYSSTATUS (*(volatile unsigned long *)0x49058014)
#define GPIO6_SYSSTATUS_OFFSET 0x14

#define GPIO6_IRQSTATUS1 (*(volatile unsigned long *)0x49058018)
#define GPIO6_IRQSTATUS1_OFFSET 0x18

#define GPIO6_IRQENABLE1 (*(volatile unsigned long *)0x4905801C)
#define GPIO6_IRQENABLE1_OFFSET 0x1C

#define GPIO6_WAKEUPENABLE (*(volatile unsigned long *)0x49058020)
#define GPIO6_WAKEUPENABLE_OFFSET 0x20

#define GPIO6_IRQSTATUS2 (*(volatile unsigned long *)0x49058028)
#define GPIO6_IRQSTATUS2_OFFSET 0x28

#define GPIO6_IRQENABLE2 (*(volatile unsigned long *)0x4905802C)
#define GPIO6_IRQENABLE2_OFFSET 0x2C

#define GPIO6_CTRL (*(volatile unsigned long *)0x49058030)
#define GPIO6_CTRL_OFFSET 0x30

#define GPIO6_OE (*(volatile unsigned long *)0x49058034)
#define GPIO6_OE_OFFSET 0x34

#define GPIO6_DATAIN (*(volatile unsigned long *)0x49058038)
#define GPIO6_DATAIN_OFFSET 0x38

#define GPIO6_DATAOUT (*(volatile unsigned long *)0x4905803C)
#define GPIO6_DATAOUT_OFFSET 0x3C

#define GPIO6_LEVELDETECT0 (*(volatile unsigned long *)0x49058040)
#define GPIO6_LEVELDETECT0_OFFSET 0x40

#define GPIO6_LEVELDETECT1 (*(volatile unsigned long *)0x49058044)
#define GPIO6_LEVELDETECT1_OFFSET 0x44

#define GPIO6_RISINGDETECT (*(volatile unsigned long *)0x49058048)
#define GPIO6_RISINGDETECT_OFFSET 0x48

#define GPIO6_FALLINGDETECT (*(volatile unsigned long *)0x4905804C)
#define GPIO6_FALLINGDETECT_OFFSET 0x4C

#define GPIO6_DEBOUNCENABLE (*(volatile unsigned long *)0x49058050)
#define GPIO6_DEBOUNCENABLE_OFFSET 0x50

#define GPIO6_DEBOUNCINGTIME (*(volatile unsigned long *)0x49058054)
#define GPIO6_DEBOUNCINGTIME_OFFSET 0x54

#define GPIO6_CLEARIRQENABLE1 (*(volatile unsigned long *)0x49058060)
#define GPIO6_CLEARIRQENABLE1_OFFSET 0x60

#define GPIO6_SETIRQENABLE1 (*(volatile unsigned long *)0x49058064)
#define GPIO6_SETIRQENABLE1_OFFSET 0x64

#define GPIO6_CLEARIRQENABLE2 (*(volatile unsigned long *)0x49058070)
#define GPIO6_CLEARIRQENABLE2_OFFSET 0x70

#define GPIO6_SETIRQENABLE2 (*(volatile unsigned long *)0x49058074)
#define GPIO6_SETIRQENABLE2_OFFSET 0x74

#define GPIO6_CLEARWKUENA (*(volatile unsigned long *)0x49058080)
#define GPIO6_CLEARWKUENA_OFFSET 0x80

#define GPIO6_SETWKUENA (*(volatile unsigned long *)0x49058084)
#define GPIO6_SETWKUENA_OFFSET 0x84

#define GPIO6_CLEARDATAOUT (*(volatile unsigned long *)0x49058090)
#define GPIO6_CLEARDATAOUT_OFFSET 0x90

#define GPIO6_SETDATAOUT (*(volatile unsigned long *)0x49058094)
#define GPIO6_SETDATAOUT_OFFSET 0x94

#endif
