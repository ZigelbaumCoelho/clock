/*******************************************************************************
* File Name: cyPm.c
* Version 2.20
*
*  Description:
*   Provides an API for the power management.
*
*  Note:
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "cyPm.h"

static CY_PM_BACKUP_STRUCT  cyPmBackup;

/* Convertion table between CyIMO_SetFreq() parameters and register's value */
const uint8 CYCODE cyPmImoFreqMhz2Reg[7u] = {
    CY_IMO_FREQ_12MHZ, CY_IMO_FREQ_6MHZ,  CY_IMO_FREQ_24MHZ, CY_IMO_FREQ_3MHZ,
    CY_IMO_FREQ_48MHZ, CY_IMO_FREQ_62MHZ, CY_IMO_FREQ_74MHZ};

/* Convertion table between register's values and frequency in MHz  */
const uint8 CYCODE cyPmImoFreqReg2Mhz[7u] = {12u, 6u, 24u, 3u, 48u, 62u, 74u};

/* Function Prototypes */
void CyPmCtwSaveSet(uint8 ctwInterval);
void CyPmFtwSaveSet(uint8 ftwInterval);
void CyPmOppsSaveSet(void);

void CyPmHibSaveSet(void);
void CyPmHibRestore(void) ;

void CyPmHibSlpSaveSet(uint8 powerMode);
void CyPmHibSlpRestore(void);


/*******************************************************************************
* Function Name: CyPmSaveClocks
********************************************************************************
*
* Summary:
*  This function is called in preparation for entering sleep or hibernate low
*  power modes. Saves all state of the clocking system that doesn?t persist
*  during sleep/hibernate or that needs to be altered in preparation for
*  sleep/hibernate. Shutdowns all the digital and analog clock dividers.
*
*  Switches the master clock over to the IMO and shuts down the PLL and MHz
*  Crystal. If the IMO is not currently running it is enabled at 3 MHz before
*  the switch over. The ILO and 32 KHz oscillators are not impacted. The current
*  Flash wait state setting is saved and the Flash wait state setting is
*  set for the current IMO speed.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  All peripheral clocks are going to be off after this API method call.
*
*******************************************************************************/
void CyPmSaveClocks(void)
{
    /* Digital and analog clocks - save enable state and disable them all */
    cyPmBackup.enClkA = CY_PM_ACT_CFG1_REG & CY_PM_ACT_EN_CLK_A_MASK;
    cyPmBackup.enClkD = CY_PM_ACT_CFG2_REG;
    CY_PM_ACT_CFG1_REG &= ~CY_PM_ACT_EN_CLK_A_MASK;
    CY_PM_ACT_CFG2_REG &= ~CY_PM_ACT_EN_CLK_D_MASK;

    /* Master clock - save current source */
    cyPmBackup.masterClkSrc = CY_PM_CLKDIST_MSTR1_REG & CY_PM_MASTER_CLK_SRC_MASK;

    /* IMO - save current IMO MHZ OSC */
    cyPmBackup.imoFreqMhz = cyPmImoFreqReg2Mhz[(CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK)];

    /* Save current flash wait cycles and set the maximum new one */
    cyPmBackup.flashWaitCycles = (CY_PM_CACHE_CR_CYCLES_MASK & CY_PM_CACHE_CR_REG);

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        CyFlash_SetWaitCycles(45u);
                    
    #endif  /* End of (CY_PM_PSOC3_ES3) */

    /* Device is PSoC 3 and the revision is ES2 or earlier */
    #if(CY_PM_PSOC3_ES2)
        
        CyFlash_SetWaitCycles(49u);
        
    #endif  /* End of (CY_PM_PSOC3_ES2) */
    
    /* Device is PSoC 5 */
    #if(CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2)

        CyFlash_SetWaitCycles(73u);

    #endif  /* End of (CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2) */

    /* IMO - save enable state and enable with wait to settle */
    if(0u != (CY_PM_ACT_CFG0_IMO & CY_PM_ACT_CFG0_REG))
    {
        /* IMO - save enabled state */
        cyPmBackup.imoEnable = CY_PM_ENABLED;
        
        /* IMO - set it below 48 MHz */
        if((CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK) > CY_IMO_FREQ_48MHZ)
        {
            /* IMO - set 48 MHz frequency */
            CyIMO_SetFreq(CY_IMO_FREQ_48MHZ);
        }
    }
    else
    {
        /* IMO - save disabled state */
        cyPmBackup.imoEnable = CY_PM_DISABLED;
        
        /* IMO - set 3 MHz frequency if not yet */
        if(cyPmImoFreqReg2Mhz[(CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK)] != CY_IMO_FREQ_3MHZ)
        {
            CyIMO_SetFreq(CY_IMO_FREQ_3MHZ);
        }   /* Do nothing if IMO MHZ OSC is already set */
        
        /* IMO - enable */
        CyIMO_Start(CY_PM_WAIT_IMO);
    }

    /* IMO - save the current IMOCLK source and set to IMO if not yet */
    if(0u != (CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_XCLKEN))
    {
        /* DSI or XTAL CLK */
        cyPmBackup.imoClkSrc =
            (CY_PM_CLKDIST_CR_REG & CY_PM_CLKDIST_IMO2X_SRC) ? CY_IMO_SOURCE_XTAL : CY_IMO_SOURCE_DSI;

        /* IMO -  set IMOCLK source to MHz OSC */
        CyIMO_SetSource(CY_IMO_SOURCE_IMO);
    }
    else
    {
        /* IMO */
        cyPmBackup.imoClkSrc = CY_IMO_SOURCE_IMO;
    }

    /* Save clk_imo source */
    cyPmBackup.clkImoSrc = (CY_PM_CLKDIST_CR_REG & CY_PM_CLKDIST_IMO_OUT_MASK);

    if(CY_PM_CLKDIST_IMO_OUT_IMO != cyPmBackup.clkImoSrc)
    {
        /* IMOCLK2X or SPC OSC is source for clk_imo */

        /* Set IMOCLK to source for clk_imo */
        CY_PM_CLKDIST_CR_REG = (CY_PM_CLKDIST_CR_REG & ~CY_PM_CLKDIST_IMO_OUT_MASK) |
                            CY_PM_CLKDIST_IMO_OUT_IMO;
    }    /* Need to change nothing if IMOCLK is source clk_imo */

    /* IMO doubler - save enable state and disable */
    if(0u != (CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_F2XON))
    {
        /* IMO doubler enabled - save and disable */
        cyPmBackup.imo2x = CY_PM_ENABLED;
        CyIMO_DisableDoubler();
    }
    else
    {
        /* IMO doubler disabled */
        cyPmBackup.imo2x = CY_PM_DISABLED;
    }

    /* Master clock - save divider and set it to divide-by-one (if no yet) */
    cyPmBackup.clkSyncDiv = CY_PM_CLKDIST_MSTR0_REG;
    if(CY_PM_DIV_BY_ONE != cyPmBackup.clkSyncDiv)
    {
        /* Divider is > 1 */
        CyMasterClk_SetDivider(CY_PM_DIV_BY_ONE);
    }    /* Need to change nothing if master clock divider is 1 */

    /* Master clock source - set it to IMO if not yet. Saved above. */
    if(CY_MASTER_SOURCE_IMO != cyPmBackup.masterClkSrc)
    {
        CyMasterClk_SetSource(CY_MASTER_SOURCE_IMO);
    }    /* Need to change nothing if master clock source is IMO */

    /* Bus clock - save divider and set it, if needed, to divide-by-one */
    cyPmBackup.clkBusDiv = ((uint16) CY_PM_CLK_BUS_MSB_DIV_REG << 8u) | CY_PM_CLK_BUS_LSB_DIV_REG;
    if(CY_PM_BUS_CLK_DIV_BY_ONE != cyPmBackup.clkBusDiv)
    {
        CyBusClk_SetDivider(CY_PM_BUS_CLK_DIV_BY_ONE);
    }    /* Do nothing if saved and actual values are equal */


    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Save CPU frequency in MHz */
        cyPmBackup.cpuFreqMhz = (cyPmImoFreqReg2Mhz[CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK] /
                    (((CY_PM_CLKDIST_MSTR1_REG & CY_PM_CLKDIST_CPU_DIV_MASK) >> 4u) + 1u));

    #endif  /* End of (CY_PM_PSOC3_ES3) */

    /* Device is PSoC 3 and the revision is ES2 or earlier */
    #if(CY_PM_PSOC3_ES2)

        /* Save CPU frequency in MHz */
        cyPmBackup.cpuFreqMhz = (cyPmImoFreqReg2Mhz[CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK] /
                    ((CY_GET_XTND_REG8(CYREG_SFR_USER_CPUCLK_DIV) & CY_PM_SFR_USER_CPUCLK_MASK) + 1u));

    #endif  /* End of (CY_PM_PSOC3_ES2) */
    
    /* Device is PSoC 5 */
    #if(CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2)

        /* Save CPU frequency in MHz. The CPU clock is directly derived from
        *  bus clock.
        */
        cyPmBackup.cpuFreqMhz = (cyPmImoFreqReg2Mhz[CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK]);

    #endif  /* End of (CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2) */
    
    /* Set number of wait cycles for the flash */
    CyFlash_SetWaitCycles(cyPmBackup.cpuFreqMhz);

    /* PLL - check enable state, disable if needed */
    if(0u != (CY_PM_FASTCLK_PLL_CFG0_REG & CY_PM_PLL_CFG0_ENABLE))
    {
        /* PLL is enabled - save state and disable */
        cyPmBackup.pllEnableState = CY_PM_ENABLED;
        CyPLL_OUT_Stop();
    }
    else
    {
        /* PLL is disabled - save state */
        cyPmBackup.pllEnableState = CY_PM_DISABLED;
    }

    /* MHz ECO - check enable state and disable if needed */
    if(0u != (CY_PM_FASTCLK_XMHZ_CSR_REG & CY_PM_XMHZ_CSR_ENABLE))
    {
        /* MHz ECO is enabled - save state and disable */
        cyPmBackup.xmhzEnableState = CY_PM_ENABLED;
        CyXTAL_Stop();
    }
    else
    {
        /* MHz ECO is disabled - save state */
        cyPmBackup.xmhzEnableState = CY_PM_DISABLED;
    }
}


