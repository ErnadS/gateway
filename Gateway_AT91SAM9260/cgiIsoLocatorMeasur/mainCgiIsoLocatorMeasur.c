/*
 * mainCgiTrafoguardMeas.c
 *
 *  Created on: 11. mai 2010
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


	printf("<script type=\"text/javascript\">\n");

	printf("window.onload = updateIL_t; \n");

	printf("function updateIL_t() {\n");
		printf("updateIL_m();\n");
		printf("t=setTimeout(\"updateIL_t()\", 5000);");
		printf("}\n");

	printf("function updateIL_m() {\n");

	///////////////////////////////////////////////////////////////
	// ADD CORRECTLY CAN BUS ADDRESS, RECEIVED AS "GET" PARAMETER
	///////////////////////////////////////////////////////////////
	sprintf(tmp, "var address = %d;\n", nAdr);
	printf(tmp);
/*
	/// CLEAR
	int i;
	for(i = 0; i < 16; i++) {
		printf("document.getElementById(\"ch%u\").value = \"\";\n", (i + 1));
	}*/

	printf("var url = '/cgi-bin/cgiCommonData';\n");

	printf("if (window.XMLHttpRequest){ xmlhttp=new XMLHttpRequest();}\n");
	printf("else {xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}\n");

	printf("xmlhttp.open(\"POST\",url,true);\n");
	printf(
			"xmlhttp.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');\n");

	printf("xmlhttp.onreadystatechange = function() {\n");
	printf(
			"if (xmlhttp.readyState == 4) { onReceivedILM(xmlhttp.responseXML);}}\n");
	// Add CAN BUS address and send to CGI (cgiCommonData)
	printf(
			"xmlhttp.send(\"comm=r&formID=il_m&canID=\" + address + \"&loginID=22222&END=2\");}\n");

	printf("function onReceivedILM(str){\n");
	printf("var temp;\n");
	printf("var response  = str.documentElement;\n");
	printf("if (response == null ) {window.location = \"/login.html\";}\n");

	printf("if (response.getElementsByTagName('err') != null && response.getElementsByTagName('err')[0] != null) {\n");
	printf("alert(response.getElementsByTagName('err')[0].firstChild.data);\n");
	printf("window.location = \"/index.htm\";\n");
	printf("return;}\n");

	printf(
			"document.getElementById(\"ch1\").value = (response.getElementsByTagName('ch1')[0].firstChild.data / 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch2\").value = (response.getElementsByTagName('ch2')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch3\").value = (response.getElementsByTagName('ch3')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch4\").value = (response.getElementsByTagName('ch4')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch5\").value = (response.getElementsByTagName('ch5')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch6\").value = (response.getElementsByTagName('ch6')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch7\").value = (response.getElementsByTagName('ch7')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch8\").value = (response.getElementsByTagName('ch8')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch9\").value = (response.getElementsByTagName('ch9')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch10\").value = (response.getElementsByTagName('ch10')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch11\").value = (response.getElementsByTagName('ch11')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch12\").value = (response.getElementsByTagName('ch12')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch13\").value = (response.getElementsByTagName('ch13')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch14\").value = (response.getElementsByTagName('ch14')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch15\").value = (response.getElementsByTagName('ch15')[0].firstChild.data/ 1000).toFixed(3);\n");
	printf(
			"document.getElementById(\"ch16\").value = (response.getElementsByTagName('ch16')[0].firstChild.data/ 1000).toFixed(3); } </script>\n");

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
	printf("</head>");

	printf("<body>");

	printf("<div id=\"wrapper\">");
	printf("<div id=\"header\">");

	printf("<script type=\"text/javascript\" src=\"/header.js\"></script>\n");
	printf("</div>\n");
	printf("<div id=\"content\">\n");

	printf("<table id=\"mytable\" cellspacing=\"0\">\n");
	printf("<caption><img src=\"/images/page_white_wrench.png\" width=\"16\" height=\"16\" alt=\"\" align=\"top\" />    IsoLocator measurements    </caption>\n");
	printf("</table>\n");

	printf("<table class=\"tablecontent\" cellspacing=\"0\">\n");
	//printf("<caption>IsoLocator measurements</caption>\n");

	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch1:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch1\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch2:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch2\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	///////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch3:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch3\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch4:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch4\" />mArms</td>");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch5:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch5\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	///////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch6:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch6\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch7:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch7\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	/////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch8:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch8\" />mArms</td>");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	/////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch9:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch9\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	//////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch10:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch10\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	/////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch11:</th>");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch11\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");


	/////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch12:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch12\" />mArms</td>");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	//////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch13:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch13\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");


	//////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch14:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch14\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch15:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch15\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	////////////////////////////////
	printf("<tr> <th class=\"spec\">Insulation current Ch16:</th>\n");
	printf(
			"<td><input type=\"text\" readonly=\"readonly\" class=\"text\" id=\"ch16\" />mArms</td>\n");
	printf("</tr>\n");
	printf("<tr> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>\n");

	/////////////////////////////////////
	printf("</table>\n");

	printf("<form name=\"il_m\" action=\"javascript:updateIL_m()\">\n");

	printf("<div id=\"contentfooter\" style=\"padding-left:30px;\">\n");

	printf("</div> </form> </div>\n");

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

	free(canAdr);

	return 0;
}

