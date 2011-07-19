/*******************************************************************************
* File Name: PRISM.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the PrISM
*  Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_PrISM_PRISM_H)    /* CY_PrISM_PRISM_H */
#define CY_PrISM_PRISM_H

#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/***************************************
* Conditional Compilation Parameters
***************************************/

/* PSoC3 ES2 or early */
#define PRISM_PSOC3_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A) && \
                                     (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_3A_ES2))

/* PSoC5 ES1 or early */
#define PRISM_PSOC5_ES1  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A) && \
                                     (CYDEV_CHIP_REVISION_USED <= CYDEV_CHIP_REVISION_5A_ES1))
/* PSoC3 ES3 or later */
#define PRISM_PSOC3_ES3  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_3A) && \
                                     (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_3A_ES2))

/* PSoC5 ES2 or later */
#define PRISM_PSOC5_ES2  ((CYDEV_CHIP_MEMBER_USED == CYDEV_CHIP_MEMBER_5A) && \
                                     (CYDEV_CHIP_REVISION_USED > CYDEV_CHIP_REVISION_5A_ES1))
                                     
#define PRISM_RESOLUTION             (16u)
#define PRISM_PULSE_TYPE_HARDCODED   (0u)


/***************************************
*       Type defines
***************************************/

/* Sleep Mode API Support */
typedef struct _PRISM_backupStruct
{
    uint8 enableState;
    #if(!PRISM_PULSE_TYPE_HARDCODED)
        uint8 cr;
    #endif /*End PRISM_PULSE_TYPE_HARDCODED*/
    uint16 seed;
    uint16 seed_copy;
    uint16 polynom;
    #if(PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1) /* PSoC3 ES2 or early, PSoC5 ES1*/
        uint16 density0;
        uint16 density1;
    #endif /*End PRISM_PSOC3_ES2 || PRISM_PSOC5_ES1*/
} PRISM_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void PRISM_Start(void);
void PRISM_Stop(void) ;
void PRISM_SetPulse0Mode(uint8 pulse0Type) ;
void PRISM_SetPulse1Mode(uint8 pulse1Type) ;
uint16 PRISM_ReadSeed(void) ;
void PRISM_WriteSeed(uint16 seed) ;
uint16 PRISM_ReadPolynomial(void) ;
void PRISM_WritePolynomial(uint16 polynomial) \
                                                                ;
uint16 PRISM_ReadPulse0(void) ;
void PRISM_WritePulse0(uint16 pulseDensity0) \
                                                                ;
uint16 PRISM_ReadPulse1(void) ;
void PRISM_WritePulse1(uint16 pulseDensity1) \
                                                                ;
void PRISM_Sleep(void);
void PRISM_Wakeup(void) ;
void PRISM_SaveConfig(void);
void PRISM_RestoreConfig(void) ;
void PRISM_Enable(void) ;
void PRISM_Init(void) ;


/***************************************
*          API Constants
***************************************/

/* Constants for SetPulse0Mode(), SetPulse1Mode(), pulse type */
#define PRISM_LESSTHAN_OR_EQUAL      (0x00u)
#define PRISM_GREATERTHAN_OR_EQUAL   (0x01u)


/***************************************
*    Initial Parameter Constants
***************************************/

#define PRISM_POLYNOM                (0xB400u)
#define PRISM_SEED                   (0xFFFFu)
#define PRISM_DENSITY0               (0x1u)
#define PRISM_DENSITY1               (0x1u)


/***************************************
*              Registers
***************************************/

