/*******************************************************************************
* File Name: CyFlash.c  
* Version 2.20
*
*  Description:
*   Provides an API for the FLASH/EEPROM.
*
*  Note:
*   This code is endian agnostic.
*
*  Note: 
*   Documentation of the API's in this file is located in the
*   System Reference Guide provided with PSoC Creator.
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <Cydevice_trm.h>
#include "CYFLASH.H"

uint8 dieTemperature[2];

#if (CYDEV_ECC_ENABLE == 0)
    static uint8 * rowBuffer = 0;
#endif


/*******************************************************************************
* Function Name: CyFlash_Start
********************************************************************************
*
* Summary:
*  Enable the EEPROM/Flash. In PSOC3_ES2/PSOC5_ES1, this will enable both
*  Flash and EEPROM. In PSOC3_ES3/PSOC5_ES2, this will enable only Flash.
*   
* Parameters:
*  void
* 
* Return:
*  void  
* 
* Reentrant
*  No
*
*******************************************************************************/
void CyFlash_Start(void) 
{
    #if (PSOC3_ES2 || PSOC5_ES1)
        /* Active Power Mode */
        *PM_ACT_EEFLASH |= PM_FLASH_EE_MASK;
        /* Standby Power Mode */
        *PM_STBY_EEFLASH |= PM_FLASH_EE_MASK;
    #endif
    
    #if (PSOC3_ES3 || PSOC5_ES2)
        /* Active Power Mode */
        *PM_ACT_FLASH_PTR |= PM_FLASH_MASK;
        /* Standby Power Mode */
        *PM_STBY_FLASH_PTR |= PM_FLASH_MASK;
    #endif
}


/*******************************************************************************
* Function Name: CyFlash_Stop
********************************************************************************
*
* Summary:
*  Disable the EEPROM/Flash. In PSOC3_ES2/PSOC5_ES1, this will disable both
*  Flash and EEPROM. In PSOC3_ES3/PSOC5_ES2, this will disable only Flash.
*
* Parameters:
*  void
*
* Return:
*  void  
* 
* Side Effects:
*  This setting is ignored as long as the CPU is currently running.  This will 
*  only take effect when the CPU is later disabled.
*
* Reentrant
*  Yes
*
*******************************************************************************/
void CyFlash_Stop (void) 
{
    #if (PSOC3_ES2 || PSOC5_ES1)
        /* Active Power Mode */
        *PM_ACT_EEFLASH &= ~PM_FLASH_EE_MASK;
        /* Standby Power Mode */
        *PM_STBY_EEFLASH &= ~PM_FLASH_EE_MASK;
    #endif
    
    #if (PSOC3_ES3 || PSOC5_ES2)
        /* Active Power Mode */
        *PM_ACT_FLASH_PTR &= ~PM_FLASH_MASK;
        /* Standby Power Mode */
        *PM_STBY_FLASH_PTR &= ~PM_FLASH_MASK;
    #endif
}


/*******************************************************************************
* Function Name: CySetTemp
********************************************************************************
*
* Summary:
*  Sends a command to the SPC to read the die temperature. Sets a global value
*  used by the Write functions. This function must be called once before executing
*  a series of Flash writing functions.

*
* Parameters:
*  void
*
* Return:
*  status:
*   CYRET_SUCCESS if successful. The first byte is the sign of the temperature 
*   (0 = negative, 1 = positive).
*   The second byte is the magnitude.
*   CYRET_LOCKED if Flash writing already in use
*   CYRET_UNKNOWN if there was an SPC error.
*  
* Side Effects:
*  Execution of this function takes an extended period of time.
*
* Reentrant
*  Yes
*
*******************************************************************************/
cystatus CySetTempInt(void) 
{
    uint8 ticks;
    cystatus status;

    /* Make sure SPC is powered. */
    CySpcActivePower(1);

    /* Plan for failure. */
    status = CYRET_UNKNOWN;

    /* See if we can get the SPC. */
    if(CySpcLock() == CYRET_SUCCESS)
    {
        /* Create the command. */
        if(CySpcCreateCmdGetTemp(TEMP_NUMBER_OF_SAMPLES, TEMP_TIMER_PERIOD, TEMP_CLK_DIV_SELECT) == CYRET_SUCCESS)
        {
            /* Write the command. */
            if(CySpcWriteCommand(0, 0) == CYRET_STARTED)
            {
                ticks = (uint8) TEMP_MAX_WAIT;

                do
                {
                    if(CySpcReadData(dieTemperature, 2) == 2)
                    {
                        status = CYRET_SUCCESS;
                        break;
                    }
                    
                    /* Wait for the reading to come in. */
                    CyDelay(1);
                    ticks--;

                } while(ticks && !(*SPC_STATUS & SPC_SPC_IDLE));
            }
        }

        /* Unlock the SPC so someone else can use it. */
        CySpcUnlock();
    }
    else
    {
        status = CYRET_LOCKED;
    }

    return status;
}