/*******************************************************************************
* Function Name: CyPmRestoreClocks
********************************************************************************
*
* Summary:
*  Restores any state that was preserved by the last call to CyPmSaveClocks().
*  The Flash wait state setting is also restored.
*
*  The merge region could be used to process state when  the megahertz crystal
*  is not ready after CY_PM_MHZ_XTAL_WAIT_MS milliseconds.
*
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmRestoreClocks(void) 
{
    cystatus status = CYRET_TIMEOUT;
    uint8 i;
    
    /* MHz ECO restore state */
    if(CY_PM_ENABLED == cyPmBackup.xmhzEnableState)
    {
        /* Enabling XMHZ XTAL. The actual CyXTAL_Start() with non zero wait
           period uses FTW for period measurement. This could cause a problem
           if CTW/FTW is used as a wake up time in the low power modes APIs.
           So, the XTAL wait procedure is implemented with a software delay.
        */
        
        /* Enable XMHZ XTAL with no wait */
        CyXTAL_Start(CY_PM_XTAL_MHZ_NO_WAIT);
        
        /* Read XERR bit to clear it */
        CY_PM_FASTCLK_XMHZ_CSR_REG;
        
        /* Wait 5 times for CY_PM_WAIT_200_US */
        for(i = 5u; i > 0u; i--)
        {
            /* Make a us delay */
            CyDelayCycles(CY_PM_WAIT_200_US * cyPmBackup.cpuFreqMhz);
            
            /* High output indicates oscillator failure */
            if(0u == (CY_PM_FASTCLK_XMHZ_CSR_REG & CY_PM_XMHZ_CSR_XERR)) 
            {
                status = CYRET_SUCCESS;
                break;
            }
        }
        
        if(CYRET_TIMEOUT == status)
        {
            /* `#START MHZ ECO TIMEOUT` */

            /* `#END` */
        }
    }
    else
    {
        /* Disable MHz ECO */
        CyXTAL_Stop();
    }   /* End of (CY_PM_ENABLED == cyPmBackup.xmhzEnableState) */


    /* PLL restore state */
    if(CY_PM_ENABLED == cyPmBackup.pllEnableState)
    {
        /* Enabling PLL. The actual CyPLL_OUT_Start() without wait period uses
           FTW for period measurement. This could cause a problem if CTW/FTW is
           used as a wake up time in the low power modes APIs. So, the PLL wait
           procedure is implemented with a software delay.
        */
        
        /* Enable PLL */
        CyPLL_OUT_Start(CY_PM_PLL_OUT_NO_WAIT);

        /* Make a 250 us delay */
        CyDelayCycles(CY_PM_WAIT_250_US * cyPmBackup.cpuFreqMhz);
    }
    else
    {
        /* Disable PLL */
        CyPLL_OUT_Stop();
    }   /* End of (CY_PM_ENABLED == cyPmBackup.pllEnableState) */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Temprorary set the maximum flash wait cycles */
        CyFlash_SetWaitCycles(45u);
                    
    #endif  /* End of (CY_PM_PSOC3_ES3) */

    /* Device is PSoC 3 and the revision is ES2 or earlier */
    #if(CY_PM_PSOC3_ES2)

        /* Temprorary set the maximum flash wait cycles */
        CyFlash_SetWaitCycles(49u);
        
    #endif  /* End of (CY_PM_PSOC3_ES2) */
    
    /* Device is PSoC 5 */
    #if(CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2)

        /* Temprorary set the maximum flash wait cycles */
        CyFlash_SetWaitCycles(73u);

    #endif  /* End of (CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2) */

    if(CY_PM_MASTER_CLK_SRC_IMO != cyPmBackup.masterClkSrc)
    {
        /* If not IMO, then PLL or XMHZ or DSI_CLKIN.
        *  All clocks above are ready to be source for Master clock,
        *  so restore Master clock source and Master clock divider.
        */
        if(CY_PM_CLKDIST_MSTR0_REG != cyPmBackup.clkSyncDiv)
        {
            /* Restore Master clock divider */
            CyMasterClk_SetDivider(cyPmBackup.clkSyncDiv);
        }
        
        /* Restore Master clock source */
        CyMasterClk_SetSource(cyPmBackup.masterClkSrc);
    }

    /* IMO - restore IMO frequency */
    if(cyPmBackup.imoFreqMhz != cyPmImoFreqReg2Mhz[(CY_PM_FASTCLK_IMO_CR_REG & CY_PM_FASTCLK_IMO_CR_FREQ_MASK)])
    {
        CyIMO_SetFreq(cyPmImoFreqMhz2Reg[cyPmBackup.imoFreqMhz]);
    }    /* Do nothing if IMO freq is equal to saved */

    /* IMO - restore enable state if needed */
    if((CY_PM_ENABLED == cyPmBackup.imoEnable) &&
            (0u == (CY_PM_ACT_CFG0_IMO & CY_PM_ACT_CFG0_REG)))
    {
        /* IMO - restore enabled state */
        CyIMO_Start(CY_PM_WAIT_IMO);
    }    /* Do nothing if saved and actual values are equal */

    /* IMO - restore enable state if needed */
    if((CY_PM_ENABLED != cyPmBackup.imoEnable) &&
            (0u != (CY_PM_ACT_CFG0_IMO & CY_PM_ACT_CFG0_REG)))
    {
        /* IMO - restore disabled state */
        CyIMO_Stop();
    }    /* Do nothing if saved and actual values are equal */

    /* IMO - restore IMOCLK source */
    CyIMO_SetSource(cyPmBackup.imoClkSrc);

    /* Restore IMO doubler enable state */
    if(CY_PM_ENABLED == cyPmBackup.imo2x)
    {
        /* IMO doubler - enable */
        CyIMO_EnableDoubler();
    }
    else
    {
        /* IMO doubler - disable */
        CyIMO_DisableDoubler();
    }

    /* IMO - restore clk_imo source, if needed */
    if(cyPmBackup.clkImoSrc != (CY_PM_CLKDIST_CR_REG & CY_PM_CLKDIST_IMO_OUT_MASK))
    {
        CY_PM_CLKDIST_CR_REG = (CY_PM_CLKDIST_CR_REG & ~CY_PM_CLKDIST_IMO_OUT_MASK) |
                            cyPmBackup.clkImoSrc;
    }    /* Do nothing if saved and actual values are equal */

    if(CY_PM_MASTER_CLK_SRC_IMO == cyPmBackup.masterClkSrc)
    {
        /* The IMO is ready to be source for Master clock */
        if(CY_PM_CLKDIST_MSTR0_REG != cyPmBackup.clkSyncDiv)
        {
            /* Restore Master clock divider */
            CyMasterClk_SetDivider(cyPmBackup.clkSyncDiv);
        }
        
        /* Restore Master clock source */
        CyMasterClk_SetSource(cyPmBackup.masterClkSrc);
        
    }   /* End of (CY_PM_MASTER_CLK_SRC_IMO == cyPmBackup.masterClkSrc) */

    /* Bus clock - restore divider, if needed */
    if(cyPmBackup.clkBusDiv != (((uint16) CY_PM_CLK_BUS_MSB_DIV_REG << 8u) | CY_PM_CLK_BUS_LSB_DIV_REG))
    {
        CyBusClk_SetDivider(cyPmBackup.clkBusDiv);
    }
    
    /* Restore flash wait cycles */
    CY_PM_CACHE_CR_REG = ((CY_PM_CACHE_CR_REG & ~CY_PM_CACHE_CR_CYCLES_MASK) |
                            cyPmBackup.flashWaitCycles);

    /* Digital and analog clocks - restore state */
    CY_PM_ACT_CFG1_REG = cyPmBackup.enClkA;
    CY_PM_ACT_CFG2_REG = cyPmBackup.enClkD;
}


