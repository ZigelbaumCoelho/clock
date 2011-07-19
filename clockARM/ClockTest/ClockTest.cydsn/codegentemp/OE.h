/*******************************************************************************
* File Name: OE.h  
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

#if !defined(CY_PINS_OE_H) /* Pins OE_H */
#define CY_PINS_OE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "OE_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    OE_Write(uint8 value) ;
void    OE_SetDriveMode(uint8 mode) ;
uint8   OE_ReadDataReg(void) ;
uint8   OE_Read(void) ;
uint8   OE_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define OE_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define OE_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define OE_DM_RES_UP          PIN_DM_RES_UP
#define OE_DM_RES_DWN         PIN_DM_RES_DWN
#define OE_DM_OD_LO           PIN_DM_OD_LO
#define OE_DM_OD_HI           PIN_DM_OD_HI
#define OE_DM_STRONG          PIN_DM_STRONG
#define OE_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define OE_MASK               OE__MASK
#define OE_SHIFT              OE__SHIFT
#define OE_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define OE_PS                     (* (reg8 *) OE__PS)
/* Data Register */
#define OE_DR                     (* (reg8 *) OE__DR)
/* Port Number */
#define OE_PRT_NUM                (* (reg8 *) OE__PRT) 
/* Connect to Analog Globals */                                                  
#define OE_AG                     (* (reg8 *) OE__AG)                       
/* Analog MUX bux enable */
#define OE_AMUX                   (* (reg8 *) OE__AMUX) 
/* Bidirectional Enable */                                                        
#define OE_BIE                    (* (reg8 *) OE__BIE)
/* Bit-mask for Aliased Register Access */
#define OE_BIT_MASK               (* (reg8 *) OE__BIT_MASK)
/* Bypass Enable */
#define OE_BYP                    (* (reg8 *) OE__BYP)
/* Port wide control signals */                                                   
#define OE_CTL                    (* (reg8 *) OE__CTL)
/* Drive Modes */
#define OE_DM0                    (* (reg8 *) OE__DM0) 
#define OE_DM1                    (* (reg8 *) OE__DM1)
#define OE_DM2                    (* (reg8 *) OE__DM2) 
/* Input Buffer Disable Override */
#define OE_INP_DIS                (* (reg8 *) OE__INP_DIS)
/* LCD Common or Segment Drive */
#define OE_LCD_COM_SEG            (* (reg8 *) OE__LCD_COM_SEG)
/* Enable Segment LCD */
#define OE_LCD_EN                 (* (reg8 *) OE__LCD_EN)
/* Slew Rate Control */
#define OE_SLW                    (* (reg8 *) OE__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define OE_PRTDSI__CAPS_SEL       (* (reg8 *) OE__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define OE_PRTDSI__DBL_SYNC_IN    (* (reg8 *) OE__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define OE_PRTDSI__OE_SEL0        (* (reg8 *) OE__PRTDSI__OE_SEL0) 
#define OE_PRTDSI__OE_SEL1        (* (reg8 *) OE__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define OE_PRTDSI__OUT_SEL0       (* (reg8 *) OE__PRTDSI__OUT_SEL0) 
#define OE_PRTDSI__OUT_SEL1       (* (reg8 *) OE__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define OE_PRTDSI__SYNC_OUT       (* (reg8 *) OE__PRTDSI__SYNC_OUT) 


#if defined(OE__INTSTAT)  /* Interrupt Registers */

    #define OE_INTSTAT                (* (reg8 *) OE__INTSTAT)
    #define OE_SNAP                   (* (reg8 *) OE__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins OE_H */


/* [] END OF FILE */
