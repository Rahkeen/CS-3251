#include "netfileio.h"



void recvFile(int socket, char *directory){
    FILE *file;
    FileInfo f;
    int size = 0;
    char *buff;
    
    chdir(directory);
    if (recv(socket, &f, sizeof(FileInfo), 0) < 0){
        fprintf(stderr, "Error recieving the FileInfo in recvFile\n");
        exit(-1);
    }
    if (recv(socket, &size, sizeof(int), 0) < 0){
        fprintf(stderr, "Error recieving the size of the file in recvFile\n");
        exit(-1);
    }
    buff = calloc(size, sizeof(char));
    if (buff == NULL){
        fprintf(stderr, "Error allocating memory for the file in recvFile\n");
        exit(-1);
    }
    
    if (recv(socket, buff, size*sizeof(char), 0) < 0){
        fprintf(stderr, "Error recieving the file in recvFile\n");
        exit(-1);
    }
    
    file = fopen(f.name, "w");
    if (file == NULL){
        fprintf(stderr, "could not open the file for writing... in recvFile\n");
        exit(-1);
    }
    
    if (fwrite(buff, sizeof(char), size, file) != sizeof(char)*size){
        fprintf(stderr, "error writing to file in recvFile\n");
        exit(-1);
    }
    
    fclose(file);
    free(buff);
    chdir("..");
}


void sendFile(int socket, char *directory, FileInfo *file) {
    chdir(directory);
    struct stat st;
    FILE * f;
    char * buff;
        
    stat(file->name, &st);
    send(socket, file, sizeof(FileInfo), 0);
    
    //send the size
    send(socket, &st.st_size, sizeof(int), 0);
    
    
    buff = calloc(st.st_size, sizeof(char));
    if (! buff) {
        fprintf(stderr, "memory allocation error in sendFile\n");
        exit(1);
    }
    
    f = fopen(file->name, "r");
    if (f == NULL){
        fprintf(stderr, "error opening the file for reading in sendFile\n");
        exit(1);
    }
    if (fread(buff, sizeof(char), st.st_size, f) != sizeof(char)*st.st_size){
        fprintf(stderr, "error reading the file in sendFile\n");
        exit(1);
    }
    
    if (send(socket, buff, sizeof(char)*st.st_size, 0) != sizeof(char)*st.st_size){
        fprintf(stderr, "error sending the buff in sendFile\n");
        exit(1);
    }
    
    fclose(f);
    free(buff);
    chdir("..");
}


