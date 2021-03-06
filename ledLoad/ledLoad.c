/*
 * ledLoad.c
 *
 *  Created on: Dec 8, 2015
 *      Author: student
 */

#include "ledLoad.h"

int main(void) {
	int rc = -1;
	int loadFile = open("/proc/loadavg", O_RDONLY);
	int ledFile = open("/sys/class/gpio/gpio20/value", O_WRONLY);
	if (-1 != loadFile) {
		char loadFileData[5] = { };
		while (1) {
			sleep(10);
			size_t resSize = read(loadFile, &loadFileData, (size_t) 4);
			if (4 == resSize) {
				loadFileData[4] = '\0';
				float load = atof(loadFileData);
				if(load > 0.3)
				{
					write(ledFile,'1',1);
				}else
				{
					write(ledFile,'0',1);
				}

			}
		}
	}
	return rc;
}

