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

void printExcel(int nDevType, int nCanAddress, int nMsgId);
int appenMeasuringFile(int nDevType, int nDevId, int nMsgId, char bOld);

// USED BY cgiSwitchoverMeasur
char * messageTextTg[] = { "Temperature", "Load Current", "Insulation" };
//char * messageTextSw[] = { "Prioritized supply", "Secondary supply 1", "Secondary supply 2",
//		"IT net" };
char * messageTextSw[] = { "Preferred Supply (V)", "SecSupply 1 (V)", "SecSupply 2 (V)", "IT supply (V)" };

char * messageTextTg_file[] = { "Temperature", "LoadCurrent", "Insulation" };
char * messageTextSw_file[] = { "PreferredSupply", "SecSupply_1", "SecSupply_2", "ITsupply" };


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

enum devType {
	TG = 1, // TODO: 1,
	IL = 2,
	SW = 3, //TODO: 3,
	AP = 4,
	SP = 6
};

char* devName[] = {"Unknown", "TrafoGuard", "IsoLocator", "Switchover", "AlarmPanel", "", "AlarmPanelSlave"};

void makeFileName(int devType, int nDevAdr, int nMsgId, char* fileName) {
	if (devType == TG) {
		sprintf(fileName, "TG_%d_%s", nDevAdr, messageTextTg_file[nMsgId]);
	} else if (devType== SW) {
		sprintf(fileName, "SW_%d_%s", nDevAdr, messageTextSw_file[nMsgId]);
	} else if (devType == IL) {
		sprintf(fileName, "IL_%d_Channel_%d", nDevAdr, (nMsgId + 1));
	}
}

void printExcel(int nDevType, int nCanAddress, int nMsgId) {
	char* bufferXML;
		char  buffer[1000];

	//	DEV * dev = deviceLinkedList_getDevice(nCanAddress);
		int nOffset = 0;
		//DEV_MEASSUR_LIST devMeasList;
		//char temp[200];
		char msgTypeText[30];

		//int lineLen;

		if (nDevType == TG) {
				sprintf(msgTypeText, "%s", messageTextTg_file[nMsgId]);
		} else if (nDevType == SW) {
				sprintf(msgTypeText, "%s", messageTextSw_file[nMsgId]);
		} else if (nDevType == IL) {
				sprintf(msgTypeText, "Channel_%d", (nMsgId + 1));
		} else { // never?
			strcpy(msgTypeText, "Error type");
		}

		nOffset += sprintf(buffer, "<TABLE><TR><TD><b>UNIT:</b></TD><TD>%s</TD></TR><TR><TD><b>ADDRESS:</b></TD><TD>%d</TD></TR><TR><TD><b>Msg type:</b></TD><TD>%s</TD></TR></TABLE>",
				devName[nDevType], nCanAddress, msgTypeText); //devName[dev->type], dev->adr, msgTypeText);

		printf(buffer);
		//nOffset += sprintf(&buffer[nOffset], "<TABLE><TR><TD><b>UNIT:</b></TD><TD>%s</TD></TR><TR><TD><b>ADDRESS:</b></TD><TD>%d</TD></TR><TR><TD><b>Msg type:</b></TD><TD>%s</TD></TR></TABLE>",
			//			devName[dev->type], dev->adr, msgTypeText);


		printf("<pre>Date\t\tTime\t\tValue</pre>");
		//nOffset += sprintf(&buffer[nOffset], "<pre>Date\t\tTime\t\tValue</pre>");

		appenMeasuringFile(nDevType, nCanAddress, nMsgId, 1);
		appenMeasuringFile(nDevType, nCanAddress, nMsgId, 0);
		// nOffset += appenMeasuringFile(dev, nMsgId, 1, &buffer[nOffset]);
		// nOffset += appenMeasuringFile(dev, nMsgId, 0, &buffer[nOffset]);
}

#define MEASURE_FOLDER 				"/srv/www/logFiles/"

int appenMeasuringFile(int nDevType, int nDevId, int nMsgId, char bOld) {
	char pathName[100];
	char fileName[100];
	FILE * hFile;

	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	int nOffset = 0;

	makeFileName(nDevType, nDevId, nMsgId, fileName);
	//sprintf(fileName, "%s", "TG_17_Temperature");
	//makeFileName(dev, nMsgId, fileName);

	if (bOld)
		sprintf(pathName, "%s%s_old", MEASURE_FOLDER, fileName);
	else
		sprintf(pathName, "%s%s", MEASURE_FOLDER, fileName);

	hFile = fopen(pathName, "r");

	if (hFile == NULL) {
		// printf("!!! CANNOT open file: %s\n", pathName);
		return 0;
	}

	printf("<pre style=\"height: 10pc;\">"); //style=\"display:inline-block;white-space: pre-wrap; word-wrap:break-word;\">");

	// Example reading from file:
	// http://man7.org/linux/man-pages/man3/getline.3.html
	while ((read = getline(&line, &len, hFile)) != -1) {
			if (read < 5)  // min 5 characters in line (should bi more...)
				continue;

			printf(line);
	}

	printf("</pre>");
	if (line)
	     free(line);

	fclose(hFile);

	return nOffset;
}


int main() {
	//char* temp;
	char* adr;
	char* msid; // message id to log
	int nMsgId;
	int nAdr;
	int nDevType;

	int status;

	llist entries;
	llist entriesCookie;

	status = read_cgi_input(&entries);

	// Take a cookie from request. The same cookie must be sent to gateway before reading/writing parameters
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
		nDevType = atoi(cgi_val(entries, "typ"));

		nMsgId = atoi(msid);
		nAdr = atoi(adr);



		if (nAdr > 0 && nMsgId >= 0 && nDevType > 0) {

			/*int nConnRes = connectToServer();
			if (nConnRes > 0) {
				char postResult[MAX_EXCEL_SIZE];
				postResult[0] = 0;
				//char* IP = getenv("REMOTE_ADDR");

				// We must first send a cookie to gateway (or connection will be refused)
				sendCookie(cookiVal);//IP);
				char par[30];
				// ES: best to make short pause after sending cookie to gateway (for at melding ikke blir limet sammen med parameter som sendes deretter)
				// derfor sender vi her med "sprintf"
				sprintf(par, "formID=loging&canID=%d&END=2&msid=%d", nAdr,
						nMsgId);
				// and get parameters
				getParam(par, postResult, MAX_EXCEL_SIZE - 1);

				if (postResult[0] == 0) {
					refuseUser();

					list_clear(&entries);
					list_clear(&entriesCookie);
					return 0;
				}*/
				printFirstPart();

				printExcel(nDevType, nAdr, nMsgId);
			   // printf(postResult);

				printSecondPart();

				fflush(stdout);
			//}
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
