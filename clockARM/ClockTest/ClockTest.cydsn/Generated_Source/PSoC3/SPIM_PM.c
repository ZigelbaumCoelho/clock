/*******************************************************************************
* File Name: SPIM_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPIM.h"

static SPIM_BACKUP_STRUCT SPIM_backup = {
                                        0u,
                                        SPIM_BITCTR_INIT,
                                        #if(SPIM_PSOC3_ES2 || SPIM_PSOC5_ES1)
                                            SPIM_TX_INIT_INTERRUPTS_MASK,
                                            SPIM_RX_INIT_INTERRUPTS_MASK
                                        #endif /* SPIM_PSOC3_ES2 || SPIM_PSOC5_ES1 */
                                        };

#if (SPIM_TXBUFFERSIZE > 4u)

    extern volatile uint8 SPIM_txBufferRead;
    extern volatile uint8 SPIM_txBufferWrite;
    
#endif /* SPIM_TXBUFFERSIZE > 4u */

#if (SPIM_RXBUFFERSIZE > 4u)

    extern volatile uint8 SPIM_rxBufferRead;
    extern volatile uint8 SPIM_rxBufferWrite;
    
#endif /* SPIM_RXBUFFERSIZE > 4u */


/*******************************************************************************
* Function Name: SPIM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
* 
* Global Variables:
*  SPIM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_SaveConfig(void)
{
    /* Store Status Mask registers */
    #if (SPIM_PSOC3_ES2 || SPIM_PSOC5_ES1)     
    
       SPIM_backup.saveSrTxIntMask = SPIM_TX_STATUS_MASK_REG;
       SPIM_backup.saveSrRxIntMask = SPIM_RX_STATUS_MASK_REG;
       SPIM_backup.cntrPeriod = SPIM_COUNTER_PERIOD_REG;
       
    #endif /* (SPIM_PSOC3_ES2 || SPIM_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: SPIM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPIM_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer: 
*  SPIM_STATUS_MASK_REG and SPIM_COUNTER_PERIOD_REG.
*
*******************************************************************************/
void SPIM_RestoreConfig(void) 
{
    /* Restore the data, saved by SaveConfig() function */
    #if (SPIM_PSOC3_ES2 || SPIM_PSOC5_ES1)
    
        SPIM_TX_STATUS_MASK_REG = SPIM_backup.saveSrTxIntMask;
        SPIM_RX_STATUS_MASK_REG = SPIM_backup.saveSrRxIntMask;
        SPIM_COUNTER_PERIOD_REG = SPIM_backup.cntrPeriod;
        
    #endif /* (SPIM_PSOC3_ES2 || SPIM_PSOC5_ES1) */
}


/*******************************************************************************
* Function Name: SPIM_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  SPIM_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_Sleep(void)
{
    /* Save components enable state */
    if ((SPIM_TX_STATUS_ACTL_REG & SPIM_INT_ENABLE) == SPIM_INT_ENABLE)
    {
        SPIM_backup.enableState = 1u;
    }
    else /* Components block is disabled */
    {
        SPIM_backup.enableState = 0u;
    }

    SPIM_Stop();

    SPIM_SaveConfig();
}


/*******************************************************************************
* Function Name: SPIM_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
* Global Variables:
*  SPIM_backup - used when non-retention registers are restored.
*  SPIM_txBufferWrite - modified every function call - resets to 
*  zero.
*  SPIM_txBufferRead - modified every function call - resets to 
*  zero.
*  SPIM_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPIM_rxBufferRead - modified every function call - resets to
*  zero. 
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPIM_Wakeup(void)
{        
    SPIM_RestoreConfig();
         
    #if (SPIM_TXBUFFERSIZE > 4u)
    
        SPIM_txBufferRead = 0u;
        SPIM_txBufferWrite = 0u;
        
    #endif /* SPIM_TXBUFFERSIZE > 4u */
    
    #if (SPIM_RXBUFFERSIZE > 4u)    
    
        SPIM_rxBufferRead = 0u;
        SPIM_rxBufferWrite = 0u;
        
    #endif /* SPIM_RXBUFFERSIZE > 4u */ 
    
    SPIM_ClearFIFO();
    
    /* Restore components block enable state */
    if (SPIM_backup.enableState != 0u)
    {
         /* Components block was enabled */
         SPIM_Enable();
    } /* Do nothing if components block was disabled */
}


/* [] END OF FILE */
