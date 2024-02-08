/*
* W25Qxx.h
*
*  Created on: Feb 7, 2024
*      Author: Lenovo
*/
 
#ifndef INC_W25QXX_H_
#define INC_W25QXX_H_
 
#define SECTOR_SIZE 4096
#define PAGE_SIZE 256
#define TOTAL_PAGES 256
#define TOTAL_SIZE PAGE_SIZE*TOTAL_PAGES
 
typedef enum ErrorType_
{
	PAGE_OVERSIZE,
	WRITE_ERROR,
	WRITE_OK,
	READ_OK,
}ErrorType;
 
void ReadId(uint8_t *read, uint8_t datasize);
void WriteEnable(void);
void WritePage(uint32_t address, uint8_t *data, uint8_t datasize);
uint32_t GetCurrentAddress(uint32_t addr, uint8_t *data, uint8_t datasize);
void ReadPage(uint32_t address, uint8_t *data, uint8_t datasize);
void WriteDataToFlash(uint32_t startAddress, uint8_t *data, uint32_t dataSize);
void EraseSector(uint32_t address, uint8_t *data, uint8_t datasize);
 
#endif /* INC_W25QXX_H_ */