/*******************************************************************************
* Function Name: CyPmAltAct
********************************************************************************
*
* Summary:
*  Puts the part into the Alternate Active (Standby) state.  Before switching to
*  Alternate Active, if a wakeupTime other than NONE is specified, then the
*  appropriate timer state is configured as specified with the interrupt for
*  that timer disabled.  The wakeup source will be the combination of the values
*  specified in the wakeupSource and any timer specified in the wakeupTime
*  argument.  Once the wakeup condition is satisfied, then all saved state is
*  restored and the function returns in the Active state.
*
*  Note that if the wakeupTime is made with a different value, the period before
*  the wakeup occurs can be significantly shorter than the specified time.  If
*  the next call is made with the same wakeupTime value, then the wakeup will
*  occur the specified period after the previous wakeup occurred.
*
*  If a wakeupTime other than NONE is specified, then upon exit the state of the
*  specified timer will be left as specified by wakeupTime with the timer
*  enabled and the interrupt disabled.  If the CTW, FTW or One PPS is already
*  configured for wakeup, for example with the SleepTimer or RTC components,
*  then specify NONE for the wakeupTime and include the appropriate source for
*  wakeupSource. 
*
*  The Alternate Active state can allow for any of the capabilities of the
*  device to be active, but the operation of this function is dependent on the
*  CPU being disabled during the Alternate Active state.  The configuration code
*  and the component APIs will configure the template for the Alternate Active
*  state to be the same as the Active state with the exception that the CPU will
*  be disabled during Alternate Active.
*
* Parameters:
*  wakeupTime: Specifies a timer wakeup source and the frequency of that
*               source.
*
*           Define                      Time
*  PM_ALT_ACT_TIME_NONE             None
*  PM_ALT_ACT_TIME_ONE_PPS          One PPS: 1 second
*  PM_ALT_ACT_TIME_CTW_2MS          CTW: 2 ms
*  PM_ALT_ACT_TIME_CTW_4MS          CTW: 4 ms
*  PM_ALT_ACT_TIME_CTW_8MS          CTW: 8 ms
*  PM_ALT_ACT_TIME_CTW_16MS         CTW: 16 ms
*  PM_ALT_ACT_TIME_CTW_32MS         CTW: 32 ms
*  PM_ALT_ACT_TIME_CTW_64MS         CTW: 64 ms
*  PM_ALT_ACT_TIME_CTW_128MS        CTW: 128 ms
*  PM_ALT_ACT_TIME_CTW_256MS        CTW: 256 ms
*  PM_ALT_ACT_TIME_CTW_512MS        CTW: 512 ms
*  PM_ALT_ACT_TIME_CTW_1024MS       CTW: 1024 ms
*  PM_ALT_ACT_TIME_CTW_2048MS       CTW: 2048 ms
*  PM_ALT_ACT_TIME_CTW_4096MS       CTW: 4096 ms
*  PM_ALT_ACT_TIME_FTW(1-256)*       FTW: 10us to 2.56ms
*
*  *Note:   PM_ALT_ACT_TIME_FTW() is a macro that takes an argument that
*           specifies how many increments of 10us to delay. For PSoC 3 ES2
*           silicon and earlier and PSoC 5 ES1 silicon and earlier the valid
*           range of values is 1 to 32. For later silicon the valid range of
*           values is 1 to 256.
*
*  wakeUpSource:    Specifies a bitwise mask of wakeup sources. In addition, if
*                   a wakeupTime has been specified the associated timer will be
*                   included as a wakeup source.
*
*           Define                      Source
*  PM_ALT_ACT_SRC_NONE              None
*  PM_ALT_ACT_SRC_COMPARATOR0       Comparator 0
*  PM_ALT_ACT_SRC_COMPARATOR1       Comparator 1
*  PM_ALT_ACT_SRC_COMPARATOR2       Comparator 2
*  PM_ALT_ACT_SRC_COMPARATOR3       Comparator 3
*  PM_ALT_ACT_SRC_INTERRUPT         Interrupt
*  PM_ALT_ACT_SRC_PICU              PICU
*  PM_ALT_ACT_SRC_I2C               I2C
*  PM_ALT_ACT_SRC_BOOSTCONVERTER    Boost Converter
*  PM_ALT_ACT_SRC_FTW               Fast Timewheel
*  PM_ALT_ACT_SRC_CTW               Central Timewheel*
*  PM_ALT_ACT_SRC_ONE_PPS           One PPS*
*  PM_ALT_ACT_SRC_LCD               LCD
*
*  *Note: CTW and One PPS wakeup signals are in the same mask bit. For
*         PSoC 5 ES1 these are also the same address as the FTW.
*
*  When specifying a Comparator as the wakeupSource an instance specific define
*  should be used that will track with the specific comparator that the instance
*  is placed into. As an example, for a Comparator instance named ?MyComp? the
*  value to OR into the mask is: MyComp_ctComp__CMP_MASK.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  For PSoC 3 ES2 silcon or earlier the wakeup source is not selectable. In this
*  case the wakeupSource argument is ignored and any of the available wakeup
*  sources will wake the device.
*
*  For PSoC 5 ES1 silicon or earlier the LCD is not a selectable wakeup source.
*
*  If a wakeupTime other than NONE is specified, then upon exit the state of the
*  specified timer will be left as specified by wakeupTime with the timer
*  enabled and the interrupt disabled.  Also, ILO power mode will be left in a
*  low power mode, the ILO 1 KHz (if CTW timer is used as wake up time) or
*  ILO 100 KHz (if FTW timer is used as wake up time) will be left started.
*
*******************************************************************************/
void CyPmAltAct(uint16 wakeupTime, uint16 wakeupSource)
{
    /* FTW - save current and set new configuration */
    if((wakeupTime >= PM_ALT_ACT_TIME_FTW(1u)) && (wakeupTime <= PM_ALT_ACT_TIME_FTW(256u)))
    {
        /* Device is PSoC 3 and the revision is ES2 or earlier, or
        *  device is PSoC 5 and the revision is ES1 or earlier.
        */
        #if(CY_PM_PSOC3_ES2 || CY_PM_PSOC5_ES1)
        
            /* If out of range - set to max value */
            if(wakeupTime >= PM_ALT_ACT_TIME_FTW(32u))
            {
				/* Save FTW configuration and set new one to max value */
                CyPmFtwSaveSet((PM_ALT_ACT_TIME_FTW(32u) - CY_PM_FTW_INTERVAL_SHIFT - 1u) & CY_PM_FTW_INTERVAL_MASK);
            }
            else    /* value is in range - just pass parameter as is */
            {
                /* Save current FTW configuration and set new one */
                CyPmFtwSaveSet((wakeupTime - CY_PM_FTW_INTERVAL_SHIFT - 1u) & CY_PM_FTW_INTERVAL_MASK);
            }
        
        /* Device is PSoC 3 and the revision is ES3 or later, or
        *  device is PSoC 5 and the revision is ES2 or later.
        */
        #else

            /* Save current FTW configuration and set new one */
            CyPmFtwSaveSet((wakeupTime - CY_PM_FTW_INTERVAL_SHIFT - 1u) & CY_PM_FTW_INTERVAL_MASK);
        
        #endif  /* End of (CY_PM_PSOC3_ES2 || CY_PM_PSOC5_ES1) */
        
        /* Include associated timer to the wakeupSource */
        wakeupSource |= PM_ALT_ACT_SRC_FTW;
    }

    /* CTW - save current and set new configuration */
    if((wakeupTime >= PM_ALT_ACT_TIME_CTW_2MS) && (wakeupTime <= PM_ALT_ACT_TIME_CTW_4096MS))
    {
        /* Save current CTW configuration and set new one */
        CyPmCtwSaveSet((wakeupTime - 1u) & CY_PM_CTW_INTERVAL_MASK);

        /* Include associated timer to the wakeupSource */
        wakeupSource |= PM_ALT_ACT_SRC_CTW;
    }

    /* 1PPS - save current and set new configuration */
    if(PM_ALT_ACT_TIME_ONE_PPS == wakeupTime)
    {
        /* Save current 1PPS configuration and set new one */
        CyPmOppsSaveSet();

        /* Include associated timer to the wakeupSource */
        wakeupSource |= PM_ALT_ACT_SRC_ONE_PPS;
    }


    /* Device is PSoC 3 and the revision is ES3 or later, or
    *  device is PSoC 5 and the revision is ES1 or earlier.
    */
    #if(CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1)
        
        /* Save and set new wake up configuration */

        /* Interrupt, PICU, I2C, Boost converter, CTW/1PPS */
        cyPmBackup.wakeupCfg0 = CY_PM_WAKEUP_CFG0_REG;
        CY_PM_WAKEUP_CFG0_REG = ((uint8) ((wakeupSource >> 4u) & 0xFFu));

        /* Comparators */
        cyPmBackup.wakeupCfg1 = CY_PM_WAKEUP_CFG1_REG;
        CY_PM_WAKEUP_CFG1_REG = (((uint8) wakeupSource) & CY_PM_WAKEUP_SRC_CMPS_MASK);

        /* Device is PSoC 3 and the revision is ES3 or later */
        #if(CY_PM_PSOC3_ES3)

            /* LCD */
            cyPmBackup.wakeupCfg2 = CY_PM_WAKEUP_CFG2_REG;
            CY_PM_WAKEUP_CFG2_REG = ((uint8) ((wakeupSource >> 12u) & 0x01u));

        #endif  /* End of (CY_PM_PSOC3_ES3) */
    
    /* Device is PSoC 3 and the revision is ES2 or earlier, or
    *  device is PSoC 5 and the revision is ES2 or later.
    */
    #else

        /* To remove unreferenced local variable warning */
        wakeupSource++;

    #endif /* End of (CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1) */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Save current global interrupt enable and disable it,
        *  this should be done only if clk_bus is disabled during
        *  Alternate Active mode. By default, clk_bus is enabled
        *  during this mode, but user is able to disable it manually,
        *  so implement this workaround for sure.
        */
        cyPmBackup.interruptEnable = CyEnterCriticalSection();

    #endif  /* End of (CY_PM_PSOC3_ES3)*/

    /* Switch to the Sleep mode */
    CY_PM_MODE_CSR_REG = ((CY_PM_MODE_CSR_REG & ~CY_PM_MODE_CSR_MASK) | CY_PM_MODE_CSR_ALT_ACT);

    /* Recommended readback. */
    CY_PM_MODE_CSR_REG;

    /* Two recommended NOPs to get into the mode. */
    CY_NOP;
    CY_NOP;

    /* Device is PSoC 5 */
    #if(CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2)

        /* PSoC3 and PSoC5 has different low power modes entry way */

        #if defined(__ARMCC_VERSION)

            /* Instristic for Keil compilers */
            __wfi();

        #else

            /* ASM for GCC */
            asm volatile ("WFI \n");

        #endif /* End of (__ARMCC_VERSION) */

    #endif /* End of (CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2) */
    
    /* Point of return from Alternate Active Mode */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)
    
        /* Restore global interrupt enable state */
        CyExitCriticalSection(cyPmBackup.interruptEnable);
    
    #endif  /* End of (CY_PM_PSOC3_ES3)*/

    /* Device is PSoC 3 and the revision is ES3 or later, or
    *  device is PSoC 5 and the revision is ES1 or earlier.
    */
    #if(CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1)

        /* Wake up source configuration */

        /* Restore current wake up configuration */
        CY_PM_WAKEUP_CFG0_REG = cyPmBackup.wakeupCfg0;
        CY_PM_WAKEUP_CFG1_REG = cyPmBackup.wakeupCfg1;

        /* Device is PSoC 3 and the revision is ES3 or later */
        #if(CY_PM_PSOC3_ES3)

            CY_PM_WAKEUP_CFG2_REG = cyPmBackup.wakeupCfg2;

        #endif  /* End of (CY_PM_PSOC3_ES3) */

    #endif /* End of (CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: CyPmSleep
