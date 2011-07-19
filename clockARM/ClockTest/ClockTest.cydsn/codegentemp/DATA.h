/*******************************************************************************
* File Name: DATA.h  
* Version 1.50
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PINS_DATA_H) /* Pins DATA_H */
#define CY_PINS_DATA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "DATA_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    DATA_Write(uint8 value) ;
void    DATA_SetDriveMode(uint8 mode) ;
uint8   DATA_ReadDataReg(void) ;
uint8   DATA_Read(void) ;
uint8   DATA_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define DATA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define DATA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define DATA_DM_RES_UP          PIN_DM_RES_UP
#define DATA_DM_RES_DWN         PIN_DM_RES_DWN
#define DATA_DM_OD_LO           PIN_DM_OD_LO
#define DATA_DM_OD_HI           PIN_DM_OD_HI
#define DATA_DM_STRONG          PIN_DM_STRONG
#define DATA_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define DATA_MASK               DATA__MASK
#define DATA_SHIFT              DATA__SHIFT
#define DATA_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define DATA_PS                     (* (reg8 *) DATA__PS)
/* Data Register */
#define DATA_DR                     (* (reg8 *) DATA__DR)
/* Port Number */
#define DATA_PRT_NUM                (* (reg8 *) DATA__PRT) 
/* Connect to Analog Globals */                                                  
#define DATA_AG                     (* (reg8 *) DATA__AG)                       
/* Analog MUX bux enable */
#define DATA_AMUX                   (* (reg8 *) DATA__AMUX) 
/* Bidirectional Enable */                                                        
#define DATA_BIE                    (* (reg8 *) DATA__BIE)
/* Bit-mask for Aliased Register Access */
#define DATA_BIT_MASK               (* (reg8 *) DATA__BIT_MASK)
/* Bypass Enable */
#define DATA_BYP                    (* (reg8 *) DATA__BYP)
/* Port wide control signals */                                                   
#define DATA_CTL                    (* (reg8 *) DATA__CTL)
/* Drive Modes */
#define DATA_DM0                    (* (reg8 *) DATA__DM0) 
#define DATA_DM1                    (* (reg8 *) DATA__DM1)
#define DATA_DM2                    (* (reg8 *) DATA__DM2) 
/* Input Buffer Disable Override */
#define DATA_INP_DIS                (* (reg8 *) DATA__INP_DIS)
/* LCD Common or Segment Drive */
#define DATA_LCD_COM_SEG            (* (reg8 *) DATA__LCD_COM_SEG)
/* Enable Segment LCD */
#define DATA_LCD_EN                 (* (reg8 *) DATA__LCD_EN)
/* Slew Rate Control */
#define DATA_SLW                    (* (reg8 *) DATA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define DATA_PRTDSI__CAPS_SEL       (* (reg8 *) DATA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define DATA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) DATA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define DATA_PRTDSI__OE_SEL0        (* (reg8 *) DATA__PRTDSI__OE_SEL0) 
#define DATA_PRTDSI__OE_SEL1        (* (reg8 *) DATA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define DATA_PRTDSI__OUT_SEL0       (* (reg8 *) DATA__PRTDSI__OUT_SEL0) 
#define DATA_PRTDSI__OUT_SEL1       (* (reg8 *) DATA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define DATA_PRTDSI__SYNC_OUT       (* (reg8 *) DATA__PRTDSI__SYNC_OUT) 


#if defined(DATA__INTSTAT)  /* Interrupt Registers */

    #define DATA_INTSTAT                (* (reg8 *) DATA__INTSTAT)
    #define DATA_SNAP                   (* (reg8 *) DATA__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins DATA_H */


/* [] END OF FILE */
