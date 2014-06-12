/*
 * leftMenu.c
 *
 *  Created on: 6. aug. 2010
 *      Author: Ernad
 */
/*
 * treeNodes.c
 *
 *  Created on: 4. mai 2010
 *      Author: Ernad
 */

#include "leftMenu.h"
#include <stdio.h>
#include<sys/stat.h>
#include "../../config.h"
#include "../model/linkedList/deviceLinkedList.h"
#include "../model/device.h"
#include <string.h>
#include "../gateway.h"
#include "../utility/gwDataLogerUtility.h"
#include "../../log/dataLogerLogic.h"
#include "../../can/canInterface.h"

void convertIdToText(char* text, int nAdr);

unsigned int treeNodes_addForUser(char* buffer, char admin);

unsigned int treeNodes_addStart(char* buffer, char admin);

unsigned int treeNodes_addTgLog(char* buffer, unsigned char adr,
		unsigned char msid);
unsigned int treeNodes_addIlLog(char* buffer, unsigned char adr,
		unsigned char msid);
unsigned int treeNodes_addSwLog(char* buffer, unsigned char adr,
		unsigned char msid);

unsigned int treeNodes_addGatewayUnit(char* buffer, char admin);
unsigned int treeNodes_addTgUnit(char* buffer, DEV* dev, char admin);
unsigned int treeNodes_addIlUnit(char* buffer, DEV* dev, char admin);
unsigned int treeNodes_addSwUnit(char* buffer, DEV* dev, char admin);
unsigned int treeNodes_addApUnit(char* buffer, DEV* dev, char admin);


char *heartBeatPointer;

unsigned int appendString(char* buffer, char * string);

unsigned int appendString(char* buffer, char * string) {
	strcpy(buffer, string);
	return strlen(string);
}

unsigned int treeNodes_createMenu(char* buffer, char admin) {
	return treeNodes_addForUser(buffer, admin); // 1- admin, 0 none admin
}

unsigned int treeNodes_addForUser(char* buffer, char admin) {
	unsigned int nLength = 0;
	heartBeatPointer = canInterface_getHeardBeatPointer();

	nLength += treeNodes_addStart(buffer, admin);
	printf("%u\n", nLength);
	nLength += treeNodes_addGatewayUnit(&buffer[nLength], admin);
	printf("%u\n", nLength);
	int i;
	int nDevicesCount = deviceLinkedList_getSize();

	DEV * dev;
	char hasDisabledDevices = 0;

	for (i = 0; i < nDevicesCount; i++) {
		dev = deviceLinkedList_getElementAt(i);

		if (dev->enab == 1) {
			if (dev->type == TG) {
				nLength += treeNodes_addTgUnit(&buffer[nLength], dev, admin);
			} else if (dev->type == IL) {
				nLength += treeNodes_addIlUnit(&buffer[nLength], dev, admin);
			} else if (dev->type == SW) {
				nLength += treeNodes_addSwUnit(&buffer[nLength], dev, admin);
			} else if (dev->type == AP) {
				nLength += treeNodes_addApUnit(&buffer[nLength], dev, admin);
			} else if (dev->type == SP) {
				nLength += treeNodes_addApUnit(&buffer[nLength], dev, admin);
			}
		} else {
			hasDisabledDevices = 1;
		}
	}
	nLength += appendString(&buffer[nLength], "]");

	if (hasDisabledDevices) {
		nLength
				+= appendString(
						&buffer[nLength],
						", ['Disabled devices', null, \"clsNodeSelected\",{format:{expanded:true,folders:[\"/images/blank.gif\", \"/images/blank.gif\", \"/images/blank.gif\"]}}\n");
		for (i = 0; i < nDevicesCount; i++) {
			dev = deviceLinkedList_getElementAt(i);

			if (dev->enab == 0) {
				if (dev->type == TG) {
					nLength
							+= treeNodes_addTgUnit(&buffer[nLength], dev, admin);
				} else if (dev->type == IL) {
					nLength
							+= treeNodes_addIlUnit(&buffer[nLength], dev, admin);
				} else if (dev->type == SW) {
					nLength
							+= treeNodes_addSwUnit(&buffer[nLength], dev, admin);
				} else if (dev->type == AP) {
					nLength
							+= treeNodes_addApUnit(&buffer[nLength], dev, admin);
				} else if (dev->type == SP) {
					nLength
							+= treeNodes_addApUnit(&buffer[nLength], dev, admin);
				}
			}
		}
		nLength += appendString(&buffer[nLength], "]");
	}

	nLength += appendString(&buffer[nLength], "];");

	return nLength;
}