********************************************************************************
*
* Summary:
*  Puts the part into the Sleep state.  Before switching to Sleep, if a
*  wakeupTime other than NONE is specified, then the appropriate timer state is
*  configured as specified with the interrupt for that timer disabled.  The
*  wakeup source will be the combination of the values specified in the
*  wakeupSource and any timer specified in the wakeupTime argument.  Once the
*  wakeup condition is satisfied, then all saved state is restored and the
*  function returns in the Active state.
*
*  Note that if the wakeupTime is made with a different value, the period before
*  the wakeup occurs can be significantly shorter than the specified time.  If
*  the next call is made with the same wakeupTime value, then the wakeup will
*  occur the specified period after the previous wakeup occurred.
*
*  If a wakeupTime other than NONE is specified, then upon exit the state of the
*  specified timer will be left as specified by wakeupTime with the timer
*  enabled and the interrupt disabled.  If the CTW or One PPS is already
*  configured for wakeup, for example with the SleepTimer or RTC components,
*  then specify NONE for the wakeupTime and include the appropriate source for
*  wakeupSource. 
*
* Parameters:
*  wakeupTime:      Specifies a timer wakeup source and the frequency of that
*                   source.
*
*           Define                      Time
*  PM_SLEEP_TIME_NONE               None
*  PM_SLEEP_TIME_ONE_PPS            One PPS: 1 second
*  PM_SLEEP_TIME_CTW_2MS            CTW: 2 ms
*  PM_SLEEP_TIME_CTW_4MS            CTW: 4 ms
*  PM_SLEEP_TIME_CTW_8MS            CTW: 8 ms
*  PM_SLEEP_TIME_CTW_16MS           CTW: 16 ms
*  PM_SLEEP_TIME_CTW_32MS           CTW: 32 ms
*  PM_SLEEP_TIME_CTW_64MS           CTW: 64 ms
*  PM_SLEEP_TIME_CTW_128MS          CTW: 128 ms
*  PM_SLEEP_TIME_CTW_256MS          CTW: 256 ms
*  PM_SLEEP_TIME_CTW_512MS          CTW: 512 ms
*  PM_SLEEP_TIME_CTW_1024MS         CTW: 1024 ms
*  PM_SLEEP_TIME_CTW_2048MS         CTW: 2048 ms
*  PM_SLEEP_TIME_CTW_4096MS         CTW: 4096 ms
*
*  wakeUpSource:    Specifies a bitwise mask of wakeup sources. In addition, if
*                   a wakeupTime has been specified the associated timer will be
*                   included as a wakeup source.
*
*           Define                      Source
*  PM_SLEEP_SRC_NONE                None
*  PM_SLEEP_SRC_COMPARATOR0         Comparator 0
*  PM_SLEEP_SRC_COMPARATOR1         Comparator 1
*  PM_SLEEP_SRC_COMPARATOR2         Comparator 2
*  PM_SLEEP_SRC_COMPARATOR3         Comparator 3
*  PM_SLEEP_SRC_PICU                PICU
*  PM_SLEEP_SRC_I2C                 I2C
*  PM_SLEEP_SRC_BOOSTCONVERTER      Boost Converter
*  PM_SLEEP_SRC_CTW                 Central Timewheel*
*  PM_SLEEP_SRC_ONE_PPS             One PPS*
*  PM_SLEEP_SRC_LCD                 LCD
*
*  *Note:   CTW and One PPS wakeup signals are in the same mask bit. For
*           PSoC 5 ES1 these are in a different bit.
*
*  When specifying a Comparator as the wakeupSource an instance specific define
*  should be used that will track with the specific comparator that the instance
*  is placed into. As an example for a Comparator instance named ?MyComp? the
*  value to OR into the mask is: MyComp_ctComp__CMP_MASK.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  For PSoC 3 ES2 silcon or earlier the wakeup source is not selectable. In this
*  case the wakeupSource argument is ignored and any of the available wakeup
*  sources will wake the device.
*
*  For PSoC 5 ES1 silicon or earlier the LCD is not a selectable wakeup source.
*
*  If a wakeupTime other than NONE is specified, then upon exit the state of the
*  specified timer will be left as specified by wakeupTime with the timer
*  enabled and the interrupt disabled.  Also, ILO power mode will be left in a
*  low power mode, the ILO 1 KHz (if CTW timer is used as wake up time) will be
*  left started.
*
*  Note: PSoC 3 ES2 and PSoC 5 ES1 silicon have a defect that causes connections
*  to several analog resources to be unreliable when the device is placed in a
*  low power mode. See the silicon errata for details.
*
*  Be aware of potential silent failures when using CyPmSleep().
*
*******************************************************************************/
void CyPmSleep(uint8 wakeupTime, uint16 wakeupSource)
{
    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Hibernate/Sleep regulator stabilization */
        if(0u == (CY_PM_MODE_CSR_REG & CY_PM_MODE_CSR_PWRUP_PULSE_Q))
        {
            /* Disable hold off - no action on restore */
            CY_PM_PWRSYS_SLP_TR_REG &= CY_PM_PWRSYS_SLP_TR_HIBSLP_HOLDOFF_MASK;    
        }
        else
        {
            /* Abort, device is not ready for low power mode entry */
            return;
        }

    #endif /* End of (CY_PM_PSOC3_ES3) */
    
    /* CTW - save current and set new configuration */
    if((wakeupTime >= PM_SLEEP_TIME_CTW_2MS) && (wakeupTime <= PM_SLEEP_TIME_CTW_4096MS))
    {
        /* Save current and set new configuration of the CTW */
        CyPmCtwSaveSet((wakeupTime - 1u) & CY_PM_CTW_INTERVAL_MASK);

        /* Include associated timer to the wakeupSource */
        wakeupSource |= PM_SLEEP_SRC_CTW;
    }
    
    /* 1PPS - save current and set new configuration */
    if(PM_SLEEP_TIME_ONE_PPS == wakeupTime)
    {
        /* Save current and set new configuration of the 1PPS */
        CyPmOppsSaveSet();
        
        /* Include associated timer to the wakeupSource */
        wakeupSource |= PM_SLEEP_SRC_ONE_PPS;
    }

    /* Device is PSoC 3 and the revision is ES3 or later, or
    *  device is PSoC 5 and the revision is ES1 or earlier.
    */
    #if(CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1)

        /* Save and set new wake up configuration */

        /* Interrupt, PICU, I2C, Boost converter, CTW/1PPS */
        cyPmBackup.wakeupCfg0 = CY_PM_WAKEUP_CFG0_REG;
        CY_PM_WAKEUP_CFG0_REG = ((uint8) ((wakeupSource >> 4u) & 0xFFu));

        /* Comparators */
        cyPmBackup.wakeupCfg1 = CY_PM_WAKEUP_CFG1_REG;
        CY_PM_WAKEUP_CFG1_REG = (((uint8) wakeupSource) & CY_PM_WAKEUP_SRC_CMPS_MASK);

        /* Device is PSoC 3 and the revision is ES3 or later */
        #if(CY_PM_PSOC3_ES3)

            /* LCD */
            cyPmBackup.wakeupCfg2 = CY_PM_WAKEUP_CFG2_REG;
            CY_PM_WAKEUP_CFG2_REG = ((uint8) ((wakeupSource >> 12u) & 0x01u));

        #endif  /* End of (CY_PM_PSOC3_ES3) */
    
    /* Device is PSoC 3 and the revision is ES2 or earlier, or
    *  device is PSoC 5 and the revision is ES2 or later.
    */
    #else

        /* To remove unreferenced local variable warning */
        wakeupSource++;

    #endif /* End of (CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1) */

    /* Tune LVI/HVI and wait for Hibernate/Sleep regulator to stabilize */
    CyPmHibSlpSaveSet(CY_PM_MODE_CSR_SLEEP);

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Save current global interrupt enable and disable it */
        cyPmBackup.interruptEnable = CyEnterCriticalSection();

        /* Disable LDO-A regulator prior to entering sleep when buzzing is needed */
        if(CY_PM_PWRSYS_WAKE_TR2_EN_BUZZ == CY_PM_PWRSYS_WAKE_TR2_REG & CY_PM_PWRSYS_WAKE_TR2_EN_BUZZ)
        {
            /* Prepares for disabling LDO-A by moving bandgap reference to VCCD */
            CY_PM_PWRSYS_CR1_REG = CY_PM_PWRSYS_CR1_LDOA_ISO | (CY_PM_PWRSYS_CR1_REG & ~CY_PM_PWRSYS_CR1_LDOA_DIS);
        
            /* Disables LDO-A */
            CY_PM_PWRSYS_CR1_REG |= CY_PM_PWRSYS_CR1_LDOA_DIS;
        }

    #endif  /* End of (CY_PM_PSOC3_ES3)*/

    /* Switch to the Sleep mode */
    CY_PM_MODE_CSR_REG = ((CY_PM_MODE_CSR_REG & ~CY_PM_MODE_CSR_MASK) | CY_PM_MODE_CSR_SLEEP);

    /* Recommended readback. */
    CY_PM_MODE_CSR_REG;

    /* Two recommended NOPs to get into the mode. */
    CY_NOP;
    CY_NOP;

    /* Device is PSoC 5 */
    #if(CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2)

        /* PSoC3 and PSoC5 has different low power modes entry way */

        #if defined(__ARMCC_VERSION)

            /* Instristic for Keil compilers */
            __wfi();

        #else

            /* ASM for GCC */
            asm volatile ("WFI \n");

        #endif /* End of (__ARMCC_VERSION) */

    #endif /* End of (CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2) */

    /* Point of return from Sleep Mode */

    /* Gives the sleep regulator time to stabilize (20us) */
    CyDelayCycles(CY_PM_WAIT_20_US * cyPmBackup.cpuFreqMhz); 

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Re-enable LDO-A after waking up from sleep when buzzing is needed */
        if(CY_PM_PWRSYS_WAKE_TR2_EN_BUZZ == CY_PM_PWRSYS_WAKE_TR2_REG & CY_PM_PWRSYS_WAKE_TR2_EN_BUZZ)
        {
            /* Enable LDO-A */
            CY_PM_PWRSYS_CR1_REG &= ~CY_PM_PWRSYS_CR1_LDOA_DIS;
            
            /* Stop preparing the system to disable the LDO-A */
            CY_PM_PWRSYS_CR1_REG &= ~CY_PM_PWRSYS_CR1_LDOA_ISO;
        }

        /* Restore global interrupt enable state */
        CyExitCriticalSection(cyPmBackup.interruptEnable);
    
    #endif  /* End of (CY_PM_PSOC3_ES3)*/

    /* Restore LVI/HVI configuration */
    CyPmHibSlpRestore();

    /* Device is PSoC 3 and the revision is ES3 or later, or
    *  device is PSoC 5 and the revision is ES2 or later.
    */
    #if(CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES2)

        /* Wake up source configuration */

        /* Restore current wake up configuration */
        CY_PM_WAKEUP_CFG0_REG = cyPmBackup.wakeupCfg0;
        CY_PM_WAKEUP_CFG1_REG = cyPmBackup.wakeupCfg1;

        /* Device is PSoC 3 and the revision is ES3 or later */
        #if(CY_PM_PSOC3_ES3)

            CY_PM_WAKEUP_CFG2_REG = cyPmBackup.wakeupCfg2;

        #endif  /* End of (CY_PM_PSOC3_ES3) */

    #endif /* End of (CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES2) */
}


