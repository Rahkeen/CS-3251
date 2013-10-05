/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Stephen Pardue	
* PROJECT:	CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:	Network Client Code
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>	    /* for OpenSSL EVP digest libraries/SHA256 */

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define MDLEN 32

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1337

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* The server address */

    char *studentName;		    /* Your Name */

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int i;			    /* Counter Value */

    /* Get the Student Name from the command line */
    if (argc != 2) 
    {
	printf("Incorrect input format. The correct format is:\n\tnameChanger your_name\n");
	exit(1);
    }
    studentName = argv[1];
    memset(&sndBuf, 0, RCVBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    memcpy(sndBuf, studentName, strlen(studentName));


    /* Create a new TCP socket*/
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSock < 0) {
        fprintf(stderr, "Yo bro, clientsock wasn't made\n");
        exit(1);
    }


    /* Construct the server address structure */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);


    /* Establish connecction to the server */
    if (connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        fprintf(stderr, "Could not establish a connection\n");
        exit(1);
    }

    
    /* Send the string to the server */
    ssize_t numBytes = send(clientSock, sndBuf, strlen(sndBuf), 0);
    if (numBytes < 0) {
        fprintf(stderr, "send() failed bro\n");
        exit(1);
    }

        

    memset(rcvBuf, 0, RCVBUFSIZE);
    /* Receive and print response from the server */
    if (recv(clientSock, rcvBuf, RCVBUFSIZE-1, 0) < 0){
        fprintf(stderr, "could not recieve data :(\n");
        exit(1);
    }

    //printf("%s\n", studentName);
    printf("Transformed input is: ");
    for(i = 0; i < MDLEN; i++) printf("%02x", (unsigned char) rcvBuf[i]);
    printf("\n");

    return 0;
}

