/*
 * mainLeftMenu.c
 *
 *  Created on: 5. mai 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include <string.h>
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"
#include "cgi_common/html_lib.h"
#include "templateGateway.h"

#include "socketClient.h"


int main() {
	llist entriesCookie;

	char postResult[30000];
	postResult[0] = 0;
	postResult[1] = 0;
/*	char* postRequest;
	printf("Content-type: text/html\n\n");
	printf("elloH");
*/

	//postRequest = get_POST();


	parse_cookies(&entriesCookie);
	char* cookiVal = cgi_val(entriesCookie, "cookievalue");
	if(cookiVal == 0) {
		printf("Content-type: text/javascript\n\n");
		printf("window.location = \"/login.html\"");

		list_clear(&entriesCookie);
		return 0;
	}

	int nConnRes = connectToServer();
	if (nConnRes > 0) {
		//char* IP = getenv("REMOTE_ADDR");

		sendCookie(cookiVal);//IP);
		getParam("formID=lm&END=2", postResult, MAX_LINE_LENGTH);

		printf("Content-type: text/javascript\n\n");

		if  (postResult[0] != 0 && postResult[1] != 0) {
			printf(postResult);
		} else {
			printf("window.location = \"/login.html\"");
		}

		fflush(stdout);
		closeSocket();
		//free(IP);
	//	free(postRequest);
	}
	list_clear(&entriesCookie);
	return 0;
}


