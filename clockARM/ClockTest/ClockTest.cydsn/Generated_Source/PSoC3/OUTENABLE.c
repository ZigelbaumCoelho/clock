/*******************************************************************************
* File Name: OUTENABLE.c  
* Version 1.50
*
* Description:
*  This file contains API to enable firmware control of a control register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "OUTENABLE.h"

#if !defined(OUTENABLE_ctrl_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: OUTENABLE_Write
********************************************************************************
*
* Summary:
*  Write a byte to a control register.
*
* Parameters:  
*  control:  The value to be assigned to the control register. 
*
* Return: 
*  void 
*
* Reentrant:
*  Yes
*  
*******************************************************************************/
void OUTENABLE_Write(uint8 control) 
{
    OUTENABLE_Control = control;
}


/*******************************************************************************
* Function Name: OUTENABLE_Read
********************************************************************************
*
* Summary:
*  Read the current value assigned to a control register.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the control register
*
* Reentrant:
*  Yes
*  
*******************************************************************************/
uint8 OUTENABLE_Read(void) 
{
    return OUTENABLE_Control;
}

#endif /* End check for removal by optimization */
