/*
 * deviceLinkedList.c
 *
 *  Created on: 22. apr. 2010
 *      Author: Ernad
 */
#include "deviceLinkedList.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct myList {
	DEV device;
	struct myList * next_device;
} linkedListElement;

linkedListElement* p_startLinkedList = NULL;
int nListSize = 0;
pthread_mutex_t list_mutex;

/////////////////////////////////////////////////////////
// prototypes
/////////////////////////////////////////////////////////
linkedListElement* deviceLinkedList_createEmptyElement();
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
//
// return: list size (OK) or -1 (ERROR)
/////////////////////////////////////////////////////////
int deviceLinkedList_addElement(DEV* newDev) {
	if (newDev == NULL)
		return -1;
	pthread_mutex_lock(&list_mutex); // new 2014.02.23
	linkedListElement* p_newListElement = deviceLinkedList_createEmptyElement();
	memcpy(p_newListElement, newDev, sizeof(DEV));

	if (p_startLinkedList != NULL) { // list is not empty
		if (p_startLinkedList->next_device != NULL)
			p_newListElement->next_device = p_startLinkedList->next_device;
		p_startLinkedList->next_device = p_newListElement;
	} else {
		p_startLinkedList = p_newListElement;
	}

	nListSize++;
	pthread_mutex_unlock(&list_mutex); // new 2014.02.23
	return nListSize;
}

char deviceLinkedList_removeDevice(int devAddr) {
	pthread_mutex_lock(&list_mutex); // new 2014.02.23
	linkedListElement * p_linkedListElement;
	linkedListElement * previousElement = NULL;
	p_linkedListElement = p_startLinkedList;

	while (p_linkedListElement != NULL) {
		if (p_linkedListElement->device.adr == devAddr) {
			if (previousElement != NULL)
				previousElement->next_device = p_linkedListElement->next_device;
			else
				p_startLinkedList = p_linkedListElement->next_device;
			free(p_linkedListElement);
			p_linkedListElement = NULL;
			nListSize --;
			pthread_mutex_unlock(&list_mutex); // new 2014.02.23
			return 1;
		}else {
			previousElement = p_linkedListElement;
			p_linkedListElement = p_linkedListElement->next_device;
		}
	}
	pthread_mutex_unlock(&list_mutex); // new 2014.02.23
	return 0;
}

DEV* deviceLinkedList_getDevice(int devAddr) {
	DEV * dev;
	pthread_mutex_lock(&list_mutex); // new 2014.02.23
	linkedListElement * p_linkedList;
	p_linkedList = p_startLinkedList;

	while (p_linkedList != NULL) {

		if (p_linkedList->device.adr == devAddr) {
			dev = &(p_linkedList->device);
			pthread_mutex_unlock(&list_mutex); // new 2014.02.23
			return dev;
		}
		p_linkedList = p_linkedList->next_device;
	}
	pthread_mutex_unlock(&list_mutex); // new 2014.02.23
	return NULL;
}

int deviceLinkedList_getSize(void) {
	return nListSize;
}

DEV* deviceLinkedList_getElementAt(int pos) {
	int i;
	DEV * dev;
	pthread_mutex_lock(&list_mutex); // new 2014.02.23
	linkedListElement * p_listElement;
	p_listElement = p_startLinkedList;

	for (i = 0; i < pos; i++) {
		if (p_listElement == NULL) {
			pthread_mutex_unlock(&list_mutex); // new 2014.02.23
			return NULL;
		}
		p_listElement = p_listElement->next_device;
	}
	dev = &(p_listElement->device);
	pthread_mutex_unlock(&list_mutex); // new 2014.02.23
	return dev;
}

void deviceLinkedList_setFound(int nAddr, char bFound) {
	DEV * dev;
	int i;
	//pthread_mutex_lock(&list_mutex); // !!! CAN NOT DO THAT. It is using "deviceLinkedList_getElementAt" which also using mutex
	int nDevicesCount = deviceLinkedList_getSize();

	for (i = 0; i < nDevicesCount; i++) {
		dev = deviceLinkedList_getElementAt(i);
		if(dev != NULL && dev->adr == nAddr) {
			dev->found = bFound;
			//pthread_mutex_unlock(&list_mutex); // new 2014.02.23
			return;
		}
	}
	//pthread_mutex_unlock(&list_mutex); // new 2014.02.23
}


/////////////////////////////////////////////////////////
// private functions
/////////////////////////////////////////////////////////


/**
 * reserverer minne for et nytt element i list
 */
linkedListElement * deviceLinkedList_createEmptyElement() {
	linkedListElement * p_newListElement;
	p_newListElement = (linkedListElement *) malloc(sizeof(linkedListElement));
	p_newListElement->next_device = NULL;
	return (p_newListElement);
}