/*******************************************************************************
* Function Name: CyPmHibernate
********************************************************************************
*
* Summary:
*  Puts the part into the Hibernate state. Before switching to Hibernate the
*  PICU wakeup source bit is saved and then set. The PICU is the only available
*  source to wakeup from the Hibernate state. Once the wakeup condition is
*  satisfied, then the PICU wakeup source bit is restored and the function
*  returns in the Active state.
*
*  There is a firmware requirement that specifies applications cannot wakeup
*  from hibernate and enter sleep for 20 us. The time starts from the wakeup
*  event and gives the sleep regulator time to stabilize. There is no hardware
*  check that this requirement is met. The specified delay should be done on
*  ISR entry.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  None.
*
*  Note: PSoC 3 ES2 and PSoC 5 ES1 silicon have a defect that causes connections
*  to several analog resources to be unreliable when the device is placed in a
*  low power mode. See the silicon errata for details.
*
*  Be aware of potential silent failures when using CyPmHibernate().
*
*******************************************************************************/
void CyPmHibernate(void)
{
    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Hibernate/Sleep regulator stabilization */
        if(0u == (CY_PM_MODE_CSR_REG & CY_PM_MODE_CSR_PWRUP_PULSE_Q))
        {
            /* Disable hold off - no action on restore */
            CY_PM_PWRSYS_SLP_TR_REG &= CY_PM_PWRSYS_SLP_TR_HIBSLP_HOLDOFF_MASK;    
        }
        else
        {
            /* Abort, device is not ready for low power mode entry */
            /* This section will be redesign to be correct till Beta 5
            *  when PSoC3/5 PM API files become device version specific.
            */            
            return;
        }

    #endif /* End of (CY_PM_PSOC3_ES3) */
    
    /* Prepare device for proper Hibernate mode entry */
    CyPmHibSaveSet();

    /* Device is PSoC 3 and the revision is ES3 or later, or
    *  device is PSoC 5 and the revision is ES1 or earlier.
    */
    #if(CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1)

        /* Save and set new wake up configuration */

        /* Save and enable only wake up on PICU */
        cyPmBackup.wakeupCfg0 = CY_PM_WAKEUP_CFG0_REG;
        CY_PM_WAKEUP_CFG0_REG = CY_PM_WAKEUP_PICU;

        /* Comparators */
        cyPmBackup.wakeupCfg1 = CY_PM_WAKEUP_CFG1_REG;
        CY_PM_WAKEUP_CFG1_REG = 0x00u;

        /* Device is PSoC 3 and the revision is ES3 or later */
        #if(CY_PM_PSOC3_ES3)

            /* LCD */
            cyPmBackup.wakeupCfg2 = CY_PM_WAKEUP_CFG2_REG;
            CY_PM_WAKEUP_CFG2_REG = 0x00u;

        #endif  /* End of (CY_PM_PSOC3_ES3) */

    #endif /* End of (CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES1) */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Save current global interrupt enable and disable it */
        cyPmBackup.interruptEnable = CyEnterCriticalSection();

    #endif  /* End of (CY_PM_PSOC3_ES3)*/

    /* Switch to Hibernate Mode */
    CY_PM_MODE_CSR_REG = ((CY_PM_MODE_CSR_REG & ~CY_PM_MODE_CSR_MASK) | CY_PM_MODE_CSR_HIBERNATE);

    /* Recommended readback. */
    CY_PM_MODE_CSR_REG;

    /* Two recommended NOPs to get into the mode. */
    CY_NOP;
    CY_NOP;

    /* Device is PSoC 5 */
    #if(CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2)

        /* PSoC3 and PSoC5 has different low power modes entry way */

        #if defined(__ARMCC_VERSION)

            /* Instristic for Keil compilers */
            __wfi();

        #else

            /* ASM for GCC */
            asm volatile ("WFI \n");

        #endif /* End of (__ARMCC_VERSION) */

    #endif /* End of (CY_PM_PSOC5_ES1 || CY_PM_PSOC5_ES2) */

    /* Point of return from Hibernate mode */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)
    
        /* Restore global interrupt enable state */
        CyExitCriticalSection(cyPmBackup.interruptEnable);
    
    #endif  /* End of (CY_PM_PSOC3_ES3)*/

    /* Restore device for proper Hibernate mode exit*/
    CyPmHibRestore();

    /* Device is PSoC 3 and the revision is ES3 or later, or
    *  device is PSoC 5 and the revision is ES2 or later.
    */
    #if(CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES2)

        /* Restore saved wake up configuration */
        CY_PM_WAKEUP_CFG0_REG = cyPmBackup.wakeupCfg0;
        CY_PM_WAKEUP_CFG1_REG = cyPmBackup.wakeupCfg1;

        /* Device is PSoC 3 and the revision is ES3 or later */
        #if(CY_PM_PSOC3_ES3)

            CY_PM_WAKEUP_CFG2_REG = cyPmBackup.wakeupCfg2;

        #endif  /* End of (CY_PM_PSOC3_ES3) */

    #endif /* End of (CY_PM_PSOC3_ES3 || CY_PM_PSOC5_ES2) */
}


