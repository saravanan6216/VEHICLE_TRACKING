///*
// * W25Qxx.h
// *
// */


/*

* W25Qxx.h

*

*/

#ifndef INC_W25QXX_H_

#define INC_W25QXX_H_


#define START_ADDRESS 0x00000000
void W25Q_Reset (void);

uint32_t W25Q_ReadID (void);

void W25Q_Read (uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);

void W25Q_FastRead (uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData);


void W25Q_Erase_Sector (uint16_t numsector);

void W25Q_Write_Clean(uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);

void W25Q_Write (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);

void W25Q_Write_Byte (uint32_t Addr, uint8_t data);

uint8_t W25Q_Read_Byte (uint32_t Addr);


#endif /* INC_W25QXX_H_ */
