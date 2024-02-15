/*
 * W25Qxx.c
 *
 *  Created on: Feb 7, 2024
 *      Author: Lenovo
 */


#include "main.h"
#include "W25Qxx.h"
#include<string.h>

extern SPI_HandleTypeDef hspi1;

#define W25QXX_WRITE_CMD	0x02
#define W25QXX_READ_CMD		0x03
#define W25QXX_ERASE_SECTOR 0x20
#define W25QXX_READ_STATUS 0x05

#define BUFFER_EMPTY 0
#define HSPI1 hspi1

#define SECTOR_SIZE 4096
uint32_t gAddr;

void ReadId(uint8_t *read, uint8_t datasize)
{
	static uint8_t cmd = 0x9F;
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	  HAL_SPI_Transmit(&HSPI1, &cmd, 1, 1000);
	  HAL_SPI_Receive(&HSPI1, read, 3, 3000);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
void WriteEnable(void)
{
    static uint8_t cmd = 0x06;
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	  HAL_SPI_Transmit(&HSPI1, &cmd, 1, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
W25qxxType BuildCmd(uint8_t opcode, uint32_t reg_addr) {
    W25qxxType cmd;
    cmd.opcode = opcode;
    cmd.addr[0] = (uint8_t)(reg_addr >> 16);
    cmd.addr[1] = (uint8_t)(reg_addr >> 8);
    cmd.addr[2] = (uint8_t)reg_addr;
    return cmd;
}
void ProcessCommand(W25qxxType *type)
{
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	 HAL_SPI_Transmit(&HSPI1, &(type->opcode), sizeof(type->opcode), HAL_MAX_DELAY);
	 HAL_SPI_Transmit(&HSPI1, type->addr, sizeof(type->opcode), HAL_MAX_DELAY);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
void EraseSector(uint32_t sectoraddr)
{
	sectoraddr=sectoraddr*16*256;
	W25qxxType cmd = BuildCmd(W25QXX_ERASE_SECTOR, sectoraddr);
		ProcessCommand(&cmd);
	HAL_Delay(450);// 450ms delay for sector erase

}
void WritePage(uint32_t address, uint8_t *data, uint8_t length)
{
	W25qxxType cmd = BuildCmd(W25QXX_WRITE_CMD, 0);
	ProcessCommand(&cmd);
	WriteEnable();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	 HAL_SPI_Transmit(&HSPI1, &data, length, HAL_MAX_DELAY);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
void ReadPage(uint8_t address, uint8_t *data, uint8_t length)
{
	W25qxxType cmd = BuildCmd(W25QXX_WRITE_CMD, 0);
		ProcessCommand(&cmd);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		 HAL_SPI_Receive(&HSPI1, &data, length, HAL_MAX_DELAY);
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
void Set_Offset(uint8_t address, uint8_t *data, uint8_t length)
{
	 W25qxxType cmd = BuildCmd(W25QXX_WRITE_CMD, 0);
	 ProcessCommand(&cmd);
	 WriteEnable();
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	 HAL_SPI_Transmit(&HSPI1, &data, length, HAL_MAX_DELAY);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