/*******************************************************************************
* Function Name: CyPMReadStatus
********************************************************************************
*
* Summary:
*  Manages the Power Manager Interrupt Status Register.  This register has the
*  interrupt status for the one pulse per second, central timewheel and fast
*  timewheel timers.  This hardware register clears on read.  To allow for only
*  clearing the bits of interest and preserving the other bits, this function
*  uses a shadow register that retains the state.  This function reads the
*  status register and ORs that value with the shadow register.  That is the
*  value that is returned.  Then the bits in the mask that are set are cleared
*  from this value and written back to the shadow register.
*
* Parameters:
*  mask: Bits in the shadow register to clear.
*
*       Define                      Source
*  CY_PM_FTW_INT                Fast Timewheel
*  CY_PM_CTW_INT                Central Timewheel
*  CY_PM_ONEPPS_INT             One Pulse Per Second
*
* Return:
*  Status.  Same bits values as the mask parameter.
*
* Reentrant:
*  No.
*
* Side Effects:
*  None.
*
*******************************************************************************/
uint8 CyPmReadStatus(uint8 mask)
{
    static uint8 interruptStatus;
    uint8 interruptState;
    uint8 tmpStatus;

    /* Enter critical section */
    interruptState = CyEnterCriticalSection();

    /* Save value of the register, copy it and clear desired bit */
    interruptStatus |= CY_PM_INT_SR_REG;
    tmpStatus = interruptStatus;
    interruptStatus &= ~mask;

    /* Exit critical section */
    CyExitCriticalSection(interruptState);

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: CyPmHibSaveSet
********************************************************************************
*
* Summary:
*  Prepare device for proper Hibernate low power mode entry: disable I2C backup
*  regulator, save enable state of the 1 kHz and 100 kHz ILO and disable them.
*  Save ILO power down mode state and enable it. Disable use_vccdclk_det.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmHibSaveSet(void)
{
    /* I2C backup reg must be off when the sleep reg is unavailable */
    if(0u != (CY_PM_PWRSYS_CR1_REG & CY_PM_PWRSYS_CR1_I2CREG_BACKUP))
    {
        /* I2C backup reg is enabled, save enable state and disable */
        cyPmBackup.i2cRegBackup = CY_PM_ENABLED;
        CY_PM_PWRSYS_CR1_REG &= ~CY_PM_PWRSYS_CR1_I2CREG_BACKUP;
    }
    else
    {
        /* Save disabled state of the I2C backup regulator */
        cyPmBackup.i2cRegBackup = CY_PM_DISABLED;
    }

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Save current ILO power mode and ensure low power mode */
        cyPmBackup.iloPowerMode = CyILO_SetPowerMode(CY_PM_POWERDOWN_MODE);
    
        /* Save current 1kHz ILO enable state. Disabled automatically. */
        cyPmBackup.ilo1kEnable = (0u != (CY_PM_SLOWCLK_ILO_CR0_REG & CY_PM_ILO_CR0_EN_1K)) ?
                                    CY_PM_ENABLED : CY_PM_DISABLED;
    
        /* Save current 100kHz ILO enable state. Disabled automatically. */
        cyPmBackup.ilo100kEnable = (0u != (CY_PM_SLOWCLK_ILO_CR0_REG & CY_PM_ILO_CR0_EN_100K)) ?
                                    CY_PM_ENABLED : CY_PM_DISABLED;

    #endif  /* End of (CY_PM_PSOC3_ES3) */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

	    if(0u == (CY_PM_PWRSYS_SLP_TR_REG & CY_PM_PWRSYS_SLP_TR_BYPASS))
	    {
	        /* Save current bypass state */
	        cyPmBackup.slpTrBypass = CY_PM_DISABLED;
        
	        /* When set, disables the sleep regulator and shorts vccd to vpwrsleep */
	        CY_PM_PWRSYS_SLP_TR_REG |= CY_PM_PWRSYS_SLP_TR_BYPASS;
	    }
	    else
	    {
	        /* Save current bypass state */
	        cyPmBackup.slpTrBypass = CY_PM_ENABLED;
	    }

	#endif  /* End of (CY_PM_PSOC3_ES3) */


    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Only affects when BOTH ext_vccd=1 and ext_vcca=1 */
        if((0u != (CY_PM_PWRSYS_CR0_REG & CY_PM_PWRSYS_CR0_EXT_VCCA)) &&
        (0u != (CY_PM_PWRSYS_CR0_REG & CY_PM_PWRSYS_CR0_EXT_VCCD)))
        {

            if(0u != CY_PM_PWRSYS_WAKE_TR2_REG & CY_PM_PWRSYS_WAKE_TR2_VCCD_CLK_DET)
            {
                /* Save alternate powergood detector state */
                cyPmBackup.vccdClkDet = CY_PM_ENABLED;

                /* Enable LPCOMPs (slower) instead of faster powergood detector */
                CY_PM_PWRSYS_WAKE_TR2_REG &= ~CY_PM_PWRSYS_WAKE_TR2_VCCD_CLK_DET;
            }
            else
            {
                /* Save alternate powergood detector state */
                cyPmBackup.vccdClkDet = CY_PM_DISABLED;
            }
        }

    #endif /* End of (CY_PM_PSOC3_ES3) */

    /* Tune LVI/HVI and wait for Hibernate/Sleep regulator to stabilize */
    CyPmHibSlpSaveSet(CY_PM_MODE_CSR_HIBERNATE);
}


/*******************************************************************************
* Function Name: CyPmHibRestore
********************************************************************************
*
* Summary:
*  Restore device for proper Hibernate mode exit: restore state of I2C backup
*  regulator, restore enable state of the 1 kHz ILO and 100 kHz ILO. Restore
*  ILO power down mode state.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmHibRestore(void) 
{
    /* Restore LVI/HVI configuration */
    CyPmHibSlpRestore();

    /* Restore I2C backup regulator configuration */
    if(CY_PM_ENABLED == cyPmBackup.i2cRegBackup)
    {
        /* Enable I2C backup regulator state */
        CY_PM_PWRSYS_CR1_REG |= CY_PM_PWRSYS_CR1_I2CREG_BACKUP;
    }

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)
    
        /* Restore 1kHz ILO enable state */
        if(CY_PM_ENABLED == cyPmBackup.ilo1kEnable)
        {
            /* Enable 1kHz ILO */
            CyILO_Start1K();
        }
    
        /* Restore 100kHz ILO enable state */
        if(CY_PM_ENABLED == cyPmBackup.ilo100kEnable)
        {
            /* Enable 100kHz ILO */
            CyILO_Start100K();
        }
    
        /* Restore ILO power mode */
        CyILO_SetPowerMode(cyPmBackup.iloPowerMode);

    #endif  /* End of (CY_PM_PSOC3_ES3) */

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)
    
        if(CY_PM_DISABLED == cyPmBackup.slpTrBypass)
        {
            /* Enable the sleep regulator and short vccd to vpwrsleep */
            CY_PM_PWRSYS_SLP_TR_REG &= ~CY_PM_PWRSYS_SLP_TR_BYPASS;    
        }

    #endif  /* End of (CY_PM_PSOC3_ES3) */
    
    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Restore alternate powergood detector state */
        if(CY_PM_ENABLED == cyPmBackup.vccdClkDet)
        {
            /* Enable LPCOMPs (slower) instead of faster powergood detector */
            CY_PM_PWRSYS_WAKE_TR2_REG |= CY_PM_PWRSYS_WAKE_TR2_VCCD_CLK_DET;
        }

    #endif /* End of (CY_PM_PSOC3_ES3) */
}


