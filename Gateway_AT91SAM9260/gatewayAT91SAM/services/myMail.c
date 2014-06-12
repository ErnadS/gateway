/*
 * myMail.c
 *
 *  Created on: 2. des. 2010
 *      Author: Ernad
 */

#include "myMail.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include <pthread.h> // mutex
//#define DEBUG_E_MAIL

#define HELO "HELO imed.no\r\n"
#define ELHO "EHLO imed.no\r\n"
#define DATA "DATA\r\n"
#define QUIT "QUIT\r\n"
#define SMTP_CODE_OK 220
#define SMTP_CODE_OK_HELO 250
#define SMTP_CODE_OK_PSW 334
#define SMTP_CODE_OK_AUT 235
#define SMTP_CODE_OK_DATA 354
#define SMTP_CODE_OK_BYE 221

FILE *fin;
int sock;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
char receivedBuffer[BUFSIZ + 1];

char wkstr[100];
char* base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

typedef struct {
	char subject[100];
	char filePath[100];
	char errMsg[300];
	int nMaxErrMsg;
} eMail;

pthread_mutex_t mail_mutex;

int nCurrentMailIndex = 0;
eMail newMail[5];

int myMail_dispatch(const char* subject, const char* filePath, char *errMsg, int nMaxErrMsg);


void send_socket(char *s);
int read_socket();
int myMail_parseAnswer(char* result, int expectedCode, int nMaxErrMsg);

void base64_encode(const char* bytes_to_encode, unsigned int in_len, char * ret);

void send_socket(char *s) {
#ifdef DEBUG_E_MAIL
	printf("-->\n %s\n", s);
#endif
	write(sock, s, strlen(s));
}

int read_socket() {
	int receivedLength;
	receivedLength = read(sock, receivedBuffer, BUFSIZ);
	if (receivedLength > 0) {
		receivedBuffer[receivedLength] = 0;
#ifdef DEBUG_E_MAIL
		printf("<-- readLength: %d\n %s\n", receivedLength, receivedBuffer);
#endif
	} else {
		printf("read SMTP socket, nothing received?\n");
	}
	return receivedLength;
}

int myMail_parseAnswer(char* result, int expectedCode, int nMaxErrMsg) {
	int nResultCode;
	int i;
	int nLen;
	/////////////////  PARSE ANSWER  ////////////////////
	if (read_socket() <= 0) {
		strcpy(result, "Error 101");
		close(sock);
		return SMTP_SOCK_ERR;
	}
	nResultCode = atoi(receivedBuffer);
#ifdef DEBUG_E_MAIL
	printf("myMail_parseAnswer, RETURN CODE: %u\n", nResultCode);
#endif

	if (nResultCode != expectedCode) {
		nLen = strlen(receivedBuffer);
		if (nLen > nMaxErrMsg) {
			receivedBuffer[nMaxErrMsg - 1] = 0;
		}
		for (i = 0; i < nLen; i++) {
			if (receivedBuffer[i] == '<' || receivedBuffer[i] == '>')
				receivedBuffer[i] = ' ';
		}
		strcpy(result, receivedBuffer);
		close(sock);
		return SMTP_UNEXPECTED;
	}
	return SMTP_OK;
	//////////////////////////////////////////////////////
}


int myMail_appendMail(const char* subject, const char* filePath, char *errMsg, int nMaxErrMsg) {
	pthread_mutex_lock(&mail_mutex);
	strcpy(newMail[nCurrentMailIndex].subject, subject);
	strcpy(newMail[nCurrentMailIndex].filePath, filePath);
	strcpy(newMail[nCurrentMailIndex].errMsg, errMsg);
	newMail[nCurrentMailIndex].nMaxErrMsg = nMaxErrMsg;
	pthread_mutex_unlock(&mail_mutex);

	nCurrentMailIndex++;
	if (nCurrentMailIndex > 4)
		nCurrentMailIndex = 0;

	return SMTP_OK;
}

