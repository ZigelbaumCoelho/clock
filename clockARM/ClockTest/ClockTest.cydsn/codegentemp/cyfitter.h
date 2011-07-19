#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include <cydevice.h>
#include <cydevice_trm.h>

/* Counter_1_CounterHW */
#define Counter_1_CounterHW__CAP0 CYREG_TMR0_CAP0
#define Counter_1_CounterHW__CAP1 CYREG_TMR0_CAP1
#define Counter_1_CounterHW__CFG0 CYREG_TMR0_CFG0
#define Counter_1_CounterHW__CFG1 CYREG_TMR0_CFG1
#define Counter_1_CounterHW__CFG2 CYREG_TMR0_CFG2
#define Counter_1_CounterHW__CNT_CMP0 CYREG_TMR0_CNT_CMP0
#define Counter_1_CounterHW__CNT_CMP1 CYREG_TMR0_CNT_CMP1
#define Counter_1_CounterHW__PER0 CYREG_TMR0_PER0
#define Counter_1_CounterHW__PER1 CYREG_TMR0_PER1
#define Counter_1_CounterHW__PM_ACT_CFG CYREG_PM_ACT_CFG3
#define Counter_1_CounterHW__PM_ACT_MSK 0x01u
#define Counter_1_CounterHW__PM_STBY_CFG CYREG_PM_STBY_CFG3
#define Counter_1_CounterHW__PM_STBY_MSK 0x01u
#define Counter_1_CounterHW__RT0 CYREG_TMR0_RT0
#define Counter_1_CounterHW__RT1 CYREG_TMR0_RT1
#define Counter_1_CounterHW__SR0 CYREG_TMR0_SR0

/* Clock_1 */
#define Clock_1__CFG0 CYREG_CLKDIST_DCFG0_CFG0
#define Clock_1__CFG1 CYREG_CLKDIST_DCFG0_CFG1
#define Clock_1__CFG2 CYREG_CLKDIST_DCFG0_CFG2
#define Clock_1__CFG2_SRC_SEL_MASK 0x07u
#define Clock_1__PM_ACT_CFG CYREG_PM_ACT_CFG2
#define Clock_1__PM_ACT_MSK 0x01u
#define Clock_1__PM_STBY_CFG CYREG_PM_STBY_CFG2
#define Clock_1__PM_STBY_MSK 0x01u

/* Clock_2 */
#define Clock_2__CFG0 CYREG_CLKDIST_DCFG1_CFG0
#define Clock_2__CFG1 CYREG_CLKDIST_DCFG1_CFG1
#define Clock_2__CFG2 CYREG_CLKDIST_DCFG1_CFG2
#define Clock_2__CFG2_SRC_SEL_MASK 0x07u
#define Clock_2__PM_ACT_CFG CYREG_PM_ACT_CFG2
#define Clock_2__PM_ACT_MSK 0x02u
#define Clock_2__PM_STBY_CFG CYREG_PM_STBY_CFG2
#define Clock_2__PM_STBY_MSK 0x02u

/* MAGNET */
#define MAGNET__0__MASK 0x80u
#define MAGNET__0__PC CYREG_PRT1_PC7
#define MAGNET__0__PORT 1
#define MAGNET__0__SHIFT 7
#define MAGNET__AG CYREG_PRT1_AG
#define MAGNET__AMUX CYREG_PRT1_AMUX
#define MAGNET__BIE CYREG_PRT1_BIE
#define MAGNET__BIT_MASK CYREG_PRT1_BIT_MASK
#define MAGNET__BYP CYREG_PRT1_BYP
#define MAGNET__CTL CYREG_PRT1_CTL
#define MAGNET__DM0 CYREG_PRT1_DM0
#define MAGNET__DM1 CYREG_PRT1_DM1
#define MAGNET__DM2 CYREG_PRT1_DM2
#define MAGNET__DR CYREG_PRT1_DR
#define MAGNET__INP_DIS CYREG_PRT1_INP_DIS
#define MAGNET__LCD_COM_SEG CYREG_PRT1_LCD_COM_SEG
#define MAGNET__LCD_EN CYREG_PRT1_LCD_EN
#define MAGNET__MASK 0x80u
#define MAGNET__PORT 1
#define MAGNET__PRT CYREG_PRT1_PRT
#define MAGNET__PRTDSI__CAPS_SEL CYREG_PRT1_CAPS_SEL
#define MAGNET__PRTDSI__DBL_SYNC_IN CYREG_PRT1_DBL_SYNC_IN
#define MAGNET__PRTDSI__OE_SEL0 CYREG_PRT1_OE_SEL0
#define MAGNET__PRTDSI__OE_SEL1 CYREG_PRT1_OE_SEL1
#define MAGNET__PRTDSI__OUT_SEL0 CYREG_PRT1_OUT_SEL0
#define MAGNET__PRTDSI__OUT_SEL1 CYREG_PRT1_OUT_SEL1
#define MAGNET__PRTDSI__SYNC_OUT CYREG_PRT1_SYNC_OUT
#define MAGNET__PS CYREG_PRT1_PS
#define MAGNET__SHIFT 7
#define MAGNET__SLW CYREG_PRT1_SLW