/*******************************************************************************
* Function Name: CyPmCtwSaveSet
********************************************************************************
*
* Summary:
*  Saves current CTW interval, ILO power mode, 1 kHz ILO enable state, CTW
*  interrupt enable state and CTW enable state. Sets new CTW interval, sets ILO
*  to the low power mode, enables 1 kHz ILO (if needed), disables CTW interrupt
*  and enables CTW (if needed).
*
* Parameters:
*  ctwInterval: the CTW interval to be set.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmCtwSaveSet(uint8 ctwInterval)
{
    /* Enable low power mode */
	CyILO_SetPowerMode(CY_PM_POWERDOWN_MODE);

    /* Disable CTW interrupt enable if not yet */
    if(0u != (CY_PM_TW_CFG2_REG & CY_PM_CTW_IE))
    {
        /* Disable CTW interrupt */
        CY_PM_TW_CFG2_REG &= ~CY_PM_CTW_IE;
    }   /* Do nothing if CTW interrupt is disabled */

    /* Enable 1kHz ILO if needed */
    if(0u == (CY_PM_SLOWCLK_ILO_CR0_REG & CY_PM_ILO_CR0_EN_1K))
    {
        /* Enable the 1kHz ILO */
        CyILO_Start1K();
    }
		
    /* Enable CTW and update interval if needed */
    if(0u != (CY_PM_TW_CFG2_REG & CY_PM_CTW_EN))
    {
		/* Set CTW interval if needed */
		if(CY_PM_TW_CFG1_REG != ctwInterval)
		{
			/* Disable the CTW, set new CTW interval and enable it again */
			CY_PM_TW_CFG2_REG &= ~CY_PM_CTW_EN;         
			CY_PM_TW_CFG1_REG = ctwInterval;
			CY_PM_TW_CFG2_REG |= CY_PM_CTW_EN;
		}
    }
    else
    {
		/* Set CTW interval if needed */
		if(CY_PM_TW_CFG1_REG != ctwInterval)
		{
			/* Set the new CTW interval. Could be changed if CTW is disabled */
			CY_PM_TW_CFG1_REG = ctwInterval;
		}

        /* Enable the CTW */
        CY_PM_TW_CFG2_REG |= CY_PM_CTW_EN;
    }
}


/*******************************************************************************
* Function Name: CyPmOppsSaveSet
********************************************************************************
*
* Summary:
*  Saves current 32kHz XTAL power mode state, saves current 32kHz XTAL enable
*  state, saves 1PPS interrupt enable state, saves 1PPS enable state. Enable
*  32kHz XTAL (if needed), disable 1PPS interrupt (if needed), enable 1PPS
*  (if needed).
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmOppsSaveSet(void)
{
    /* Set 32kHz XTAL low power mode */
	CyXTAL_32KHZ_SetPowerMode(CY_PM_POWERDOWN_MODE);

    /* Enable 32kHz XTAL if needed */
    if(0u == (CY_PM_SLOWCLK_X32_CR_REG & CY_PM_X32_CR_X32EN))
    {
        /* Enable 32kHz XTAL */
        CyXTAL_32KHZ_Start();
    }

    /* Disable 1PPS interrupt enable if needed */
    if(0u != (CY_PM_TW_CFG2_REG & CY_PM_1PPS_IE))
    {
        /* Disable 1PPS interrupt */
        CY_PM_TW_CFG2_REG &= ~CY_PM_1PPS_IE;
    }

    /* Enable 1PPS if needed */
    if(0u == (CY_PM_TW_CFG2_REG & CY_PM_1PPS_EN))
    {
        /* Enable 1PPS operation */
        CY_PM_TW_CFG2_REG |= CY_PM_1PPS_EN;
    }
}


/*******************************************************************************
* Function Name: CyPmFtwSaveSet
********************************************************************************
*
* Summary:
*  Saves current FTW interval and set new one, saves ILO power mode state and
*  sets low power mode, saves 100kHz ILO enable state and enables 100kHz ILO,
*  saves current FTW interrupt enable state and disable it if needed, saves
*  current FTW enable state and enable if needed.
*
* Parameters:
*  ftwInterval - FTW counter interval.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmFtwSaveSet(uint8 ftwInterval)
{
    /* Enable ILO low power mode */
    CyILO_SetPowerMode(CY_PM_POWERDOWN_MODE);

    /* Enable 100kHz ILO if needed */
    if(0u == (CY_PM_SLOWCLK_ILO_CR0_REG & CY_PM_ILO_CR0_EN_100K))
    {
        /* Enable the 100kHz ILO */
        CyILO_Start100K();
    }

    /* Enable FTW and set new FTW interval if needed */
    if(0u != (CY_PM_TW_CFG2_REG & CY_PM_FTW_EN))
    {
        /* Disable FTW, set new FTW interval if needed and enable it again */
		if(CY_PM_TW_CFG0_REG != ftwInterval)
		{
			/* Disable the CTW, set new CTW interval and enable it again */
            CY_PM_TW_CFG2_REG &= ~CY_PM_FTW_EN;
            CY_PM_TW_CFG0_REG = ftwInterval;
            CY_PM_TW_CFG2_REG |= CY_PM_FTW_EN;
		}
    }
    else
    {
        /* Set new FTW counter interval if nedded. FTW is disabled. */
		if(CY_PM_TW_CFG0_REG != ftwInterval)
		{
			/* Set the new CTW interval. Could be changed if CTW is disabled */
			CY_PM_TW_CFG0_REG = ftwInterval;
		}

        /* Enable the FTW */
        CY_PM_TW_CFG2_REG |= CY_PM_FTW_EN;
    }

    /* Disable FTW interrupt enable if needed */
    if(0u != (CY_PM_TW_CFG2_REG & CY_PM_FTW_IE))
    {
        /* Disable FTW interrupt */
        CY_PM_TW_CFG2_REG &= ~CY_PM_FTW_IE;
    }
}