cystatus CySetTemp(void) 
{
    CySetTempInt();
    return CySetTempInt();
}

/*******************************************************************************
* Function Name: CySetFlashEEBuffer
********************************************************************************
*
* Summary:
*  Sets the user supplied temporary buffer to store SPC data while performing
*  flash and EEPROM commands. This buffer is only necessary when Flash ECC is 
*  disabled.
*
*
* Parameters:
*  buffer:
*       Address of block of memory to store temporary memory. The size of the
*       block of memory is SIZEOF_FLASH_ROW + SIZEOF_ECC_ROW.
*   
* Return:
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*
* Reentrant
*  Yes
*
*******************************************************************************/
cystatus CySetFlashEEBuffer(uint8 * buffer) 
{
    cystatus status;

    /* Make sure SPC is powerd. */
    CySpcActivePower(1);

    if(!buffer)
    {
        status = CYRET_BAD_PARAM;
    }
    /* See if we can get the SPC. */
    else if(CySpcLock() != CYRET_SUCCESS)
    {
        status = CYRET_LOCKED;
    }
    else
    {
        #if (CYDEV_ECC_ENABLE == 0)
            rowBuffer = buffer;
        #endif
        status = CYRET_SUCCESS;

        /* Unlock the SPC so someone else can use it. */
        CySpcUnlock();
    }

    return status;
}

                      
#if (CYDEV_ECC_ENABLE == 1)

/*******************************************************************************
* Function Name: CyWriteRowData
********************************************************************************
*
* Summary:
*   Sends a command to the SPC to load and program a row of data in flash.
*
*
* Parameters:
*  arrayID: 
*   ID of the array to write.
*  rowAddress:
*   rowAddress of flash row to program.
*  rowData:
*   Array of bytes to write.
* 
* Return:
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*   CYRET_CANCELED if command not accepted
*   CYRET_UNKNOWN if there was an SPC error.
*
* Reentrant
*  Yes
*
*******************************************************************************/
cystatus CyWriteRowData(uint8 arrayId, uint16 rowAddress, uint8 * rowData) 
{
    uint16 rowSize;
    cystatus status;

    rowSize = (arrayId > LAST_FLASH_ARRAYID) ? SIZEOF_EEPROM_ROW : SIZEOF_FLASH_ROW;

    status = CyWriteRowFull(arrayId, rowAddress, rowData, rowSize);
    return status;
}

#else /* CYDEV_ECC_ENABLE == 0 */

/*******************************************************************************
* Function Name: CyWriteRowData
********************************************************************************
*
* Summary:
*   Sends a command to the SPC to load and program a row of data in flash.
*
*
* Parameters:
*  arrayID: 
*   ID of the array to write.
*  rowAddress:
*   rowAddress of flash row to program.
*  rowData:
*   Array of bytes to write.
* 
* Return:
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*   CYRET_CANCELED if command not accepted
*   CYRET_UNKNOWN if there was an SPC error.
*
* Reentrant
*  Yes
*
*******************************************************************************/
cystatus CyWriteRowData(uint8 arrayId, uint16 rowAddress, uint8 * rowData) 
{
    uint8 index;
    uint32 offset;
    uint16 rowSize;
    cystatus status;

    rowSize = (arrayId > LAST_FLASH_ARRAYID) ? SIZEOF_EEPROM_ROW : (SIZEOF_FLASH_ROW + SIZEOF_ECC_ROW);

    if (rowSize != SIZEOF_EEPROM_ROW)
    {
        /* Save the ECC area. */
        offset = CYDEV_ECC_BASE + ((uint32) rowAddress * SIZEOF_ECC_ROW);
        for (index = 0; index < SIZEOF_ECC_ROW; index++)
        {
            rowBuffer[SIZEOF_FLASH_ROW + index] = CY_GET_XTND_REG8((void CYFAR *)(offset + index));
        }
    }

    /* Copy the rowdata to the temporary buffer. */
    cymemcpy(rowBuffer, rowData, SIZEOF_FLASH_ROW);

    status = CyWriteRowFull(arrayId, rowAddress, rowBuffer, rowSize);
    return status;
}

