/*
 * gwDhcpUtil.c
 *
 *  Created on: 19. apr. 2010
 *      Author: Ernad
 */

#include "gwDhcpUtility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utility/xml.h"
#include "../../utility/paramUtil.h"

#include "dhcpConfig/dhcpConfigFile.h"
#include "../../services/dhcpLinux.h"
#include "../../config.h"
#include <unistd.h>

char gwDhcpUtility_saveStruct(GW_DHCP dhcpStruct) {
	dhcpLinux_save(dhcpStruct);
	return  dhcpConfig_saveToFile(dhcpStruct);
}

char gwDhcpUtility_readStruct(GW_DHCP* pdhcpStruct) {
	gwDhcpUtility_clearStruct(pdhcpStruct);
	dhcpLinux_read(pdhcpStruct);
	return 1;
}

int gwDhcpUtility_updateStructFromBuffer(char* bufferRec,
		GW_DHCP* pdhcpStruct, char * separator) {
	int nLength = 0;
	int buffRecPos = 0;
	char paramName[MAX_PARAM_NAME_LENGTH];
	char paramValue[MAX_PARAM_VALUE_LENGTH];

	while (1) {
		nLength = paramUtil_getNextParam(&bufferRec[buffRecPos], separator,
				paramName, paramValue);
		if (nLength < 0)
			break;

		if (strcmp(paramName, "dhcp") == 0) {
			strcpy((pdhcpStruct->dhcp), paramValue);
		} else if (strcmp(paramName, "rout") == 0) {
			strcpy((pdhcpStruct->rout), paramValue);
		} else if (strcmp(paramName, "ip") == 0) {
			strcpy((pdhcpStruct->ip), paramValue);
		} else if (strcmp(paramName, "sm") == 0) {
			strcpy((pdhcpStruct->sm), paramValue);
		} else if (strcmp(paramName, "gat") == 0) {
			if (strlen(paramValue) > 0) {
				strcpy((pdhcpStruct->gat), paramValue);
			} else {
				pdhcpStruct->gat[0] = 0;
			}
		} else if (strcmp(paramName, "dns") == 0) {
			if (strlen(paramValue) > 0) {
				strcpy((pdhcpStruct->dns), paramValue);
			} else {
				pdhcpStruct->dns[0] = 0;
			}
		}

		if (nLength == 0)
			break;
		buffRecPos += nLength;
	}

	return 1;
}

int gwDhcpUtility_getXMLfromStruct(GW_DHCP* pdhcpStruct, char* bufferXML) {
	int nLength = 0;
	char chTemp[MAX_PARAM_VALUE_LENGTH];

	sprintf(chTemp, "%s", pdhcpStruct->dhcp);
	nLength += xml_addTag(&bufferXML[nLength], "dhcp", chTemp);

	sprintf(chTemp, "%s", pdhcpStruct->rout);
	nLength += xml_addTag(&bufferXML[nLength], "rout", chTemp);

	sprintf(chTemp, "%s", pdhcpStruct->ip);
	nLength += xml_addTag(&bufferXML[nLength], "ip", chTemp);

	sprintf(chTemp, "%s", pdhcpStruct->sm);
	nLength += xml_addTag(&bufferXML[nLength], "sm", chTemp);

	sprintf(chTemp, "%s", pdhcpStruct->gat);
	nLength += xml_addTag(&bufferXML[nLength], "gat", chTemp);

	sprintf(chTemp, "%s", pdhcpStruct->dns);
		nLength += xml_addTag(&bufferXML[nLength], "dns", chTemp);

	return nLength;
}

void gwDhcpUtility_clearStruct(GW_DHCP* pdhcpStruct) {

	strcpy(pdhcpStruct->dhcp, "0");
	strcpy(pdhcpStruct->gat, "192.168.1.1");
	strcpy(pdhcpStruct->ip, "192.168.1.222");
	strcpy(pdhcpStruct->rout, "router");
	strcpy(pdhcpStruct->sm, "255.255.255.0");
	strcpy(pdhcpStruct->dns, "192.168.1.1");
}


