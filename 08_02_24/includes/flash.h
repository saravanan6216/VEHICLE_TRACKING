/*
 * flash.h
 *
 *  Created on: Feb 7, 2024
 *      Author: mahim
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "stm32l4xx_hal.h"
#define FLASH_START_ADDRESS 0x08080000

void writeStringToFlash(uint64_t address, const char* str);
void readDataFromFlash(uint64_t address, char* buffer, uint64_t dataSize);
void eraseFlashPage(uint64_t pageAddress);


#endif /* INC_FLASH_H_ */
