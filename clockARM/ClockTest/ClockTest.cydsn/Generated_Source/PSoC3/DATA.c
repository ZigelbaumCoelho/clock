/*******************************************************************************
* File Name: DATA.c  
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
#include "DATA.h"


/*******************************************************************************
* Function Name: DATA_Write
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
void DATA_Write(uint8 value) 
{
    uint8 staticBits = DATA_DR & ~DATA_MASK;
    DATA_DR = staticBits | ((value << DATA_SHIFT) & DATA_MASK);
}


/*******************************************************************************
* Function Name: DATA_SetDriveMode
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
void DATA_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(DATA_0, mode);
}


/*******************************************************************************
* Function Name: DATA_Read
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
*  Macro DATA_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 DATA_Read(void) 
{
    return (DATA_PS & DATA_MASK) >> DATA_SHIFT;
}


/*******************************************************************************
* Function Name: DATA_ReadDataReg
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
uint8 DATA_ReadDataReg(void) 
{
    return (DATA_DR & DATA_MASK) >> DATA_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(DATA_INTSTAT) 

    /*******************************************************************************
    * Function Name: DATA_ClearInterrupt
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
    uint8 DATA_ClearInterrupt(void) 
    {
        return (DATA_INTSTAT & DATA_MASK) >> DATA_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */ 
