/*
 * mail.c
 *
 *  Created on: 8. apr. 2010
 *      Author: Ernad
 */

#include "mail.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../unitGW/utility/gwEmailParser.h"
#include "../can/ObjDict.h"
#include "myMail.h"
#include "../utility/myString.h"

#include "../unitGW/utility/gwParConfig/gwParamSaver.h"

char email_sendToAll(char* subject, char * filePath, char* errCode, int nMaxErrMsg) {
	GW_EMAIL peMailStruct;
	gwEmail_readStruct(&peMailStruct);
	myMail_init(&peMailStruct);
	return myMail_appendMail(subject, filePath, errCode, nMaxErrMsg);
	/*
	GW_EMAIL peMailStruct;

	gwEmail_readStruct(&peMailStruct);

	if (peMailStruct.enabled[0] == '0') {
		printf("E-mail disabled\n");
		return 2; // cannot send
	} else {
		printf("ok, E-mail is enabled\n");
	}

	char dest[300];
	sprintf(dest, "%s %s %s %s %s", peMailStruct.recip1, peMailStruct.recip2, peMailStruct.recip3, peMailStruct.recip4,
			peMailStruct.recip5);
    return email_it(dest, subject, filePath); */
}
/*******************************
 * email_it() -
 ********************************/

char email_it(char* destination, char* subject, char * filename) {
	char mailBuffer[512] = { 0x0 };
	int nResult;

	GW_TIME* gwTimeData =  getTimeStruct();

	printf("Sending e-mail to: %s\n", destination);



	sprintf(mailBuffer, "/usr/bin/mail -s '%s, %s' %s < %s", subject, gwTimeData->name_UTF8_format/* ObjDict_obj2002*/, destination,
			filename);

	nResult = system(mailBuffer);
	if (nResult == (-1)) {
		printf("email failure\n");
		return 1;
	}
	printf("E-mail sent, result: %d\n", nResult);
	return 0; //success
}
