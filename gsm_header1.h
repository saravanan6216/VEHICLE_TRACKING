/*
 * gsm_header1.h
 *
 *  Created on: 29-Jan-2024
 *      Author: Dell
 */

#ifndef SRC_GSM_HEADER1_H_
#define SRC_GSM_HEADER1_H_

#include "main.h"
#include "string.h"
#include "stdio.h"


void tcp_connect(void)
{

  gsm_send_command("AT+NETOPEN",1000);
  gsm_send_command("AT+CGDCONT=\"airtelgprs.com\",\" \",\" \"", 1000);
  gsm_send_command("AT+IPADDR", 10000); //
  gsm_send_command("AT+CIPOPEN=\"TCP\",\"192.168.1.20\",\"21\"", 10000);
}

void gsm_send_command(const char *command, uint32_t timeout)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
  HAL_Delay(timeout);
}

void gsm_receive_response(char *response, uint32_t timeout)
{
  HAL_UART_Receive(&huart2, (uint8_t *)response, 255, timeout);
  response[255] = '\0'; // Null-terminate the response string
}

void get_gps_data(char *latitude, char *longitude, char *timestamp)
{

  //gsm_send_command("AT+CGPSINFO", 5000);

  char response[256];
  gsm_receive_response(response, 5000);

  sscanf(response, "lOCATION INFORMATION: %[^,],%[^,],%[^.]", latitude, longitude, timestamp);
}

void send_gps_data(const char *latitude, const char *longitude, const char *timestamp)
{
  //gsm_send_command("AT+CIPOPEN=\"TCP\",\"192.168.1.20\",\"21\"", 10000);

  char gps_data[100];
  snprintf(gps_data, sizeof(gps_data), "latitude: %s, longitude: %s, timestamp: %s", latitude, longitude, timestamp);
  gsm_send_command("AT+CIPSEND", 1000);
  HAL_Delay(1000);

  gsm_send_command(gps_data, 10000); //



  char response[256];
  gsm_receive_response(response, 5000);  // servr response
  printf("server rresponse: %s\r\n", response);


  gsm_send_command("AT+CIPCLOSE", 1000);
}




#endif /* SRC_GSM_HEADER1_H_ */
