#ifndef HELPER_H_
#define HELPER_H_

/* #endif 
#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP
*/

#include <unistd.h>             /*  for ssize_t data type  */

#define LISTENQ        (1024)   /*  Backlog for listen()   */


/*  Function declarations  */

ssize_t Readline(int sockd, char *buffer, size_t maxlen);
ssize_t Writeline(int fc, const void *vptr, size_t maxlen);


//#endif  /*  PG_SOCK_HELP  */

#endif /*HELPER_H_*/
