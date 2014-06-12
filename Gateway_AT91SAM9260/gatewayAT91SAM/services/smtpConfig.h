/*
 * mailConfig.h
 *
 *  Created on: 8. apr. 2010
 *      Author: Ernad
 */

#ifndef SMTPCONFIG_H_
#define SMTPCONFIG_H_

typedef struct {
	char smtpAddress[40];
	int port;
	char sender[40];
	char user[40];
	char pass[40];
} SSMTP;

char smtpConfig_save(SSMTP ssmtp);
char smtpConfig_read(SSMTP* ssmtp);
void smtpConfig_clearStruct(SSMTP* pssmtpStruct);

#endif /* SMTPCONFIG_H_ */
