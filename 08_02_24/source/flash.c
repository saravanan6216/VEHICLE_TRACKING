/*
 * flash.c
 *
 *  Created on: Feb 7, 2024
 *      Author: mahim
 */


#include "flash.h"
#include "stm32l4xx_hal.h"
/************ writing data to flash memory ********/
void writeStringToFlash(uint64_t address, const char* str) {
    HAL_FLASH_Unlock();

    // Calculate the length of the string
    uint32_t len = strlen(str);

    // Ensure the length is even to write complete double words
    if (len % 8 != 0) {
        len += (8 - len % 8);
    }

    // Write the string to flash
    for (uint32_t i = 0; i < len; i += 8) {
        uint64_t data = 0;
        memcpy(&data, str + i, 8);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address + i, data);
    }

    HAL_FLASH_Lock();
}


/***************reading data from flash memory **************/
void readDataFromFlash(uint64_t address, char* buffer, uint64_t dataSize)
{
	uint32_t i,j=0;
	for (i = 0; i < dataSize; i++)
	{
	       uint64_t data = *(__IO uint64_t*)address;
	       // Copy each byte from the 64-bit data
	               for (int k = 0; k < 8; k++) {
	                   if (j < dataSize - 1) {  // Ensure not to overflow the buffer
	                       buffer[j++] = (char)(data & 0xFF);
	                   }
	                   data >>= 8;
	               }
	       address += 8;
	}
	buffer[i] = '\0';

}



/**************Erasing data from flash memory ****************/
void eraseFlashPage(uint64_t pageAddress) {
    FLASH_EraseInitTypeDef eraseConfig;
    uint32_t pageError;

    eraseConfig.TypeErase   = FLASH_TYPEERASE_PAGES;
    eraseConfig.Page        = pageAddress / FLASH_PAGE_SIZE; // Specify the page number, not the address
    eraseConfig.Banks       = FLASH_BANK_2; // Specify the bank
    eraseConfig.NbPages     = 1; // Number of pages to erase

    HAL_FLASH_Unlock();

    HAL_FLASHEx_Erase(&eraseConfig, &pageError);

    HAL_FLASH_Lock();
}