unsigned int treeNodes_addStart(char * buffer, char admin) {
	unsigned int nLength = 0;

	nLength = appendString(buffer, "var TREE_NODES = [\n");

	nLength
			+= appendString(
					&buffer[nLength],
					"['Menu', null, \"clsNodeSelected\", {format:{expanded:true,folders:[\"/images/blank.gif\", \"/images/blank.gif\", \"/images/blank.gif\"]}},\n");
	nLength
			+= appendString(
					&buffer[nLength],
					"['Welcome', '/index.htm', null, {format:{folder:['/images/page_white_edit.png', '/images/page_white_text.png', '/images/page_white_wrench.png']}}], \n");

	//nLength += appendString(&buffer[nLength],
	//		"['System outline', '/UnderConstruction.html', null],\n");

	nLength += appendString(&buffer[nLength],
			"['Alarm messages', '/gw_alarmMsgs.htm', null],\n");

	////////////////////////////////////////////////////////////////////
	// ADD DATA LOGER (ONE FOR EACH MEASSURING)
	////////////////////////////////////////////////////////////////////
	// nLength += appendString(&buffer[nLength], "['Data logger', null, null \n");

	int i;

	DEV_MEASSUR_TASK * devMeassuringsTask;
	devMeassuringsTask = gwDagaLogerUtility_getStruct();

	DEV * dev;
	char hasLog = 0;
	for (i = 0; i < MAX_LOG_TASKS; i++) {
		if (devMeassuringsTask[i].adr > 0) {
			nLength += appendString(&buffer[nLength],
					"['Data logger', null, null \n");
			hasLog = 1;
			break;
		}
	}

	if (hasLog != 1) {
		nLength += appendString(&buffer[nLength],
							"['Data logger empty', null, null ],\n");
	} else {
		for (i = 0; i < MAX_LOG_TASKS; i++) {
			if (devMeassuringsTask[i].adr > 0) {
				dev = deviceLinkedList_getDevice(devMeassuringsTask[i].adr);
				if (dev != NULL) {
					if (dev->type == TG) {
						printf("add log for TG\n");
						nLength += treeNodes_addTgLog(&buffer[nLength],
								devMeassuringsTask[i].adr,
								devMeassuringsTask[i].msid);
					} else if (dev->type == IL) {
						printf("add log for IL\n");
						nLength += treeNodes_addIlLog(&buffer[nLength],
								devMeassuringsTask[i].adr,
								devMeassuringsTask[i].msid);
					} else if (dev->type == SW) {
						printf("add log for SW\n");
						nLength += treeNodes_addSwLog(&buffer[nLength],
								devMeassuringsTask[i].adr,
								devMeassuringsTask[i].msid);
					} else
						printf("adding left menu, unknown device\n");
				} else {
					devMeassuringsTask[i].adr = 0; // remove device, does not exist
					gwDataLogerUtility_saveStruct();
				}
			}
		}

		nLength += appendString(&buffer[nLength], "],\n");
	}

	//////////////////////////////////////////////////////////////////////////
	nLength += appendString(&buffer[nLength],
			"['Help', '/cgi-bin/cgiHelp', null]],\n");
	//fputs("['Help', '/cgi-bin/cgiHelp', null]],\n", hFile);
	nLength
			+= appendString(
					&buffer[nLength],
					"['Devices', null, \"clsNodeSelected\",{format:{expanded:true,folders:[\"/images/blank.gif\", \"/images/blank.gif\", \"/images/blank.gif\"]}}\n");
	/*fputs(
	 "['Devices', null, \"clsNodeSelected\",{format:{expanded:true,folders:[\"/images/blank.gif\", \"/images/blank.gif\", \"/images/blank.gif\"]}}\n",
	 hFile);*/

	return nLength;
}

unsigned int treeNodes_addTgLog(char* buffer, unsigned char adr,
		unsigned char msid) { //DEV* dev) {
	char temp[300];
	unsigned int nLength = 0;

	sprintf(
			temp,
			",['TG adr: %d, %s', '/cgi-bin/cgiLog?adr=%d&msid=%d', null, {format:{folders:[null, null, \"/images/excel.png\"]}}]\n",
			adr, messageTextTg[msid], adr, msid);
	nLength += appendString(&buffer[nLength], temp);
	//fputs(temp, hFile);

	return nLength;
}

