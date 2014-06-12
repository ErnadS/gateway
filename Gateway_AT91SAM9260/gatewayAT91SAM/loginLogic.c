/*
 * loginLogic.c
 *
 *  Created on: 5. mai 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include "loginLogic.h"
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "unitGW/utility/gwPassUtility.h"

#include <stdlib.h>
#include <time.h>
#include "config.h"


LoginState loginState[MAX_USERS];

char bRun;

void * loginLogic_run(void *ptr); // thread


void loginLogic_init(void) {
	int ret;
	int i;
	pthread_t thread1;
	bRun = 1;

	for (i = 0; i < MAX_USERS; i++) {
		loginState[i].cookieLogin[0] = 0; //null
		loginState[i].counter = 0;
	}
	ret = pthread_create(&thread1, NULL, loginLogic_run, (void*) NULL);
	srand ( time(NULL) );
}

void * loginLogic_run(void *ptr) {
	int i;

	while (bRun == 1) {
		sleep(1);
		for (i = 0; i < MAX_USERS; i++) {
			if (loginState[i].loginLevel != LOGIN_LEVEL_NOT_LOGED)
				loginState[i].counter++;
			if (loginState[i].counter > 600) { // 10 min
				loginState[i].loginLevel = LOGIN_LEVEL_NOT_LOGED;
				loginState[i].counter = 0;
				printf("TIMEOUT, user inactive in 10 min. Logout\n");
			}
		}
	}
	return NULL;
}

void loginLogic_generateCookie(char* cookie) {
	int i;
	int j;

	int random;
	char temp;
	for (j = 0; j < 5; j++) {
		random = rand();
		for (i = 0; i < 4; i++) {
			temp = random % 78 + 0x2D;

			while(!((temp >= 'A' && temp <= 'Z') || (temp >= 'a' && temp <= 'z') ||
				(temp >= '0' && temp <= '9') || temp == '-')) {
				temp += 3;
			}

			cookie[i + j*4] = temp;

			random = random / 78;
		}
		usleep(1000);
	}
	cookie[20] = 0;
	printf("generated cookie: %s\n", cookie);
}

int loginLogic_checkPassword(char* psw, char* cookie) {
	GW_PASS gwPass;
	int i;
	gwPassUtility_readStruct(&gwPass);

	// CHECK IF WE HAVE PLACE FOR NEW USER
	for (i = 0; i < MAX_USERS; i++) {
		if (loginState[i].loginLevel == LOGIN_LEVEL_NOT_LOGED) {
			break;
		}
	}

	if (i < MAX_USERS) {
		if (strcmp(psw, gwPass.apass1) == 0) {
			loginLogic_generateCookie(cookie);
			loginState[i].loginLevel = LOGIN_LEVEL_ADMIN;
		} else if (strcmp(psw, gwPass.lpass1) == 0) {
			loginLogic_generateCookie(cookie);
			loginState[i].loginLevel = LOGIN_LEVEL_USER;
		} else {
			return LOGIN_LEVEL_NOT_LOGED;
		}
		strcpy(loginState[i].cookieLogin, cookie);
		loginState[i].counter = 0;
		return loginState[i].loginLevel;
	} else {
		return LOGIN_TO_BUSY;
	}
}
/*
int loginLogic_updateLoginState(char* psw, char* userCookie) {
	GW_PASS gwPass;
	int i;
	gwPassUtility_readStruct(&gwPass);

	for (i = 0; i < MAX_USERS; i++) {
		if (strcmp(loginState[i].cookieLogin, userCookie) == 0) {
			break;
		}
	}

	if (i == MAX_USERS) { //not found this IP, try to found empty struct
		for (i = 0; i < MAX_USERS; i++) {
			if (loginState[i].loginLevel == LOGIN_LEVEL_NOT_LOGED) {
				break;
			}
		}
	}

	if (i < MAX_USERS) {
		if (strcmp(psw, gwPass.apass1) == 0)
			loginState[i].loginLevel = LOGIN_LEVEL_ADMIN;
		else if (strcmp(psw, gwPass.lpass1) == 0) {
			loginState[i].loginLevel = LOGIN_LEVEL_USER;
		} else {
			return LOGIN_LEVEL_NOT_LOGED;
		}
		strcpy(loginState[i].cookieLogin, userCookie);
		loginState[i].counter = 0;
		return loginState[i].loginLevel;
	} else {
		return LOGIN_TO_BUSY;
	}

	return LOGIN_LEVEL_NOT_LOGED;
}*/

void loginLogic_updateLoginTimer(char* userCookie) {
	int i;

	for (i = 0; i < MAX_USERS; i++) {
		if (strcmp(loginState[i].cookieLogin, userCookie) == 0) {
			loginState[i].counter = 0;
			return;
		}
	}
}

int loginLogic_getLoginState(char* userCookie) {
	int i;

	for (i = 0; i < MAX_USERS; i++) {
		if (strcmp(loginState[i].cookieLogin, userCookie) == 0) {
#ifdef DEBUG_CLIENT
			printf("User is logged in, cookie: %s, level: %d\n", loginState[i].cookieLogin,
					loginState[i].loginLevel);
#endif
			return loginState[i].loginLevel;
		}
	}

	printf("User not logged in, cookie %s refused\n", userCookie);
	return LOGIN_LEVEL_NOT_LOGED;
}
