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
#define PAGE_OFFSET_START   0x000000
#define PAGE_OFFSET_END		0x0000FF

typedef enum ErrorType_
{
	PAGE_OVERSIZE,
	WRITE_ERROR,
	WRITE_OK,
	READ_OK,
}ErrorType;

typedef struct W25qxxType_
{
	uint8_t opcode;
	uint8_t addr[3];
}W25qxxType;


void ReadId(uint8_t *read, uint8_t datasize);
void WriteEnable(void);
W25qxxType BuildCmd(uint8_t opcode, uint32_t reg_addr);
void ProcessCommand(W25qxxType *type);
void WritePage(uint32_t address, uint8_t *data, uint8_t length);
void ReadPage(uint8_t address, uint8_t *data, uint8_t length);
void Set_Offset(uint8_t address, uint8_t *data, uint8_t length);
void EraseSector(uint32_t sectoraddr);


#endif /* INC_W25QXX_H_ */
