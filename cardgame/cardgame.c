/*
 * cardgame.c
 *
 *  Created on: Dec 9, 2015
 *      Author: student
 */
#include "cardgame.h"

static card_t s_cardDeck[52] = { };
static int s_pickColour = 0;
static int s_linkedListSize = 0;
static cardColour_e s_colour = colour_spades;

void initCardDeck(void) {
	for (int i = 0; i < 52; i++) {
		card_t card = { };
		card.cardColour = (i % colour_size) + 1;
		card.cardNumber = (i % number_size) + 1;
		s_cardDeck[i] = card;
	}
}
int listSize(linkedListNode_t * list) {
	int rc = -1;
	int count = 0;
	if (list->prev != NULL) {
		count++;
	}
	while (list->next != NULL) {
		list = list->next;
		count++;
	}
	return rc;
}
void listPrint(linkedListNode_t * list) {
	int count = 0;
	while (list != NULL) {
		printf("card %d is : colour:%d number:%d\n", count,
				list->card.cardColour, list->card.cardNumber);
		count++;
		list = list->next;
	}

}
int listAppend(linkedListNode_t * list, card_t * card) {
	int rc = 0;
	if (list->card.cardColour == 0) {
		list->card = *card;
		return 0;
	}
	while (list->next != NULL) {
		list = list->next;
	}
	linkedListNode_t * node = malloc(sizeof(linkedListNode_t));
	node->card = *card;
	node->prev = list;
	list->next = node;
	return rc;
}

int main(int argc, char * argv[]) {
	int rc = 0;
	initCardDeck();
	if (argc == 1) {
		s_pickColour = 0;
		s_linkedListSize = 0;
	} else if (argc == 2) {
		s_colour = *argv[1] - '0';
		s_pickColour = 1;
		s_linkedListSize = 0;
	} else {
		s_linkedListSize = (int) atof(argv[1]);
		s_pickColour = 0;
	}

	time_t t;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	int randNumber = (rand() % 52);
	if (0 == s_linkedListSize) {
		for (int i = 0; i < 10; i++) {
			card_t card = { };
			if (0 != s_pickColour) {
				int randNumber = (rand() % number_size);
				card.cardColour = s_colour;
				card.cardNumber = randNumber;
			} else {
				int randNumber = (rand() % 52);
				card = s_cardDeck[randNumber];
			}
			printf("card is : colour:%d number:%d\n", card.cardColour,
					card.cardNumber);
		}
	} else {
		linkedListNode_t *list = malloc(sizeof(linkedListNode_t));
		list->next = NULL;
		list->prev = NULL;
		for (int i = 0; i < s_linkedListSize; i++) {
			card_t card = { };

			int randNumber = (rand() % 52);
			card = s_cardDeck[randNumber];
			listAppend(list, &card);

		}
		listPrint(list);
	}
	return rc;
}
