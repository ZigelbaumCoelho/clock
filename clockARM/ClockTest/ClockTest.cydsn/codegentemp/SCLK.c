/*******************************************************************************
* File Name: SCLK.c  
* Version 1.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
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
#include "SCLK.h"


/*******************************************************************************
* Function Name: SCLK_Write
********************************************************************************
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  void 
*  
*******************************************************************************/
void SCLK_Write(uint8 value) 
{
    uint8 staticBits = SCLK_DR & ~SCLK_MASK;
    SCLK_DR = staticBits | ((value << SCLK_SHIFT) & SCLK_MASK);
}


/*******************************************************************************
* Function Name: SCLK_SetDriveMode
********************************************************************************
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  void
*
*******************************************************************************/
void SCLK_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(SCLK_0, mode);
}


/*******************************************************************************
* Function Name: SCLK_Read
********************************************************************************
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro SCLK_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 SCLK_Read(void) 
{
    return (SCLK_PS & SCLK_MASK) >> SCLK_SHIFT;
}


/*******************************************************************************
* Function Name: SCLK_ReadDataReg
********************************************************************************
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  void 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 SCLK_ReadDataReg(void) 
{
    return (SCLK_DR & SCLK_MASK) >> SCLK_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(SCLK_INTSTAT) 

    /*******************************************************************************
    * Function Name: SCLK_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  void 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 SCLK_ClearInterrupt(void) 
    {
        return (SCLK_INTSTAT & SCLK_MASK) >> SCLK_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
