/* template.c */
#include <stdio.h>
//#include "cgivars.h"
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"
int template_page(char **postvars) {
int i = 0;
int j =1;
char *a=NULL;

for (i = 0; i < 10; i++)
	j = j+1;
addTitleElement("Parameter Setting");
/* GET */
printf("<FORM ACTION=\"%s\" METHOD=POST>", "/cgi-bin/TrafoGuard");//"/cgi-bin/result");
printf("<P>Parameter 1 :\"%d\"",j);
printf("<INPUT NAME=\"Parameter_1\" VALUE=%s SIZE=\"16\"MAXLENGTH=\"16\">(Max. 16 char. No space) </BR>",a);
printf("<P>Parameter 2 ");
printf("<INPUT NAME=\"Parameter_2\" VALUE=%s SIZE=\"16\"MAXLENGTH=\"16\"></BR>", a);
printf("<P>Parameter 3 ");
printf("<INPUT NAME=\"Parameter_3\" VALUE=%s SIZE=\"16\"MAXLENGTH=\"16\"></BR>", a);
printf("<BR><INPUT TYPE=submit VALUE=\"Enter\">");
printf("<INPUT TYPE=reset VALUE=\"Reset\">");
printf("</FORM>");
return 0;
}
