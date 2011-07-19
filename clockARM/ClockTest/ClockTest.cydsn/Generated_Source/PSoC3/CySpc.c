/*******************************************************************************
* File Name: CySpc.c  
* Version 2.20
*
*  Description:
*   Provides an API for the System Performance Component. 
*   The SPC functions are not meant to be called directly by the user application.
*
*  Note:
*
*   
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#include <CySpc.h>


/* Gate calls to the SPC. */
unsigned int SpcLockState = 0;

/* We only need storage for one comand since we can only do one command at a time. */
uint8 cyCommand[16];
uint8 cyCommandSize;



/*******************************************************************************
* Function Name: CySpcLock
********************************************************************************
* Summary:
*   Locks the SPC so it can not be used by someone else.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
* Parameters:
*   void.
*
*   
* Return:
*   .
*   CYRET_SUCCESS if the resource was free.
*   CYRET_LOCKED if the SPC is in use.
*
*
*******************************************************************************/
cystatus CySpcLock(void)
{
    cystatus status;
    uint8 interruptState;


    /* Enter critical section! */
    interruptState = CyEnterCriticalSection();

    if(SpcLockState == 0)
    {
        SpcLockState = 1;
        status = CYRET_SUCCESS;
    }
    else
    {
        status = CYRET_LOCKED;
    }

    /* Exit critical section! */
    CyExitCriticalSection(interruptState);

    return status;
}


/*******************************************************************************
* Function Name: CySpcWriteCommand
********************************************************************************
* Summary:
*   Writes the command created by one of the "CySpcCreateCmd..." functions, then
*   writes the data parameters passed into this function. 
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
* Parameters:
* parameters:
*   Address of the parameters associated with the SPC function being executed.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
* size:
*   size of data in bytes.
*
*   
* Return:
*   CYRET_LOCKED if the SPC is in use.
*   CYRET_CANCELED if the SPC didn't accept the commnad.
*   CYRET_STARTED if the command and data was correctly started.
*
* Theory:
*   This function must be called after one of the "CySpcCreateCmd..." functions.
*   This function writes the command and data parameters to the
*   SPC.
*
*
*******************************************************************************/
cystatus CySpcWriteCommand(uint8 * parameters, uint16 size)
{
    uint8 * pointer;
    uint16 index;
    cystatus status;


    /* Has the SPC controller started something? */
    if(*SPC_STATUS & SPC_SPC_IDLE)
    {
        /* Create packet. */
        pointer = cyCommand;
        *SPC_CPU_DATA = *pointer++;
        *SPC_CPU_DATA = *pointer++;
        *SPC_CPU_DATA = *pointer++;

        /* Make sure the command was accepted. */
        if(!(*SPC_STATUS & SPC_SPC_IDLE))
        {
            /* Write the parameters. */
            for(index = 0; index < cyCommandSize; index++)
                *SPC_CPU_DATA = *pointer++;

            /* Write parameters. */
            if(size != 0)
            {
                for(index = 0; index < size; index++)
                    *SPC_CPU_DATA = parameters[index];
            }

            /* We successfuly wrote the command, the caller can check for errors. */
            status = CYRET_STARTED;
        }
        else
        {
            /* Get the status. */
            status = CYRET_CANCELED;
        }
    }
    else
    {
        status = CYRET_LOCKED;
    }

    return status;
}


/*******************************************************************************
* Function Name: CySPCReadData
********************************************************************************
* Summary:
*   Reads data back from the SPC.  
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
* Parameters:
* buffer:
*   Address to store data read.
*
* size:
*   number of bytes to read from the SPC.
*
* Return:
*   The number of bytes read from the SPC.
*
*
*******************************************************************************/
cystatus CySpcReadData(uint8 * buffer, uint8 size) 
{
    uint8 index;
    
    for(index = 0; index < size; index++)
    {    
        while(!(*SPC_STATUS & SPC_DATA_READY))
        {
        }

        buffer[index] = *SPC_CPU_DATA;
    }

    return index;
}


