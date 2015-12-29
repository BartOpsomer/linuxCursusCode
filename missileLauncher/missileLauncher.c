/*
 * missileLauncher.c
 *
 *  Created on: Dec 10, 2015
 *      Author: student
 */
#include "missilleLauncher.h"

const unsigned char turretUpData[] = { 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00 }; //data to write
const unsigned char turretDownData[] = { 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 }; //data to write
const unsigned char turretLeftData[] = { 0x02, 0x04, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 }; //data to write
const unsigned char turretRightData[] = { 0x02, 0x08, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 }; //data to write
const unsigned char turretStopData[] = { 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 }; //data to write
const unsigned char turretFireData[] = { 0x02, 0x10, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 }; //data to write

static struct usb_device s_missileLauncher = { };
static usb_dev_handle * s_dev_handle = NULL;

void setDevice(struct usb_device * dev) {
	s_missileLauncher = *dev;
}

int openDevice(void) {
	s_dev_handle = NULL;
	usb_dev_handle *dev_handle; //a device handle
	struct usb_device * deviceOfUsb = &s_missileLauncher; //a libusb session
	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n------------------------------------\n");
	dev_handle = usb_open(deviceOfUsb);
	if (dev_handle == NULL) {
		//printf("Cannot open device \n");
	} else {
		//printf("Device Opened \n");
		s_dev_handle = dev_handle;
	}

	int actual; //used to find out how many bytes were written
	char name[200];
	size_t size = 200;
	if (usb_get_driver_np(dev_handle, 0, name, size) == 0) { //find out if kernel driver is attached
		//printf("Kernel Driver Active \n");
		if (usb_detach_kernel_driver_np(dev_handle, 0) == 0) //detach it
				{
			//printf("Kernel Driver Detached! \n");
		}
	}

	int r = usb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
	if (r < 0) {
		printf("Cannot Claim Interface \n");
		printf("%s\n", strerror(errno));
		usb_close(dev_handle);
		return 1;
	}
	//printf("Claimed Interface \n");
	return 0;
}

int closeDevice(void) {
	int r = usb_release_interface(s_dev_handle, 0); //release the claimed interface
	if (r != 0) {
		printf("Cannot Release Interface \n");
		usb_close(s_dev_handle);
		return 1;
	}
	//printf("Released Interface \n");

	usb_close(s_dev_handle); //close the device we opened
	return 0;
}

int sendAction(action_e action) {
	//printf("Data-> %s \n", data); //just to see the data we want to write : abcd
	//printf("Writing Data... \n");
	int numberOfiterations = 1;
	const unsigned char * data = turretStopData;
	switch (action) {
	case action_left:
		numberOfiterations = 1;
		data = turretLeftData;
		break;
	case action_right:
		numberOfiterations = 1;
		data = turretRightData;
		break;
	case action_up:
		numberOfiterations = 1;
		data = turretUpData;
		break;
	case action_down:
		numberOfiterations = 1;
		data = turretDownData;
		break;
	case action_stop:
		numberOfiterations = 1;
		data = turretStopData;
		break;
	case action_fire:
		numberOfiterations = 1;
		data = turretFireData;
		break;
	default:
		numberOfiterations = 1;
		data = turretStopData;
		break;
	}

	int r = 1;
	for (int i = 0; i < numberOfiterations && r > 0; i++) {
		r = usb_control_msg(s_dev_handle, 0x21, 0x09, 0, 0, (char *) data, 8,
				5000); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
		/*		if (r > 0) //we wrote the 4 bytes successfully*/
		/*			printf("Writing Successful! \n");*/
		/*		else*/
		/*			printf("Write Error \n");*/
		//printf("Writing Successful! \n");
	}
	if(action == action_fire)
	{
		sleep(7);
	}
	return r;
}

int testSendData(action_e action) {

	int rc = openDevice();

	rc = sendAction(action);

	rc = closeDevice();

	return 0;
}

static volatile int keepRunning = 1;

void intHandler(int dummy) {
	keepRunning = 0;
}

int main(void) {

	signal(SIGINT, intHandler);
	discoverUSB();
	openDevice();

	keyboard_e keyboardinput = keyboard_unknown;
	action_e action = action_unkown;
	while (keepRunning) {
		keyboardinput = keyboard_unknown;
		keyboardinput = detectArrow();
		action = convertKeyboardToAction(keyboardinput);
		sendAction(action);

	}
	printf("\nreset the terminal\n");
	resetDetectArrow();
	closeDevice();

	return 0;
}

action_e convertKeyboardToAction(keyboard_e key) {
	action_e rc = action_unkown;
	switch (key) {
	case keyboard_arrow_down:
		rc = action_down;
		break;
	case keyboard_arrow_up:
		rc = action_up;
		break;
	case keyboard_arrow_right:
		rc = action_right;
		break;
	case keyboard_arrow_left:
		rc = action_left;
		break;
	case keyboard_enter:
		rc = action_fire;
		break;
	default:
		rc = action_stop;
		break;
	}
	//printf("the key is :%d\n",rc);
	return rc;
}

