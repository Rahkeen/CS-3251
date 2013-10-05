/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:   Stephen Pardue	
* PROJECT:	CS 3251 Project 1 - Professor Traynor
* DESCRIPTION:	Network Server Code
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */
#include <openssl/evp.h>  /* for OpenSSL EVP digest libraries/SHA256 */

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/

#define MAXPENDING 5

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
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

    /* Loop server forever*/
    while(1)
    {

        /* Accept incoming connection */
        memset(&changeClntAddr, 0, clntLen);
        clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen);
        if (clientSock < 0){
            fprintf(stderr, "error accepting the client socket\n");
            exit(1);
        }
 


        /* Extract Your Name from the packet, store in nameBuf */
        memset(nameBuf, 0, BUFSIZE);
        if (recv(clientSock, nameBuf, BUFSIZE - 1, 0) < 0){
            fprintf(stderr, "Yo, can't recv dat data bro\n");
            exit(1);
        }

       


        memset(md_value, 0, md_len);

        /* Run this and return the final value in md_value to client */
        /* Takes the client name and changes it */
        /* Students should NOT touch this code */
        OpenSSL_add_all_digests();
        md = EVP_get_digestbyname("SHA256");
        mdctx = EVP_MD_CTX_create();
        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, nameBuf, strlen(nameBuf));
        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
        EVP_MD_CTX_destroy(mdctx);

        /* Return md_value to client */
        //printf("md_len: %d, %s, %s\n", md_len, md_value, nameBuf);
        if (send(clientSock, md_value, EVP_MAX_MD_SIZE, 0) < 0){
            fprintf(stderr, "could not send back name");
            exit(1);
        } 
    }
}


                                                       

