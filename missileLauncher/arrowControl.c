/*
 * arrowControl.c
 *
 *  Created on: Dec 27, 2015
 *      Author: bart
 */
#include "missilleLauncher.h"

#include <termios.h>
#include <stdio.h>

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	new.c_cc[VTIME] = 1; //set timeout to 0.1s
	new.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void) {
	return getch_(0);
}

void resetDetectArrow(void) {
	resetTermios();
}

keyboard_e detectArrow(void) {
	keyboard_e rc = keyboard_unknown;
	char c = getch();
	if (c == '\033') { // if the first value is esc
		getch(); // skip the [
		switch (getch()) { // the real value
		case 'A':
			//printf("arrow up\n");
			rc = keyboard_arrow_up;
			break;
		case 'B':
			//printf("arrow down\n");
			rc = keyboard_arrow_down;
			break;
		case 'C':
			//printf("arrow right\n");
			rc = keyboard_arrow_right;
			break;
		case 'D':
			//printf("arrow left\n");
			rc = keyboard_arrow_left;
			break;
		}
	} else if (c == '\n') {
		//printf("enter\n");
		rc = keyboard_enter;

	} else {
		//printf("timeout:%c\n",c);
	}

	return rc;
}
