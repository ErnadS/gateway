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
//#include "trafoGuardPage.h"
#include <string.h>

//#include <wchar.h>

int main() {
	llist entriesCookie;

	// dynamic allocated. Must be "free" later.
	char *postResult= NULL; //[150000];
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

	// Take a cookie from request. The same cookie must be sent to gateway before reading/writing parameters
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



	ssize_t retSize;

	int conn_s = connectToServer();
	if (conn_s > 0) {
		// We must first send a cookie to gateway (or connection will be refused)
		sendCookie(conn_s, cookiVal);
		// !!!!!!!
		// ES: best to make short pause after sending cookie to gateway (for at melding ikke blir limet sammen med parameter som sendes deretter)
		printf("Content-type: text/xml\n\n"); // use this if sending XML

		// and send parameters to gateway
		postResult = getParam(conn_s, postRequest, &retSize, MAX_LINE_LENGTH);

		if (retSize <= 0 || postResult == NULL) {
			if (postRequest != NULL)
				free(postRequest);

			list_clear(&entriesCookie);
			closeSocket(conn_s);
			printf("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?> <response>2error</response>");
			return 0; //TODO: send some error msg?
		}

		printf("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?> <response>");


		// getParam(postRequest, postResult, MAX_LINE_LENGTH);


		printf(postResult);
		printf("</response>");
		closeSocket(conn_s);

		fflush(stdout);
		if (postResult != NULL)
			free(postResult);
	}
	if (postRequest != NULL)
		free(postRequest);
	list_clear(&entriesCookie);
	return 0;
}

