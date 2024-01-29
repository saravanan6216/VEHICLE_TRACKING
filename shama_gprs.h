/*
 * shama_gprs.h
 *
 *  Created on: Jan 29, 2024
 *      Author: aksha
 */

#ifndef INC_SHAMA_GPRS_H_
#define INC_SHAMA_GPRS_H_


void sendATCommand(const char* command);
void initGPRS(void);
void establishGPRSConnection(void);


void sendATCommand(const char* command)
{
    // Send AT command to GPRS module
    HAL_UART_Transmit(&huart2, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);

    // Receive and check response
  //  char responseBuffer[100];
    //HAL_UART_Receive(&huart2, (uint8_t*)responseBuffer, sizeof(responseBuffer), HAL_MAX_DELAY);


}

void initGPRS(void)
{
    sendATCommand("AT\r\n");         // Check communication link
    sendATCommand("ATE0\r\n");       // Disable command echo
    sendATCommand("AT+CGATT?\r\n");  // Check GPRS attachment status and Checking SIM card has internet connection or not
}

void establishGPRSConnection(void)
{
	sendATCommand("AT+CIPCLOSE\r\n"); 	//AT+CIPCLOSE is used to close a TCP or UDP Socket
	sendATCommand("AT+CGATT=1\r\n");  	//Attach to gprs network
	sendATCommand("AT+CGDCONT=1,\"IP\",\"airtel.gprs\"\r\n");  //set APN
	sendATCommand("AT+CGACT=1,1\r\n");   //Activate PDP context
	HAL_Delay(1000);
	sendATCommand("AT+CGPADDR\r\n");


}
#endif /* INC_SHAMA_GPRS_H_ */
