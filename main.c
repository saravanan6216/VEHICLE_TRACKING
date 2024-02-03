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
//#include "gsm.h"
int _write(int file,char *ptr,int len)
{
  int i=0;
for(i=0;i<len;i++)
  ITM_SendChar((*ptr++));
return len;
}
uint8_t command[100];
uint8_t response[100];
uint8_t rx[200];
uint8_t RxBuf[200];
//uint8_t *raw;
uint8_t var=0;
//char *rx1;
//char *str;
int k=0;

UART_HandleTypeDef huart4;

int send_mesg_to_server(char *mes,int size)
{
		char buf[20];
		memset(buf,'\0',sizeof(buf));
		sprintf(buf, "AT+CIPSEND=0,%d\r", strlen(mes));
		memset(rx,'\0',sizeof(rx));
		HAL_UART_Transmit_IT(&huart4,(uint8_t*)buf,16);
		HAL_UART_Receive(&huart4, rx, 200,3000);
		printf("Received data %s\n",rx);
		memset(rx,'\0',sizeof(rx));
		HAL_UART_Transmit_IT(&huart4,(uint8_t*)mes,size);
		HAL_UART_Receive(&huart4, rx, 200,3000);
		printf("Received data %s\n",rx);
		if(strstr((char *)rx,"\r\nOK\r\n")!=NULL)
		{
			printf("mesg send successfully\n");
			return HAL_OK;
		}
		else
		{
			printf("mesg NOT send\n");
			return 1;
		}
	return 1;
}


int check(char *command, char *response)
{
	 if((strstr(response,command))==0)
	 {
		 printf("match found\n");
		 return 1;
	 }
	 else
	 {
		 printf("match not found\n");
		 return 0;
	 }
	 printf("response data is %s\n",response);
  }
int check_signal(char *response)
{
	  int rssi = 0, ber = 0;
	  printf("enter\n");
	    for (int i = 0; response[i]; i++) {
	        if (response[i] == ',' && i > 0 && response[i + 1] >= '0' && response[i - 1] >= '0') {
	            // Extracting the RSSI and BER values
	            rssi = (response[i - 2] - '0') * 10 + (response[i - 1] - '0');
	            ber = (response[i + 1] - '0') * 10 + (response[i + 2] - '0');
	            break;
	        }
	    }

	    printf("RSSI: %d, BER: %d\n", rssi, ber);
	    if(rssi > 20)
	    {
	            printf("Signal quality is good\n");
	    		return 1;
	    }
	            else
	            {
	            printf("Signal quality is not good\n");
	    		return 0;
	            }
}

/*void send_at_basic(char *command,char *response)
{
	memset(rx,'\0',sizeof(rx));
	 printf("transmit data %s\n",command);
	 //HAL_UART_Transmit(&huart4, (uint8_t *)command,strlen(command),10);
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)command,strlen(command));
	 HAL_UART_Receive(&huart4, rx, 20,1000);
	 printf("rx data %s\n",(char *)rx);
	 var=check(rx,response);
	 HAL_Delay(3000);
	// return (char *)rx;
}*/
int send_at(char *command,char *response)
{
	memset(rx,'\0',200);
	 printf("transmit data %s\n",command);
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)command,strlen(command));
	 HAL_UART_Receive(&huart4, rx, 200,1000);
	 printf("rx data %s\n",(char *)rx);
	 var=check(rx,response);
	// HAL_Delay(1000);
	 return var;
	// return (char *)rx;
}
int creg(char *response)
{
	int stat = 0;
	    for(int i=0;response[i];i++)
	    {
	        if(response[i] == ',')
	        {
	              stat = response[i+1] - '0';
	              break;
	        }
	    }
	     if (stat == 3)
	     {
	        printf("Registration denied.\n");
	        return 0;
	     }
	    else if ((stat == 6)||(stat==1))
	    {
	        printf("Registered for 'SMS only', home network.\n");
	     	 return 1;
	    }
}
int gprs(char *response)
{


    if (strstr(response, "+CGATT: 1") != NULL) {
        printf("GPRS already attached\n");
        return 1;
    }

    if (strstr(response, "OK\r\n") == NULL) {
        printf("Failed to attach GPRS\n");
        return 0;
    }

    printf("GPRS attached successfully\n");
}
int send_at_fun(char *command,void *(fp)(char *))
{

	memset(rx,'\0',100);
	 printf("transmit data %s\n",command);
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)command,strlen(command));
	 HAL_UART_Receive(&huart4, rx, 100,1000);
	 printf("rx data %s\n",(char *)rx);
	 var=fp(rx);
	 HAL_Delay(1000);
	 return var;
}



