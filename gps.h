/*
 * gps.h
 *
 *  Created on: Feb 7, 2024
 *      Author: mahim
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

typedef struct
{
	double time;
	float latitude;
	float longitude;
	int Date;
	char N_OR_S;
	char E_OR_W;
	int fix;
	char Data[750];
	char buffer[250];
	char rmc_buffer[250];
}gpsdata;


void gpsData(gpsdata* data);
void gpslocation_extraction(gpsdata* data);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void gps_init(void);




#endif /* INC_GPS_H_ */
