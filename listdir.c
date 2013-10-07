/*
Listdir.c
Provides methods that returns a link list of all the files in a directory
with the nodes of that linked list containing the name of the file
and its checksum.
It also provides a method that subtracts the files
from one list from another based on the checksum

By Stephen Pardue - 10/4/2013

*/

#include "listdir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FileInfo * getFileInfo(char * name) {
    FILE *fp;
    FileInfo *fi;
    
    MD5_CTX c;
    char buff[MAX_BUFF];
    ssize_t bytes;
    
    fp = fopen(name, "r");
    if (! fp) {
        return NULL;
    }
    
    fi = (FileInfo *) malloc(sizeof(FileInfo));
    if (fi == NULL) {
        return NULL;
    }
    
    //copy the name
    memset(fi, 0, sizeof(FileInfo));
    memcpy(fi->name, name, strlen(name)+1);
    
    //calculate the checksum
    MD5_Init(&c);
    bytes = fread(buff, sizeof(char), MAX_BUFF, fp);
    while (bytes > 0){
        MD5_Update(&c, buff, bytes);
        bytes = fread(buff, sizeof(char), MAX_BUFF, fp);
    }
    MD5_Final(fi->checksum, &c);
    
    return fi;
}

int listDirectory(DirectoryInfo *dirInfo, char *dirName){
    DIR *d;
    FileInfo *buff;
    struct dirent *dir;
    
    LIST_INIT(&dirInfo->head);
    dirInfo->length = 0;

    printf(dirName);
    d = opendir(dirName);
    chdir(dirName);
    if (d) {
        while ((dir = readdir(d)) != NULL){
            if (! (
                (dir->d_name[0] == '.' && dir->d_name[1] == '\0') || 
                (dir->d_name[0] == '.' && dir->d_name[1] == '.' && dir->d_name[2] == '\0'))){
                buff = getFileInfo(dir->d_name);
                LIST_INSERT_HEAD(&dirInfo->head, buff, FileInfoEntry);
                dirInfo->length++;
            }
        }
        closedir(d);
    } else {
        return -1;
    }
    chdir("..");
}

void freeDirectoryInfo(DirectoryInfo *dirInfo) {
    FileInfo *file;
    
    while (! LIST_EMPTY(&(dirInfo->head)) ){
        file = LIST_FIRST(&(dirInfo->head));
        LIST_REMOVE(file, FileInfoEntry);
        free(file);
    }
}

int FileInfoEqualChecksum(FileInfo *a, FileInfo *b){
    int i;
    
    for (i = 0; i < MD5_DIGEST_LENGTH; i++){
        if (a->checksum[i] != b->checksum[i]){
            return 0;
        }
    }
    return 1;
}


DirectoryInfo *DirectoryInfoMinus(DirectoryInfo *a, DirectoryInfo *b) {  
    FileInfo *fileA, *fileB;
    FileInfo *buff;
    int isIn;
    
 
    DirectoryInfo *di = (DirectoryInfo *) malloc(sizeof(DirectoryInfo));
    memset(di, 0, sizeof(DirectoryInfo));
    LIST_INIT(&di->head);
    di->length = 0;
    
    LIST_FOREACH(fileA, &(a->head), FileInfoEntry){
        isIn = 0;
        LIST_FOREACH(fileB, &(b->head), FileInfoEntry){
            if (FileInfoEqualChecksum(fileA, fileB)){
                isIn = 1;
                break;
            }           
        }
        if (! isIn ) {
            buff = (FileInfo *) malloc(sizeof(FileInfo));
            memcpy(buff, fileA, sizeof(FileInfo));
            
            LIST_INSERT_HEAD(&di->head, buff, FileInfoEntry);
            di->length++;
        }
    }
    return di;
}

void printDirectoryInfo(DirectoryInfo *di){
    FileInfo *file;
    int n;
    
    LIST_FOREACH(file, &(di->head), FileInfoEntry){
        printf("%s, ", file->name);
        for (n = 0; n < MD5_DIGEST_LENGTH; n++){
            printf("%02x", (unsigned char) file->checksum[n]);
        }
        printf("\n");
    }
}



/*
int main() {
    DirectoryInfo dirInfo, current, parent;
    listDirectory(&current, "./theProject");
    listDirectory(&parent, "./clientFiles");
    
    int n;
    FileInfo *file;
    FileInfo *back;
    
    DirectoryInfo *pt = &dirInfo;
    
    
    pt = DirectoryInfoMinus(&current, &parent);
    
    printDirectoryInfo(pt);
    
    freeDirectoryInfo(pt);
    free(pt);
    freeDirectoryInfo(&current);
    freeDirectoryInfo(&parent);
}*/

