/*
 * dhcpConfigFile.h
 *
 *  Created on: 19. apr. 2010
 *      Author: Ernad
 */

#ifndef DHCPCONFIGFILE_H_
#define DHCPCONFIGFILE_H_

#include "../../model/gwDHCP.h"

char dhcpConfig_init(void);
char dhcpConfig_readFromFile(GW_DHCP * pdhcpStruct);
char dhcpConfig_saveToFile(GW_DHCP dhcpStruct);

#endif /* DHCPCONFIGFILE_H_ */