/* PRISM */
#define PRISM_sC16_PrISMdp_u0__16BIT_A0_REG CYREG_B1_UDB08_09_A0
#define PRISM_sC16_PrISMdp_u0__16BIT_A1_REG CYREG_B1_UDB08_09_A1
#define PRISM_sC16_PrISMdp_u0__16BIT_D0_REG CYREG_B1_UDB08_09_D0
#define PRISM_sC16_PrISMdp_u0__16BIT_D1_REG CYREG_B1_UDB08_09_D1
#define PRISM_sC16_PrISMdp_u0__16BIT_DP_AUX_CTL_REG CYREG_B1_UDB08_09_ACTL
#define PRISM_sC16_PrISMdp_u0__16BIT_F0_REG CYREG_B1_UDB08_09_F0
#define PRISM_sC16_PrISMdp_u0__16BIT_F1_REG CYREG_B1_UDB08_09_F1
#define PRISM_sC16_PrISMdp_u0__A0_A1_REG CYREG_B1_UDB08_A0_A1
#define PRISM_sC16_PrISMdp_u0__A0_REG CYREG_B1_UDB08_A0
#define PRISM_sC16_PrISMdp_u0__A1_REG CYREG_B1_UDB08_A1
#define PRISM_sC16_PrISMdp_u0__D0_D1_REG CYREG_B1_UDB08_D0_D1
#define PRISM_sC16_PrISMdp_u0__D0_REG CYREG_B1_UDB08_D0
#define PRISM_sC16_PrISMdp_u0__D1_REG CYREG_B1_UDB08_D1
#define PRISM_sC16_PrISMdp_u0__DP_AUX_CTL_REG CYREG_B1_UDB08_ACTL
#define PRISM_sC16_PrISMdp_u0__F0_F1_REG CYREG_B1_UDB08_F0_F1
#define PRISM_sC16_PrISMdp_u0__F0_REG CYREG_B1_UDB08_F0
#define PRISM_sC16_PrISMdp_u0__F1_REG CYREG_B1_UDB08_F1
#define PRISM_sC16_PrISMdp_u1__16BIT_A0_REG CYREG_B1_UDB09_10_A0
#define PRISM_sC16_PrISMdp_u1__16BIT_A1_REG CYREG_B1_UDB09_10_A1
#define PRISM_sC16_PrISMdp_u1__16BIT_D0_REG CYREG_B1_UDB09_10_D0
#define PRISM_sC16_PrISMdp_u1__16BIT_D1_REG CYREG_B1_UDB09_10_D1
#define PRISM_sC16_PrISMdp_u1__16BIT_DP_AUX_CTL_REG CYREG_B1_UDB09_10_ACTL
#define PRISM_sC16_PrISMdp_u1__16BIT_F0_REG CYREG_B1_UDB09_10_F0
#define PRISM_sC16_PrISMdp_u1__16BIT_F1_REG CYREG_B1_UDB09_10_F1
#define PRISM_sC16_PrISMdp_u1__A0_A1_REG CYREG_B1_UDB09_A0_A1
#define PRISM_sC16_PrISMdp_u1__A0_REG CYREG_B1_UDB09_A0
#define PRISM_sC16_PrISMdp_u1__A1_REG CYREG_B1_UDB09_A1
#define PRISM_sC16_PrISMdp_u1__D0_D1_REG CYREG_B1_UDB09_D0_D1
#define PRISM_sC16_PrISMdp_u1__D0_REG CYREG_B1_UDB09_D0
#define PRISM_sC16_PrISMdp_u1__D1_REG CYREG_B1_UDB09_D1
#define PRISM_sC16_PrISMdp_u1__DP_AUX_CTL_REG CYREG_B1_UDB09_ACTL
#define PRISM_sC16_PrISMdp_u1__F0_F1_REG CYREG_B1_UDB09_F0_F1
#define PRISM_sC16_PrISMdp_u1__F0_REG CYREG_B1_UDB09_F0
#define PRISM_sC16_PrISMdp_u1__F1_REG CYREG_B1_UDB09_F1
#define PRISM_sC16_PrISMdp_u1__MSK_DP_AUX_CTL_REG CYREG_B1_UDB09_MSK_ACTL
#define PRISM_sC16_PrISMdp_u1__PER_DP_AUX_CTL_REG CYREG_B1_UDB09_MSK_ACTL
#define PRISM_SyncCtl_ControlReg__0__MASK 0x01u
#define PRISM_SyncCtl_ControlReg__0__POS 0
#define PRISM_SyncCtl_ControlReg__1__MASK 0x02u
#define PRISM_SyncCtl_ControlReg__1__POS 1
#define PRISM_SyncCtl_ControlReg__16BIT_CONTROL_AUX_CTL_REG CYREG_B1_UDB09_10_ACTL
#define PRISM_SyncCtl_ControlReg__16BIT_CONTROL_CONTROL_REG CYREG_B1_UDB09_10_CTL
#define PRISM_SyncCtl_ControlReg__16BIT_CONTROL_COUNT_REG CYREG_B1_UDB09_10_CTL
#define PRISM_SyncCtl_ControlReg__16BIT_COUNT_CONTROL_REG CYREG_B1_UDB09_10_CTL
#define PRISM_SyncCtl_ControlReg__16BIT_COUNT_COUNT_REG CYREG_B1_UDB09_10_CTL
#define PRISM_SyncCtl_ControlReg__16BIT_MASK_MASK_REG CYREG_B1_UDB09_10_MSK
#define PRISM_SyncCtl_ControlReg__16BIT_MASK_PERIOD_REG CYREG_B1_UDB09_10_MSK
#define PRISM_SyncCtl_ControlReg__16BIT_PERIOD_MASK_REG CYREG_B1_UDB09_10_MSK
#define PRISM_SyncCtl_ControlReg__16BIT_PERIOD_PERIOD_REG CYREG_B1_UDB09_10_MSK
#define PRISM_SyncCtl_ControlReg__CONTROL_AUX_CTL_REG CYREG_B1_UDB09_ACTL
#define PRISM_SyncCtl_ControlReg__CONTROL_REG CYREG_B1_UDB09_CTL
#define PRISM_SyncCtl_ControlReg__CONTROL_ST_REG CYREG_B1_UDB09_ST_CTL
#define PRISM_SyncCtl_ControlReg__COUNT_REG CYREG_B1_UDB09_CTL
#define PRISM_SyncCtl_ControlReg__COUNT_ST_REG CYREG_B1_UDB09_ST_CTL
#define PRISM_SyncCtl_ControlReg__MASK 0x03u
#define PRISM_SyncCtl_ControlReg__MASK_CTL_AUX_CTL_REG CYREG_B1_UDB09_MSK_ACTL
#define PRISM_SyncCtl_ControlReg__PER_CTL_AUX_CTL_REG CYREG_B1_UDB09_MSK_ACTL
#define PRISM_SyncCtl_ControlReg__PERIOD_REG CYREG_B1_UDB09_MSK

