/*
 * gsm.c
 *
 *  Created on: Feb 7, 2024
 *      Author: mahim
 */


#include "main.h"
#include <stdio.h>
#include <string.h>
#include "gsm.h"

extern UART_HandleTypeDef huart4;
uint8_t command[100];
uint8_t response[100];
uint8_t rx[300];

uint8_t var=0;

int k=0;

/*check signal quality*/

int signal_qualityy()
{
	int check_datcheck_data=0;
	 check_datcheck_data=send_at_fun("AT+CSQ\r\n",check_signal);   //checking signal quality
	 return a;
}



/* sending data to server */

int send_mesg_to_server(char *mes,int size)
{
		char buf[20];
		memset(buf,'\0',sizeof(buf));
		sprintf(buf, "AT+CIPSEND=2,%d\r", strlen(mes));     // server 2 is open and size of message
		memset(rx,'\0',sizeof(rx));
		HAL_UART_Transmit_IT(&huart4,(uint8_t*)buf,16);     // data
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


/*checking  AT command  and Response */



int check(char *command, char *response)
{
	 if(strstr(command,response)!=NULL)
	 {
		 printf("match found\n");
		// printf("response data is %s\n",response);
		 return 1;
	 }
	 else
	 {
		 printf("match not found\n");
		// printf("response data is %s\n",response);
		 return 0;
	 }
	 printf("response data is %s\n",response);
  }


/* checking signal range values is valid or not */
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



/* sending AT command and response is passing to another function */


int send_at(char *command,char *response)
{
	memset(rx,'\0',300);
	 printf("transmit data %s\n",command);
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)command,strlen(command));
	 HAL_UART_Receive(&huart4, rx, 300,1000);
	 printf("rx data %s\n",(char *)rx);
	 var=check(rx,response);
	 return var;
}


/*checking registration  AT command response*/


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


/* GPRS internet connection */


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



/* send AT command and response is called another function */


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


/* Transmit AT commands and receiving response */

void send(char *command,char *data)
{
	 memset(rx,'\0',100);
	 printf("tx start\n");
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)command,100);
	 HAL_Delay(2000);
	 HAL_UART_Receive(&huart4, rx, 100,1000);
	 printf("rx data %s\n",(char *)rx);
	// memset(rx,'\0',100);
	 HAL_UART_Transmit_IT(&huart4, (uint8_t *)data,100);
	 printf("tx data %s\n",(char *)data);
	 HAL_UART_Receive(&huart4, rx, 100,1000);
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


/*checking APN operators name */

