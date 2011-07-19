/*******************************************************************************
* File Name: PRISM.c
* Version 2.0
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality of the PrISM component
*
* Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PRISM.h"


/***************************************
* Forward function references
***************************************/
void PRISM_Enable(void);


/***************************************
* Local data allocation
***************************************/
static PRISM_BACKUP_STRUCT  PRISM_backup = 
{
    /* enableState */
    0u,
    /* cr */
    #if(!PRISM_PULSE_TYPE_HARDCODED)
        (PRISM_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PRISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL : 0) |
        (PRISM_GREATERTHAN_OR_EQUAL == 0 ? \
                                                PRISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL : 0),
    #endif /* End PRISM_PULSE_TYPE_HARDCODED */
    /* seed */    
    PRISM_SEED,
    /* seed_copy */    
    PRISM_SEED,
    /* polynom */
    PRISM_POLYNOM,
    #if(PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1) /* PSoC3 ES2 or early, PSoC5 ES1*/
        /* density0 */
        PRISM_DENSITY0,
        /* density1 */
        PRISM_DENSITY1,
    #endif /*End PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1*/
};


/*******************************************************************************
* Function Name: PRISM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRISM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRISM_SaveConfig(void)
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1)

        #if(!PRISM_PULSE_TYPE_HARDCODED)
            PRISM_backup.cr = PRISM_CONTROL_REG;
        #endif /* End PRISM_PULSE_TYPE_HARDCODED */
        PRISM_backup.seed = PRISM_ReadSeed();
        PRISM_backup.seed_copy = CY_GET_REG16(PRISM_SEED_COPY_PTR);
        PRISM_backup.polynom = PRISM_ReadPolynomial();
        PRISM_backup.density0 = PRISM_ReadPulse0();
        PRISM_backup.density1 = PRISM_ReadPulse1();
    
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (PRISM_PSOC3_ES3 || PRISM_PSOC5_ES2)

        #if(!PRISM_PULSE_TYPE_HARDCODED)
            PRISM_backup.cr = PRISM_CONTROL_REG;
        #endif /* End PRISM_PULSE_TYPE_HARDCODED */
        PRISM_backup.seed = PRISM_ReadSeed();
        PRISM_backup.seed_copy = CY_GET_REG16(PRISM_SEED_COPY_PTR);
        PRISM_backup.polynom = PRISM_ReadPolynomial();
    
    #endif  /* Unknown chip die is not taken into account */
}


/*******************************************************************************
* Function Name: PRISM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRISM_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PRISM_RestoreConfig(void) 
{
    /* PSoC3 ES2 or early, PSoC5 ES1*/
    #if (PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1)
    
        uint8 enableInterrupts;
        
        #if(!PRISM_PULSE_TYPE_HARDCODED)
            PRISM_CONTROL_REG = PRISM_backup.cr;
        #endif /* End PRISM_PULSE_TYPE_HARDCODED */

        CY_SET_REG16(PRISM_SEED_COPY_PTR, PRISM_backup.seed_copy);
        CY_SET_REG16(PRISM_SEED_PTR, PRISM_backup.seed);
        PRISM_WritePolynomial(PRISM_backup.polynom);
        PRISM_WritePulse0(PRISM_backup.density0);
        PRISM_WritePulse1(PRISM_backup.density1);
        
        enableInterrupts = CyEnterCriticalSection();
        /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
        #if (PRISM_RESOLUTION <= 8u)      /* 8bit - PrISM */
            PRISM_AUX_CONTROL_REG |= PRISM_FIFO0_CLR;
        #elif (PRISM_RESOLUTION <= 16u)   /* 16bit - PrISM */
            CY_SET_REG16(PRISM_AUX_CONTROL_PTR, CY_GET_REG16(PRISM_AUX_CONTROL_PTR) | 
                                            PRISM_FIFO0_CLR | PRISM_FIFO0_CLR << 8u);
        #elif (PRISM_RESOLUTION <= 24)   /* 24bit - PrISM */
            CY_SET_REG24(PRISM_AUX_CONTROL_PTR, CY_GET_REG24(PRISM_AUX_CONTROL_PTR) |
                                            PRISM_FIFO0_CLR | PRISM_FIFO0_CLR << 8u );
            CY_SET_REG24(PRISM_AUX_CONTROL2_PTR, CY_GET_REG24(PRISM_AUX_CONTROL2_PTR) | 
                                            PRISM_FIFO0_CLR );
        #else                                 /* 32bit - PrISM */
            CY_SET_REG32(PRISM_AUX_CONTROL_PTR, CY_GET_REG32(PRISM_AUX_CONTROL_PTR) |
                                            PRISM_FIFO0_CLR | PRISM_FIFO0_CLR << 8u );
            CY_SET_REG32(PRISM_AUX_CONTROL2_PTR, CY_GET_REG32(PRISM_AUX_CONTROL2_PTR) |
                                            PRISM_FIFO0_CLR | PRISM_FIFO0_CLR << 8u );
        #endif                                /* End PRISM_RESOLUTION */
        CyExitCriticalSection(enableInterrupts);
   
    /* PSoC3 ES3 or later, PSoC5 ES2 or later*/
    #elif (PRISM_PSOC3_ES3 || PRISM_PSOC5_ES2)

        #if(!PRISM_PULSE_TYPE_HARDCODED)
            PRISM_CONTROL_REG = PRISM_backup.cr;
        #endif /* End PRISM_PULSE_TYPE_HARDCODED */

        CY_SET_REG16(PRISM_SEED_COPY_PTR, PRISM_backup.seed_copy);
        CY_SET_REG16(PRISM_SEED_PTR, PRISM_backup.seed);
        PRISM_WritePolynomial(PRISM_backup.polynom);
    
    #endif  /* End PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1 */
}


/*******************************************************************************
* Function Name: PRISM_Sleep
********************************************************************************
*
* Summary:
*  Stops and saves the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRISM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PRISM_Sleep(void)
{
    #if(!PRISM_PULSE_TYPE_HARDCODED)
        if((PRISM_CONTROL_REG & PRISM_CTRL_ENABLE) != 0u) 
        {
            PRISM_backup.enableState = 1u;
        }
        else
        {
            PRISM_backup.enableState = 0u;
        }
    #endif /* End PRISM_PULSE_TYPE_HARDCODED */
    PRISM_Stop();
    PRISM_SaveConfig();
}


/*******************************************************************************
* Function Name: PRISM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  PRISM_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void PRISM_Wakeup(void) 
{
    PRISM_RestoreConfig();
    if(PRISM_backup.enableState != 0u)
    {
        PRISM_Enable();
    } 
}


/* [] END OF FILE */
