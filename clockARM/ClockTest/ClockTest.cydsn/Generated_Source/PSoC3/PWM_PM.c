/*******************************************************************************
* File Name: PWM_PM.c
* Version 2.0
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "cytypes.h"
#include "PWM.h"

static PWM_backupStruct PWM_backup;


/*******************************************************************************
* Function Name: PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PWM_SaveConfig(void)
{
    
    #if(!PWM_UsingFixedFunction)
        #if (PWM_PSOC3_ES2 || PWM_PSOC5_ES1)
            PWM_backup.PWMUdb = PWM_ReadCounter();
            PWM_backup.PWMPeriod = PWM_ReadPeriod();
            #if (PWM_UseStatus)
				PWM_backup.InterruptMaskValue = PWM_STATUS_MASK;
            #endif
			
            #if(PWM_UseOneCompareMode)
                PWM_backup.PWMCompareValue = PWM_ReadCompare();
            #else
                PWM_backup.PWMCompareValue1 = PWM_ReadCompare1();
                PWM_backup.PWMCompareValue2 = PWM_ReadCompare2();
            #endif
            
           #if(PWM_DeadBandUsed)
                PWM_backup.PWMdeadBandValue = PWM_ReadDeadTime();
            #endif
          
            #if ( PWM_KillModeMinTime)
                PWM_backup.PWMKillCounterPeriod = PWM_ReadKillTime();
            #endif
        #endif
        
        #if (PWM_PSOC3_ES3 || PWM_PSOC5_ES2)
            PWM_backup.PWMUdb = PWM_ReadCounter();
            #if (PWM_UseStatus)
				PWM_backup.InterruptMaskValue = PWM_STATUS_MASK;
			#endif
            
            #if(PWM_DeadBandMode == PWM__B_PWM__DBM_256_CLOCKS || PWM_DeadBandMode == PWM__B_PWM__DBM_2_4_CLOCKS)
                PWM_backup.PWMdeadBandValue = PWM_ReadDeadTime();
            #endif
            
            #if(PWM_KillModeMinTime)
                 PWM_backup.PWMKillCounterPeriod = PWM_ReadKillTime();
            #endif
        #endif
        
        #if(PWM_UseControl)
            PWM_backup.PWMControlRegister = PWM_ReadControlRegister();
        #endif
    #endif  
}


/*******************************************************************************
* Function Name: PWM_RestoreConfig
********************************************************************************
* 
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_backup:  Variables of this global structure are used to restore 
*  the values of non retention registers on wakeup from sleep mode.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void PWM_RestoreConfig(void) 
{
        #if(!PWM_UsingFixedFunction)
            #if (PWM_PSOC3_ES2 || PWM_PSOC5_ES1)
                PWM_WriteCounter(PWM_backup.PWMUdb);
                PWM_WritePeriod(PWM_backup.PWMPeriod);
                #if (PWM_UseStatus)
					PWM_STATUS_MASK = PWM_backup.InterruptMaskValue;
                #endif
				
                #if(PWM_UseOneCompareMode)
                    PWM_WriteCompare(PWM_backup.PWMCompareValue);
                #else
                    PWM_WriteCompare1(PWM_backup.PWMCompareValue1);
                    PWM_WriteCompare2(PWM_backup.PWMCompareValue2);
                #endif
                
               #if(PWM_DeadBandMode == PWM__B_PWM__DBM_256_CLOCKS || PWM_DeadBandMode == PWM__B_PWM__DBM_2_4_CLOCKS)
                    PWM_WriteDeadTime(PWM_backup.PWMdeadBandValue);
                #endif
            
                #if ( PWM_KillModeMinTime)
                    PWM_WriteKillTime(PWM_backup.PWMKillCounterPeriod);
                #endif
            #endif
            
            #if (PWM_PSOC3_ES3 || PWM_PSOC5_ES2)
                PWM_WriteCounter(PWM_backup.PWMUdb);
                #if (PWM_UseStatus)
                    PWM_STATUS_MASK = PWM_backup.InterruptMaskValue;
                #endif
                
                #if(PWM_DeadBandMode == PWM__B_PWM__DBM_256_CLOCKS || PWM_DeadBandMode == PWM__B_PWM__DBM_2_4_CLOCKS)
                    PWM_WriteDeadTime(PWM_backup.PWMdeadBandValue);
                #endif
                
                #if(PWM_KillModeMinTime)
                    PWM_WriteKillTime(PWM_backup.PWMKillCounterPeriod);
                #endif
            #endif
            
            #if(PWM_UseControl)
                PWM_WriteControlRegister(PWM_backup.PWMControlRegister); 
            #endif
        #endif  
    }


/*******************************************************************************
* Function Name: PWM_Sleep
********************************************************************************
* 
* Summary:
*  Disables block's operation and saves the user configuration. Should be called 
*  just prior to entering sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_backup.PWMEnableState:  Is modified depending on the enable state
*  of the block before entering sleep mode.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void PWM_Sleep(void)
{
    #if(PWM_UseControl)
        if(PWM_CTRL_ENABLE == (PWM_CONTROL & PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_backup.PWMEnableState = 0u;
        }
    #endif
    /* Stop component */
    PWM_Stop();
    
    /* Save registers configuration */
    PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Wakeup
********************************************************************************
* 
* Summary:
*  Restores and enables the user configuration. Should be called just after 
*  awaking from sleep.
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  PWM_backup.pwmEnable:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
* Reentrant:
*  Yes
*
*******************************************************************************/
void PWM_Wakeup(void) 
{
     /* Restore registers values */
    PWM_RestoreConfig();
    
    if(PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Enable();
    } /* Do nothing if component's block was disabled before */
    
}

/* [] END OF FILE */
