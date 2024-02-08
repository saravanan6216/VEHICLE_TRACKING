/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "W25Qxx.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define W25Q32_ID (uint32_t)0xef4016
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */
int _write(int file,char *ptr,int len)

{

  int i=0;

for(i=0;i<len;i++)

  ITM_SendChar((*ptr++));

return len;

}
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */
//UART_HandleTypeDef huart2;
//UART_HandleTypeDef huart2;
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
uint32_t ID = 0;
uint32_t W25QXX_ID;
uint8_t rxData[13];
uint8_t txData[10];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
//Date:070224,Time: 120145,Latitude: 1727.405,Longitude: 7822.510
uint32_t lastAddress = 0;
char str[]="Date:070224,Time:120145,Latitude:1727.405,Longitude:7822.510";
//int len=strlen(str);
char rxr[256];
uint32_t address = 0; // Initial address for writing

// Function to write data to the next address after the last one
/*void writeToNextAddress(uint32_t size, uint8_t *data) {
    // Calculate the next address
    uint32_t nextAddress = lastAddress + 1;

    // Write data to the next address
    W25Q_Write(nextAddress, 0, size, data);

    // Update the last address variable
    lastAddress = nextAddress;
}*/

/*void W25Q_Write (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data)

{

	uint16_t startSector  = page/16;

	uint16_t endSector  = (page + ((size+offset-1)/256))/16;

	uint16_t numSectors = endSector-startSector+1;

	uint8_t previousData[4096];

	uint32_t sectorOffset = ((page%16)*256)+offset;

	uint32_t dataindx = 0;

	for (uint16_t i=0; i<numSectors; i++)

	{

		uint32_t startPage = startSector*16;

		W25Q_FastRead(startPage, 0, 4096, previousData);

		uint16_t bytesRemaining = bytestomodify(size, sectorOffset);

		for (uint16_t i=0; i<bytesRemaining; i++)

		{

			previousData[i+sectorOffset] = data[i+dataindx];

		}

		W25Q_Write_Clean(startPage, 0, 4096, previousData);

		startSector++;

		sectorOffset = 0;

		dataindx = dataindx+bytesRemaining;

		size = size-bytesRemaining;

	}

}
*/
uint8_t buffer[256];
int main(void)
{
  /* USER CODE BEGIN 1 */
	//ReadOffsetFromMemory()
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
 // MX_USART2_UART_Init();
  MX_SPI1_Init();
    /* USER CODE BEGIN 2 */

 // W25QXX_NumberOfSector(64);
   W25Q_Reset();
   HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 // while (1)
 // {
    /* USER CODE END WHILE */
	  ID = W25Q_ReadID();
	  HAL_Delay(200);
	  /*
    uint32_t numWrites = 4; // Change this value as needed

    // Calculate the size of your string
    uint32_t strSize = sizeof(str);

    // Calculate the number of strings that can fit in a page
    uint32_t stringsPerPage = 256 / strSize;

    // Loop to write the string multiple times
    for (uint32_t i = 0; i < numWrites; ++i) {
        // Calculate the page and offset for this write operation
        uint32_t page = i / stringsPerPage;
        uint16_t offset = (i % stringsPerPage) * strSize;

        // Write the string to flash memory
        W25Q_Write(page, offset, strSize, (uint8_t*)str);

    }
    W25Q_FastRead(0, 0, 4*strlen(str), rxr);
    printf("recieved data:%s",rxr);
*/

	// Calculate the number of pages needed to store the string
	      uint32_t numPagesNeeded = (sizeof(str) / 256) + 1; // Add 1 to account for partial page writes

	      // Calculate the number of sectors needed to store the string
	      uint32_t numSectorsNeeded = (numPagesNeeded / 16) + 1; // Add 1 to account for partial sector writes

	      // Calculate the number of blocks needed to store the string
	      uint32_t numBlocksNeeded = (numSectorsNeeded / 16) + 1; // Add 1 to account for partial block writes

	      // Loop to write the string multiple times
	      for (uint32_t block = 0; block < numBlocksNeeded; ++block) {
	          for (uint32_t sector = 0; sector < numSectorsNeeded; ++sector) {
	              for (uint32_t page = 0; page < numPagesNeeded; ++page) {
	                  // Calculate the address for this write operation
	                  uint32_t address = (block * 64 * 1024) + (sector * 4 * 1024) + (page * 256);

	                  // Write the string to flash memory
	                  W25Q_Write(address, 0, sizeof(str), (uint8_t*)str);
	              }
	          }
	      }


	         // Calculate the number of pages needed to read the data
	         uint32_t numPagesToRead = (sizeof(buffer) / 256); // Assuming page size is 256 bytes

	         // Loop to read the data page-wise
	         for (uint32_t page = 0; page < numPagesToRead; ++page) {
	             // Calculate the address for this read operation
	             uint32_t addr = page * 256; // Assuming page size is 256 bytes

	             // Read data from flash memory
	             W25Q_Read(addr, 0, sizeof(buffer), buffer);

	             // Process the read data as needed
	             // For example, you can print the read data
	             printf("Data from page %lu:\r\n", page);
	             for (int i = 0; i < sizeof(buffer); ++i) {
	                 printf("%02X ", buffer[i]);
	                 if ((i + 1) % 16 == 0) {
	                     printf("\r\n");
	                 }
	             }
	             printf("\r\n");
	         }

/*	  W25Q_Read(0, 0, sizeof(buffer), buffer);
	  HAL_Delay(500);
	  printf("recieved data%s\n",buffer);*/
    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