/* DATA */
#define DATA__0__MASK 0x08u
#define DATA__0__PC CYREG_PRT5_PC3
#define DATA__0__PORT 5
#define DATA__0__SHIFT 3
#define DATA__AG CYREG_PRT5_AG
#define DATA__AMUX CYREG_PRT5_AMUX
#define DATA__BIE CYREG_PRT5_BIE
#define DATA__BIT_MASK CYREG_PRT5_BIT_MASK
#define DATA__BYP CYREG_PRT5_BYP
#define DATA__CTL CYREG_PRT5_CTL
#define DATA__DM0 CYREG_PRT5_DM0
#define DATA__DM1 CYREG_PRT5_DM1
#define DATA__DM2 CYREG_PRT5_DM2
#define DATA__DR CYREG_PRT5_DR
#define DATA__INP_DIS CYREG_PRT5_INP_DIS
#define DATA__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define DATA__LCD_EN CYREG_PRT5_LCD_EN
#define DATA__MASK 0x08u
#define DATA__PORT 5
#define DATA__PRT CYREG_PRT5_PRT
#define DATA__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define DATA__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define DATA__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define DATA__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define DATA__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define DATA__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define DATA__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define DATA__PS CYREG_PRT5_PS
#define DATA__SHIFT 3
#define DATA__SLW CYREG_PRT5_SLW

