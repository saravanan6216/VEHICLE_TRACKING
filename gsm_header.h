/*
 * hi.h
 *
 *  Created on: Jan 27, 2024
 *      Author: DELL
 */

#ifndef SRC_HI_H_
#define SRC_HI_H_
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void hi()
{
	printf("hi\n");
	HAL_Delay(1000);
}

#define GsmHuart  huart2

//extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

  char AT[80];
  char buf[30];
  uint8_t AT_OK = 0;
  uint8_t result=0;

void send(char *AT)
{
	HAL_UART_Transmit(&huart2,(uint8_t*)AT,strlen(AT),1000);
	printf("SEND %s\n",AT);
}

void receive(void)
{
	HAL_UART_Receive(&huart2,(uint8_t*)buf, 30, 100);
	HAL_Delay(1000);
	printf("REC %s %s\n",AT,buf);

}
int receive_i(void)
{
	HAL_UART_Receive(&huart2,(uint8_t*)result,sizeof(result), 100);
	HAL_Delay(1000);
	printf("REC %s %d\n",AT,result);
	return result;

}
void send_r(void)
{
	HAL_UART_Transmit(&huart2,(uint8_t*)buf,strlen(buf), 100);
	HAL_Delay(1000);
	printf("SEND_r %s\n",buf);

}
void test1()
{
	 printf("working\n");
		  hi();
		  send("AT");
		 receive();
}
void AT_cmd(char *AT)
{
	send(AT);
	receive();

}

void basic_AT(void)
{
	 while(!AT_OK){
	  		//sprintf(AT,"AT\r\n");
	  		send("AT");
	  		receive();
	  		if(strstr((char *)buf,"OK")){
	  			AT_OK = 1;
	  		}
	  		HAL_Delay(1000);
	  		memset(buf,0,sizeof(buf));
	  }
}
void sendATCommand(const char* command)
{

    HAL_UART_Transmit(&huart2, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
    HAL_Delay(1000);
}
void sendSMS(const char* phoneNumber, const char* message) {
    char command[100];
    sendATCommand("AT+CMGF=1");
    sprintf(command, "AT+CMGS=\"%s\"", phoneNumber);
    sendATCommand(command);
    sendATCommand(message);
    sendATCommand("\x1A");
}
void initGPRS(void)
{
    sendATCommand("AT\r\n");         // Check communication link
    sendATCommand("AT+CGATT?\r\n");  // Check GPRS attachment status and Checking SIM card has internet connection or not
}
int check_gprs()
{
	 sendATCommand("AT+CGATT?\r\n");  // Check GPRS attachment status and Checking SIM card has internet connection or not
	 if(receive_i())
	 {
		 printf("internet connected \n");
		 return 1;
	 }
	 else
	 {
		 printf("internet not available \n");
		 return 0;
	 }
}
void establishGPRSConnection(void)
{
    sendATCommand("AT+CIPSHUT\r\n");                             // Shut down GPRS connection
    sendATCommand("AT+CGATT=1\r\n");                             // Attach to GPRS network
    sendATCommand("AT+CSTT=\"airtelgprs.com\",\"\",\"\"\r\n");         // Set APN
    sendATCommand("AT+CIICR\r\n");                               // Bring up wireless connection
    HAL_Delay(1000); // Allow some time for the connection to be established
}

/*
 *
AT+NETOPEN Start Socket Service
AT+NETCLOSE Stop Socket Service
AT+CIPOPEN Establish Connection in Multi-Socket Mode
AT+CIPSEND Send data through TCP or UDP Connection
AT+CIPRXGET Set the Mode to Retrieve Data
AT+CIPCLOSE Close TCP or UDP Socket
AT+IPADDR Inquire Socket PDP address
AT+CIPHEAD Add an IP Header When Receiving Data
AT+CIPSRIP ShowRemoteIP Address and Port
AT+CIPMODE Set TCP/IP Application Mode
AT+CIPSENDMODE Set Sending Mode
AT+CIPTIMEOUT Set TCP/IP Timeout Value
AT+CIPCCFG Configure Parameters of Socket
AT+SERVERSTART Startup TCP Sever
AT+SERVERSTOP Stop TCP Sever
AT+CIPACK Query TCP Connection Data Transmitting Status
AT+CDNSGIP Query the IP Address of Given Domain Name
AT+CSOCKSETPN Set acitve PDP context's profile
 *
 */

void tcp_ip()
{
	 sendATCommand("AT+NETOPEN");  //Start Socket Service
	 sendATCommand("AT+CIPOPEN");  //Establish Connection in Multi-Socket Mode
	 	 	 	 	 	 	 	   //AT+CIPOPEN=0,"TCP","remote_ip_address",remote_port

	 sendATCommand("AT+CIPSEND");  //Send data through TCP or UDP Connection
	 	 	 	 	 	 	 	 // AT+CIPSEND=0,data_length          Terminate with CTRL + Z.
	 	 	 	 	 	 	 	 //AT+CIPRXGET=2,1


	 sendATCommand("AT+CIPCLOSE");  //Close TCP or UDP Socket
	 sendATCommand("AT+NETCLOSE");  //Stop Socket Service
}

void fun()
{
	basic_AT();
	HAL_Delay(1000);
	sendSMS("+919629055521","hello");
	HAL_Delay(1000);
	if(check_gprs())
	{
		tcp_ip();
	}
	else
	{
		//.....
	}


}

#endif /* SRC_HI_H_ */
