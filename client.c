/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Stephen Pardue	
* PROJECT:	CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:	Network Client Code
*
*////////////////////////////////////////////////////////////

#include "sync.h"

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




    char buff;
    char input;
    
    while(1){
        
        input = getc(stdin);
        ssize_t numBytes = send(clientSock, &input, sizeof(char), 0);
        if (numBytes < 0) {
            fprintf(stderr, "send() failed bro\n");
            exit(1);
        }

            

        memset(rcvBuf, 0, RCVBUFSIZE);
        /* Receive and print response from the server */
        if (recv(clientSock, &buff, sizeof(char), 0) < 0){
            fprintf(stderr, "could not recieve data :(\n");
            exit(1);
        }
        if (buff == 's'){
            break;
        }
    }


    return 0;
}

