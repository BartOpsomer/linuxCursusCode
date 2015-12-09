/*
 * buttonPush.c
 *
 *  Created on: Dec 8, 2015
 *      Author: student
 */
#include "buttonPush.h"

int main(void) {
	int rc = -1;
	int fileDesc = open("/sys/class/gpio/gpio67/value", O_RDONLY);

	if (0 > fileDesc) {
		rc = 1;
		return rc;
	}
	char value[1] = { };
	int sleepTime = 150000;

	while (1) {
		usleep(sleepTime);
		if (0 > read(fileDesc, value, 1)) {
			write(2, ERROR_MESSAGE_READ_GPIO, sizeof(ERROR_MESSAGE_READ_GPIO));
		} else {
			if (0 == value[0]) {
				write(1, value, 1);
				write(1, "\n", 1);
			}
		}
		if (0 > lseek(fileDesc, 0, SEEK_SET)) {
			rc = 1;
			break;
		}
	}
	if (0 > close(fileDesc)) {
		rc = 1;
	}
	return rc;
}
