/*******************************************************************************
* File Name: LE.h  
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

#if !defined(CY_PINS_LE_H) /* Pins LE_H */
#define CY_PINS_LE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "LE_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    LE_Write(uint8 value) ;
void    LE_SetDriveMode(uint8 mode) ;
uint8   LE_ReadDataReg(void) ;
uint8   LE_Read(void) ;
uint8   LE_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define LE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define LE_DM_RES_UP          PIN_DM_RES_UP
#define LE_DM_RES_DWN         PIN_DM_RES_DWN
#define LE_DM_OD_LO           PIN_DM_OD_LO
#define LE_DM_OD_HI           PIN_DM_OD_HI
#define LE_DM_STRONG          PIN_DM_STRONG
#define LE_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define LE_MASK               LE__MASK
#define LE_SHIFT              LE__SHIFT
#define LE_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LE_PS                     (* (reg8 *) LE__PS)
/* Data Register */
#define LE_DR                     (* (reg8 *) LE__DR)
/* Port Number */
#define LE_PRT_NUM                (* (reg8 *) LE__PRT) 
/* Connect to Analog Globals */                                                  
#define LE_AG                     (* (reg8 *) LE__AG)                       
/* Analog MUX bux enable */
#define LE_AMUX                   (* (reg8 *) LE__AMUX) 
/* Bidirectional Enable */                                                        
#define LE_BIE                    (* (reg8 *) LE__BIE)
/* Bit-mask for Aliased Register Access */
#define LE_BIT_MASK               (* (reg8 *) LE__BIT_MASK)
/* Bypass Enable */
#define LE_BYP                    (* (reg8 *) LE__BYP)
/* Port wide control signals */                                                   
#define LE_CTL                    (* (reg8 *) LE__CTL)
/* Drive Modes */
#define LE_DM0                    (* (reg8 *) LE__DM0) 
#define LE_DM1                    (* (reg8 *) LE__DM1)
#define LE_DM2                    (* (reg8 *) LE__DM2) 
/* Input Buffer Disable Override */
#define LE_INP_DIS                (* (reg8 *) LE__INP_DIS)
/* LCD Common or Segment Drive */
#define LE_LCD_COM_SEG            (* (reg8 *) LE__LCD_COM_SEG)
/* Enable Segment LCD */
#define LE_LCD_EN                 (* (reg8 *) LE__LCD_EN)
/* Slew Rate Control */
#define LE_SLW                    (* (reg8 *) LE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define LE_PRTDSI__CAPS_SEL       (* (reg8 *) LE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define LE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) LE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define LE_PRTDSI__OE_SEL0        (* (reg8 *) LE__PRTDSI__OE_SEL0) 
#define LE_PRTDSI__OE_SEL1        (* (reg8 *) LE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define LE_PRTDSI__OUT_SEL0       (* (reg8 *) LE__PRTDSI__OUT_SEL0) 
#define LE_PRTDSI__OUT_SEL1       (* (reg8 *) LE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define LE_PRTDSI__SYNC_OUT       (* (reg8 *) LE__PRTDSI__SYNC_OUT) 


#if defined(LE__INTSTAT)  /* Interrupt Registers */

    #define LE_INTSTAT                (* (reg8 *) LE__INTSTAT)
    #define LE_SNAP                   (* (reg8 *) LE__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins LE_H */


/* [] END OF FILE */
