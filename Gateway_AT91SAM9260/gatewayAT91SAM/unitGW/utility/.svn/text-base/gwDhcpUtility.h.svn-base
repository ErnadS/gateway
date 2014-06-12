/*
 * gwDhcpUtil.h
 *
 *  Created on: 19. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDHCPUTILITY_H_
#define GWDHCPUTILITY_H_

#include <string.h>
#include <stdio.h>
#include "../model/gwDHCP.h"

char gwDhcpUtility_saveStruct(GW_DHCP dhcpStruct);
char gwDhcpUtility_readStruct(GW_DHCP* pdhcpStruct);
int gwDhcpUtility_updateStructFromBuffer(char* bufferRec,
		GW_DHCP* pdhcpStruct, char * separator);
int gwDhcpUtility_getXMLfromStruct(GW_DHCP* pdhcpStruct, char* bufferXML);
void gwDhcpUtility_clearStruct(GW_DHCP* pdhcpStruct);


#endif /* GWDHCPUTILITY_H_ */
