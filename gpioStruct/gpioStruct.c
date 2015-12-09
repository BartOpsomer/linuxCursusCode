/*
 * gpioStruct.c
 *
 *  Created on: Dec 9, 2015
 *      Author: student
 */

#include "gpioStruct.h"

int initStruct(gpioStruct * gpio) {
	int rc = -1;
	if (pinStatus_notInitialised == gpio->initStatus) {
		int gpioExportFile = open("/sys/class/gpio/export", O_WRONLY);
		if (0 <= gpioExportFile) {
			char stringPtr[5] = { };
			int resultCode = snprintf(stringPtr, 4, "%d\n",
					gpio->gpioPinNumber);
			if (5 > resultCode && 0 < resultCode) {
				if (0 < write(gpioExportFile, stringPtr, resultCode+1)) {

					char gpioFile[128] = { };
					int gpioFileLength = snprintf(gpioFile, 128,"/sys/class/gpio/gpio%d/direction",gpio->gpioPinNumber);
					int gpioDirFileDesc = open(gpioFile, O_WRONLY);
					if (0 <= gpioDirFileDesc) {

						const char * dir = NULL;
						if (pinDir_in == gpio->direction)
							dir = "in";
						else if (pinDir_out == gpio->direction)
							dir = "out";
						size_t stringLenth = strlen(dir);
						write(gpioDirFileDesc, dir,stringLenth+1 );
						close(gpioDirFileDesc);
						gpio->initStatus = pinStatus_initialised;
						rc = 0;
					}
				}
				else
				{
					printf("export open failed because:%s\n",strerror(errno));
				}
				close(gpioExportFile);
			}
		}
	} else {
		rc = 0;
	}

	return rc;
}

int readPin(gpioStruct * gpio) {
	int rc = -1;
	if ((pinStatus_initialised == gpio->initStatus)
			&& (pinDir_in == gpio->direction)) {
		char gpioFile[128] = { };
		int gpioFileLength = snprintf(gpioFile, 128,
				"/sys/class/gpio/gpio%d/value", gpio->gpioPinNumber);
		int gpioDirFileDesc = open(gpioFile, O_RDONLY);
		if (0 <= gpioDirFileDesc) {
			char value[1];
			if (0 < read(gpioDirFileDesc, value, 1)) {
				gpio->value = value[0] - '0';
				rc = 0;
			}
			else{
				printf("eroor: %s\n",strerror(errno));
			}
		}
		else
		{
			printf("error: %s",strerror(errno));
		}
	}
	return rc;
}

int writePin(gpioStruct * gpio) {
	int rc = -1;
	if ((pinStatus_initialised == gpio->initStatus)
			&& (pinDir_out == gpio->direction)) {
		char gpioFile[128] = { };
		int gpioFileLength = snprintf(gpioFile, 128,
				"/sys/class/gpio/gpio%d/value", gpio->gpioPinNumber);
		int gpioDirFileDesc = open(gpioFile, O_WRONLY);
		if (0 > gpioDirFileDesc) {
			char value[1] = { (char) (gpio->value + '0') };
			if (0 < write(gpioDirFileDesc, value, 1)) {
				rc = 0;
			}
			else{
				printf("error: %s\n",strerror(errno));
			}
		}
		else
		{
			printf("error: %s\n",strerror(errno));
		}
	}
	return rc;
}

int main(void) {
	int rc = -1;

	gpioStruct gpio = {};
	gpio.direction = pinDir_in;
	gpio.gpioPinNumber = 67;
	gpio.initStatus = pinStatus_notInitialised;
	gpio.value = 0;

	rc = initStruct(&gpio);
	printf("the init of the struct was: %d\n",rc);

	while(1)
	{
		sleep(1);
		rc = readPin(&gpio);
		printf("the reading of the pin was: %d\n",rc);
		printf("the reading of the value was: %d\n",gpio.value);
	}


	return rc;
}