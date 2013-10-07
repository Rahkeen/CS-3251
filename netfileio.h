#pragma once
#include "listdir.h"

struct File {
    FileInfo info;
    char *contents;
    int size;
}

typedef struct File File;
