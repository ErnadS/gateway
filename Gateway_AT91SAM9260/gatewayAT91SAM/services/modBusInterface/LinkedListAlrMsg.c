/*
 * deviceLinkedList.c
 *
 *  Created on: 22. apr. 2010
 *      Author: Ernad
 */
#include "LinkedListAlrMsg.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct msgList {
	char msg[24];
	struct msgList * next_device;
} msgListElement;

msgListElement* p_alr_startLinkedList = NULL;
int nAlrListSize = 0;
pthread_mutex_t alr_list_mutex;

#define MAX_ALR_LIST_SIZE   5

/////////////////////////////////////////////////////////
// prototypes
/////////////////////////////////////////////////////////
msgListElement* alrLinkedList_createEmptyElement(void);
int alr_LinkedList_removeFirst(void);
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////
int alr_LinkedList_addElement(char* newMsg) {
	if (newMsg[0] == 0)
		return -1;

	int size = alr_LinkedList_getSize();
	if (size == MAX_ALR_LIST_SIZE)
		alr_LinkedList_removeFirst();

	pthread_mutex_lock(&alr_list_mutex); // new 2014.02.23
	msgListElement* p_newListElement = alrLinkedList_createEmptyElement();
	memcpy(p_newListElement, newMsg, 24);

	if (p_alr_startLinkedList != NULL) { // list is not empty
		if (p_alr_startLinkedList->next_device != NULL)
			p_newListElement->next_device = p_alr_startLinkedList->next_device;
		p_alr_startLinkedList->next_device = p_newListElement;
	} else {
		p_alr_startLinkedList = p_newListElement;
	}

	nAlrListSize++;
	pthread_mutex_unlock(&alr_list_mutex); // new 2014.02.23
	return nAlrListSize;
}

int alr_LinkedList_removeElement(char* rmvMsg) {
	pthread_mutex_lock(&alr_list_mutex); // new 2014.02.23
	msgListElement * p_linkedListElement;
	msgListElement * previousElement = NULL;
	p_linkedListElement = p_alr_startLinkedList;

	while (p_linkedListElement != NULL) {
		if (strcmp(p_linkedListElement->msg, rmvMsg) == 0) {
			if (previousElement != NULL)
				previousElement->next_device = p_linkedListElement->next_device;
			else
				p_alr_startLinkedList = p_linkedListElement->next_device;
			free(p_linkedListElement);
			p_linkedListElement = NULL;
			nAlrListSize --;
			pthread_mutex_unlock(&alr_list_mutex); // new 2014.02.23
			return 1;
		}else {
			previousElement = p_linkedListElement;
			p_linkedListElement = p_linkedListElement->next_device;
		}
	}
	pthread_mutex_unlock(&alr_list_mutex); // new 2014.02.23
	return 0;
}

int alr_LinkedList_removeFirst(void) {
	pthread_mutex_lock(&alr_list_mutex); // new 2014.02.23
	msgListElement * p_nextListElement;
	p_nextListElement = p_alr_startLinkedList->next_device;

	if (p_alr_startLinkedList != NULL) {
		free(p_alr_startLinkedList);
		nAlrListSize--;
		p_alr_startLinkedList = p_nextListElement;
		pthread_mutex_unlock(&alr_list_mutex); // new 2014.02.23
		return 0;
	}
	else {
		pthread_mutex_unlock(&alr_list_mutex); // new 2014.02.23
		return -1;
	}
}

void alr_LinkedList_copyAll(char* addressToCopy) {
	int i;
	int pos = 0;

	pthread_mutex_lock(&alr_list_mutex); //
	msgListElement * p_listElement;
	p_listElement = p_alr_startLinkedList;

	for (i = 0; i < MAX_ALR_LIST_SIZE; i++) {
		if (p_listElement == NULL) {
			break;
		}
		memcpy(&addressToCopy[pos], p_listElement->msg, 24);
		pos += 24;
		p_listElement = p_listElement->next_device;
	}
	// Clear unused registers
	for (; i < MAX_ALR_LIST_SIZE; i++) {
		memset(&addressToCopy[pos], 0, 24);
		pos += 24;
	}
	pthread_mutex_unlock(&alr_list_mutex);
}

int alr_LinkedList_getSize(void) {
	return nAlrListSize;
}

/*
int alr_LinkedList_getElementAt(int pos, char* returnMsg) {
	int i;

	pthread_mutex_lock(&alr_list_mutex); //
	msgListElement * p_listElement;
	p_listElement = p_alr_startLinkedList;

	for (i = 0; i < pos; i++) {
		if (p_listElement == NULL) {
			pthread_mutex_unlock(&alr_list_mutex); //
			return -1;
		}
		p_listElement = p_listElement->next_device;
	}
	memcpy(returnMsg, p_listElement->msg, 24);
	pthread_mutex_unlock(&alr_list_mutex); //
	return 0;
}
*/

/////////////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////////////


/**
 * allocate memory for new element
 */
msgListElement * alrLinkedList_createEmptyElement() {
	msgListElement * p_newListElement;
	p_newListElement = (msgListElement *) malloc(sizeof(msgListElement));
	p_newListElement->next_device = NULL;
	return (p_newListElement);
}
