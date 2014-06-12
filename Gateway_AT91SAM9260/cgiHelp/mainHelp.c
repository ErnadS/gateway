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

	int nConnRes = connectToServer();
	if (nConnRes > 0) {
		char postResult[200];
		postResult[0] = 0;
		//char* IP = getenv("REMOTE_ADDR");
		sendCookie(cookiVal);//IP);
		getParam("formID=help&END=2", postResult, 199);

		if (postResult[0] != 0) {
			printf("Content-type: text/html\n\n"); //send back html (web page)
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
			refuseUser();
			list_clear(&entriesCookie);
			return 0;
		}

		fflush(stdout);
		closeSocket();
	}
	list_clear(&entriesCookie);
	return 0;
}

void refuseUser() {
	printf("Content-type: text/html\n\n"); //send back html (web page)
//	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf(
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n");
	printf("<head>\n");
	printf("<meta http-equiv=\"Refresh\" content=\"1; url=/login.html\">\n");
	printf("</head>\n");
	printf("</html>\n");
	fflush(stdout);
}
