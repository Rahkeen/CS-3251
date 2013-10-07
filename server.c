/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:   Stephen Pardue	
* PROJECT:	CS 3251 Project 2 - Professor Traynor
* DESCRIPTION:	Network Server Code
*
*////////////////////////////////////////////////////////////

#include "sync.h"

int handle_list(int clientSock){
	/*
	DirectoryInfo dInfo;
	memset(&dInfo, 0, sizeof(dInfo));
	listDirectory(&dInfo, "./theProject");
	ssize_t numBytes = send(clientSock, &dInfo, sizeof(dInfo), 0);
	if(numBytes < 0)
	{
		printf("Failed to send()");
		return -1;
	}
>>>>>>> bb6c6392912b3d5e4ca79c7e344e7615168f4e91
    return 0;
	*/
	

	printf("Hi there buddy");
	DirectoryInfo *dInfo;
	FileInfo *file;
	char* file_message;
	memset(dInfo, 0, sizeof(DirectoryInfo));
	listDirectory(dInfo, "./theProject");     // populates the dInfo struct

	
	if(dInfo->length == 0)
	{
		file_message = "No Files In Directory\n";
		send(clientSock, file_message, strlen(file_message), 0);
		return 0;
	}

	LIST_FOREACH(file, &(dInfo->head), FileInfoEntry)
	{
		if(asprintf(&file_message, "%s\n", file->name) < 0)
		{
			printf("Something went wrong with populating the file names\n");
		}
	}

	printf("Sending the List!");
	send(clientSock, file_message, strlen(file_message), 0);
	
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


void *handleClient(void *clientSocket) {

    char buff;
    char toSend;
    int clientSock = *((int *)clientSocket);
    
    while (1) {

        if (recv(clientSock, &buff, sizeof(char), 0) < 0){
            fprintf(stderr, "Yo, can't recv dat data bro\n");
            exit(1);
        }

        switch(buff) {
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

       //toSend = 'a';

        

        /* Return md_value to client */
        //printf("md_len: %d, %s, %s\n", md_len, md_value, nameBuf);
        //if (send(clientSock, &toSend, sizeof(char), 0) < 0){
        //    break;
        //}
        
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
    unsigned short changeServPort = SERVER_PORT;		/* Server port */
    unsigned int clntLen = sizeof(changeServAddr);			/* Length of address data struct */

    //system("mkdir theProject"); ---- we can make a new directory and set it to that directory, or we can add choice.

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
    printf("Listening for incoming connections.\n");
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
        printf("Handling a new client.\n");
        pthread_create((pthread_t *) malloc(sizeof(pthread_t)), NULL, handleClient,clientSock);
    }
}


                                                       

