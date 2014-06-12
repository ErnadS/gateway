/*
 * xml.c
 *
 *  Created on: 27. jan. 2010
 *      Author: Ernad
 */

#include <string.h>
#include "xml.h"
/**
 * add XML tag with paramName and paramValue to buffer.
 * Return is new position in buffer.
 */
int xml_addTag(char* buffer, char* paramName, char* paramValue) {
	int buffPos = 0;

	strncpy(&buffer[buffPos], "<", 1);
	buffPos += 1;
	strncpy(&buffer[buffPos], paramName, strlen(paramName));
	buffPos += strlen(paramName);
	strncpy(&buffer[buffPos], ">", 1);
	buffPos += 1;

	strncpy(&buffer[buffPos], paramValue, strlen(paramValue));
	buffPos += strlen(paramValue);

	strncpy(&buffer[buffPos], "</", 2);
	buffPos += 2;
	strncpy(&buffer[buffPos], paramName, strlen(paramName));
	buffPos += strlen(paramName);
	strncpy(&buffer[buffPos], ">", 1);
	buffPos += 1;

	return buffPos;
}