#if (PRISM_RESOLUTION  <= 8u) /* 8bit - PrISM */
    #define PRISM_DENSITY0_REG       (* (reg8 *) PRISM_sC8_PrISMdp_u0__D0_REG)
    #define PRISM_DENSITY0_PTR       (  (reg8 *) PRISM_sC8_PrISMdp_u0__D0_REG)
    #define PRISM_DENSITY1_REG       (* (reg8 *) PRISM_sC8_PrISMdp_u0__D1_REG)
    #define PRISM_DENSITY1_PTR       (  (reg8 *) PRISM_sC8_PrISMdp_u0__D1_REG)
    #define PRISM_POLYNOM_REG        (* (reg8 *) PRISM_sC8_PrISMdp_u0__A1_REG)
    #define PRISM_POLYNOM_PTR        (  (reg8 *) PRISM_sC8_PrISMdp_u0__A1_REG)
    #define PRISM_SEED_REG           (* (reg8 *) PRISM_sC8_PrISMdp_u0__A0_REG)
    #define PRISM_SEED_PTR           (  (reg8 *) PRISM_sC8_PrISMdp_u0__A0_REG)
    #define PRISM_SEED_COPY_REG      (* (reg8 *) PRISM_sC8_PrISMdp_u0__F0_REG)
    #define PRISM_SEED_COPY_PTR      (  (reg8 *) PRISM_sC8_PrISMdp_u0__F0_REG)
    #define PRISM_AUX_CONTROL_REG    (* (reg8 *) PRISM_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PRISM_AUX_CONTROL_PTR    (  (reg8 *) PRISM_sC8_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PRISM_RESOLUTION <= 16u) /* 16bit - PrISM */
    #define PRISM_DENSITY0_PTR       ((reg16 *) PRISM_sC16_PrISMdp_u0__D0_REG)
    #define PRISM_DENSITY1_PTR       ((reg16 *) PRISM_sC16_PrISMdp_u0__D1_REG)
    #define PRISM_POLYNOM_PTR        ((reg16 *) PRISM_sC16_PrISMdp_u0__A1_REG)
    #define PRISM_SEED_PTR           ((reg16 *) PRISM_sC16_PrISMdp_u0__A0_REG)
    #define PRISM_SEED_COPY_PTR      ((reg16 *) PRISM_sC16_PrISMdp_u0__F0_REG)
    #define PRISM_AUX_CONTROL_PTR    ((reg16 *) PRISM_sC16_PrISMdp_u0__DP_AUX_CTL_REG)
#elif (PRISM_RESOLUTION <= 24u) /* 24bit - PrISM */
    #define PRISM_DENSITY0_PTR       ((reg32 *) PRISM_sC24_PrISMdp_u0__D0_REG)
    #define PRISM_DENSITY1_PTR       ((reg32 *) PRISM_sC24_PrISMdp_u0__D1_REG)
    #define PRISM_POLYNOM_PTR        ((reg32 *) PRISM_sC24_PrISMdp_u0__A1_REG)
    #define PRISM_SEED_PTR           ((reg32 *) PRISM_sC24_PrISMdp_u0__A0_REG)
    #define PRISM_SEED_COPY_PTR      ((reg32 *) PRISM_sC24_PrISMdp_u0__F0_REG)
    #define PRISM_AUX_CONTROL_PTR    ((reg32 *) PRISM_sC24_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PRISM_AUX_CONTROL2_PTR   ((reg32 *) PRISM_sC24_PrISMdp_u2__DP_AUX_CTL_REG)
#else /* 32bit - PrISM */
    #define PRISM_DENSITY0_PTR       ((reg32 *) PRISM_sC32_PrISMdp_u0__D0_REG)
    #define PRISM_DENSITY1_PTR       ((reg32 *) PRISM_sC32_PrISMdp_u0__D1_REG)
    #define PRISM_POLYNOM_PTR        ((reg32 *) PRISM_sC32_PrISMdp_u0__A1_REG)
    #define PRISM_SEED_PTR           ((reg32 *) PRISM_sC32_PrISMdp_u0__A0_REG)
    #define PRISM_SEED_COPY_PTR      ((reg32 *) PRISM_sC32_PrISMdp_u0__F0_REG)
    #define PRISM_AUX_CONTROL_PTR    ((reg32 *) PRISM_sC32_PrISMdp_u0__DP_AUX_CTL_REG)
    #define PRISM_AUX_CONTROL2_PTR   ((reg32 *) PRISM_sC32_PrISMdp_u2__DP_AUX_CTL_REG)
#endif /* End PRISM_RESOLUTION */

#define PRISM_CONTROL_REG   (* (reg8 *) PRISM_SyncCtl_ControlReg__CONTROL_REG)
#define PRISM_CONTROL_PTR   (  (reg8 *) PRISM_SyncCtl_ControlReg__CONTROL_REG) 

/***************************************
*       Register Constants
***************************************/

#define PRISM_CTRL_ENABLE                                (0x01u)
#define PRISM_CTRL_COMPARE_TYPE0_GREATER_THAN_OR_EQUAL   (0x02u)
#define PRISM_CTRL_COMPARE_TYPE1_GREATER_THAN_OR_EQUAL   (0x04u)

#define PRISM_FIFO0_CLR                                  (0x01u)

/***************************************
* Renamed global variables or defines 
* for backward compatible
***************************************/
#define PRISM_ReadPusle0     PRISM_ReadPulse0
#define PRISM_ReadPusle1     PRISM_ReadPulse1


#endif  /* End CY_PrISM_PRISM_H */


/* [] END OF FILE */
