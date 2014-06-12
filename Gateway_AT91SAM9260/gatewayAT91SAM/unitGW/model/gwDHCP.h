/*
 * gwDHCP.h
 *
 *  Created on: 19. apr. 2010
 *      Author: Ernad
 */

#ifndef GWDHCP_H_
#define GWDHCP_H_

typedef struct {
	char dhcp[2];  // dhcp = 0 or 1
	char rout[40]; // router
	char ip[40];   // static ip
	char sm[40];   // mask
	char gat[40];  // gateway
	char dns[40];  // dns
} GW_DHCP;

#endif /* GWDHCP_H_ */