/*******************************************************************************
* Function Name: CyPmHibSlpSaveSet
********************************************************************************
*
* Summary:
*  Saves en_lvia, en_lvid, en_hvi enable state, saves wakeup trims (timeout and
*  holdoff). Disable LVI/HVI if entering hibernate power mode to avoid spurious
*  interrupts on wakeup. For the Sleep power mode, leaves LVI/HVI as is but
*  update wakeup time if they are used.
*
*  Before entering a mode that can lose state, make sure pwrup_pulse is finished
*  and clear the powerup pulse holdoff setting.
*
* Parameters:
*  CY_PM_MODE_CSR_HIBERNATE - prepare for the Hibernate low power mode.
*  CY_PM_MODE_CSR_SLEEP     - prepare for the Sleep low power mode.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
* Side Effects:
*  Currently, the is no way to define device grade. This API method must be
*  invoked only for I- & A-grades.
*
*  This API could be blocking as it waits for Hibernate/Sleep regulator to
*  stabilize.
*
*******************************************************************************/
void CyPmHibSlpSaveSet(uint8 powerMode)
{
    /* Save en_lvia, en_lvid, en_hvi */
    cyPmBackup.lviaLvidHvi = (CY_PM_RESET_CR1_REG & CY_PM_RESET_CR1_HVI_LVI_EN_MASK);

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)
    
        /* Save SC/CT routing registers */
        cyPmBackup.scct_data[0u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW0 );
        cyPmBackup.scct_data[1u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW2 );
        cyPmBackup.scct_data[2u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW3 );
        cyPmBackup.scct_data[3u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW4 );
        cyPmBackup.scct_data[4u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW6 );
        cyPmBackup.scct_data[5u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW8 );
        cyPmBackup.scct_data[6u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC0_SW10);
    
        cyPmBackup.scct_data[7u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW0 );
        cyPmBackup.scct_data[8u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW2 );
        cyPmBackup.scct_data[9u]   = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW3 );
        cyPmBackup.scct_data[10u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW4 );
        cyPmBackup.scct_data[11u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW6 );
        cyPmBackup.scct_data[12u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW8 );
        cyPmBackup.scct_data[13u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC1_SW10);
    
        cyPmBackup.scct_data[14u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW0 );
        cyPmBackup.scct_data[15u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW2 );
        cyPmBackup.scct_data[16u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW3 );
        cyPmBackup.scct_data[17u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW4 );
        cyPmBackup.scct_data[18u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW6 );
        cyPmBackup.scct_data[19u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW8 );
        cyPmBackup.scct_data[20u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC2_SW10);
    
        cyPmBackup.scct_data[21u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW0 );
        cyPmBackup.scct_data[22u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW2 );
        cyPmBackup.scct_data[23u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW3 );
        cyPmBackup.scct_data[24u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW4 );
        cyPmBackup.scct_data[25u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW6 );
        cyPmBackup.scct_data[26u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW8 );
        cyPmBackup.scct_data[27u]  = CY_GET_REG8(CYDEV_ANAIF_RT_SC3_SW10);

        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW0 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW2 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW3 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW4 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW6 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW8 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW10, 0u);
    
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW0 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW2 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW3 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW4 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW6 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW8 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW10, 0u);
    
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW0 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW2 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW3 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW4 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW6 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW8 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW10, 0u);
    
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW0 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW2 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW3 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW4 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW6 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW8 , 0u);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW10, 0u);

        /* LVI/HVI workaround  - save wake up trims */
        cyPmBackup.wakeupTrimTimeout= CY_PM_PWRSYS_WAKE_TR0_REG;
        cyPmBackup.wakeupTrim1      = CY_PM_PWRSYS_WAKE_TR1_REG;
        cyPmBackup.wakeupTrim2      = CY_PM_PWRSYS_WAKE_TR2_REG;
        cyPmBackup.wakeupTrimHoldoff= CY_PM_PWRSYS_WAKE_TR3_REG;

        /* Enable replica regulated vcca supply state */
        CY_PM_PWRSYS_WAKE_TR2_REG |= CY_PM_PWRSYS_WAKE_TR2_USE_VCCA_BACKUP;
                
        switch(powerMode)
        {
            case CY_PM_MODE_CSR_HIBERNATE:

                if(0u == (CY_PM_PWRSYS_HIB_TR1_REG & CY_PM_PWRSYS_HIB_TRIM))
                {
                    /* Disable LVI/HVI for hibernate - clear en_lvia, en_lvid, en_hvi */
                    CY_PM_RESET_CR1_REG &= ~CY_PM_RESET_CR1_HVI_LVI_EN_MASK;
                    break;
                }
                /* Intentional fall through */

            case CY_PM_MODE_CSR_SLEEP:

                /* Reconfigure power mode wakeup trim registers */
                if(0u != (CY_PM_RESET_CR1_REG & (CY_PM_RESET_CR1_HVIA_EN | 
                          CY_PM_RESET_CR1_LVIA_EN | CY_PM_RESET_CR1_LVID_EN)))
                {
                    CY_PM_PWRSYS_WAKE_TR0_REG = CY_PM_PWRSYS_WAKE_TR0_LHVI_DIS;
                    CY_PM_PWRSYS_WAKE_TR1_REG = CY_PM_PWRSYS_WAKE_TR1_LHVI_DIS;
                    CY_PM_PWRSYS_WAKE_TR2_REG = CY_PM_PWRSYS_WAKE_TR2_LHVI_DIS;
                    CY_PM_PWRSYS_WAKE_TR3_REG = CY_PM_PWRSYS_WAKE_TR3_LHVI_DIS;
                }
                else
                {
                    /* Extend wakeup timeline */
                    CY_PM_PWRSYS_WAKE_TR0_REG = CY_PM_PWRSYS_WAKE_TIMEOUT_IA;
                    CY_PM_PWRSYS_WAKE_TR3_REG = CY_PM_PWRSYS_WAKE_HOLFOFF_IA;
                }
                break;

            default:

                /* No action */
                break;
        }   /* End of switch(powerMode) */
        
        /* Serial Wire Viewer (SWV) workaround */
    
        /* Disable SWV before entering low power mode */
        if(0u != (CY_PM_MLOGIC_DBG_REG & CY_PM_MLOGIC_DBG_SWV_CLK_EN))
        {
            /* Save SWV clock enabled state */
            cyPmBackup.swvClkEnabled = CY_PM_ENABLED;
            
            /* Save current ports drive mode settings */
            cyPmBackup.prt1Dm = ((CY_PM_PRT1_PC3_REG & CY_PM_PRT1_PC3_DM_MASK) |
                                CY_PM_PRT1_PC3_DM_STRONG);
    
            /* Disable SWV clocks */
            CY_PM_MLOGIC_DBG_REG &= ~CY_PM_MLOGIC_DBG_SWV_CLK_EN;
        }
        else
        {
            /* Save SWV clock disabled state */
            cyPmBackup.swvClkEnabled = CY_PM_DISABLED;
        }
    
    #else   /* Not PSoC3 ES3 */

        /* Disable LVI/HVI to save power - doesn't work during buzzing in these revisions */
        CY_PM_RESET_CR1_REG &= ~CY_PM_RESET_CR1_HVI_LVI_EN_MASK;

        /* Suppress unreferenced local variable warning */
        powerMode++;
        
    #endif  /* End of (CY_PM_PSOC3_ES3) */

}


/*******************************************************************************
* Function Name: CyPmHibSlpRestore
********************************************************************************
*
* Summary:
*  Restores en_lvia, en_lvid, en_hvi enable state and wakeup trims (timeout and
*  holdoff).
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
*******************************************************************************/
void CyPmHibSlpRestore(void) 
{
    /* Restore en_lvia, en_lvid, en_hvi */
    CY_PM_RESET_CR1_REG = cyPmBackup.lviaLvidHvi;

    /* Device is PSoC 3 and the revision is ES3 or later */
    #if(CY_PM_PSOC3_ES3)

        /* Restore SC/CT routing registers */
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW0 , cyPmBackup.scct_data[0u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW2 , cyPmBackup.scct_data[1u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW3 , cyPmBackup.scct_data[2u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW4 , cyPmBackup.scct_data[3u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW6 , cyPmBackup.scct_data[4u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW8 , cyPmBackup.scct_data[5u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC0_SW10, cyPmBackup.scct_data[6u] );

        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW0 , cyPmBackup.scct_data[7u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW2 , cyPmBackup.scct_data[8u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW3 , cyPmBackup.scct_data[9u] );
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW4 , cyPmBackup.scct_data[10u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW6 , cyPmBackup.scct_data[11u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW8 , cyPmBackup.scct_data[12u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC1_SW10, cyPmBackup.scct_data[13u]);

        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW0 , cyPmBackup.scct_data[14u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW2 , cyPmBackup.scct_data[15u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW3 , cyPmBackup.scct_data[16u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW4 , cyPmBackup.scct_data[17u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW6 , cyPmBackup.scct_data[18u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW8 , cyPmBackup.scct_data[19u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC2_SW10, cyPmBackup.scct_data[20u]);

        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW0 , cyPmBackup.scct_data[21u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW2 , cyPmBackup.scct_data[22u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW3 , cyPmBackup.scct_data[23u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW4 , cyPmBackup.scct_data[24u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW6 , cyPmBackup.scct_data[25u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW8 , cyPmBackup.scct_data[26u]);
        CY_SET_REG8(CYDEV_ANAIF_RT_SC3_SW10, cyPmBackup.scct_data[27u]);        

        /* Restore wake up trims */ 
        CY_PM_PWRSYS_WAKE_TR0_REG = cyPmBackup.wakeupTrimTimeout;
        CY_PM_PWRSYS_WAKE_TR1_REG = cyPmBackup.wakeupTrim1;
        CY_PM_PWRSYS_WAKE_TR2_REG = cyPmBackup.wakeupTrim2;
        CY_PM_PWRSYS_WAKE_TR3_REG = cyPmBackup.wakeupTrimHoldoff;

        /* Serial Wire Viewer (SWV) workaround */
        if(CY_PM_ENABLED == cyPmBackup.swvClkEnabled)
        {
            /* Restore ports drive mode */
            cyPmBackup.prt1Dm = ((CY_PM_PRT1_PC3_REG & CY_PM_PRT1_PC3_DM_MASK) |
                                    cyPmBackup.prt1Dm);
    
            /* Enable SWV clocks */
            CY_PM_MLOGIC_DBG_REG |= CY_PM_MLOGIC_DBG_SWV_CLK_EN;
        }
    
    #endif /* End of (CY_PM_PSOC3_ES3) */
}

/* [] END OF FILE */

