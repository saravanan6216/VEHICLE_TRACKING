 /*
 * gps.c
 *
 *  Created on: Feb 7, 2024
 *      Author: mahim
 */


#include "main.h"
#include <stdio.h>
#include <string.h>
#include "gps.h"

extern UART_HandleTypeDef huart1;
extern char gps_info[100];
gpsdata gps;
int dd,mm,yy;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        gpsData(&gps);
    }
}
/**********gps initalisation *****/
void gps_init(void)
{
	HAL_UART_Receive_IT(&huart1,&gps.Data,1000);
		printf("received start from MODULE\n");
	   // printf("%s\n",gps.Data);
	    printf("received end  from MODULE\n");
}

/******Function for extracting GNGGA and GNRMC NMEA strings
  extracting latitude, longitude, time and date
   checking the fix quality*****/
void gpsData(gpsdata *data)
{
	char *gga_ptr,*rmc_ptr;
	int n=0;
	int rmc_len;
	if(data->Data!=NULL)
	{
	gga_ptr=strstr(data->Data,"GNGGA");
	rmc_ptr=strstr(data->Data,"GNRMC");
	}
	else
	{
		printf("no data in buffer\n");
	}
	if(gga_ptr!=NULL)
	{
	if(*gga_ptr=='G')
	{
	   n=0;
	   strncpy(data->buffer,gga_ptr,80);
	   printf("GGA string is %s\n", data->buffer);
	   sscanf(data->buffer,"GNGGA,%lf,%lf,%c,%lf,%c,%d,",&data->time,&data->latitude,&data->N_OR_S,&data->longitude,&data->E_OR_W,&data->fix);
	}
	else
		{
			printf("GGA string not found\n");
		}

	}
	else
	{
		printf("GGA string not found\n");
	}


	if(rmc_ptr!=NULL)
		{
		if(*rmc_ptr=='G')
		{
				   n=0;
				   strncpy(data->rmc_buffer,rmc_ptr,90);
				   printf("RMC string is %s\n", data->rmc_buffer);
				   rmc_len=strlen(data->rmc_buffer);
				   if(rmc_len>=64)
				   {
				   	 sscanf(data->rmc_buffer,"GNRMC,%*lf,%*c,%*f,%*c,%*f,%*c,%*f,%*f,%d,",&data->Date);
				   }
		}
	    else
	    {
		           printf("RMC string not found\n");
	    }
    }
	else
		{
			printf("RMC string not found\n");
		}
	if(data->fix==1)
	{
			printf("valid fix\n");
			gpslocation_extraction(data);
	 }
	else
	{
			sprintf(gps_info, "%s","invalid sig:no fix:Unable to get accurate location\n");
	}
}

/***function for converting latitude and longitude values, converting to IST time***/
void gpslocation_extraction(gpsdata* data)
{
	double Deg_Val=0.0,Sec_Val=0.0,lon=0.0,lat=0.0;
	int hr,min,sec;
	lon=data->longitude;
	lat=data->latitude;
	if((data->E_OR_W=='E' || data->N_OR_S=='S')||(data->E_OR_W=='W' || data->N_OR_S=='N'))
	{
		            // Extract degrees and seconds directly
				    Deg_Val = (int)(lon/100); // Extract degrees
				    Sec_Val = fmod(lon, 100); // Extract remaining seconds
				    lon= Deg_Val + Sec_Val / 60.0; // Convert seconds to minutes
				    data->longitude=lon;

				    Deg_Val = (int)(lat/100);
				    Sec_Val = fmod(lat, 100);
				    lat = Deg_Val + Sec_Val / 60.0;
				    data->latitude=lat;
	}
	    hr=(int)((data->time)/10000);
	   	min=(int)(data->time-(hr*10000))/100;
	   	sec=(int)(data->time-((hr*10000)+(min*100)));
	   	int ist_hr = hr + 5;  // 5 hours difference
	   	int ist_min = min + 30;  // 30 minutes difference
        if(ist_hr>=24)
        {
        	ist_hr-=24;
        }
	   	if (ist_min >= 60)
	   	{
	   	    ist_hr++;
	   	    ist_min -= 60;
	   	}
	    dd=(data->Date)/10000;
	    mm=(data->Date-(dd*10000))/100;
	    yy=(data->Date-((dd*10000)+(mm*100)));

		sprintf(gps_info,"$%lf:%c:%lf:%c:%02d.%02d.%02d:%02d/%02d/%02d#",data->latitude,data->N_OR_S, data->longitude,data->E_OR_W,ist_hr,ist_min,sec,dd,mm,yy);

}

