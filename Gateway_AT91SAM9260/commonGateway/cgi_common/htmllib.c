/* htmllib.c 
 * HTML common library functions for the CGI programs. */  
  
#include <stdio.h>   
#include "htmllib.h"   
  
  
void htmlHeader(char *title) {  
  printf("Content-type: text/html\n\n<HTML><HEAD>");
  printf("<title>\" %s \"</title>",  title);

}  
  
void htmlBody() {  
    printf("");  
}  
  
void htmlFooter() {  
    printf("");  
}  
  
void addTitleElement(char *title) {  
    printf("<H1>%s</H1>", title);  
}  
