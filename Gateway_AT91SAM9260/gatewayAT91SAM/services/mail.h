/*
 * mail.h
 *
 *  Created on: 8. apr. 2010
 *      Author: Ernad
 */

#ifndef MAIL_H_
#define MAIL_H_

char email_sendToAll(char* subject, char * filePath, char* errCode, int nMaxErrMsg);
char email_it(char* desination, char* subject, char * filename);

#endif /* MAIL_H_ */