unsigned treeNodes_addIlLog(char* buffer, unsigned char adr, unsigned char msid) { //
	char temp[300];
	unsigned int nLength = 0;

	sprintf(
			temp,
			",['IL adr %d, CH%d', '/cgi-bin/cgiLog?adr=%d&msid=%d', null, {format:{folders:[null, null, \"/images/excel.png\"]}}]\n",
			adr, (msid + 1), adr, msid);
	nLength += appendString(&buffer[nLength], temp);
	//fputs(temp, hFile);

	return nLength;
}

unsigned int treeNodes_addSwLog(char* buffer, unsigned char adr,
		unsigned char msid) { //
	char temp[300];
	unsigned int nLength = 0;

	sprintf(
			temp,
			",['SW adr: %d, %s', '/cgi-bin/cgiLog?adr=%d&msid=%d', null, {format:{folders:[null, null, \"/images/excel.png\"]}}]\n",
			adr, messageTextSw[msid], adr, msid);
	nLength += appendString(&buffer[nLength], temp);
	//fputs(temp, hFile);

	return nLength;
}

unsigned int treeNodes_addGatewayUnit(char* buffer, char admin) {
	char temp[300];
	unsigned int nLength = 0;
	char idText[4];

	int nAddr = gateway_getCanAddress();
	convertIdToText(idText, nAddr);

	sprintf(temp, ", ['%s: GateWay', null, null,\n", idText);
	nLength += appendString(&buffer[nLength], temp);
	//fputs(temp, hFile);

	/*fputs(
	 "['Unit description', '/gw_deviceinfo.htm', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
	 hFile);*/

	nLength
			+= appendString(
					&buffer[nLength],
					"['Unit description', '/gw_deviceinfo.htm', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n");

	if (admin == 1) {
		nLength
				+= appendString(
						&buffer[nLength],
						"['Device parameters', '/gw_setup_device_par.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['Device parameters', '/gw_setup_device_par.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/
/* not used
		nLength
				+= appendString(
						&buffer[nLength],
						"['System description', '/UnderConstruction.html', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		*/
		/*fputs(
		 "['System description', '/gw_setup_system_description.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['Data logger setup', '/gw_setup_datalogger.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['Data logger', '/gw_setup_datalogger.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['History logg', '/gatewayLog', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");

		nLength
				+= appendString(
						&buffer[nLength],
						"['Device monitoring', '/gw_setup_devicemonitor.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['Device monitoring', '/gw_setup_devicemonitor.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['Network parameters', '/gw_setup_network.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['Network parameters', '/gw_setup_network.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['Url links', '/gw_setup_urllinks.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['Url links', '/gw_setup_urllinks.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['E-mail parameters', '/gw_setup_email.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['E-mail parameters', '/gw_setup_email.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['Passwords', '/gw_setup_psw.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n");
		/*fputs(
		 "['Passwords', '/gw_setup_psw.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}],\n",
		 hFile);*/

		nLength
				+= appendString(
						&buffer[nLength],
						"['ModBus', '/gw_modBus.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}]\n");
		/*fputs(
		 "['ModBus', '/gw_setup_modbus.htm', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}]\n",
		 hFile);*/
	}

	nLength += appendString(&buffer[nLength], "]\n");

	/*fputs("]\n", hFile);*/

	return nLength;
}

void convertIdToText(char* text, int nAdr) {
	if (nAdr < 10) {
		text[0] = '0';
		text[1] = '0';
		sprintf(&text[2], "%u", nAdr);
	} else if (nAdr < 100) {
		text[0] = '0';
		sprintf(&text[1], "%u", nAdr);
	} else {
		sprintf(&text[0], "%u", nAdr);
	}
	text[3] = 0;
}

unsigned int treeNodes_addTgUnit(char* buffer, DEV* dev, char admin) {
	char temp[300];
	unsigned int nLength = 0;
	char idText[4];

	convertIdToText(idText, dev->adr);

	if (heartBeatPointer[dev->adr] == 1 || dev->found == 0) {
		sprintf(temp, ", ['%s: TrafoGuard ERROR', null, null,\n", idText);
	} else {
		sprintf(temp, ", ['%s: TrafoGuard', null, null,\n", idText);
	}
	nLength += appendString(&buffer[nLength], temp);
	//fputs(temp, hFile);


	sprintf(
			temp,
			"['Measurements', '/cgi-bin/cgiTrafoGuardMeasur?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);


	sprintf(
			temp,
			"['Unit info', '/cgi-bin/cgiTrafoguardInfo?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);
	//fputs(temp, hFile);

	if (admin == 1) {
		sprintf(
				temp,
				"['Setup', '/cgi-bin/cgiTrafoguard?%d', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}]\n",
				dev->adr);
		nLength += appendString(&buffer[nLength], temp);
		//fputs(temp, hFile);
	}
	nLength += appendString(&buffer[nLength], "]\n");
	//fputs("]\n", hFile);

	return nLength;
}

unsigned int treeNodes_addIlUnit(char* buffer, DEV* dev, char admin) {
	char temp[300];
	unsigned int nLength = 0;
	char idText[4];
	convertIdToText(idText, dev->adr);

	if (heartBeatPointer[dev->adr] == 1 || dev->found == 0) {
		sprintf(temp, ", ['%s: IsoLocator ERROR', null, null,\n", idText);
	} else {
		sprintf(temp, ", ['%s: IsoLocator', null, null,\n", idText);
	}
	nLength += appendString(&buffer[nLength], temp);
	// fputs(temp, hFile);

	sprintf(
			temp,
			"['Measurements', '/cgi-bin/cgiIsoLocatorMeasur?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);
	// fputs(temp, hFile);

	sprintf(
			temp,
			"['Unit info', '/cgi-bin/cgiIsoLocatorInfo?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);
	// fputs(temp, hFile);
	if (admin == 1) {
		sprintf(
				temp,
				"['Setup', '/cgi-bin/cgiIsoLocator?%d', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}]\n",
				dev->adr);
		nLength += appendString(&buffer[nLength], temp);
		// fputs(temp, hFile);
	}

	nLength += appendString(&buffer[nLength], "]\n");
	// fputs("]\n", hFile);

	return nLength;
}

unsigned int treeNodes_addSwUnit(char* buffer, DEV* dev, char admin) {
	char temp[300];
	unsigned int nLength = 0;
	char idText[4];
	convertIdToText(idText, dev->adr);

	if (heartBeatPointer[dev->adr] == 1 || dev->found == 0) {
		sprintf(temp, ", ['%s: SwitchoverUnit ERROR', null, null,\n", idText);
	} else {
		sprintf(temp, ", ['%s: SwitchoverUnit', null, null,\n", idText);
	}

	nLength += appendString(&buffer[nLength], temp);

	sprintf(
			temp,
			"['Measurements', '/cgi-bin/cgiSwitchoverMeasur?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);
	// fputs(temp, hFile);

	sprintf(
			temp,
			"['Unit info', '/cgi-bin/cgiSwitchoverInfo?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);
	// fputs(temp, hFile);
	if (admin == 1) {
		sprintf(
				temp,
				"['Setup', '/cgi-bin/cgiSwitchover?%d', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}]\n",
				dev->adr);
		nLength += appendString(&buffer[nLength], temp);
		// fputs(temp, hFile);
	}

	nLength += appendString(&buffer[nLength], "]\n");
	// fputs("]\n", hFile);

	return nLength;
}

char const AL_PAN_TEXT[] = "AlarmPanel";
char const SL_PAN_TEXT[] = "AlarmPanelSlave";

unsigned int treeNodes_addApUnit(char* buffer, DEV* dev, char admin) {
	char temp[300];
	unsigned int nLength = 0;
	char idText[4];
	char name[20];


	convertIdToText(idText, dev->adr);

	if (dev->type == SP)
		strcpy(name, SL_PAN_TEXT);
	else
		strcpy(name, AL_PAN_TEXT);

	if (heartBeatPointer[dev->adr] == 1 || dev->found == 0) {
		sprintf(temp, ", ['%s: %s ERROR', null, null,\n", idText, name);
	} else {
		sprintf(temp, ", ['%s: %s', null, null,\n", idText, name);
	}
	nLength += appendString(&buffer[nLength], temp);
	// fputs(temp, hFile);


	sprintf(
			temp,
			"['Unit info', '/cgi-bin/cgiAlarmPanelInfo?%d', null, {format:{folders:[null, null, '/images/page_white_text.png']}}],\n",
			dev->adr);
	nLength += appendString(&buffer[nLength], temp);

	if (admin == 1) {
		sprintf(
				temp,
				"['Setup', '/cgi-bin/cgiAlarmPanel?%d', null, {format:{folders:[null, null, '/images/page_white_wrench.png']}}]\n",
				dev->adr);

		nLength += appendString(&buffer[nLength], temp);
	}

	nLength += appendString(&buffer[nLength], "]\n");
	//fputs("]\n", hFile);

	return nLength;
}
