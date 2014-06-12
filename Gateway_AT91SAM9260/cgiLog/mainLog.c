/*
 * mainLog.c
 *
 *  Created on: 23. mai 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include <string.h>
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"
#include "cgi_common/html_lib.h"
#include "templateGateway.h"

#include "socketClient.h"
#define MAX_EXCEL_SIZE  160000  // 90000

void refuseUser();
void printFirstPart();
void printSecondPart();

void printFirstPart() {
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
			//printf(
				//	"<script type=\"text/javascript\" src=\"/tree_nodes.js\"></script>\n");

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
}

void printSecondPart() {
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
}




int main() {
	//char* temp;
	char* adr;
	char* msid; // message id to log
	int nMsgId;
	int nAdr;

	int status;

	llist entries;
	llist entriesCookie;

	status = read_cgi_input(&entries);

	parse_cookies(&entriesCookie);
	char* cookiVal = cgi_val(entriesCookie, "cookievalue");
	if (cookiVal == 0) {
		refuseUser();

		list_clear(&entries);
		list_clear(&entriesCookie);
		return 0;
	}

	if (status > 0) {
		adr = cgi_val(entries, "adr");
		msid = cgi_val(entries, "msid");
		nMsgId = atoi(msid);
		nAdr = atoi(adr);

		if (nAdr > 0 && nMsgId >= 0) {
			int nConnRes = connectToServer();
			if (nConnRes > 0) {
				char postResult[MAX_EXCEL_SIZE];
				postResult[0] = 0;
				//char* IP = getenv("REMOTE_ADDR");
				sendCookie(cookiVal);//IP);
				char par[30];
				sprintf(par, "formID=loging&canID=%d&END=2&msid=%d", nAdr,
						nMsgId);
				getParam(par, postResult, MAX_EXCEL_SIZE - 1);

				if (postResult[0] == 0) {
					refuseUser();

					list_clear(&entries);
					list_clear(&entriesCookie);
					return 0;
				}
				printFirstPart();

			    printf(postResult);

				printSecondPart();

				fflush(stdout);
			}
		} else {
			//printf("Content-type: text/html\n\n");
			printf("Content-type: application/vnd.ms-excel\n\n");
			printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
			printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
			printf("<head>\n");
			printf("</head>\n");
			printf("<body>\n");
			printf(
					"<TABLE><TR><TD><b>adr: %s, msgid: %s</b></TD></TR></TABLE>",
					adr, msid);
			printf("</body>\n");
			printf("</html>\n");
			fflush(stdout);
		}
		list_clear(&entries);
		list_clear(&entriesCookie);
	}

	return 0;
}

void refuseUser() {
	printf("Content-type: text/html\n\n"); //send back html (web page)
	//printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf(
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	printf("<head>\n");
	printf("<meta http-equiv=\"Refresh\" content=\"1; url=/login.html\">\n");
	printf("</head>\n");
	printf("</html>\n");
	fflush(stdout);
}
