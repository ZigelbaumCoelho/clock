/*******************************************************************************
* File Name: PRISM.c
* Version 2.0
*
* Description:
*  This file provides the source code of the API for the PrISM Component.
*
* Note:
*  The PRiSM Component consists of a 8, 16, 24, 32 - bit PrISM, it
*  depends on length Polynomial value and user selected Seed Value. 
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PRISM.h"


/***************************************
* Local data allocation
***************************************/

uint8 PRISM_initVar = 0u;


/*******************************************************************************
* FUNCTION NAME:   PRISM_Start
********************************************************************************
*
* Summary:
*  The start function sets Polynomial, Seed and Pulse Density registers provided
*  by customizer. PrISM computation starts on rising edge of input clock.
*
* Parameters:  
*  None.
*
* Return:  
*  None.
*
* Global variables:
*  The PRISM_initVar variable is used to indicate initial
*  configuration of this component.  The variable is initialized to zero and
*  set to 1 the first time PRISM_Start() is called. This allows for
*  component initialization without re-initialization in all subsequent calls
*  to the PRISM_Start() routine. 

* Reentrant:
*  No.
*
*******************************************************************************/
void PRISM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PRISM_initVar == 0u)
    {
        PRISM_Init();
        PRISM_initVar = 1u;
    }
    PRISM_Enable();

}


