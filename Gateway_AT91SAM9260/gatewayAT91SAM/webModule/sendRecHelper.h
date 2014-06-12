/*
 * sendRecHelper.h
 *
 *  Created on: Mar 9, 2014
 *      Author: ernad
 */

#ifndef SENDRECHELPER_H_
#define SENDRECHELPER_H_

#include <unistd.h>             /*  for ssize_t data type  */

#define LISTENQ        (1024)   /*  Backlog for listen()   */


/*  Function declarations  */

ssize_t Readline(int sockd, char *buffer, size_t maxlen);
ssize_t Writeline(int fc, const void *vptr, size_t maxlen);

#endif /* SENDRECHELPER_H_ */