void send(char *command,char *data)
{
	 memset(rx,'\0',100);
	 printf("tx start\n");
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)command,100);
	 HAL_Delay(2000);
	 HAL_UART_Receive(&huart4, rx, 200,1000);
	 printf("rx data %s\n",(char *)rx);
	// memset(rx,'\0',100);
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)data,100);
	 printf("tx data %s\n",(char *)data);
	 HAL_UART_Receive(&huart4, rx, 200,1000);
	 printf("rx data %s\n",(char *)rx);
	 HAL_Delay(3000);

}
void ful_flow()
{
			send_at("ATE0\r\n", "OK");
		  	send_at("AT+CSQ\r\n", "CSQ");
		  	send_at("AT+CFUN?\r\n","+CFUN: 1");
		  	send_at("AT+CREG?\r\n","+CREG: 0,6");
		  	send_at("AT+CGREG?\r\n","+CGREG: 0,1");
		  	send_at("AT+CPSI?\r\n","+CPSI: LTE,");
		  	send_at("AT+CMEE=2\r\n","OK");
		 // 	send_at("AT+CGACT=1,1\r\n","OK");
		  	send_at("AT+CGDCONT=1,\"IP\",\"jionet\"\r\n","OK");
		  	send_at("AT+CGREG=1\r\n", "OK");
		  	//send_at("AT+CGATT=1\r\n", "OK");
		  	send_at("AT+NETOPEN\r\n", "OK");
		  	send_at("AT+CIPMODE=0\r\n", "ERROR");
		 // 	send_at("AT+CIPCCFG=2\r\n", "OK");
		  	send_at("AT+CIPOPEN=0,\"TCP\",\"139.59.78.252\",49791\r\n", "OK");
		//  	send("AT+CIPSEND=0,5\r\n");
		  //	send("teamc");
		  	send_at("AT+CIPCLOSE=0\r\n", "OK");
}

int gsm_apn(void)
{

	int a=0,b=0;
			a=send_at("AT+COPS?\r\n","405854");
			if(a==1)
			{
				printf("match found\n");
									b=send_at("AT+CGDCONT=1,\"IP\",\"jionet\"\r\n","OK");
									if(b==1)
									{
										printf("set for jionet\n");
										return 1;
									}
									else
									{
										printf("not setted for jionet\n");
										return 0;
									}

			}
			else
			{
				a=send_at("AT+COPS?\r\n","40449"); //
				if(a==1)
				{

					              printf("match found\n");
									b=send_at("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n","OK");
									if(b==1)
									{
										printf("set for airtel\n");
										return 1;
									}
									else
									{
										printf("not setted for airtel\n");
										return 0;
									}

				}
			}

	}