/*******************************************************************************
* Function Name: CySpcUnlock
********************************************************************************
* Summary:
*   Unlocks the SPC so it can be used by someone else.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
* Parameters:
*   void.
*
*   
* Return:
*   void.
*
*
*******************************************************************************/
void CySpcUnlock(void)
{
    /* Release the SPC object. */
    SpcLockState = 0;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdLoadMultiByte
********************************************************************************
* Summary:
*   Sets up the command to LoadMultiByte.
*
* Parameters:
* array:
*   Id of the array.
*
* address:
*   flash/eeprom addrress
*
* size:
*   number bytes to load.
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_LOCKED if the SPC is in use.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdLoadMultiByte(uint8 array, uint16 address, uint16 number)
{
    cystatus status;


    /* Check if number is correct for array. */
    if((array < LAST_FLASH_ARRAYID && number == 32) ||
       (array > LAST_FLASH_ARRAYID && number == 16))
    {
        /* Create packet command. */
        cyCommand[0] = SPC_KEY_ONE;
        cyCommand[1] = SPC_KEY_TWO(SPC_CMD_LD_MULTI_BYTE);
        cyCommand[2] = SPC_CMD_LD_MULTI_BYTE;
        
        /* Create packet parameters. */
        cyCommand[3] = array;
        cyCommand[4] = 1 & HI8(address);
        cyCommand[5] = LO8(address);
        cyCommand[6] = number - 1;

        cyCommandSize = SIZEOF_CMD_LOAD_MBYTE;

        status = CYRET_SUCCESS;
    }
    else
    {
        status = CYRET_BAD_PARAM;
    }

    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdLoadRow
********************************************************************************
* Summary:
*   Sets up the command to LoadRow.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* array:
*   Id of the array.
*
*
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdLoadRow(uint8 array)
{
    cystatus status;


    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_LD_ROW);
    cyCommand[2] = SPC_CMD_LD_ROW;
    
    /* Create packet parameters. */
    cyCommand[3] = array;

    cyCommandSize = SIZEOF_CMD_LOAD_ROW;

    status = CYRET_SUCCESS;
    
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdReadMultiByte
********************************************************************************
* Summary:
*   Sets up the command to ReadMultiByte.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
* Parameters:
* array:
*   Id of the array.
*
* ecc:
*   0x80 if reading ecc data.
*   0x00 if user data.
*
* address:
*   flash addrress.
*
* size:
*   number bytes to load.
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdReadMultiByte(uint8 array, uint8 ecc, uint16 address, uint8 number)
{
    cystatus status;


    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_RD_MULTI_BYTE);
    cyCommand[2] = SPC_CMD_RD_MULTI_BYTE;
    
    /* Create packet parameters. */
    cyCommand[3] = array;
    cyCommand[4] = ecc;
    cyCommand[5] = HI8(address);
    cyCommand[6] = LO8(address);
    cyCommand[7] = number - 1;

    cyCommandSize = SIZEOF_CMD_READ_MBYTE;

    status = CYRET_SUCCESS;
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdWriteRow
********************************************************************************
* Summary:
*   Sets up the command to WriteRow.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* array:
*   Id of the array.
*
*
* address:
*   flash/eeprom addrress
*
*
* tempPolarity:
*
*
* tempMagnitute:
*
*
*
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdWriteRow(uint8 array, uint16 address, uint8 tempPolarity, uint8 tempMagnitute)
{
    cystatus status;

    
    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_WR_ROW);
    cyCommand[2] = SPC_CMD_WR_ROW;
    
    /* Create packet parameters. */
    cyCommand[3] = array;
    cyCommand[4] = HI8(address);
    cyCommand[5] = LO8(address);
    cyCommand[6] = tempPolarity;
    cyCommand[7] = tempMagnitute;

    cyCommandSize = SIZEOF_CMD_WR_ROW;

    status = CYRET_SUCCESS;
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdProgramRow
********************************************************************************
* Summary:
*   Sets up the command to ProgramRow.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* array:
*   Id of the array.
*
*
* address:
*   flash/eeprom addrress
*
*
* par3:
*
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdProgramRow(uint8 array, uint16 address)
{
    cystatus status;


    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_PRG_ROW);
    cyCommand[2] = SPC_CMD_PRG_ROW;
    
    /* Create packet parameters. */
    cyCommand[3] = array;
    cyCommand[4] = HI8(address);
    cyCommand[5] = LO8(address);

    cyCommandSize = SIZEOF_CMD_PRGM_ROW;

    status = CYRET_SUCCESS;
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdEraseSector
********************************************************************************
* Summary:
*   Sets up the command to EraseSector.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* array:
*   Id of the array.
*
*
* address:
*   flash/eeprom addrress
*
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdEraseSector(uint8 array, uint16 address)
{
    cystatus status;

    
    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_ER_SECTOR);
    cyCommand[2] = SPC_CMD_ER_SECTOR;
    
    /* Create packet parameters. */
    cyCommand[3] = array;

    if(array < LAST_FLASH_ARRAYID)
    {
        cyCommand[4] = FLASH_SECTOR_ADDRESS(address);  
    }
    else
    {
        cyCommand[4] = EEPROM_SECTOR_ADDRESS(address);  
    }

    cyCommandSize = SIZEOF_CMD_ER_SECTOR;

    status = CYRET_SUCCESS;
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdGetTemp
********************************************************************************
* Summary:
*   Sets up the command to GetTemp.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* numSamples:
*
*
* timerPeriod:
*
*
* clkDivSelect:
*
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdGetTemp(uint8 numSamples, uint16 timerPeriod, uint8 clkDivSelect)
{
    cystatus status;


    /* Check parameters. */
    if(numSamples)
    {
        /* Create packet command. */
        cyCommand[0] = SPC_KEY_ONE;
        cyCommand[1] = SPC_KEY_TWO(SPC_CMD_GET_TEMP);
        cyCommand[2] = SPC_CMD_GET_TEMP;
    
        /* Create packet parameters. */
        cyCommand[3] = numSamples;
        cyCommand[4] = HI8(timerPeriod);
        cyCommand[5] = LO8(timerPeriod);
        cyCommand[6] = clkDivSelect;

        cyCommandSize = SIZEOF_CMD_GET_TEMP;

        status = CYRET_SUCCESS;
    }
    else
    {
        status = CYRET_BAD_PARAM;
    }

    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdSetupTs
********************************************************************************
* Summary:
*   Sets up the command to Setup Temperature Sensor.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* SeqSelect:
*
*
* SeqFreeze:
*
*
* ClkDivSel:
*
*
* CurvCompEnable:
*
*
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdSetupTs(uint8 SeqSelect, uint8 SeqFreeze, uint8 ClkDivSel, uint8 CurvCompEnable)
{
    cystatus status;

    
    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_SETUP_TS);
    cyCommand[2] = SPC_CMD_SETUP_TS;
    
    /* Create packet parameters. */
    cyCommand[3] = SeqSelect;
    cyCommand[4] = SeqFreeze;  
    cyCommand[5] = ClkDivSel;  
    cyCommand[6] = CurvCompEnable;  

    cyCommandSize = SIZEOF_CMD_SETUP_TS;

    status = CYRET_SUCCESS;
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcCreateCmdDisableTs
********************************************************************************
* Summary:
*   Sets up the command to Temperature Sensor.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*   
*
* Parameters:
* array:
*   Id of the array.
*
*
* address:
*   flash/eeprom addrress
*
*   
* Return:
*   CYRET_SUCCESS if the command was created sucessfuly.
*   CYRET_BAD_PARAM if an invalid parameter was passed.
*
*
* Theory:
*
*
*******************************************************************************/
cystatus CySpcCreateCmdDisableTs(void)
{
    cystatus status;

    
    /* Create packet command. */
    cyCommand[0] = SPC_KEY_ONE;
    cyCommand[1] = SPC_KEY_TWO(SPC_CMD_DISABLE_TS);
    cyCommand[2] = SPC_CMD_DISABLE_TS;
    
    cyCommandSize = SIZEOF_CMD_DISABLE_TS;

    status = CYRET_SUCCESS;
    
    return status;
}


/*******************************************************************************
* Function Name: CySpcActivePower
********************************************************************************
* Summary:
*   Selects the power for active operation mode.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
*
* Parameters:
*   state:
*       Active power state. 1 is active, 0 inactive.
*
*   
* Return:
*   void.
*
*
*******************************************************************************/
void CySpcActivePower(uint8 state) 
{
    if(state == 0)
    {
        *PM_ACT_SPC &= ~PM_SPC_MASK;
    }
    else
    {
        *PM_ACT_SPC |= PM_SPC_MASK;
    }
}

/*******************************************************************************
* Function Name: CySpcStandbyPower
********************************************************************************
* Summary:
*   Selects the power for standby operation modes.
*   NOTE: SPC functions are not meant to be called directly by the user application.
*
*
* Parameters:
*   state:
*       Standby power state. 1 is active, 0 inactive.
*
*   
* Return:
*   void.
*
*
*******************************************************************************/
void CySpcStandbyPower(uint8 state) 
{
    if(state == 0)
    {
        *PM_STBY_SPC &= ~PM_SPC_MASK;
    }
    else
    {
        *PM_STBY_SPC |= PM_SPC_MASK;
    }
}
