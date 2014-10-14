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

	char *postResult = NULL; //[30000];
	ssize_t retSize;

	//postResult[0] = 0;
	// postResult[1] = 0;

	parse_cookies(&entriesCookie);
	char* cookiVal = cgi_val(entriesCookie, "cookievalue");
	if(cookiVal == 0) {
		printf("Content-type: text/javascript\n\n");
		printf("window.location = \"/login.html\"");

		list_clear(&entriesCookie);
		return 0;
	}

	int conn_s = connectToServer();
	if (conn_s > 0) {
		sendCookie(conn_s, cookiVal);//IP);

		printf("Content-type: text/javascript\n\n");

		//char* IP = getenv("REMOTE_ADDR");
		postResult = getParam(conn_s, "formID=lm&END=2", &retSize, MAX_LINE_LENGTH);


		if (retSize <= 0) {
			list_clear(&entriesCookie);
			closeSocket(conn_s);
			return 0; //TODO: send some error msg?
		}





		if  (postResult[0] != 0 && postResult[1] != 0) {
			printf(postResult);
		} else {
			printf("window.location = \"/login.html\"");
		}

		fflush(stdout);
		closeSocket(conn_s);
		free(postResult);
	}
	list_clear(&entriesCookie);
	return 0;
}