int gsm_apn(void)
{

	int check_datcheck_data=0,check_con0;
			check_datcheck_data=send_at("AT+COPS?\r\n","405854");
			if(check_datcheck_data==1)
			{
				printf("match found\n");
									check_consend_at("AT+CGDCONT=1,\"IP\",\"jionet\"\r\n","OK");
									if(check_con=1)
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
				check_datcheck_data=send_at("AT+COPS?\r\n","40449"); //
				if(check_datcheck_data==1)
				{

					              printf("match found\n");
									check_consend_at("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n","OK");
									if(check_con=1)
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


/* GSM BASIC AT COMMANDS INITIALIZATION */


void gsm_condition()
{
	    int state=1;
	    int substate = 1;
	    int check_data,check_con0;
	    int count=0;
	 while(state==1)
	 {
	    switch (substate)
	    {
	    	check_datcheck_data=0;
	        case 1:
	            printf("at\n");
	            check_datcheck_data=send_at("ATE0\r\n", "OK");    //ATE0 commands and response ok
	            if(check_datcheck_data==1)
	            {
	            printf("Case 1 is Matched.\n");
	            substate=2;
	            }
	            else if(check_con=10)
	            {
	            	check_con0;
	             state=0;
	            }
	            else{
	                substate=1;
	            }
	            b++;
	            break;

	        case 2:
	           printf("cpin?\n");
	           check_datcheck_data=send_at("AT+CPIN?\r\n", "+CPIN: READY");  // +CPIN SIM READY
	            if(check_datcheck_data==1)
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
	            check_data=send_at_fun("AT+CSQ\r\n",check_signal);  // CHECK SIGNAL QUALITY
	            if(check_data==1)
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
	              //  send_at("AT+CFUN=1\r\n","OK");        // CHECK FUNCTIONALITY 
	                substate=3;
	                count++;
	            }
	            break;
	        case 4:
	            printf("creg?\n");
	            check_data=send_at_fun("AT+CREG?\r\n",creg);   //CHECK SIM REGISTRATION
	            if(check_data==1)
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
	            check_data=send_at("AT+CGREG?\r\n","+CGREG");  // CHECK GSM Network Registration Status
	            if(check_data==1)
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
	            check_data=send_at("AT+CPSI?\r\n","+CPSI: LTE,Online"); //Cellular Positioning System Interface
	            if(check_data==1)
	            {
	            printf("Case 6 is Matched.\n");
	            substate=7;
	            }
	            else{
	                substate=1;
	            }
	            break;
	        case 7:
	            printf("cgdcont\n");//Check query the parameters of the Packet Data Protocol (PDP) context for establishing a data connection.
	           // check_data=send_at("AT+CGDCONT=1,\"IP\",\"jionet\"\r\n","OK"); 
	            check_data= gsm_apn();
	            if(check_data==1)
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
	            check_data=send_at("AT+CMEE=2\r\n","OK"); // checking the error string
	            check_data=1;
	            if(check_data==1)
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
	            check_data=send_at("AT+CGACT=1,1\r\n","OK");   //check control the activation or deactivation of a PDP
	            if(check_data==1)
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



/*CHECKING and opening the TCP server*/ 



void tcp_ip_protocol()
	{
		int state1=1;
		int substate1=0;
		int check_data=0,check_con0,count=0,c=0;
		while(state1==1)
		{
			check_data=0;check_con0;
			switch(substate1)
			{


				case 0:
					check_data=send_at("AT+CIPMODE?\r\n", "+CIPMODE: 0");   //check TCP/IP protocol stack for internet communication.
					if(check_data==1)
					{
						substate1=1;
					}
					else
					{
						check_consend_at("AT+CIPMODE=0\r\n", "OK");    //TCP/IP protocol stack for internet communication
						if(check_con=1)
						{
							substate1=1;
						}
						else
						{
							if(c==2)
							{
								c=0;
								state1=0;
							}
							else
							{
								substate1=0;
							}
							c++;

						}

					}break;
				case 1:

					check_data=send_at("AT+NETOPEN\r\n", "NETOPEN");  // Telit cellular modules, such as those used in IoT (Internet of Things) applications
					if(check_data==1)
					{
						substate1=3;
					 }
					else
					{
						substate1=2;
					}

					 break;
				case 2:
							check_data=send_at("AT+NETOPEN\r\n", "opened"); 
							if(check_data==1)
							{
								substate1=3;
							}
							else
							{
								substate1=1;
							}
							break;

				 case 3:
					 check_data=send_at("AT+CIPOPEN?\r\n","139.59.78.252");  //  establishing a TCP or UDP connection over a GPRS (General Packet Radio Service) network
					 printf("ip already network opened\n");
						if(check_data==1)
						{
							 //send("AT+CIPSEND=0,5\r","TEAMC\r");
							// check_data=send_at("AT+CIPSEND=0,5\r","");
							send_mesg_to_server("TEAMC",5);
							 //substate1=3;
							state1=0;
						}
						else
						{
							 substate1=4;
						}
						break;
				 case 4:
					 check_data=send_at("AT+CIPOPEN=2,\"TCP\",\"139.59.78.252\",52102\r","CIPOPEN: 2,0"); // check the server opened or not
					 if(check_data==1){
						 printf("ip network opened\n");
						 //send("AT+CIPSEND=0,5\r","TEAMC\r");

						 send_mesg_to_server("TEAMC",5);
									//substate1=3;
						 	 	 	 state1=0;
					 }
					 else{

						 substate1=6;

					 }
					 break;

				 case 5:
				 	 	 check_data=send_at("AT+CIPCLOSE=2\r","+CIPCLOSE: 2");  //server close 
				 	 	 if(check_data==1)
				 	 	 {
				 	 		 printf("closed");
				 	 		 state1=0;
				 	 	 }
				 	 	 	 break;

				 case 6:
				 	 	 {
				 	 		 	 	 	 	if(count==2)
				 	 						{
				 	 							state1=0;
				 	 							count=0;
				 	 						}
				 	 		 	 	 	 	else
				 	 		 	 	 	 	{
				 	 		 	 	 	 	substate1=3;
				 	 		 	 	 	 	}
				 	 						count++;

				 	 	 	 }
			}


		}
	}



/* check GPRS Connection or not */


int gprs_check(void)
{
	int  check_data=0;
			  printf("AT+CGATT=1\n");
		      check_data= send_at("AT+CGATT=1\r\n","OK");
		      printf("gprs working\n");
		      return check_data;
}



/*CHECKING and opening the TCP server and sending data*/


int tcp_server_open(char *data)
{

	int state1=1;
	int substate1=1;
	int check_data=0,check_con0,count=0;
	int size=strlen(data);
	while(state1==1)
	{
		check_data=0;check_con0;
		switch(substate1)
		{
				 case 1:
									 check_data=send_at("AT+CIPOPEN?\r\n","139.59.78.252");
									 printf("ip already network opened\n");
										if(check_data==1)
										{
											 //send("AT+CIPSEND=0,5\r","TEAMC\r");
											// check_data=send_at("AT+CIPSEND=0,5\r","");
											send_mesg_to_server(data,size);
											// substate1=1;
											return 1;
											 state1=0;
										}
										else
										{
											 substate1=2;
										}
										break;
								 case 2:
									 check_data=send_at("AT+CIPOPEN=2,\"TCP\",\"139.59.78.252\",52102\r","CIPOPEN: 2,0");
									 if(check_data==1){
										 printf("ip network opened\n");
										 //send("AT+CIPSEND=0,5\r","TEAMC\r");

										 send_mesg_to_server(data,size);
													//substate1=1;
										 state1=0;
									 }
									 else{
										// substate1=1;
										 substate1=3;
									 }
									 break;
								 case 3:
									if(count==2)
									{
										count=0;
										state1=0;

									}
									else
									{
										substate1=1;
									}
									count++;

									break;
		}
	}
}


