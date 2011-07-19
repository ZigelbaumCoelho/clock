/*******************************************************************************
* File Name: MAGNET.h  
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

#if !defined(CY_PINS_MAGNET_H) /* Pins MAGNET_H */
#define CY_PINS_MAGNET_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MAGNET_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    MAGNET_Write(uint8 value) ;
void    MAGNET_SetDriveMode(uint8 mode) ;
uint8   MAGNET_ReadDataReg(void) ;
uint8   MAGNET_Read(void) ;
uint8   MAGNET_ClearInterrupt(void) ;

/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define MAGNET_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define MAGNET_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define MAGNET_DM_RES_UP          PIN_DM_RES_UP
#define MAGNET_DM_RES_DWN         PIN_DM_RES_DWN
#define MAGNET_DM_OD_LO           PIN_DM_OD_LO
#define MAGNET_DM_OD_HI           PIN_DM_OD_HI
#define MAGNET_DM_STRONG          PIN_DM_STRONG
#define MAGNET_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define MAGNET_MASK               MAGNET__MASK
#define MAGNET_SHIFT              MAGNET__SHIFT
#define MAGNET_WIDTH              1u

/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MAGNET_PS                     (* (reg8 *) MAGNET__PS)
/* Data Register */
#define MAGNET_DR                     (* (reg8 *) MAGNET__DR)
/* Port Number */
#define MAGNET_PRT_NUM                (* (reg8 *) MAGNET__PRT) 
/* Connect to Analog Globals */                                                  
#define MAGNET_AG                     (* (reg8 *) MAGNET__AG)                       
/* Analog MUX bux enable */
#define MAGNET_AMUX                   (* (reg8 *) MAGNET__AMUX) 
/* Bidirectional Enable */                                                        
#define MAGNET_BIE                    (* (reg8 *) MAGNET__BIE)
/* Bit-mask for Aliased Register Access */
#define MAGNET_BIT_MASK               (* (reg8 *) MAGNET__BIT_MASK)
/* Bypass Enable */
#define MAGNET_BYP                    (* (reg8 *) MAGNET__BYP)
/* Port wide control signals */                                                   
#define MAGNET_CTL                    (* (reg8 *) MAGNET__CTL)
/* Drive Modes */
#define MAGNET_DM0                    (* (reg8 *) MAGNET__DM0) 
#define MAGNET_DM1                    (* (reg8 *) MAGNET__DM1)
#define MAGNET_DM2                    (* (reg8 *) MAGNET__DM2) 
/* Input Buffer Disable Override */
#define MAGNET_INP_DIS                (* (reg8 *) MAGNET__INP_DIS)
/* LCD Common or Segment Drive */
#define MAGNET_LCD_COM_SEG            (* (reg8 *) MAGNET__LCD_COM_SEG)
/* Enable Segment LCD */
#define MAGNET_LCD_EN                 (* (reg8 *) MAGNET__LCD_EN)
/* Slew Rate Control */
#define MAGNET_SLW                    (* (reg8 *) MAGNET__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MAGNET_PRTDSI__CAPS_SEL       (* (reg8 *) MAGNET__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MAGNET_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MAGNET__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MAGNET_PRTDSI__OE_SEL0        (* (reg8 *) MAGNET__PRTDSI__OE_SEL0) 
#define MAGNET_PRTDSI__OE_SEL1        (* (reg8 *) MAGNET__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MAGNET_PRTDSI__OUT_SEL0       (* (reg8 *) MAGNET__PRTDSI__OUT_SEL0) 
#define MAGNET_PRTDSI__OUT_SEL1       (* (reg8 *) MAGNET__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MAGNET_PRTDSI__SYNC_OUT       (* (reg8 *) MAGNET__PRTDSI__SYNC_OUT) 


#if defined(MAGNET__INTSTAT)  /* Interrupt Registers */

    #define MAGNET_INTSTAT                (* (reg8 *) MAGNET__INTSTAT)
    #define MAGNET_SNAP                   (* (reg8 *) MAGNET__SNAP)

#endif /* Interrupt Registers */

#endif /* End Pins MAGNET_H */


/* [] END OF FILE */
