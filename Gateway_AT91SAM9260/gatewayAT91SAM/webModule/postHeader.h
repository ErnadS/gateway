/*
 * postHeader.h
 *
 *  Created on: 29. jan. 2010
 *      Author: Ernad
 */

#ifndef POSTHEADER_H_
#define POSTHEADER_H_

/////////////////////////////////////////////
// HEADER STRUCT
// Header consist of command, form name,
// canID and login ID
/////////////////////////////////////////////
typedef struct {
	char formID[7];
	char command;
	int canID;
	char loginID[64];
} postHeader;

/*
typedef enum {
	TG_M = 0,
	TG_U,
	TG_S,
	GW_DEV_PAR,
	GW_EMAIL_FORM
}FORM_TYPE;
*/
int postHeader_getData(char* bufferRec, postHeader* postHeaderData);

#endif /* POSTHEADER_H_ */
