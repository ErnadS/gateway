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

int main() {
	int status = 0;
	llist entries;

	status = read_cgi_input(&entries);

	char* temp;
	char postResult[50];
	char postRequest[200];

	int nConnRes;

	if (status > 0) {
		nConnRes = connectToServer();

		if (nConnRes != 1) {
			printf("Content-type: text/xml\n\n");
			printf("<html> <head>\r\n");
			printf("<title>HTML Page</title>\r\n");
			printf("</head>\r\n<body>\r\n");
			printf("<h1>HTML Page</h1>\r\n");
			printf("<p>ERROR connecting to server: %d\r\n", nConnRes);
			printf("</body></html>\r\n");
			return 0;
		}

		temp = cgi_val(entries, "psw");

		sendCookie("0"); // usualy, first param is cookie. We don't have and send "0" to indicate loging

		sprintf(postRequest, "formID=cgiLog_in&END=2&psw=%s", temp);
		getParam(postRequest, postResult, 50);

		if (strlen(postResult) > 1) { // "<LOG>Please, try later. To many users at the same time.</LOG>"
			sprintf(postRequest, "Set-Cookie: cookievalue=%s\n", postResult);
			printf(postRequest);

			printf("Content-type: text/xml\n\n");
			//printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
						printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
						printf("<warn>OK</warn>");
						printf("</response>");
			/*printf("Content-type: text/html\n\n");
			printf(
					"<meta http-equiv=\"Refresh\" content=\"1; url=/index.htm\n\n\">");*/
		} else if (postResult[0] == '1') {
			printf("Content-type: text/xml\n\n");
			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
			printf("<warn>To many users at the time. Please, try later.</warn>");
			printf("</response>");
		} else {
			printf("Content-type: text/xml\n\n");
			printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\" standalone=\"yes\"?> <response>");
			printf("<warn>Wrong password</warn>");
			printf("</response>");
		}

		//printf("SetCookie(\"lastvisited\", %ld, largeExpDate);\n",(unsigned long)tm);
		//printf("Set-Cookie: CONTENT=\"cookievalue=xxx;expires=Friday, 31-Dec-10 23:59:59 GMT; path=/\">\n");
		/*
		 printf("Content-type: text/html");
		 printf("<META HTTP-EQUIV=\"Set-Cookie\" CONTENT=\"cookievalue=xxx;expires=Friday, 31-Dec-10 23:59:59 GMT; path=/\">\n");
		 printf("<meta http-equiv=\"Refresh\" content=\"1; url=http://192.168.121.3/tg_setup.htm\n\n\">"); */
		//printf("Set-Cookie: CONTENT=\"cookievalue=xxx1;expires=Friday, 31-Dec-10 23:59:59 GMT; path=/\">\n");


		/*
		 if (strcmp(temp, pass) == 0) {
		 char chIP[50];
		 char* IP = getenv("REMOTE_ADDR");
		 strcpy(chIP,IP);
		 sendIpAddress(chIP);//IP);
		 getParam("comm=w&formID=log&canID=122211&loginID=22222&END=2&psw=Andy", postResult, MAX_LINE_LENGTH);

		 printf("Set-Cookie: cookievalue=xxx1\n");
		 printf("Content-type: text/html\n\n");

		 printf("<meta http-equiv=\"Refresh\" content=\"1; url=/index.htm\n\n\">");

		 }
		 else {
		 sendIpAddress(temp);
		 printf("Content-type: text/html\n\n");
		 printf("<meta http-equiv=\"Refresh\" content=\"1; url=../login.html\n\n\">");
		 }*/
		closeSocket();
		list_clear(&entries);
	}
	return 1;
}