/* SCLK */
#define SCLK__0__MASK 0x04u
#define SCLK__0__PC CYREG_PRT5_PC2
#define SCLK__0__PORT 5
#define SCLK__0__SHIFT 2
#define SCLK__AG CYREG_PRT5_AG
#define SCLK__AMUX CYREG_PRT5_AMUX
#define SCLK__BIE CYREG_PRT5_BIE
#define SCLK__BIT_MASK CYREG_PRT5_BIT_MASK
#define SCLK__BYP CYREG_PRT5_BYP
#define SCLK__CTL CYREG_PRT5_CTL
#define SCLK__DM0 CYREG_PRT5_DM0
#define SCLK__DM1 CYREG_PRT5_DM1
#define SCLK__DM2 CYREG_PRT5_DM2
#define SCLK__DR CYREG_PRT5_DR
#define SCLK__INP_DIS CYREG_PRT5_INP_DIS
#define SCLK__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define SCLK__LCD_EN CYREG_PRT5_LCD_EN
#define SCLK__MASK 0x04u
#define SCLK__PORT 5
#define SCLK__PRT CYREG_PRT5_PRT
#define SCLK__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define SCLK__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define SCLK__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define SCLK__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define SCLK__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define SCLK__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define SCLK__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define SCLK__PS CYREG_PRT5_PS
#define SCLK__SHIFT 2
#define SCLK__SLW CYREG_PRT5_SLW

/* LE */
#define LE__0__MASK 0x02u
#define LE__0__PC CYREG_PRT5_PC1
#define LE__0__PORT 5
#define LE__0__SHIFT 1
#define LE__AG CYREG_PRT5_AG
#define LE__AMUX CYREG_PRT5_AMUX
#define LE__BIE CYREG_PRT5_BIE
#define LE__BIT_MASK CYREG_PRT5_BIT_MASK
#define LE__BYP CYREG_PRT5_BYP
#define LE__CTL CYREG_PRT5_CTL
#define LE__DM0 CYREG_PRT5_DM0
#define LE__DM1 CYREG_PRT5_DM1
#define LE__DM2 CYREG_PRT5_DM2
#define LE__DR CYREG_PRT5_DR
#define LE__INP_DIS CYREG_PRT5_INP_DIS
#define LE__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define LE__LCD_EN CYREG_PRT5_LCD_EN
#define LE__MASK 0x02u
#define LE__PORT 5
#define LE__PRT CYREG_PRT5_PRT
#define LE__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define LE__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define LE__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define LE__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define LE__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define LE__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define LE__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define LE__PS CYREG_PRT5_PS
#define LE__SHIFT 1
#define LE__SLW CYREG_PRT5_SLW

/* OE */
#define OE__0__MASK 0x01u
#define OE__0__PC CYREG_PRT5_PC0
#define OE__0__PORT 5
#define OE__0__SHIFT 0
#define OE__AG CYREG_PRT5_AG
#define OE__AMUX CYREG_PRT5_AMUX
#define OE__BIE CYREG_PRT5_BIE
#define OE__BIT_MASK CYREG_PRT5_BIT_MASK
#define OE__BYP CYREG_PRT5_BYP
#define OE__CTL CYREG_PRT5_CTL
#define OE__DM0 CYREG_PRT5_DM0
#define OE__DM1 CYREG_PRT5_DM1
#define OE__DM2 CYREG_PRT5_DM2
#define OE__DR CYREG_PRT5_DR
#define OE__INP_DIS CYREG_PRT5_INP_DIS
#define OE__LCD_COM_SEG CYREG_PRT5_LCD_COM_SEG
#define OE__LCD_EN CYREG_PRT5_LCD_EN
#define OE__MASK 0x01u
#define OE__PORT 5
#define OE__PRT CYREG_PRT5_PRT
#define OE__PRTDSI__CAPS_SEL CYREG_PRT5_CAPS_SEL
#define OE__PRTDSI__DBL_SYNC_IN CYREG_PRT5_DBL_SYNC_IN
#define OE__PRTDSI__OE_SEL0 CYREG_PRT5_OE_SEL0
#define OE__PRTDSI__OE_SEL1 CYREG_PRT5_OE_SEL1
#define OE__PRTDSI__OUT_SEL0 CYREG_PRT5_OUT_SEL0
#define OE__PRTDSI__OUT_SEL1 CYREG_PRT5_OUT_SEL1
#define OE__PRTDSI__SYNC_OUT CYREG_PRT5_SYNC_OUT
#define OE__PS CYREG_PRT5_PS
#define OE__SHIFT 0
#define OE__SLW CYREG_PRT5_SLW