/*******************************************************************************
* Function Name: PRISM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PRISM_Start().
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PRISM_Init(void) 
{
    uint8 enableInterrupts;
    
    /* Writes Seed value, polynom value and density provided by customizer */
    PRISM_WriteSeed(PRISM_SEED);
    PRISM_WritePolynomial(PRISM_POLYNOM);
    PRISM_WritePulse0(PRISM_DENSITY0);
    PRISM_WritePulse1(PRISM_DENSITY1);
    
    enableInterrupts = CyEnterCriticalSection();
    /* Set FIFO0_CLR bit to use FIFO0 as a simple one-byte buffer*/
    #if (PRISM_RESOLUTION <= 8u)      /* 8bit - PrISM */
        PRISM_AUX_CONTROL_REG |= PRISM_FIFO0_CLR;
    #elif (PRISM_RESOLUTION <= 16u)   /* 16bit - PrISM */
        CY_SET_REG16(PRISM_AUX_CONTROL_PTR, CY_GET_REG16(PRISM_AUX_CONTROL_PTR) | 
                                        PRISM_FIFO0_CLR | PRISM_FIFO0_CLR << 8u);
    #elif (PRISM_RESOLUTION <= 24u)   /* 24bit - PrISM */
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
    
    #if(!PRISM_PULSE_TYPE_HARDCODED)
        /* Writes density type provided by customizer */
        if(PRISM_GREATERTHAN_OR_EQUAL == 0 )
        {
            PRISM_CONTROL_REG |= PRISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PRISM_CONTROL_REG &= ~PRISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    
        if(PRISM_GREATERTHAN_OR_EQUAL == 0)
        {
            PRISM_CONTROL_REG |= PRISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PRISM_CONTROL_REG &= ~PRISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    #endif /* End PRISM_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* Function Name: PRISM_Enable
********************************************************************************
*  
* Summary: 
*  Enables the PrISM block operation
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PRISM_Enable(void) 
{
    #if(!PRISM_PULSE_TYPE_HARDCODED)
        /* Enable the PrISM computation */
        PRISM_CONTROL_REG |= PRISM_CTRL_ENABLE;
    #endif /* End PRISM_PULSE_TYPE_HARDCODED */
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_Stop
********************************************************************************
*
* Summary:
*  Stops PrISM computation. Outputs remain constant.
*
* Parameters:  
*  None.
*
* Return: 
*  None.
*
*******************************************************************************/
void PRISM_Stop(void) 
{
    #if(!PRISM_PULSE_TYPE_HARDCODED)
        PRISM_CONTROL_REG &= ~PRISM_CTRL_ENABLE;
    #else
        /* PulseTypeHardoded option enabled - to stop PrISM use enable input */
    #endif /* End $INSTANCE_NAME`_PULSE_TYPE_HARDCODED */
}

#if(!PRISM_PULSE_TYPE_HARDCODED)


    /***************************************************************************
    * FUNCTION NAME:     PRISM_SetPulse0Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density0. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:
    *  pulse0Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void PRISM_SetPulse0Mode(uint8 pulse0Type) 
    {
        if(pulse0Type == PRISM_GREATERTHAN_OR_EQUAL)
        {
            PRISM_CONTROL_REG |= PRISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PRISM_CONTROL_REG &= ~PRISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL;
        }
    }
    
    
    /***************************************************************************
    * FUNCTION NAME:   PRISM_SetPulse1Mode
    ****************************************************************************
    *
    * Summary:
    *  Sets the pulse density type for Density1. Less than or Equal(<=) or 
    *  Greater than or Equal(>=).
    *
    * Parameters:  
    *  pulse1Type: Selected pulse density type.
    *
    * Return:
    *  None.
    *
    ***************************************************************************/
    void PRISM_SetPulse1Mode(uint8 pulse1Type) 
    {
        if(pulse1Type == PRISM_GREATERTHAN_OR_EQUAL)
        {
            PRISM_CONTROL_REG |= PRISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
        else
        {
            PRISM_CONTROL_REG &= ~PRISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL;
        }
    }

#endif /* End PRISM_PULSE_TYPE_HARDCODED == 0 */


/*******************************************************************************
* FUNCTION NAME:   PRISM_ReadSeed
********************************************************************************
*
* Summary:
*  Reads the PrISM Seed register.
*
* Parameters:
*  None.
*
* Return:
*  Current Period register value.
*
*******************************************************************************/
uint16 PRISM_ReadSeed(void) 
{
    return( CY_GET_REG16(PRISM_SEED_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_WriteSeed
********************************************************************************
*
* Summary:
*  Writes the PrISM Seed register with the start value.
*
* Parameters:
*  seed: Seed register value.
*
* Return:
*  None.
*
*******************************************************************************/
void PRISM_WriteSeed(uint16 seed) 
{
    if(seed != 0u)
    {
        CY_SET_REG16(PRISM_SEED_COPY_PTR, seed);
        CY_SET_REG16(PRISM_SEED_PTR, seed);
    }
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_ReadPolynomial
********************************************************************************
*
* Summary:
*  Reads the PrISM polynomial.
*
* Parameters:
*  None.
*
* Return:
*  PrISM polynomial.
*
*******************************************************************************/
uint16 PRISM_ReadPolynomial(void) 
{
    return( CY_GET_REG16(PRISM_POLYNOM_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_WritePolynomial
********************************************************************************
*
* Summary:
*  Writes the PrISM polynomial.
*
* Parameters:
*  polynomial: PrISM polynomial.
*
* Return:
*  None.
*
*******************************************************************************/
void PRISM_WritePolynomial(uint16 polynomial) \
                                                                
{
    CY_SET_REG16(PRISM_POLYNOM_PTR, polynomial);
    
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_ReadPulse0
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density0 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density0 register value.
*
*******************************************************************************/
uint16 PRISM_ReadPulse0(void) 
{
    return( CY_GET_REG16(PRISM_DENSITY0_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_WritePulse0
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density0 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity0: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void PRISM_WritePulse0(uint16 pulseDensity0) \
                                                                
{
    CY_SET_REG16(PRISM_DENSITY0_PTR, pulseDensity0);
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_ReadPulse1
********************************************************************************
*
* Summary:
*  Reads the PrISM Pulse Density1 register.
*
* Parameters:
*  None.
*
* Return:
*  Pulse Density1 register value.
*
*******************************************************************************/
uint16 PRISM_ReadPulse1(void) 
{
    return( CY_GET_REG16(PRISM_DENSITY1_PTR) );
}


/*******************************************************************************
* FUNCTION NAME:   PRISM_WritePulse1
********************************************************************************
*
* Summary:
*  Writes the PrISM Pulse Density1 register with the Pulse Density value.
*
* Parameters:
*  pulseDensity1: Pulse Density value.
*
* Return:
*  None.
*
*******************************************************************************/
void PRISM_WritePulse1(uint16 pulseDensity1) \
                                                                    
{
    CY_SET_REG16(PRISM_DENSITY1_PTR, pulseDensity1);
}


/* [] END OF FILE */
