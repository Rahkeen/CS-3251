/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:   Stephen Pardue	
* PROJECT:	CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:	Network Server Code
*
*////////////////////////////////////////////////////////////

#include "sync.h"



void *handleClient(void *clientSocket) {

    char buff;
    char toSend;
    int clientSock = *((int *)clientSocket);
    
    while (1) {

        if (recv(clientSock, &buff, sizeof(char), 0) < 0){
            fprintf(stderr, "Yo, can't recv dat data bro\n");
            exit(1);
        }

       

        if (buff == 'x') {
            toSend = 's';
        } else {
            toSend = buff;
        }
        

        /* Return md_value to client */
        //printf("md_len: %d, %s, %s\n", md_len, md_value, nameBuf);
        if (send(clientSock, &toSend, sizeof(char), 0) < 0){
            break;

        }
        if (toSend == 's') {
            break;
        }
        printf("%c", toSend);
        
    }
    close(clientSock);
    free(clientSocket);
    pthread_exit(NULL);


}

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    //int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort = 1337;		/* Server port */
    unsigned int clntLen = sizeof(changeServAddr);			/* Length of address data struct */

    char nameBuf[BUFSIZE];			/* Buff to store name from client */
    unsigned char md_value[EVP_MAX_MD_SIZE];	/* Buff to store change result */
    EVP_MD_CTX *mdctx;				/* Digest data structure declaration */
    const EVP_MD *md;				/* Digest data structure declaration */
    int md_len;					/* Digest data structure size tracking */


    /* Create new TCP Socket for incoming requests*/
    serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock < 0) {
        fprintf(stderr, "could not create server socket\n");
        exit(1);
    }

    int on = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

   

    /* Construct local address structure*/
    memset(&changeServAddr, 0, clntLen);
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(changeServPort);
    
    /* Bind to local address structure */
    if (bind(serverSock, (struct sockaddr *) &changeServAddr, clntLen) < 0){
        fprintf(stderr, "could not bind to local address");
        exit(1);
    }

    /* Listen for incoming connections */
    if (listen(serverSock, MAXPENDING) < 0) {
        fprintf(stderr, "listen() failed\n");
        exit(1);
    }

    int * clientSock;
    /* Loop server forever*/
    while(1)
    {

        /* Accept incoming connection */
        memset(&changeClntAddr, 0, clntLen);
        clientSock = (int *) malloc(sizeof(int));
        *clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen);
        if (*clientSock < 0){
            continue;
            //fprintf(stderr, "error accepting the client socket\n");
            //exit(1);
        }
        pthread_t pth;
        pthread_create((pthread_t *) malloc(sizeof(pthread_t)), NULL, handleClient,clientSock);
 


        
    }
}


                                                       

