/*
 * dhcpConfig.c
 *
 *  Created on: 19. apr. 2010
 *      Author: Ernad
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dhcpLinux.h"
#include "../utility/paramUtil.h"
#include "../utility/myString.h"

#include "../config.h"
#include <unistd.h>

char dhcpLinux_updateStructFromBuffer(char* buff, GW_DHCP* dhcp);

/////////////////////////////////////////////////////
// needs for DNS functionality (gateway is DNS)
/////////////////////////////////////////////////////
char dhcpLinux_saveResolvConf(GW_DHCP dhcp);
char dhcpLinux_readResolvConf(GW_DHCP* dhcp);


char dhcpLinux_save(GW_DHCP dhcp) {

	int nDhcp = atoi(dhcp.dhcp);

	dhcpLinux_saveResolvConf(dhcp);

	FILE * hFile;
	hFile = fopen(DHCP_LIN_CONFIG_PATH, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("#ImedSec\n", hFile);

	fputs("auto lo", hFile);
	fputs("\n", hFile);
	fputs("iface lo inet loopback", hFile);
	fputs("\n", hFile);

	fputs("auto eth0", hFile);
	fputs("\n", hFile);

	if (nDhcp == 1) {
		fputs("iface eth0 inet dhcp", hFile);
		fputs("\n", hFile);
	} else {
		fputs("iface eth0 inet static", hFile);
		fputs("\n", hFile);

		fputs("address ", hFile);
		fputs(dhcp.ip, hFile);
		fputs("\n", hFile);

		fputs("netmask ", hFile);
		fputs(dhcp.sm, hFile);
		fputs("\n", hFile);
		//TODO: missing network information
		if (dhcp.gat[0] != 0) {
			fputs("gateway ", hFile);
			fputs(dhcp.gat, hFile);
		}
		fputs("\n", hFile);
	}

	fclose(hFile);

	system("/etc/init.d/networking stop");
	sleep(1);
	system("/etc/init.d/networking start");

	return 1;
}

char dhcpLinux_read(GW_DHCP* dhcp) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(DHCP_LIN_CONFIG_PATH, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	if (dhcpLinux_updateStructFromBuffer(buff, dhcp) != 0) {
		fclose(hFile);

		strcpy(dhcp->dhcp, "0");
		strcpy(dhcp->ip, "192.168.1.222");
		strcpy(dhcp->sm, "255.255.255.0");
		strcpy(dhcp->gat, "192.168.1.1");
		strcpy(dhcp->dns, "192.168.1.1");
		dhcpLinux_save(*dhcp);
	} else {
		fclose(hFile);
	}

	return 1;
}

char dhcpLinux_updateStructFromBuffer(char* buff, GW_DHCP* dhcp) {
	int nLength;
	int nPos = 0;
	int i;

	dhcpLinux_readResolvConf(dhcp);

	nLength = getLengthOfNextLine(&buff[nPos]); //#ImedSec\n
	if (nLength <= 0 || (nameStartsWith("#ImedSec", buff) != 0))
		return -1;

	nPos += (nLength + 1);

	for (i = 0; i < 3; i++) { // "auto lo", "iface lo inet loopback", "auto eth0"
		nLength = getLengthOfNextLine(&buff[nPos]);
		if (nLength <= 0)
			return -1;
		nPos += (nLength + 1);
	}

	if (nLength > 0) {
		if (nameStartsWith("iface eth0 inet dhcp", &buff[nPos]) == 0) {
			dhcp->dhcp[0] = '1';
			dhcp->dhcp[1] = 0;

			return 0;
		} else if (nameStartsWith("iface eth0 inet static", &buff[nPos]) == 0) {
			dhcp->dhcp[0] = '0';
			dhcp->dhcp[1] = 0;
			nLength = getLengthOfNextLine(&buff[nPos]);
			nPos += (nLength + 1); // move to end

			nLength = getLengthOfNextLine(&buff[nPos]);
			if (nLength <= 0)
				return -1;
			buff[nPos + nLength] = 0; // null terminate
			strcpy(dhcp->ip, &buff[nPos + 8]); // copy IP address
			nPos += (nLength + 1); // move to end

			nLength = getLengthOfNextLine(&buff[nPos]);
			if (nLength <= 0)
				return -1;
			buff[nPos + nLength] = 0; // null terminate
			strcpy(dhcp->sm, &buff[nPos + 8]); // copy netmask
			nPos += (nLength + 1); // move to end

			nLength = getLengthOfNextLine(&buff[nPos]);
			if (nLength >= 16) { // min.: "gateway 1.1.1.1"
				buff[nPos + nLength] = 0; // null terminate
				strcpy(dhcp->gat, &buff[nPos + 8]); // copy gateway
				nPos += (nLength + 1); // move to end
			} else if (nLength >= 7) { // something wrong
				return -1;
			}
			return 0;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////
// needs for DNS functionality (gateway is DNS)
/////////////////////////////////////////////////////
char dhcpLinux_saveResolvConf(GW_DHCP dhcp) {
	FILE * hFile;
	hFile = fopen(RESOLV_CONF, "w");
	if (hFile == NULL) {
		return 0;
	}

	fputs("nameserver ", hFile);

	if (dhcp.dns[0] != 0) {
		fputs(dhcp.dns, hFile);
	} else if (dhcp.gat[0] != 0) {
		fputs(dhcp.gat, hFile);
	}
	fputs("\n", hFile);

	fclose(hFile);

	return 1;
}

char dhcpLinux_readResolvConf(GW_DHCP* dhcp) {
	int i = 0;
	int maxSize = 500;
	char buff[maxSize];

	FILE * hFile;
	hFile = fopen(RESOLV_CONF, "r");
	if (hFile == NULL) {
		return 0;
	}

	int c;
	while ((c = fgetc(hFile)) != EOF && i < maxSize) {
		buff[i++] = (char) c;
	}
	buff[i] = 0;

	int j;
	for (j= 0; j <i; j++) { // remove new line if exist
		if (buff[j] == '\n') {
			buff[j] = 0;
			break;
		}
	}
	buff[10] = 0;
	dhcp->dns[0] = 0;

	if (strcmp(buff, "nameserver") == 0) {
		if (strlen(&buff[11]) >= 7 && strlen(&buff[11]) < 16) {
			strcpy(dhcp->dns, &buff[11]);
		}
	}
	fclose(hFile);
	return 0;
}

