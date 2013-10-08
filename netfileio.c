#include <sys/stat.h>
#include "sync.h"
#include "netfileio.h"


int sendFile(int clientSocket, int serverSocket, FileInfo *fi) {
    struct stat st;
    stat(fi->name, &st);
    
    
    
    
    
    
    
}

File * int pullFile(int serverSocket, FileInfo *fi) {
    File *ret;
    
    int size = -1;
    //send over the fileinfo to be recieved
    if (send(serverSocket, fi, sizeof(FileInfo), 0) < 0 ) {
        fprintf(stderr, "Error sending the fileinfo in pullFile");
        return NULL;
    }
    
    //get the confirmation via size
    if (recv(serverSocket, &size, sizeof(int), 0) < 0){
        fprintf(stderr, "Error recieving the size in pullFile\n");
        return NULL;
    }
    
    if (size < 0) {
        fprintf(stderr, "File not found on the server\n");
        return NULL;   
    }
    
    ret = calloc(1, sizeof(File));
    if (ret == NULL) {
        fprintf(stderr, "Error allocating memory in pullFile\n");
        return NULL;
    }
    
    ret->contents = calloc(size, sizeof(char));
    if (ret->contents == NULL) {
        fprintf(stderr, "Error allocating memory for the contents in pullFile\n");
        return NULL;
    }
    
    ret->size = size;
    
    //get the contents of the file   
    if (recv(serverSocket, ret->contents, sizeof(char)*size, 0) < 0) {
        fprintf(stderr, "Could not recieve the contents of the file in pullFile\n");
    }
 
    //copy over the info   
    memcpy(ret->info, fi, sizeof(FileInfo));
    
    return ret;
}
