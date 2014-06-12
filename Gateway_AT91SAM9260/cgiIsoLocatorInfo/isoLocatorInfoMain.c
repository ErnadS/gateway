/*
 * isoLocatorMain.c
 *
 *  Created on: 3. mai 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include <string.h>

#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"


#include "cgi_common/html_lib.h"
// TODO: det finnes lignende "htmllib.h". Hvilken bruke?
#include "templateGateway.h"
//#include "cgi_lib.h"

#include "socketClient.h"

//void getEntries(llist entries);

int main() {
	char* temp;

	temp = get_GET();
	if (temp == NULL) {
		return 0;
	}

	printf("Content-type: text/html\n\n"); //send back html (web page)
	//printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf(
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");

	printf("<head>\n");
	printf("<title>ImedSecure webserver</title>\n");
	printf(
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	printf("<meta http-equiv=\"Pragma\" content=\"no-cache\" />\n");
	printf(
			"<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"/favicon.ico\">\n");
	printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"/imed.css\"/>\n");

	////////////////////////////////////////////
	printf("<script type=\"text/javascript\">\n");

	printf("window.onload = loadIL;\n");

	printf("function loadIL() {\n");
	printf("updateIL(\"r\"); }\n");

	printf("function updateIL(read_write) {\n");
	// Get CAN bus address and as parameter to javascript
	char tmp[60];
	int nAdr = atoi(temp);
	sprintf(tmp,
			"setParamsIL_info('/cgi-bin/cgiCommonData', read_write, %d); }\n",
			nAdr);
	printf(tmp);

	printf("</script>\n");
	////////////////////////////////////////////////////////////////////////////////////

	printf(
			"<script type=\"text/javascript\" src=\"/cooltreepro_uncompressed.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_format.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_nodes.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_format_setup.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_il_setupInfo.js\"></script>\n");

	printf(
				"<script type=\"text/javascript\" src=\"/cgi-bin/cgiLeftMenu\"></script>\n");

	printf("<script type=\"text/javascript\" src=\"/popUps.js\"></script>\n");

	printf(
			"<meta http-equiv=\"Page-Enter\" content=\"blendTrans(Duration=2)\"/>\n");
	printf(
			"<meta http-equiv=\"Site-Exit\" content=\"blendTrans(Duration=5)\"/>\n");

	printf("</head>\n");
	printf("<body>\n");

	printf("<div id=\"wrapper\">\n");
	printf("<div id=\"header\">\n");
	printf("<script type=\"text/javascript\" src=\"/header.js\"></script>\n");
	printf("</div>\n");
	printf("<div id=\"content\">\n");

	printf("<table id=\"mytable\" cellspacing=\"0\">\n");
	printf(
			"<caption><img src=\"/images/page_white_wrench.png\" width=\"16\" height=\"16\" alt=\"\" align=\"top\" />    IsoLocator info      </caption>\n");
	printf("</table>\n");
	printf("<form name=\"il_form\">\n");
	printf("<script>\n");
	printf(
			"var tree2 = new COOLjsTreePRO(\"tree2\", TREE_IL_SETUP_DISAB, TREE_FORMAT_SETUP);\n");
	printf("tree2.init();\n");
	printf("</script>\n");
	printf("<div id=\"contentfooter\" style=\"padding-left:30px;\">\n");

	printf("</div>\n");
	printf("</form>\n");

	printf("</div>\n");

	printf("<div id=\"menu\">\n");
	printf("<script type=\"text/javascript\">\n");
	printf(
			"var tree1 = new COOLjsTreePRO(\"tree1\", TREE_NODES, TREE_FORMAT);\n");
	printf("tree1.init();\n");
	printf("</script>\n");
	printf("</div>\n");

	printf("<div id=\"footer\">\n");
	printf("<script type=\"text/javascript\" src=\"/footer.js\"></script>\n");
	printf("</div>\n");

	printf("<script type=\"text/javascript\">\n");

	printf("RedrawAllTrees()\n");
	printf("</script>\n");

	printf("</body>\n");
	printf("</html>\n");

	//list_clear(&entries);

	fflush(stdout);

	free(temp);

	return 0;
}
