/*
 * loginLogic.h
 *
 *  Created on: 5. mai 2010
 *      Author: Ernad
 */

#ifndef LOGINLOGIC_H_
#define LOGINLOGIC_H_

#define LOGIN_LEVEL_ADMIN 		2
#define LOGIN_LEVEL_USER  		1
#define LOGIN_LEVEL_NOT_LOGED   0
#define LOGIN_TO_BUSY			-1

#define MAX_USERS   5

typedef struct {
	int loginLevel;
	unsigned int counter;
	char cookieLogin[25];
} LoginState;


void loginLogic_init(void);
int loginLogic_checkPassword(char* psw, char* cookie);
void loginLogic_generateCookie(char* cookie);

//int loginLogic_updateLoginState(char* psw, char* address);
void loginLogic_updateLoginTimer(char* address);
int loginLogic_getLoginState(char* address);

#endif /* LOGINLOGIC_H_ */
