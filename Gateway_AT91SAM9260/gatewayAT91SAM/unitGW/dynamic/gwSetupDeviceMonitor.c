/*
 * GwSetupDeviceMonitor.c
 *
 *  Created on: 22. apr. 2010
 *      Author: Ernad
 */
#include <stdio.h>
#include<sys/stat.h>
#include "../../config.h"
#include "../model/linkedList/deviceLinkedList.h"
#include "../model/device.h"
#include <string.h>
#include <syslog.h>


// prototype
char gwSetupDeviceMonitor_addFile(FILE * hFile, char* fileToAdd);
void gwSetupDeviceMonitor_addSendArguments(FILE * hFile);
void gwSetupDeviceMonitor_addOnReceiveArguments(FILE * hFile);
void gwSetupDeviceMonitor_addTableArguments(FILE * hFile);

char gwSetupDeviceMonitor_createPage(void) {
	FILE * hFile;
	hFile = fopen(SET_DEV_MON_PATH_COMPLET, "w");
	if (hFile == NULL) {
		printf("!!! CAN NOT CREATE Device Monitoring Page\n");
		syslog(LOG_ERR, "!!! CAN NOT CREATE Device Monitoring Page");
		return -1;
	}

	gwSetupDeviceMonitor_addFile(hFile, SET_DEV_MON_PATH_1);

	gwSetupDeviceMonitor_addSendArguments(hFile);
	gwSetupDeviceMonitor_addOnReceiveArguments(hFile);

	gwSetupDeviceMonitor_addFile(hFile, SET_DEV_MON_PATH_2);

	gwSetupDeviceMonitor_addTableArguments(hFile);

	gwSetupDeviceMonitor_addFile(hFile, SET_DEV_MON_PATH_3);
	fclose(hFile);
	return 1;
}

void gwSetupDeviceMonitor_addTableArguments(FILE * hFile) {
	int i;
	int nDevicesCount = deviceLinkedList_getSize();
	char temp[300];
	DEV * dev;
	printf("*** device count: %u\n", nDevicesCount);
	for (i = 0; i < nDevicesCount; i++) {
		dev = deviceLinkedList_getElementAt(i);
		sprintf(temp, "<tr><th class=\"spec\" style=\"text-align:center;\"><input type=\"text\"  readonly=\"readonly\" name=\"adr%u\" value=\"%u\"></th><td class=\"alt\"> %s", i, dev->adr, devName[dev->type]);
		fputs(temp, hFile);

		sprintf(temp, "</td><td class=\"center\"><input type=\"checkbox\" id=\"enab%u\"class=\"nobg\" name=\"\" checked>&nbsp;</td></tr>", i);
		fputs(temp, hFile);
	}
}

void gwSetupDeviceMonitor_addSendArguments(FILE * hFile) {
	int i;
	int nDevicesCount = deviceLinkedList_getSize();
	char temp[60];

	for (i = 0; i < nDevicesCount; i++) {
			sprintf(temp, "var enab%u;\n", i);
			fputs(temp, hFile);
			sprintf(temp, "if (document.getElementById('enab%u').checked)\n", i);
			fputs(temp, hFile);

			sprintf(temp, "enab%u = 1;\n", i);
			fputs(temp, hFile);

			fputs("else\n", hFile);
			sprintf(temp, "enab%u = 0;\n", i);
			fputs(temp, hFile);
		}


	fputs("xmlhttp.send(\"comm=w&formID=gw_dm&canID=122211&loginID=22222&END=2\"", hFile);

	for (i = 0; i < nDevicesCount; i++) {
		sprintf(temp, "+ \"&enab\" + document.form_gwDM.adr%d.value + \"=\" + enab%d", i, i);
		fputs(temp, hFile);
	}
	//fputs("); window.location.reload(true);}}\n", hFile);
	fputs("); }}\n", hFile);

	fputs("function updatepageGW_DM(str){", hFile);
	fputs("var temp;", hFile);
	fputs("var response  = str.documentElement;", hFile);
	fputs("if (response == null ) {", hFile);
	fputs("alert(\"Error 2127\");", hFile);
	fputs("return;}", hFile);
}

void gwSetupDeviceMonitor_addOnReceiveArguments(FILE * hFile) {
	int nDevicesCount = deviceLinkedList_getSize();
	int i;
	char temp[100];

	for (i = 0; i < nDevicesCount; i++) {
		sprintf(
				temp,
				"if (response.getElementsByTagName('enab%d')[0].firstChild.data == \"1\")\n",
				i);
		fputs(temp, hFile);
		sprintf(temp, "document.form_gwDM.enab%d.checked = true;\n", i);
		fputs(temp, hFile);
		fputs("else\n", hFile);
		sprintf(temp, "document.form_gwDM.enab%d.checked = false;\n", i);
		fputs(temp, hFile);
/*
		sprintf(
				temp,
				"document.form_gwDM.adr%d.value = response.getElementsByTagName('adr%d')[0].firstChild.data;\n",
				i, i);
		fputs(temp, hFile);
		sprintf(
				temp,
				"document.form_gwDM.dev%d.value = response.getElementsByTagName('dev%d')[0].firstChild.data;\n",
				i, i);
		fputs(temp, hFile);*/
	}
}

char gwSetupDeviceMonitor_addFile(FILE * hFile, char* fileToAdd) {
	int c;

	FILE * hAddFile;
	hAddFile = fopen(fileToAdd, "r");
	if (hAddFile == NULL) {
		return -1;
	}

	while ((c = fgetc(hAddFile)) != EOF) {
		fputc(c, hFile);
	}

	fclose(hAddFile);
	return 1;
}
