#include "templateGateway.h"
#include <stdio.h>
#include "cgi_common/cgi_lib.h"
#include "cgi_common/htmllib.h"

int templateGateway_getContentType(char **postvars) {
	printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\r\n");
	printf("<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n");
	return 0;
}


int templateGateway_getHeader(char **postvars, char* title) {
	printf("<head>\r\n");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n");
	//printf("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n");
	printf("<title> %s </title>\r\n", title);
	printf("<link href=\"Simple.css\" rel=\"stylesheet\" type=\"text/css\" />\r\n");
	printf("<SCRIPT TYPE=\"text/javascript\" src=formval.js>\r\n");
	printf("</SCRIPT>\r\n");
	printf("</head>\r\n");
	return 0;
}