#endif /* CYDEV_ECC_ENABLE == 0 */

#if (CYDEV_ECC_ENABLE == 0 && CYDEV_CONFIGURATION_ECC == 0)

/*******************************************************************************
* Function Name: CyWriteRowConfig
********************************************************************************
*
* Summary:
*  Sends a command to the SPC to load and program a row of config data in flash.
*  This function is only valid for Flash array IDs (not for EEPROM).
*
*
* Parameters:
*  arrayId:
*   ID of the array to write
*  rowAddress:
*   Address of the sector to erase.
*  rowData:  
*   Array of bytes to write.
*   
* Return:
*  status:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*   CYRET_CANCELED if command not accepted
*   CYRET_UNKNOWN if there was an SPC error.
*
* Reentrant
*  Yes
*
*******************************************************************************/
cystatus CyWriteRowConfig(uint8 arrayId, uint16 rowAddress, uint8 * rowData) 
{
    uint32 offset;
    uint16 index;
    cystatus status;

    /* Read the existing flash data. */
    offset = CYDEV_FLS_BASE + ((uint32) rowAddress * SIZEOF_FLASH_ROW);
    for (index = 0; index < SIZEOF_FLASH_ROW; index++)
    {
        rowBuffer[index] = CY_GET_XTND_REG8((void CYFAR *)(offset + index));
    }

    /* Copy the rowData to the temporary buffer. */
    cymemcpy(&rowBuffer[SIZEOF_FLASH_ROW], rowData, SIZEOF_ECC_ROW);

    status = CyWriteRowFull(arrayId, rowAddress, rowBuffer, SIZEOF_FLASH_ROW + SIZEOF_ECC_ROW);
    return status;
}

#endif  /* (CYDEV_ECC_ENABLE == 1 && CYDEV_CONFIGURATION_ECC == 0) */


/*******************************************************************************
* Function Name: CyWriteRowFull
********************************************************************************
* Summary:
*   Sends a command to the SPC to load and program a row of data in flash. 
*   rowData array is expected to contain Flash and ECC data if needed.
*
*
* Parameters:
*
*       arrayId: FLASH or EEPROM array id.
*       rowData: pointer to a row of data to write.
*       rowNumber: Zero based number of the row.
*       rowSize: Size of the row.
*   
* Return:
*   CYRET_SUCCESS if successful.
*   CYRET_LOCKED if the SPC is already in use.
*   CYRET_CANCELED if command not accepted
*   CYRET_UNKNOWN if there was an SPC error.
*
*
*******************************************************************************/
cystatus CyWriteRowFull(uint8 arrayId, uint16 rowNumber, uint8* rowData, uint16 rowSize) 
{
    cystatus status;

    /* See if we can get the SPC. */
    if (CySpcLock() == CYRET_SUCCESS)
    {
        /* Create the command. */
        status = CySpcCreateCmdLoadRow(arrayId);
        if (status == CYRET_SUCCESS)
        {
            /* Write the command. */
            status = CySpcWriteCommand(rowData, rowSize);
            if (status == CYRET_STARTED)
            {
                /* Spin until completion. */
                while(!(*SPC_STATUS & SPC_SPC_IDLE));

                status = CySpcReadStatus;
                /* See if we were successful. */
                if(status != SPC_SUCCESS)
                    status |= CYRET_SPC_ERROR_MASK;
            }
        }

        /* Create the command. */
        if (status == SPC_SUCCESS)
        {
            status = CySpcCreateCmdWriteRow(arrayId, rowNumber, dieTemperature[0], dieTemperature[1]);
            if (status == CYRET_SUCCESS)
            {
                /* Write the command. */
                status = CySpcWriteCommand(0, 0);
                if (status == CYRET_STARTED)
                {
                    /* Spin until completion. */
                    while(!(*SPC_STATUS & SPC_SPC_IDLE));

                    status = CySpcReadStatus;
                    /* See if we were successful. */
                    if(status != SPC_SUCCESS)
                        status |= CYRET_SPC_ERROR_MASK;
                }
            }
        }

        /* Unlock the SPC so someone else can use it. */
        CySpcUnlock();
    }
    else
    {
        status = CYRET_LOCKED;
    }

    return status;
}


