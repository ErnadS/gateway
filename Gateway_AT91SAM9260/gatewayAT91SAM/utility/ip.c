/*
 * ip.c
 *
 *  Created on: 22. sep. 2010
 *      Author: Ernad
 */

#include "ip.h"
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <netdb.h> /* gethostname, et al */

#include <stdio.h> // printf
#include <string.h> // strcmp, strcpy

char ip_updateIpInfo() {
	struct ifaddrs *ifaddr, *ifa;
		int family, s;
		char host[NI_MAXHOST];
		//char minTest[NI_MAXHOST];
		if (getifaddrs(&ifaddr) == -1) {
			strcpy(eth0_ip, "0.0.0.0");
			printf("getifaddrs error\n");
			return -1;
		}

		/* Walk through linked list, maintaining head pointer so we
		 can free list later */

		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
			family = ifa->ifa_addr->sa_family;

			/* Display interface name and family (including symbolic
			 form of the latter for the common families) */

			printf("%s  address family: %d%s\n", ifa->ifa_name, family, (family
					== AF_PACKET) ? " (AF_PACKET)"
					: (family == AF_INET) ? " (AF_INET)"
							: (family == AF_INET6) ? " (AF_INET6)" : "");

			/* For an AF_INET* interface address, display the address */

			if (family == AF_INET || family == AF_INET6) {


				s = getnameinfo(ifa->ifa_addr,
						(family == AF_INET) ? sizeof(struct sockaddr_in)
								: sizeof(struct sockaddr_in6), host, NI_MAXHOST,
						NULL, 0, NI_NUMERICHOST);
				if (s != 0) {
					printf("getnameinfo() failed: %s\n", gai_strerror(s));
					freeifaddrs(ifaddr);
					strcpy(eth0_ip, "0.0.0.0");
					return -1;
				}
			/*	///////////////////////
				s = getnameinfo(ifa->ifa_ifu.ifu_broadaddr,
										(family == AF_INET) ? sizeof(struct sockaddr_in)
												: sizeof(struct sockaddr_in6), minTest, NI_MAXHOST,
										NULL, 0, NI_NUMERICHOST);
				printf("RRRRRR: %s\n", minTest);*/
				//////////////////////////////////////
				printf("\taddress: <%s>\n", host);
				if (strcmp(ifa->ifa_name, "eth0") == 0) {
					strcpy(eth0_ip, host);
					freeifaddrs(ifaddr);
					return 0;
				}
			}
		}

		freeifaddrs(ifaddr);
		strcpy(eth0_ip, "0.0.0.0");
		return -1;
}

char * ip_getOwnIp() {
	return eth0_ip;
}
