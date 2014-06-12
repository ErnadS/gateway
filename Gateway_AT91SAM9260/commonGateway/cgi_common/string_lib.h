#ifndef STRING_LIB_H_
#define STRING_LIB_H_

/* string-lib.h - headers for string-lib.c
   $Id$
*/

char *newstr(char *str);
char *substr(char *str, int offset, int len);
char *replace_ltgt(char *str);
char *lower_case(char *buffer);



#endif /*STRING_LIB_H_*/
