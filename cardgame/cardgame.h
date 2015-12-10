/*
 * cardgame.h
 *
 *  Created on: Dec 9, 2015
 *      Author: student
 */

#ifndef CARDGAME_H_
#define CARDGAME_H_

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
typedef enum {
	coulour_unknown = 0,
	colour_spades = 1,
	colour_clubs = 2,
	colour_hearts = 3,
	colour_diamonds = 4,
	colour_size = 5
} cardColour_e;
typedef enum {
	number_unkown = 0,
	number_ace,
	number_2,
	number_3,
	number_4,
	number_5,
	number_6,
	number_7,
	number_8,
	number_9,
	number_jack,
	number_queen,
	number_king,
	number_size
} cardNumber_e;

typedef struct {
	cardNumber_e cardNumber;
	cardColour_e cardColour;
} card_t;
typedef struct _listnode {
	card_t card;
	struct _listnode * next;
	struct _listnode * prev;
} linkedListNode_t;

void initCardDeck(void) ;
int listAppend(linkedListNode_t * list, card_t * card);
int listSize(linkedListNode_t * list);\
int main(int argc, char * argv[]);

#endif /* CARDGAME_H_ */
