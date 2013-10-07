/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Stephen Pardue	
* PROJECT:	CS 3251 Project 2 - Professor Traynor
* DESCRIPTION:	Network Client Code
*
*////////////////////////////////////////////////////////////

#include "sync.h"

#define PROMPT "(GTMyMusic) "

char * commands = "Enter one of these characters for a command:\tL, D, P, E\n"
"(L)ist:\tlist the files on the server\n"
"(D)iff:\tperform a diff of the files on the client vs the server\n"
"(P)ull:\tget files from the server that this client doesn't have\n"
"L(E)ave:\tterminate the session\n";



DirectoryInfo *recvDirectoryInfo(int clientSock){
    int i;
	int len;
	DirectoryInfo *recDir = calloc(1, sizeof(DirectoryInfo));
	FileInfo *liBuff;
	
	memset(&len, 0, sizeof(len));
	//recv the length
	recv(clientSock, &len, sizeof(int), 0);
	
	recDir->length = len;
	
	//recv the list
	for (i = 0; i < len; i++) {
	    liBuff = calloc(1, sizeof(FileInfo));
	    recv(clientSock, liBuff, sizeof(FileInfo), 0);
	    LIST_INSERT_HEAD(&recDir->head, liBuff, FileInfoEntry);
	    //printf("%s, %s\n", liBuff.name, liBuff.checksum);
	}
	
	return recDir;
}

int handle_list(int clientSock){
	/*
	printf("recieving directory...");
	DirectoryInfo recvDir;
	unsigned char
	memset(&recvDir, 0, sizeof(recvDir));
	memset(&rcvBuf, 0, sizeof(DirectoryInfo));
	if(recv(clientSock, &rcvBuf, sizeof(recvDir), 0) < 0)
	{
		printf("recv() failed breh"); 
	}
	memcpy(&recvDir, rcvBuf, sizeof(recvDir));
	printf("%d", sizeof(recvDir));
	//printDirectoryInfo(&recvDir);
	*/
	
	DirectoryInfo *dInfo = recvDirectoryInfo(clientSock);
	printDirectoryInfo(dInfo);
	freeDirectoryInfo(dInfo);
	free(dInfo);
	
	/*
	char recvBuffer[RCVBUFSIZE];
	int recv_message_size = 0;

	char *buffer = calloc(1, sizeof(char) * RCVBUFSIZE);
	int total_message_size = 0;
	int message_cap = RCVBUFSIZE;
	
	if(recv_message_size = recv(clientSock, recvBuffer, RCVBUFSIZE, 0) < 0)
	{
		printf("There was an error with recv()");
		free(buffer);
		return -1;
	}
	else
	{
		while(recv_message_size > 0)
		{
			int i;
			for(i = 0; i < recv_message_size; i++)
			{
				buffer[total_message_size + i] = recvBuffer[i];
			}

			memset(recvBuffer, 0, RCVBUFSIZE);
			total_message_size += recv_message_size;

			if(total_message_size >= message_cap)
			{
				message_cap *= 2;
				buffer = realloc(buffer, message_cap);
			}
			
		}

		printf("LIST \n%s", buffer);
	}
	*/

}

int handle_diff(int clientSock){
    return 0;
}

int handle_pull(int clientSock){
    return 0;
}

int handle_exit(int clientSock) {
    return close(clientSock);
}

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* The server address */
    
    int i;			    /* Counter Value */

    /* Get the Student Name from the command line */
    /*if (argc != 2) 
    {
	    printf("Incorrect input format. The correct format is:\n\tnameChanger your_name\n");
	    exit(1);
    }
    studentName = argv[1];
    memset(&sndBuf, 0, RCVBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    memcpy(sndBuf, studentName, strlen(studentName));
    */


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
       // printf("%s", PROMPT);
        input = getc(stdin);
        ssize_t numBytes = send(clientSock, &input, sizeof(char), 0);
        if (numBytes < 0) {
            fprintf(stderr, "send() failed bro\n");
            exit(1);
        }
        

	if(input == 'L')
	{
            handle_list(clientSock);
	}
	else if(input == 'D')
	{
            handle_diff(clientSock);
	}
	else if(input == 'P')
	{
            handle_pull(clientSock);
	}
	else if(input == 'E')
	{
            handle_exit(clientSock);
	    break;
	}

    }
    return 0;
}

