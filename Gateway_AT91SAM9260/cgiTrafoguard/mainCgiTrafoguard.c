/*
 * mainCgiMenu.c
 *
 *  Created on: 3. mai 2010
 *      Author: Ernad
 */
/* cgi.c */
#include <stdio.h>
#include <string.h>
//#include "cgivars.h"
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"
//#include "template.h"

#include "cgi_common/html_lib.h"
#include "templateGateway.h"

#include "socketClient.h"

int main() {
	char* temp;

	temp = get_GET(); // get CAN BUS address (e.q. http://xxx.xxx.xxx.xxx/cgiTrafoGuard?11 CAN address is after "?"
	if (temp == NULL) {
		return 0;
	}
	//printf("Content-type: text/javascript\n\n");  // example: cgiLeftMenu
	printf("Content-type: text/html\n\n"); //send back html

	//printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf(
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	//missing DOCTYPE if using following:
	//printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\"> <html xml:lang=\"en\" lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n");

	printf("<head>\n");

	printf("<title>ImedSecure webserver</title>\n");
	printf(
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	printf("<meta http-equiv=\"Pragma\" content=\"no-cache\" />\n\n");

	printf(
			"<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"/favicon.ico\" />\n");
	printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"/imed.css\"/>\n");

	////////////////////////////////////////////
	printf("<script type=\"text/javascript\">\n");

	printf("window.onload = loadTG;\n");

	printf("function loadTG() {\n");
	printf("updateTG(\"r\"); }\n");

	printf("function updateTG(read_write) {\n");
	printf("document.tg_form.btn.disabled = true\n");
	// Get CAN bus address as parameter to javascript
	char tmp[60];
	int nAdr = atoi(temp);
	sprintf(tmp, "setParams('/cgi-bin/cgiCommonData', read_write, %d); }\n",
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
			"<script type=\"text/javascript\" src=\"/tree_tg_setup.js\"></script>\n");
	printf("<script type=\"text/javascript\" src=\"/validation.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/cgi-bin/cgiLeftMenu\"></script>\n");
	printf("<script type=\"text/javascript\" src=\"/popUps.js\"></script>\n");

	printf("<script type=\"text/javascript\" src=\"/spinWait.js\"></script>\n"); // new 2014.02.23
			printf("<script type=\"text/javascript\" src=\"/Spinner.js\"></script>\n");

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
	printf("<div id=\"WaitDiv\" style=\"position:absolute; top:150px; left:250px;\"></div>");

	printf("<table id=\"mytable\" cellspacing=\"0\">\n");
	printf(
			"<caption><img src=\"/images/page_white_wrench.png\" width=\"16\" height=\"16\" alt=\"\" align=\"top\" />    Trafoguard setup      </caption>\n");
	printf("</table>\n");

	printf("<form name=\"tg_form\" action=\"javascript:updateTG('w')\">\n");
	printf("<script>\n");
	printf(
			"var tree2 = new COOLjsTreePRO(\"tree2\", TREE_TG_SETUP, TREE_FORMAT_SETUP);\n");
	printf("tree2.init();\n");
	printf("</script>");

	printf("<div id=\"contentfooter\" style=\"padding-left:30px;\">\n");
	printf("<INPUT TYPE=SUBMIT VALUE=\"Submit\" name=\"btn\" class=\"btn\" onmouseover='this.className=\"btn btnhov\"' onmouseout='this.className=\"btn\"'>\n");
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

	printf("</script>\n");
	printf("</body>\n");
	printf("</html>\n");
	//list_clear(&entries);
	if (temp != NULL)
	free(temp);
	fflush(stdout);
	return 0;
}

