/*
 * dhcpConfigFile.c
 *
 *  Created on: 19. apr. 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include<sys/stat.h>

#include "dhcpConfigFile.h"
#include "../gwDhcpUtility.h"
#include "../../../config.h"

//char fileName[] = "/gateway/Config/eMail.conf";

char dhcpConfig_init(void) {
	FILE * hFile;
	hFile = fopen(DHCP_CONFIG_PATH, "r");
	if (hFile == NULL) { // FILE NOT EXIST, CREATE
		struct stat st;
		if (stat("/gateway", &st) != 0) {
			mkdir("/gateway", 0777);
			mkdir("/gateway/Config", 0777);
		} else if (stat("/gateway/Config", &st) != 0) {
			mkdir("/gateway/Config", 0777);
		}

		hFile = fopen(DHCP_CONFIG_PATH, "w"); // create file if not exist

		GW_DHCP  dhcpStruct;
		gwDhcpUtility_clearStruct(&dhcpStruct);
		dhcpConfig_saveToFile(dhcpStruct);
	}

	if (hFile == NULL) {
		return 0;
	}

	fclose(hFile);
	return 1;
}

char dhcpConfig_readFromFile(GW_DHCP * pdhcpStruct) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(DHCP_CONFIG_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	gwDhcpUtility_updateStructFromBuffer(buff, pdhcpStruct, "\n");

	fclose(hFile);
	return 1;
}

char dhcpConfig_saveToFile(GW_DHCP  dhcpStruct) {
	//char chTemp[5];

	FILE * hFile;
	hFile = fopen(DHCP_CONFIG_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("rout=", hFile);
	fputs(dhcpStruct.rout, hFile);
	fputs("\n", hFile);

	fclose(hFile);

	return 1;
}

