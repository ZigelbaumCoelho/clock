/*******************************************************************************
* File Name: LE.c  
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
#include "LE.h"


/*******************************************************************************
* Function Name: LE_Write
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
void LE_Write(uint8 value) 
{
    uint8 staticBits = LE_DR & ~LE_MASK;
    LE_DR = staticBits | ((value << LE_SHIFT) & LE_MASK);
}


/*******************************************************************************
* Function Name: LE_SetDriveMode
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
void LE_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(LE_0, mode);
}


/*******************************************************************************
* Function Name: LE_Read
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
*  Macro LE_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 LE_Read(void) 
{
    return (LE_PS & LE_MASK) >> LE_SHIFT;
}


/*******************************************************************************
* Function Name: LE_ReadDataReg
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
uint8 LE_ReadDataReg(void) 
{
    return (LE_DR & LE_MASK) >> LE_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(LE_INTSTAT) 

    /*******************************************************************************
    * Function Name: LE_ClearInterrupt
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
    uint8 LE_ClearInterrupt(void) 
    {
        return (LE_INTSTAT & LE_MASK) >> LE_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
