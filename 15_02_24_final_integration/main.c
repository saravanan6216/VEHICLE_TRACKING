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
#include "gps.h"
#include "gsm.h"
#include "stdio.h"
#include <string.h>
#include "W25xx.h"

int _write(int file,char *ptr,int len)
{
  int i=0;
for(i=0;i<len;i++)
  ITM_SendChar((*ptr++));
return len;
}
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char data[100]="TEAMC_SERVER";
char gps_info[100];
char rx_buf[200];
uint8_t inc;
uint8_t state,substate,count=0;
int mem_loc=0;
char flash_buf[100];
uint8_t flag=0;
uint32_t ID=0;
uint64_t current_offset;
uint64_t offset_INT_Flash;
uint8_t a;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  	 gsm_condition();
   	  tcp_ip_protocol();
   	  W25Q_Reset();
   	  HAL_Delay(1000);

   	  ID=W25Q_ReadID();
   	  HAL_Delay(200);
      printf("id %x\n",ID);
      printf("Erasing ext flash\n");

  		 // W25Q_Read(START_ADDRESS,0,sizeof(rx_buf),(uint8_t *)rx_buf);
  		 // printf("Initial read %s\n",(char *)rx_buf);

   	  W25Q_Erase_Sector (0);

   	//EraseSector(0x000000);
   	printf("Erased\n");
   	 printf("gps_init_started\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  gps_init();
	  	 	  	  printf("received data by gps %s\n",gps_info);
	  	 	  	  state=1;
	  	 	  	  substate=0;
	  	 	  	  count=0;
	  	 	  while(state==1)
	  	 	  {
	  	 	   switch(substate)
	  	 	   {

	  	 	   	   case 0:
	  	 	   	   {
	  	 	   		   // signal check
	  	 	   		   if(signal_qualityy())
	  	 	   		   {
	  	 	   			   printf("signal checked\n");
	  	 	   			  // substate=1; changing code
	  	 	   			  if(flag==1)
	  	 	   			     {
	  	 	   				  //gsm_condition(); // at commands checking
	  	 	   			     	flag=0;
	  	 	   			     }

	  	 	   			   substate=1;
	  	 	   		   }
	  	 	   		   else
	  	 	   		   {
	  	 	   			   //at init
	  	 	   			  // substate=2;
	  	 	   			   a=send_at("AT+CFUN=1\r\n","OK");
	  	 	   			   if(count==5)
	  	 	   			   {
	  	 	   				 count=0;
	  	 	   			   substate=2;
	  	 	   			   }
	  	 	   			   else
	  	 	   			   {

	  	 	   				   substate=0;
	  	 	   			   }

	  	 	   		   }count++;
	  	 	   	   }break;
	  	 	   	   case 1:
	  	 	   	   {
	  	 	   		   //server check
	  	 	   		   	   	   	 if(tcp_server_open(gps_info))
	  	 	   				     {
	  	 	   				    	 printf("gps_info send data %s\n",gps_info);
	  	 	   				    	// memset(gps_info,'\0',100);						//add
	  	 	   				    	 printf("server is connected and send the data to server\n");
	  	 	   				    	 state=0;
	  	 	   				    	 //connected and send data

	  	 	   				    	 //sending stored flash data
	  	 	   				    if(mem_loc!=0)
	  	 	   				      {
	  	 	   				        printf("sending FLASH data to SERVER\n");
	  	 	   				        for(int pos=0;pos<mem_loc;pos++)
	  	 	   				        {
	  	 	   				        			//	int length;
	  	 	   				     	  	 	   		//  length = strlen(rx_buf);
	  	 	   				        	//ReadPage(0x000000+PAGE_SIZE*pos, flash_buf, sizeof(flash_buf));
	  	 	   				        	//W25Q_Read(START_ADDRESS,(46*pos),sizeof(gps_info),(uint8_t *)gps_info);
	  	 	   				          // W25Q_Read(START_ADDRESS,(43*pos),43 ,(uint8_t *)rx_buf);
	  	 	   				      W25Q_Read(START_ADDRESS+pos,0,44 ,(uint8_t *)rx_buf);  // read from flash memory each page
	  	 	   				    		printf("READ in sending to server %s \n pos%d\n",rx_buf,pos);
	  	 	   				    		tcp_server_open(rx_buf);

	  	 	   				    		if(pos==mem_loc-1)
	  	 	   				    		{
	  	 	   				    		printf("Erasing flash memory");
	  	 	   				    		//EraseSector(0x000000);
	  	 	   				    		W25Q_Erase_Sector(0);
	  	 	   				    		mem_loc=0;
	  	 	   				    		printf("ERASED\n");
	  	 	   				    		}
	  	 	   				    	}
	  	 	   				      }
	  	 	   				     }
	  	 	   				    else
	  	 	   				     {

	  	 	   				    	 substate=2;
	  	 	   				    	//flash
	  	 	   				     }

	  	 	   	   }break;
	  	 	   	   case 2:
	  	 	   	   {

	  	 	   		   // Flash

	  	 	   		  printf("flash storing....\n");
	  	 	   		  printf("WRITE flash storing....\n");
	  	 	   		  int length;
	  	 	   		  length = strlen(gps_info);
	  	 	   		  //WritePage(0x000000+PAGE_SIZE*mem_loc, flash_buf, sizeof(flash_buf));
	  	 	   		//  W25Q_Write(START_ADDRESS,(length*mem_loc),sizeof(gps_info),(uint8_t *)gps_info);
	  	 	   		  printf("case2:flash gps_info %s\n",gps_info);
	  	 	   		 // W25Q_Write(START_ADDRESS,(43*mem_loc),43,(uint8_t *)gps_info);
	  	 	   		  W25Q_Write(START_ADDRESS+mem_loc,0,44,(uint8_t *)gps_info);
	  	 	   		  printf("Write over....\n");
	  	 	   		//  W25Q_Read(START_ADDRESS,(length*mem_loc),sizeof(rx_buf),(uint8_t *)rx_buf);
	  	 	   		 // W25Q_Read(START_ADDRESS,(43*mem_loc),43,(uint8_t *)rx_buf);
	  	 	   		  W25Q_Read(START_ADDRESS+mem_loc,0,44,(uint8_t *)rx_buf);
	  	 	   		  printf("READ in flash %s\n pos=%d\n",rx_buf,mem_loc);
	  	 	   		  printf("read in flash ended\n");
	  	 	   		  HAL_Delay(500);
	  	 	   		 memset(rx_buf,'\0',200);
	  	 	   		  HAL_Delay(500);
	  	 	   		  printf("memset read:%s\n",rx_buf);

	  	 	   		 // eraseFlashPage(0x08080000);
	  	 	   	      //writeValueToFlash(current_offset);
	  	 	   		  mem_loc++;
	  	 	   		  state=0;
	  	 	   		  HAL_Delay(1000);
	  	 	   		  flag=1; // checking for go back gsm command flag
	  	 	   		// memset(gps_info,'\0',100); //add
	  	 	   	   }break;
	  	 	   }
	  	 	  }
	    }

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
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