void gsm_condition()
{
	    int state=1;
	    int substate = 1;
	    int a,b;
	    int count=0;
	 while(state==1)
	 {
	    switch (substate)
	    {
	    	a=0;
	        case 1:
	            printf("at\n");
	            a=send_at("ATE0\r\n", "OK");
	            if(a==1)
	            {
	            printf("Case 1 is Matched.\n");
	            substate=2;
	            }
	            else if(b==10)
	            {
	             state=0;
	            }
	            else{
	                substate=1;
	            }
	            break;

	        case 2:
	           printf("cpin?\n");
	           a=send_at("AT+CPIN?\r\n", "+CPIN: READY");
	            if(a==1)
	            {
	            printf("Case 2 is Matched.\n");
	            substate=8;
	            }
	            else{
	                substate=1;
	            }
	            break;

	        case 3:
	            printf("csq\n");
	            a=send_at_fun("AT+CSQ\r\n",check_signal);
	            if(a==1)
	            {
	            printf("Case 3 is Matched.\n");
	            substate=4;
	            }
	            else if(count==2)
	            {
	            	substate=1;
	            	count=0;
	            }
	            else{
	                printf("at+cfun=1\n");
	              //  send_at("AT+CFUN=1\r\n","OK");
	                substate=3;
	                count++;
	            }
	            break;
	        case 4:
	            printf("creg?\n");
	            a=send_at_fun("AT+CREG?\r\n",creg);
	            if(a==1)
	            {
	            printf("Case 4 is Matched.\n");
	            substate=5;
	            }
	            else{
	                substate=1;
	            }
	            break;
	        case 5:
	            printf("cgreg?\n");
	            a=send_at("AT+CGREG?\r\n","+CGREG");
	            if(a==1)
	            {
	            printf("Case 5 is Matched.\n");
	            substate=6;
	            }
	            else{
	                substate=1;
	            }
	            break;
	        case 6:
	            printf("cpsi?\n");
	            a=send_at("AT+CPSI?\r\n","+CPSI: LTE,Online");
	            if(a==1)
	            {
	            printf("Case 6 is Matched.\n");
	            substate=7;
	            }
	            else{
	                substate=1;
	            }
	            break;
	        case 7:
	            printf("cgdcont\n");
	            a=send_at("AT+CGDCONT=1,\"IP\",\"jionet\"\r\n","OK");
	           // a= gsm_apn();
	            if(a==1)
	            {
	            printf("Case 7 and apn checking is Matched.\n");
	            state=0;
	           // substate=9;
	            }
	            else{
	            	 substate=0;
	            }
	            break;
	        case 8:
	            printf("AT+CMEE=2\n");
	            a=send_at("AT+CMEE=2\r\n","OK");
	            a=1;
	            if(a==1)
	            {
	            printf("Case 8 is Matched.\n");
	            substate=3;

	            }
	            else{
	                substate=1;
	            }
	            break;
	      /*  case 9:
	            printf("AT+CGACT=1,1\n");
	            a=send_at("AT+CGACT=1,1\r\n","OK");
	            if(a==1)
	            {
	            printf("Case 9 is Matched.\n");
	            //substate=10;
	            state=0;
	            }
	            else{
	                substate=1;
	            }
	            break;
			*/
	       }
	  }
}

  void tcp_ip_protocol()
	{
		int state1=1;
		int substate1=1;
		int a=0,b=0,count=0;
		while(state1==1)
		{
			a=0;b=0;
			switch(substate1)
			{

				if(count==10)
				{
					state1=0;
				}
				count++;
				case 0:
					a=send_at("AT+CIPMODE?\r\n", "+CIPMODE: 0");
					if(a==1)
					{
						substate1=1;
					}
					else
					{
						b=send_at("AT+CIPMODE=0\r\n", "OK");
						if(b==1)
						{
							substate1=1;
						}
						else
						{
							substate1=0;
						}

					}break;
				case 1:

					a=send_at("AT+NETOPEN\r\n", "NETOPEN");
					if(a==1)
					{
						substate1=3;
					 }
					else
					{
						substate1=2;
					}

					 break;
				case 2:
							a=send_at("AT+NETOPEN?\r\n", "opened");
							if(a==1)
							{
								substate1=3;
							}
							else
							{
								substate1=1;
							}
							break;

				 case 3:
					 a=send_at("AT+CIPOPEN?\r\n","139.59.78.252");
					 printf("ip already network opened\n");
						if(a==1)
						{
							 //send("AT+CIPSEND=0,5\r","TEAMC\r");
							 a=send_at("AT+CIPSEND=0,5\r","");
							send_mesg_to_server("TEAMC",5);
							 substate1=3;
						}
						else
						{
							 substate1=4;
						}
						break;
				 case 4:
					 a=send_at("AT+CIPOPEN=0,\"TCP\",\"139.59.78.252\",49791\r","CIPOPEN");
					 if(a==1){
						 printf("ip network opened\n");
						 //send("AT+CIPSEND=0,5\r","TEAMC\r");
						 a=send_at("AT+CIPSEND=0,5\r","");
						 send_mesg_to_server("TEAMC",5);
									substate1=3;
					 }
					 else{
						 substate1=3;
					 }
					 break;

				 case 5:
				 	 	 a=send_at("AT+CIPCLOSE=0\r","+CIPCLOSE: 0");
				 	 	 if(a==1)
				 	 	 {
				 	 		 printf("closed");
				 	 		 state1=0;
				 	 	 }
				 	 	 	 break;
			}


		}
	}


int gprs_check(void)
{
	int  a=0;
			  printf("AT+CGATT=1\n");
		      a= send_at("AT+CGATT=1\r\n","OK");
		      printf("gprs working\n");
		      return a;
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


/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
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
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  gsm_condition();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */
	 // printf("hi\n");

	 k=gprs_check();
	printf("k=%d",k);
	tcp_ip_protocol();
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