/* Miscellaneous */
/* -- WARNING: define names containting LEOPARD or PANTHER are deprecated and will be removed in a future release */
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3
#define CYDEV_CHIP_MEMBER_3A 1
#define CYDEV_CHIP_FAMILY_PSOC3 1
#define CYDEV_CHIP_DIE_LEOPARD 1
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_DIE_LEOPARD
#define BCLK__BUS_CLK__HZ 24000000U
#define BCLK__BUS_CLK__KHZ 24000U
#define BCLK__BUS_CLK__MHZ 24U
#define CYDEV_BOOTLOADER_CHECKSUM 0
#define CYDEV_BOOTLOADER_CHECKSUM_BASIC 0
#define CYDEV_BOOTLOADER_CHECKSUM_CRC 1
#define CYDEV_BOOTLOADER_WAIT_COMMAND 1
#define CYDEV_BOOTLOADER_WAIT_TIME 10
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_DIE_PANTHER 2
#define CYDEV_CHIP_DIE_UNKNOWN 0
#define CYDEV_CHIP_FAMILY_PSOC5 2
#define CYDEV_CHIP_FAMILY_UNKNOWN 0
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC3
#define CYDEV_CHIP_JTAG_ID 0x1E034069
#define CYDEV_CHIP_MEMBER_5A 2
#define CYDEV_CHIP_MEMBER_UNKNOWN 0
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_3A
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REV_LEOPARD_PRODUCTION
#define CYDEV_CHIP_REV_LEOPARD_ES1 0
#define CYDEV_CHIP_REV_LEOPARD_ES2 1
#define CYDEV_CHIP_REV_LEOPARD_ES3 3
#define CYDEV_CHIP_REV_PANTHER_ES0 0
#define CYDEV_CHIP_REV_PANTHER_ES1 1
#define CYDEV_CHIP_REVISION_3A_ES1 0
#define CYDEV_CHIP_REVISION_3A_ES2 1
#define CYDEV_CHIP_REVISION_3A_ES3 3
#define CYDEV_CHIP_REVISION_5A_ES0 0
#define CYDEV_CHIP_REVISION_5A_ES1 1
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_3A_PRODUCTION
#define CYDEV_CONFIG_FORCE_ROUTE 0
#define CYDEV_CONFIG_UNUSED_IO 0
#define CYDEV_CONFIGURATION_CLEAR_SRAM 1
#define CYDEV_CONFIGURATION_COMPRESSED 0
#define CYDEV_CONFIGURATION_DMA 1
#define CYDEV_CONFIGURATION_ECC 1
#define CYDEV_CONFIGURATION_IMOENABLED 1
#define CYDEV_CONFIGURATION_MODE 2
#define CYDEV_DEBUG_ENABLE_MASK 0x01
#define CYDEV_DEBUG_ENABLE_REGISTER CYREG_MLOGIC_DEBUG
#define CYDEV_DEBUGGING_DPS 2
#define CYDEV_DEBUGGING_ENABLE 1
#define CYDEV_DEBUGGING_REQXRES 1
#define CYDEV_DEBUGGING_XRES 0
#define CYDEV_ECC_ENABLE 0
#define CYDEV_INSTRUCT_CACHE_ENABLED 1
#define CYDEV_VDDA 5.0
#define CYDEV_VDDA_MV 5000
#define CYDEV_VDDD 5.0
#define CYDEV_VDDD_MV 5000
#define CYDEV_VDDIO0 5.0
#define CYDEV_VDDIO0_MV 5000
#define CYDEV_VDDIO1 5.0
#define CYDEV_VDDIO1_MV 5000
#define CYDEV_VDDIO2 5.0
#define CYDEV_VDDIO2_MV 5000
#define CYDEV_VDDIO3 5.0
#define CYDEV_VDDIO3_MV 5000
#define CYDEV_VIO0 5
#define CYDEV_VIO0_MV 5000
#define CYDEV_VIO1 5
#define CYDEV_VIO1_MV 5000
#define CYDEV_VIO2 5
#define CYDEV_VIO2_MV 5000
#define CYDEV_VIO3 5
#define CYDEV_VIO3_MV 5000
#define DMA_CHANNELS_USED__MASK0 0x00000000u
#define CYDEV_BOOTLOADER_ENABLE 0

#endif /* INCLUDED_CYFITTER_H */
