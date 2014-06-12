/* cgi.c */
#include <stdio.h>
#include <string.h>
//#include "cgivars.h"
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"
#include "template.h"

#include "cgi_common/html_lib.h" 
// TODO: det finnes lignende "htmllib.h". Hvilken bruke?
#include "templateGateway.h"

#include "socketClient.h"
#include "trafoGuardPage.h"
#include <string.h>

//#include <wchar.h>

int main() {
	llist entriesCookie;
	/*int status;


	 char currDate[80];
	 char currTime[80];

	 char **postvars = NULL; // POST request data repository
	 char **getvars = NULL; // GET request data repository */
	char postResult[150000];


	char* postRequest;

	//  ********************
	// REDIRECTION TEST
	/*
	 printf("Location: http://www.secic.no\n\n");
	 return 1;
	 */

	postRequest = get_POST();

	if (postRequest == NULL) {
		return 0;
	}
	// !!! IKKE TA VEKK
	//cookieRequest = getenv("HTTP_COOKIE");
	// henter alle "Cookie". De from javascript og de fra html som f.eks.
	//<META HTTP-EQUIV="Set-Cookie" CONTENT="myCookie=myValue;expires=Friday, 31-Dec-10 23:59:59 GMT; path=/">
	// Returnerte "cookie" er i format: myCookie=myValue;myCookie2=myValue";...
	//////////////////////////////////////////////////////////////////////////////////////
	//

	// fungerer:
	parse_cookies(&entriesCookie);
	char* cookiVal = cgi_val(entriesCookie, "cookievalue");
	/*if (cookiVal == 0 || strcmp(cookiVal, "xxx1") != 0) {
	 int nConnRes = connectToServer();
	 // don't send response. User is not authorized.
	 closeSocket();
	 return 1;
	 }*/

	//int form_method; /* POST = 1, GET = 0 */
	//form_method = GET;

	//status = read_cgi_input(&entries);

	// !!!!printf("Content-type: text/html\n\n");
	//	   printf("Content-type: text/plain\n\n");

	printf("Content-type: text/xml\n\n"); // use this if sending XML

	int nConnRes = connectToServer();
	if (nConnRes > 0) {
		printf("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?> <response>");
				//"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> <response>");
		/*char chIP[50];
		 char* IP = getenv("REMOTE_ADDR");
		 strcpy(chIP,IP);*/
		sendCookie(cookiVal);//chIP);//IP);
		getParam(postRequest, postResult, MAX_LINE_LENGTH);
/*
 * char temPostResult[1301];
		char* tempBuff = postResult;
		int len = strlen(tempBuff);

		while (len > 1300) {
			memcpy(temPostResult, tempBuff, 1300);
			temPostResult[1300] = 0;
			printf(postResult);
			tempBuff += 1300;
			len -= 1300;
		}
		if (tempBuff[0] != 0)
			printf(tempBuff);*/

		printf(postResult);
		printf("</response>");
		closeSocket();
		//list_clear(&entries);

		fflush(stdout);

	}
	if (postRequest != NULL)
		free(postRequest);
	list_clear(&entriesCookie);
	return 0;
}
/*
 void getEntries(llist entries)
 {

 node* nextNode = entries.head;

 while (nextNode != NULL) {
 if (nextNode->entry.name == NULL || nextNode->entry.value == NULL)
 return;
 getParam(nextNode->entry.name, nextNode->entry.value, 80);
 printf("<");
 printf(nextNode->entry.name);
 printf(">");
 printf(nextNode->entry.value);
 printf("</");
 printf(nextNode->entry.name);
 printf(">");

 nextNode = nextNode->next;
 }
 }

 void getTime(void)
 {
 getParams(entries.head->entry.name, currDate, currTime);
 printf("<time>");
 printf(currTime);
 printf("</time>");
 printf("<date>");
 printf(currDate);
 printf("</date>");
 }

 void tgGetUnitLocation(void)
 {
 char unitLoc[80];
 getParam("tgUnitLoc", unitLoc);

 printf("<tgUnitLoc>");
 printf(unitLoc);
 printf("</tgUnitLoc>");
 }

 void tgGetCanBusAddress(void)
 {

 }*/

