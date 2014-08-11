/*
 * main.c
 *
 *  Created on: 30. jan. 2010
 *      Author: Ernad
 */
#include <stdio.h>
#include <string.h>

#include "cgi_common/cgi_lib.h"
#include "socketClient.h"

char* postRequest;


void send_error(void) {
	printf("Content-type: text/xml\n\n");
	printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
							printf("<warn>Error</warn>");
							printf("</response>");
				fflush(stdout);
}

void send_error_response(void) {
	printf("Content-type: text/xml\n\n");
	printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
							printf("<warn>Wrong response from server</warn>");
							printf("</response>");
				fflush(stdout);
}

int main() {
	int status = 0;
	llist entries;

	status = read_cgi_input(&entries);

	char* temp;
	char* postResult = 0; //[50];
	ssize_t retSize;

	char postRequest[200];

	int conn_s;

	if (status > 0) {
		conn_s = connectToServer();

		if (conn_s < 1) {
			printf("Content-type: text/xml\n\n");
			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
						printf("<warn>Error connecting to server</warn>");
						printf("</response>");
						/*
			printf("<html> <head>\r\n");
			printf("<title>HTML Page</title>\r\n");
			printf("</head>\r\n<body>\r\n");
			printf("<h1>HTML Page</h1>\r\n");
			printf("<p>ERROR connecting to server: %d\r\n", nConnRes);
			printf("</body></html>\r\n");*/
			fflush(stdout);
			return 0;
		}

		temp = cgi_val(entries, "psw");

		sendCookie(conn_s, "0"); // Usually the first parameter is cookie. We don't have and send "0" to indicate loging

		sprintf(postRequest, "formID=cgiLog_in&END=2&psw=%s", temp);
		postResult = getParam(conn_s, postRequest, &retSize, 50);

		if (retSize <= 0 || postResult == 0)  {
			send_error_response();
			fflush(stdout);
			list_clear(&entries);
			closeSocket(conn_s);
			return 0;
		}

		if (strlen(postResult) > 1) {
			sprintf(postRequest, "Set-Cookie: cookievalue=%s\n", postResult);
			printf(postRequest);

			printf("Content-type: text/xml\n\n");

			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
			printf("<warn>OK</warn>");
			printf("</response>");

		} else if (postResult[0] == '1') {
			printf("Content-type: text/xml\n\n");
			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
			printf("<warn>To many users at the time. Please, try later.</warn>");
			printf("</response>");
		} else if (postResult[0] == '2') {
			printf("Content-type: text/xml\n\n");
			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
			printf("<warn>Error 76.</warn>");
			printf("</response>");
		} else {
			printf("Content-type: text/xml\n\n");
			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
			printf("<warn>Wrong password %s.</warn>", postResult);
			printf("</response>");
		}

		free(postResult);

		closeSocket(conn_s);
		list_clear(&entries);
	}
	return 1;
}