/*******************************************************************************
* Function Name: CyFlash_SetWaitCycles
********************************************************************************
*
* Summary:
*  Sets the correct number of wait cycles for the flash based on the frequency 
*  of operation of the devices. This function should be called before increasing 
*  the clock frequency.  It can optionally be called after lowering the clock 
*  frequency in order to improve CPU performance.
*   
* Parameters:
*  freq:
*   Frequency of operation in Megahertz.
* 
* Return:
*  void  
* 
* Reentrant
*  Yes
*
*******************************************************************************/
void CyFlash_SetWaitCycles(uint8 freq) 
{
    /* Clear the Flash Cycles bit fields */
    *FLASH_CYCLES_PTR &= ~FLASH_CYCLES_MASK;
    
    /* Set Flash Cycles bit fields with frequency value*/
    #if (PSOC3_ES2)
        if (freq <= 24u)            
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_24MHz << FLASH_CYCLES_MASK_SHIFT);
        else if (freq <= 48u)
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_48MHz << FLASH_CYCLES_MASK_SHIFT);
        else
            *FLASH_CYCLES_PTR |= (GREATER_48MHz << FLASH_CYCLES_MASK_SHIFT);
    #endif
    
    #if (PSOC3_ES3)
        if (freq <= 22u)            
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_22MHz << FLASH_CYCLES_MASK_SHIFT);
        else if (freq <= 44u)
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_44MHz << FLASH_CYCLES_MASK_SHIFT);
        else
            *FLASH_CYCLES_PTR |= (GREATER_44MHz << FLASH_CYCLES_MASK_SHIFT);
    #endif
    
    #if (PSOC5_ES1)
        if (freq <= 16u)            
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_16MHz << FLASH_CYCLES_MASK_SHIFT);
        else if (freq <= 33u)
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_33MHz << FLASH_CYCLES_MASK_SHIFT);
        else if (freq <= 50u)
            *FLASH_CYCLES_PTR |= (LESSER_OR_EQUAL_50MHz << FLASH_CYCLES_MASK_SHIFT);
        else
            *FLASH_CYCLES_PTR |= (GREATER_51MHz << FLASH_CYCLES_MASK_SHIFT);
    #endif
}


#if (PSOC3_ES3 || PSOC5_ES2)

/*******************************************************************************
* Function Name: CyEEPROM_Start
********************************************************************************
*
* Summary:
*  Enable the EEPROM.
*   
* Parameters:
*  void
* 
* Return:
*  void  
* 
* Reentrant
*  Yes
*
*******************************************************************************/
void CyEEPROM_Start(void) 
{    
    /* Active Power Mode */
    *PM_ACT_EE_PTR |= PM_EE_MASK;
    /* Standby Power Mode */
    *PM_STBY_EE_PTR |= PM_EE_MASK;    
}


/*******************************************************************************
* Function Name: CyEEPROM_Stop
********************************************************************************
*
* Summary:
*  Disable the EEPROM.
*   
* Parameters:
*  void
* 
* Return:
*  void  
* 
* Reentrant
*  Yes
*
*******************************************************************************/
void CyEEPROM_Stop (void) 
{
    /* Active Power Mode */
    *PM_ACT_EE_PTR &= ~PM_EE_MASK;
    /* Standby Power Mode */
    *PM_STBY_EE_PTR &= ~PM_EE_MASK;
}

#endif /* (PSOC3_ES3 || PSOC5_ES2) */


/*******************************************************************************
* Function Name: CyEEPROM_ReadReserve
********************************************************************************
*
* Summary:
*  Request access to the EEPROM for reading and wait until access is available.
*   
* Parameters:
*  void
* 
* Return:
*  void  
*   
* Reentrant
*  Yes
*
*******************************************************************************/
void CyEEPROM_ReadReserve(void) 
{
    /* Make a request for PHUB to have access */
    *AHUB_EE_REQ_ACK_PTR |= 0x01u;    
    /* Wait for acknowledgement from PHUB */
    while (!(*AHUB_EE_REQ_ACK_PTR & 0x02u));    
}


/*******************************************************************************
* Function Name: CyEEPROM_ReadRelease
********************************************************************************
*
* Summary:
*  Release the read reservation of the EEPROM.
*   
* Parameters:
*  void
* 
* Return:
*  void  
* 
* Reentrant
*  Yes
*
*******************************************************************************/
void CyEEPROM_ReadRelease(void) 
{
    *AHUB_EE_REQ_ACK_PTR |= 0x00u;   
}


/* End of File */
