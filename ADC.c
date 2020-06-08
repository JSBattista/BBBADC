///
//
//
//
// gcc -Wall -o adctest ADC.c
//
//
// special thanks  https://forum.43oh.com/topic/10366-random-beaglebone-code-snippets/
//
//
//
//
//
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>
#include <unistd.h>      
#include <fcntl.h>     //define O_WONLY and O_RDONLY  
struct adc__container {
	char adc_read[5]; // input buffer
	unsigned char blen;
	const char* ain[6];  // paths
    	unsigned char len;  // length of data from call
};
struct adc__container adc_global = {{0}, 
				    sizeof(adc_global.adc_read -1), 
				    {
					"/sys/bus/iio/devices/iio:device0/in_voltage0_raw",		      
				     	"/sys/bus/iio/devices/iio:device0/in_voltage1_raw",
				     	"/sys/bus/iio/devices/iio:device0/in_voltage2_raw",
		      		     	"/sys/bus/iio/devices/iio:device0/in_voltage3_raw",
	    	      		     	"/sys/bus/iio/devices/iio:device0/in_voltage4_raw",
		      		     	"/sys/bus/iio/devices/iio:device0/in_voltage5_raw"
				    },
      				    0};
unsigned short read__ADC(unsigned short adcp) {   
	short th =  open(adc_global.ain[adcp], O_RDONLY);
	if(th == -1){
		perror(adc_global.ain[adcp]);
		return 0; 
	}   // if
	adc_global.len = read(th, adc_global.adc_read, adc_global.blen);
	close(th);
	if(adc_global.len)return strtol(adc_global.adc_read, NULL, 10);
	return 0;
}

int main()  
{  
	while(1) {
		sleep(0.5);
		unsigned short adc_val = read__ADC(0);
		printf("value coming in: %i    NOTE**** value floats a little\n", adc_val);
		float madc = 4095.0f;
		float mvolts = 1.8f;       
		float bvolts = (mvolts / madc);
		float voltage = adc_val * bvolts;
		printf("volts = %f\n", voltage);
		if (adc_val > 4000) break;
	}
        return 0;  
}  