void * runMailDispatcher(void *ptr) {
	int i;
	char subject[100];
	char filePath[100];
	char errMsg[300];
	int nMaxErrMsg;

	for (i = 0; i < 5; i++)
		newMail[i].filePath[0] = 0;

	sleep(3);

	while(1) {
		for (i = 0; i < 5; i++) {
			if (newMail[i].filePath[0] == 0)
				continue;
#ifdef DEBUG_E_MAIL
			printf("!!!MAIL\n");
#endif

			pthread_mutex_lock(&mail_mutex);
			strcpy(subject, newMail[i].subject);
			strcpy(filePath, newMail[i].filePath);
			strcpy(errMsg, newMail[i].errMsg);
			nMaxErrMsg = newMail[i].nMaxErrMsg;
			newMail[i].filePath[0] = 0; // clear array element
			pthread_mutex_unlock(&mail_mutex);

			myMail_dispatch(subject, filePath, errMsg, nMaxErrMsg);
		}
		sleep(1);
	}
	return NULL;
}

int myMail_dispatch(const char* subject, const char* filePath, char *errMsg, int nMaxErrMsg) {
	char tempBuf[600];
	char tempEncode[100];
	int nResult;

	if (gwEmailStruct.enabled[0] == '0') {
		printf("E-mail disabled\n");
		return MAIL_DISABLED; // cannot send
	}
#ifdef DEBUG_E_MAIL
	printf("OK, E-mail enabled\n");
#endif
	if (gwEmailStruct.ssmtp.smtpAddress[0] == 0) {  // new 2014.02
		printf("E-mail: missing SMTP address\n");
		return MAIL_DISABLED; // cannot send
	}

	//=====Create Socket=====//
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("opening stream socket for SMTP");
		strcpy(errMsg, "Error opening socket");
		return SMTP_SOCK_ERR;
	}
#ifdef DEBUG_E_MAIL
	printf("OK, Socket created\n");

	//=====Verify host=====//


	printf("OK, URL: %s\n", gwEmailStruct.ssmtp.smtpAddress);
#endif
	hp = gethostbyname(gwEmailStruct.ssmtp.smtpAddress); // host_id);
	if (hp == (struct hostent *) 0) {
		fprintf(stderr, "%s: unknown host\n", gwEmailStruct.ssmtp.smtpAddress);
		sprintf(errMsg, "Error, unknown host: %s", gwEmailStruct.ssmtp.smtpAddress);
		return SMTP_UNKNOWN_HOST;
	}

	bzero((char*)&server, sizeof(server));
	server.sin_family = AF_INET;
	bcopy((char *) hp->h_addr, (char *) &server.sin_addr.s_addr, hp->h_length);
	//memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
	server.sin_port = htons(gwEmailStruct.ssmtp.port);


#ifdef DEBUG_E_MAIL
	printf("OK, HOST: %s, port: %u\n", inet_ntoa(server.sin_addr), gwEmailStruct.ssmtp.port);
	printf("Connect to host. Socket fd: %d\n", sock);
#endif

	if (connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
		printf("err connecting to SMTP server. Errno: %d\n", errno);
		// errno = 110 => timeout
		sprintf(errMsg, "Error connecting to host: %s, port: %u, errno: %d", gwEmailStruct.ssmtp.smtpAddress, gwEmailStruct.ssmtp.port, errno);
		return SMTP_CONNECT_ERR;
	}
#ifdef DEBUG_E_MAIL
	printf("Connected to: %s, port: %u\n", gwEmailStruct.ssmtp.smtpAddress, gwEmailStruct.ssmtp.port);
