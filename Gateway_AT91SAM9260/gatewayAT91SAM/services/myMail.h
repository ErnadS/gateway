/*
 * myMail.h
 *
 *  Created on: 2. des. 2010
 *      Author: Ernad
 */

#ifndef MYMAIL_H_
#define MYMAIL_H_

#include "../unitGW/model/gwEmail.h"

enum SMTP_RETURN {
	SMTP_OK,
	MAIL_DISABLED,
	SMTP_SOCK_ERR,
	SMTP_UNEXPECTED,
	SMTP_CONNECT_ERR,
	SMTP_UNKNOWN_HOST,
	SMTP_AUTH_ERR,
	SMTP_SEND_ERR,
	SMTP_FILE_ERR
};

void * runMailDispatcher(void *ptr);
void myMail_init(GW_EMAIL* gwEmail);
int myMail_appendMail(const char* subject, const char* filePath, char *errMsg, int nMaxErrMsg);
GW_EMAIL gwEmailStruct;

#endif /* MYMAIL_H_ */
