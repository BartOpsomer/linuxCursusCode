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
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  new.c_cc[VTIME] = 1;
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}

void resetDetectArrow(void)
{
	  resetTermios();
}

void detectArrow(void) {
	while (1) {
		if (getch() == '\033') { // if the first value is esc
			getch(); // skip the [
			switch (getch()) { // the real value
			case 'A':
				printf("arrow up\n");
				break;
			case 'B':
				printf("arrow down\n");
				break;
			case 'C':
				printf("arrow right\n");
				break;
			case 'D':
				printf("arrow left\n");
				break;
			}
		}
	}
}