#endif
	//=====Write some data then read some =====//

	/////////////////  PARSE ANSWER  ////////////////////
	nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK, nMaxErrMsg);
	if (nResult != SMTP_OK)
		return nResult;
	//////////////////////////////////////////////////////

	if (gwEmailStruct.ssmtp.user[0] != 0) {
		send_socket(ELHO); // introduce ourselves */
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////

		send_socket("AUTH LOGIN\r\n");
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_PSW, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////

		base64_encode(gwEmailStruct.ssmtp.user, strlen(gwEmailStruct.ssmtp.user), tempEncode); //"secicno1", strlen("secicno1"), tempEncode);
		send_socket(tempEncode);//"secic1\n");
		send_socket("\r\n");
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_PSW, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////

		base64_encode(gwEmailStruct.ssmtp.pass, strlen(gwEmailStruct.ssmtp.pass), tempEncode); //base64_encode("SaSmJB9i", strlen("SaSmJB9i"), tempEncode);
		send_socket(tempEncode);//"9BX8Ksuu\n");
		send_socket("\r\n");
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_AUT, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////
	} else {
		send_socket(HELO); // introduce ourselves
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////
	}


	// denne feil kommer n�r jeg begyner med mottagere men gjelder avsender:
	// 553 5.7.1 <hello@elreg.no>: Sender address rejected: not owned by user noratel.imed.secure@elreg.eu
	sprintf(tempBuf, "MAIL FROM:%s\r\n", gwEmailStruct.ssmtp.sender);
	send_socket(tempBuf);

	/////////////////  PARSE ANSWER  ////////////////////
	nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
	if (nResult != SMTP_OK)
		return nResult;
	//////////////////////////////////////////////////////

	//553 5.7.1 <noratel2.imed.secure@elreg.eu>: Sender address rejected: not owned by user noratel.imed.secure@elreg.eu
	// 501 ernad@secic.no;: malformed address: ; may not follow ernad@secic.no
	if (gwEmailStruct.recip1[0] != 0) {
		sprintf(tempBuf, "RCPT TO:%s\r\n", gwEmailStruct.recip1); //to_id);
		send_socket(tempBuf);
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
		if (nResult != SMTP_OK) {
			return nResult;
		}
		//////////////////////////////////////////////////////
	}

	if (gwEmailStruct.recip2[0] != 0) {
		sprintf(tempBuf, "RCPT TO:%s\r\n", gwEmailStruct.recip2); //to_id);
		send_socket(tempBuf);
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////
	}

	if (gwEmailStruct.recip3[0] != 0) {
		sprintf(tempBuf, "RCPT TO:%s\r\n", gwEmailStruct.recip3); //to_id);
		send_socket(tempBuf);
		/////////////////  PARSE ANSWER  ////////////////////
		nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		//////////////////////////////////////////////////////
	}

	if (gwEmailStruct.recip4[0] != 0) {
			sprintf(tempBuf, "RCPT TO:%s\r\n", gwEmailStruct.recip4); //to_id);
			send_socket(tempBuf);
			/////////////////  PARSE ANSWER  ////////////////////
			nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
			if (nResult != SMTP_OK)
				return nResult;
			//////////////////////////////////////////////////////
		}

	if (gwEmailStruct.recip5[0] != 0) {
			sprintf(tempBuf, "RCPT TO:%s\r\n", gwEmailStruct.recip5); //to_id);
			send_socket(tempBuf);
			/////////////////  PARSE ANSWER  ////////////////////
			nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
			if (nResult != SMTP_OK)
				return nResult;
			//////////////////////////////////////////////////////
		}



	send_socket(DATA);//body to follow
	/////////////////  PARSE ANSWER  ////////////////////
	nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_DATA, nMaxErrMsg);
	if (nResult != SMTP_OK)
		return nResult;
	//////////////////////////////////////////////////////
	// To: set all destinations
	//////////////////////////////////////////////////////
	strcpy(tempBuf, "To: ");

	if(gwEmailStruct.recip1[0] != 0) {
		strcpy(&tempBuf[strlen(tempBuf)], gwEmailStruct.recip1);
		strcpy(&tempBuf[strlen(tempBuf)], "; ");
	}

	if(gwEmailStruct.recip2[0] != 0) {
		strcpy(&tempBuf[strlen(tempBuf)], gwEmailStruct.recip2);
		strcpy(&tempBuf[strlen(tempBuf)], "; ");
	}

	if(gwEmailStruct.recip3[0] != 0) {
		strcpy(&tempBuf[strlen(tempBuf)], gwEmailStruct.recip3);
		strcpy(&tempBuf[strlen(tempBuf)], "; ");
	}

	if(gwEmailStruct.recip4[0] != 0) {
		strcpy(&tempBuf[strlen(tempBuf)], gwEmailStruct.recip4);
		strcpy(&tempBuf[strlen(tempBuf)], "; ");
	}

	if(gwEmailStruct.recip5[0] != 0) {
		strcpy(&tempBuf[strlen(tempBuf)], gwEmailStruct.recip5);
		strcpy(&tempBuf[strlen(tempBuf)], "; ");
	}
	strcpy(&tempBuf[strlen(tempBuf)], "\r\n");

	send_socket(tempBuf);
	//////////////////////////////////////////////////////

	if (gwEmailStruct.usName[0] != 0) {
		sprintf(tempBuf, "From: %s <%s>\r\n", gwEmailStruct.usName, gwEmailStruct.ssmtp.sender);
		//sprintf(tempBuf, "From: \"%s\" %s\n", gwEmailStruct.usName, gwEmailStruct.ssmtp.sender);
		send_socket(tempBuf);
	}/* else {
		sprintf(tempBuf, "From: \"%s\" %s\n", usName, gwEmailStruct->ssmtp.sender);
	}*/

	////////////////////////////////////////////////////
		// NEW FROM version 2.08 (mail error with norwegian chars
		///////////////////////////////////////

		sprintf(tempBuf, "Content-Type: text/plain; charset=UTF-8\r\nContent-Transfer-Encoding: 8bit\r\n");
		send_socket(tempBuf);

		/////////////////  PARSE ANSWER  ////////////////////
		/*nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
		if (nResult != SMTP_OK)
			return nResult;
		printf("PROSAO\r\n");*/
		/////////////////////////////////////////


	sprintf(tempBuf, "Subject: %s\r\n\r\n", subject);
	send_socket(tempBuf);
	// read_socket(); // server is not answering


	fin = fopen(filePath, "r"); //file_id, "r"); // open file
	if (fin <= 0)
		return SMTP_FILE_ERR;

	while (1) {
		if (fgets(wkstr, 100, fin) == NULL)
			break; // exit on EOF

		send_socket(wkstr);
	}
	fclose(fin); // close file

	//send_socket(fin); //send file
	send_socket("\r\n.\r\n");
	/////////////////  PARSE ANSWER  ////////////////////
	nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_HELO, nMaxErrMsg);
	if (nResult != SMTP_OK)
		return nResult;
	//////////////////////////////////////////////////////

	send_socket(QUIT); // quit
	/////////////////  PARSE ANSWER  ////////////////////
	nResult = myMail_parseAnswer(errMsg, SMTP_CODE_OK_BYE, nMaxErrMsg);
	if (nResult != SMTP_OK)
		return nResult;
	//////////////////////////////////////////////////////

	///=====Close socket and finish=====//
	close(sock);
	printf("Mail Sent!\r\n");
	return SMTP_OK;
}

void base64_encode(const char * bytes_to_encode, unsigned int in_len, char * ret) {
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++) {
        *ret = base64_chars[char_array_4[i]];
        ret++;
      }
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++) {
      *ret = base64_chars[char_array_4[j]];
      ret ++;
    }

    while((i++ < 3)) {
      *ret = '=';
      ret++;
    }

  }
*ret = 0;
 // return ret;

}

void myMail_init(GW_EMAIL* gwEmail) {
	memcpy((char*)&gwEmailStruct, (char*)gwEmail, sizeof(gwEmailStruct));
}


