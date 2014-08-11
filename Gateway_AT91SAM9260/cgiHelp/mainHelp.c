/*
 * mainHelp.c
 *
 *  Created on: 26. mai 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include <string.h>
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"
#include "cgi_common/html_lib.h"
#include "templateGateway.h"

#include "socketClient.h"

void refuseUser();

int main() {
	llist entriesCookie;
	parse_cookies(&entriesCookie);
	char* cookiVal = cgi_val(entriesCookie, "cookievalue");

	if (cookiVal == 0) { // MISING COOKIE, NOT LOGED IN
		refuseUser();
		list_clear(&entriesCookie);
		return 0;
	}

	ssize_t retSize;
	int conn_s = connectToServer();

	if (conn_s > 0) {
		char *postResult = NULL; //[200];
		//postResult[0] = 0;

		// We must first send a cookie to gateway (or connection will be refused)
		sendCookie(conn_s, cookiVal);//IP);
		// ES: best to make short pause after sending cookie to gateway (for at melding ikke blir limet sammen med parameter som sendes deretter)
		// Derfor sender vi her content type
		printf("Content-type: text/html\n\n"); //send back html (web page)
		postResult = getParam(conn_s, "formID=help&END=2", &retSize, 199);


		if (retSize <= 0) {
			list_clear(&entriesCookie);
			closeSocket(conn_s);
			return 0; //TODO: send some error msg?
		}

		if (postResult[0] != 0) {
			// printf("Content-type: text/html\n\n"); //send back html (web page)
			printf(
					"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");

			printf("<head>\n");
			printf("<title>ImedSecure webserver</title>\n");
			printf(
					"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n");
			printf("<meta http-equiv=\"Pragma\" content=\"no-cache\" />\n");
			printf(
					"<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"/favicon.ico\">\n");
			printf(
					"<link rel=\"stylesheet\" type=\"text/css\" href=\"/imed.css\"/>\n");

			////////////////////////////////////////////////////////////////////////////////////

			printf(
					"<script type=\"text/javascript\" src=\"/cooltreepro_uncompressed.js\"></script>\n");
			printf(
					"<script type=\"text/javascript\" src=\"/tree_format.js\"></script>\n");
			printf(
					"<script type=\"text/javascript\" src=\"/tree_nodes.js\"></script>\n");

			printf(
					"<script type=\"text/javascript\" src=\"/cgi-bin/cgiLeftMenu\"></script>\n");

			printf(
					"<meta http-equiv=\"Page-Enter\" content=\"blendTrans(Duration=2)\"/>\n");
			printf(
					"<meta http-equiv=\"Site-Exit\" content=\"blendTrans(Duration=5)\"/>\n");

			printf("</head>\n");
			printf("<body>\n");

			printf("<div id=\"wrapper\">\n");
			printf("<div id=\"header\">\n");
			printf(
					"<script type=\"text/javascript\" src=\"/header.js\"></script>\n");
			printf("</div>\n");
			printf("<div id=\"content\">\n");

			printf("<table id=\"mytable\" cellspacing=\"0\">\n");
			printf("<caption>Help</caption>\n");
			printf("<tr> <td class=\"my\"></td>\n");
			printf("<td class=\"my\"><br/></td>  </tr>\n");
			printf("<tr> <td class=\"my\">Get product documentation <a href=");
			//\"http://www.noratel.no/content/view/full/55\" target=\"_blank\">here</a><br/>\n");
			//printf(
			//	"Get support by e-mail <a href=\"mailto:xxxx@xxx.xx\" "
			printf(postResult);
			printf(">here</a><br/>\n");
			printf("</td> <td class=\"my\"></td> </tr> </table>\n");

			printf("</div>\n");

			printf("<div id=\"menu\">\n");
			printf("<script type=\"text/javascript\">\n");
			printf(
					"var tree1 = new COOLjsTreePRO(\"tree1\", TREE_NODES, TREE_FORMAT);\n");
			printf("tree1.init();\n");
			printf("</script>\n");
			printf("</div>\n");

			printf("<div id=\"footer\">\n");
			printf(
					"<script type=\"text/javascript\" src=\"/footer.js\"></script>\n");
			printf("</div>\n");
			printf("</div>\n");

			printf("<script type=\"text/javascript\">\n");

			printf("RedrawAllTrees()\n");
			printf("</script>\n");

			printf("</body>\n");
			printf("</html>\n");

		} else { // LOGIN_LEVEL_NOT_LOGED
			free(postResult);
			refuseUser();
			list_clear(&entriesCookie);
			return 0;
		}

		fflush(stdout);
		closeSocket(conn_s);
		free(postResult);
	}
	list_clear(&entriesCookie);
	return 0;
}

void refuseUser() {
	//printf("Content-type: text/html\n\n"); //send back html (web page)
	printf(
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n");
	printf("<head>\n");
	printf("<meta http-equiv=\"Refresh\" content=\"1; url=/login.html\">\n");
	printf("</head>\n");
	printf("</html>\n");
	fflush(stdout);
}
