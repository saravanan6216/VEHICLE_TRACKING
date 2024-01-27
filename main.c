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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
typedef struct Neo6M_GpsData
{
	double time;
	double latitude;
	double longitude;
	double Knots;
	double Date;
	char N_OR_S;
	char E_OR_W;
	int fix;
	char Data[750];
	char buffer[100];
	char location[100];
	//UART_HandleTypeDef UartPort;
}Neo6M_GpsData;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define Min_To_Degree  0.01666666667
#define Sec_To_Degree	 0.000277777778
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char rx_data[60],tx_data[50],*ptr;
int Flag=0;
Neo6M_GpsData NEO_GPS;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void Get_GpsData(Neo6M_GpsData* GpsData);
static void NEO_GPS_Location(Neo6M_GpsData* GpsData);
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	      if(HAL_UART_Receive(&huart2,(uint8_t*)NEO_GPS.Data,700,HAL_MAX_DELAY))
	      {
	    	  Get_GpsData(&NEO_GPS);
	      }
       	  /*HAL_UART_Receive(&huart2, rx_data,sizeof(rx_data), 1000);
	 	  printf("received string %s\n",rx_data);
	 	  //HAL_Delay(1000);
	 	  ptr=strstr(rx_data,"GPGGA");

	 	  if(ptr!=NULL)
	 	  {
	 		//uint8_t hi[10];
	 		//strcpy(rx_data,hi);

	 		//for(int i=0;rx_data[i]!='\0')
	 	     HAL_UART_Transmit(&huart2, rx_data, sizeof(rx_data), 100);
	 	  	HAL_Delay(1000);
	 	  	printf("transmit to serial monitor\n");
	 	  }

	 		  memset(rx_data,0,sizeof(rx_data)); */
    /* USER CODE BEGIN 3 */
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}
void Get_GpsData(Neo6M_GpsData* GpsData)
{
	char *P;
	int n;
			//HAL_UART_Receive(&GpsData->UartPort,(uint8_t*)GpsData->Data,700,HAL_MAX_DELAY);
	P=strstr(GpsData->Data,"GPGGA");
	if(*P=='G')
	{
	   n=0;
	   while(*(P+n)!='\n')
	   {
		   GpsData->location[n]=*(P+n);
		   n++;
	   }
		sprintf(GpsData->buffer,"%s\r\n\n",GpsData->location);
		sscanf(GpsData->buffer,"GPGGA,%lf,%lf,%c,%lf,%c,%d,,,,,,,*",&GpsData->time,&GpsData->latitude,&GpsData->N_OR_S,&GpsData->longitude,&GpsData->E_OR_W,&GpsData->fix);
		if(GpsData->fix==1||GpsData->fix==2||GpsData->fix==6)
		{
			NEO_GPS_Location(GpsData);
		}
		else
		{
			printf("invalid signal no fix\n");
		}
	 }
	else
	{
		printf("no data\n");
	}
}

static void NEO_GPS_Location(Neo6M_GpsData* GpsData)
{
	double Deg_Val=0.0,Min_Val=0.0,Sec_Val=0.0,lon=0.0,lat=0.0;

	lon=GpsData->longitude;
	lat=GpsData->latitude;
	if((GpsData->E_OR_W=='E' && GpsData->N_OR_S=='S')||((GpsData->E_OR_W=='W' && GpsData->N_OR_S=='N')))
	{
		Deg_Val=(int)(lon/100);
		Min_Val=(int)(lon-(Deg_Val*100));
		Sec_Val=((lon-(Deg_Val*100))-Min_Val)*100;
		GpsData->longitude=Deg_Val+(Min_Val*Min_To_Degree)+(Sec_Val*Sec_To_Degree);
		printf("longitude is %f\n",GpsData->longitude);

		Deg_Val=(int)((lat/100));
		Min_Val=(int)(lat-(Deg_Val*100));
		Sec_Val=((lat-(Deg_Val*100))-Min_Val)*10;
		GpsData->latitude=(Deg_Val+(Min_Val*Min_To_Degree)+(Sec_Val*Sec_To_Degree))*-1;
		printf("latitude is %f\n",GpsData->latitude);

	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
