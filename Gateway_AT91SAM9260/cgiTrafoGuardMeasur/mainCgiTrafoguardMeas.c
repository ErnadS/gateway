/*
 * mainCgiMenu.c
 *
 *  Created on: 3. mai 2010
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
	char* canAdr;
	canAdr = get_GET(); // get CAN BUS address (e.q. http://xxx.xxx.xxx.xxx/cgiTrafoGuard?11 CAN address is after "?"
	if (canAdr == NULL) {
		return 0;
	}
	char tmp[60];
	int nAdr = atoi(canAdr);

	printf("Content-type: text/html\n\n"); //send back html (web page)
	//printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	printf(
			"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");

	printf("<head>\n");

	printf("<title>ImedSecure webserver</title>\n");
	printf(
			"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	printf("<meta http-equiv=\"Pragma\" content=\"no-cache\" />\n\n");

	printf(
			"<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"/favicon.ico\" />\n");
	printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"/imed.css\"/>\n");

	////////////////////////////////////////////


	printf("<script type=\"text/javascript\">");

	printf("window.onload = updateTG_t; ");

	printf("function updateTG_t() {\n");
		printf("updateTG_m();\n");
		printf("t=setTimeout(\"updateTG_t()\", 5000);");
	printf("}\n");

	printf("function updateTG_m() {");

	///////////////////////////////////////////////////////////////
	// ADD CORRECTLY CAN BUS ADDRESS, RECEIVED AS "GET" PARAMETER
	///////////////////////////////////////////////////////////////
	sprintf(tmp, "var address = %d;\n", nAdr);
	printf(tmp);

	// !!! APOSLUTE PATH (not relative: "cgi-bin/cgiCommonData")
	printf("var url = '/cgi-bin/cgiCommonData';\n");

	printf("if (window.XMLHttpRequest){ xmlhttp=new XMLHttpRequest();}\n");
	printf("else {xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}\n");

	printf("xmlhttp.open(\"POST\",url,true);\n");
	printf(
			"xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');\n");

	printf("xmlhttp.onreadystatechange = function() {\n");
	printf(
			"if (xmlhttp.readyState == 4) { onReceivedTGM(xmlhttp.responseXML);}}\n");
	// Add CAN BUS address and send to CGI (cgiCommonData)
	printf(
			"xmlhttp.send(\"comm=r&formID=tg_m&canID=\" + address + \"&loginID=22222&END=2\");}\n");
	printf("function onReceivedTGM(str){\n");
	printf("var temp;\n");
	printf("var response  = str.documentElement;\n");
	printf("if (response == null ) { window.location = \"/login.html\";}\n");

	printf("if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {\n");
	printf("alert(response.getElementsByTagName('err')[0].firstChild.data);\n");
	printf("window.location = \"/index.htm\";\n");
	printf("return;}\n");

	printf(
			"document.getElementById(\"ir\").value = response.getElementsByTagName('ir')[0].firstChild.data;\n");
	printf(
			"document.getElementById(\"tt\").value = (response.getElementsByTagName('tt')[0].firstChild.data / 10).toFixed(1);\n");
	printf(
			"document.getElementById(\"tc\").value = (response.getElementsByTagName('tc')[0].firstChild.data / 100).toFixed(2);}\n");

	printf("</script>\n");

	printf(
			"<script type=\"text/javascript\" src=\"/cooltreepro_uncompressed.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_format.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_nodes.js\"></script>\n");
	printf(
			"<script type=\"text/javascript\" src=\"/tree_format_setup.js\"></script>\n");

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
				"<caption><img src=\"/images/page_white_wrench.png\" width=\"16\" height=\"16\" alt=\"\" align=\"top\" />    Trafoguard measurements    </caption>\n");
		printf("</table>\n");

	printf("<table class=\"tablecontent\" cellspacing=\"0\">\n");
	//printf("<caption>Trafoguard measurements</caption>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	printf("<tr>\n");
	printf("<th class=\"spec\">Insulation resistance:</th>\n");
	printf("<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ir\" />kOhm</td>\n");
	//printf("<td>&nbsp;</td>\n");
	printf("</tr>\n");

	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");
	///////////////////////////////////////////

	printf("<tr>\n");
	printf("<th class=\"spec\">Transformer current:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"tc\" />Arms</td>\n");
	printf("</tr>");

	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");
	////////////////////////////////////////////
	printf("<tr>\n");
	printf("<th class=\"spec\">Transformer temperature:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"tt\" />&deg;C</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");
	////////////////////////////////////////////
	printf("</table>\n");

	printf("<form name=\"tg_m\" action=\"javascript:updateTG_m()\">\n");

	printf("<div id=\"contentfooter\" style=\"padding-left:30px;\">\n");

	printf("</div> </form>\n");

	printf("</div>\n"); //<!-- content -->

	printf("<div id=\"menu\">\n");
	printf("<script type=\"text/javascript\">\n");
	printf(
			"var tree1 = new COOLjsTreePRO(\"tree1\", TREE_NODES, TREE_FORMAT);\n");
	printf("tree1.init();\n");
	printf("</script>\n");
	printf("</div>\n");

	printf("<div id=\"footer\">\n");
	printf("<script type=\"text/javascript\" src=\"footer.js\"></script>\n");
	printf("</div>\n");

	printf("<script type=\"text/javascript\">\n");

	printf("</script>\n");
	printf("</body> </html>\n");

	//list_clear(&entries);

	fflush(stdout);
	if (canAdr != NULL)
		free(canAdr);
	return 0;
}

