/*
 * gsm_gprs.h
 *
 *  Created on: Feb 3, 2024
 *      Author: mahim
 */

#ifndef INC_GSM_GPRS_H_
#define INC_GSM_GPRS_H_

int check(char *command, char *response);
int check_signal(char *response);
int send_at(char *command,char *response);
int creg(char *response);
int gprs(char *response);
int send_at_fun(char *command,void *(fp)(char *));
void send(char *command,char *data);
void ful_flow();
int gsm_apn(void);
void gsm_condition();
int tcp_server_open(char *data);
void tcp_ip_protocol();


#endif /* INC_GSM_GPRS_H_ */
