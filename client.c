/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Stephen Pardue	
* PROJECT:	CS 3251 Project 2 - Professor Traynor
* DESCRIPTION:	Network Client Code
*
*////////////////////////////////////////////////////////////

#include "sync.h"

char * commands = "Enter one of these characters for a command:\tL, D, P, E\n"
"(L)ist:\tlist the files on the server\n"
"(D)iff:\tperform a diff of the files on the client vs the server\n"
"(P)ull:\tget files from the server that this client doesn't have\n"
"L(E)ave:\tterminate the session\n";



int handle_list(int clientSock){
    return 0;
}

int handle_diff(int clientSock){
    return 0;
}

int handle_pull(int clientSock){
    return 0;
}

int handle_exit(int clientSock) {
    return 0;
}

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
    printf("Connected to the server.\n");
    printf(commands);




    char buff;
    char input;
    
    while(1){
        
        input = getc(stdin);
        ssize_t numBytes = send(clientSock, &input, sizeof(char), 0);
        if (numBytes < 0) {
            fprintf(stderr, "send() failed bro\n");
            exit(1);
        }
        
        switch(input) {
            case 'L':
                handle_list(clientSock);
                break;
            case 'D':
                handle_diff(clientSock);
                break;
            case 'P':
                handle_pull(clientSock);
                break;
            case 'E':
                handle_exit(clientSock);
                break;
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

