#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int mystrlen(char *str){
    char* len;
    for(len = str; *len; len++);
    return len-str;
}

char *mystradd(char *origin, char *addition){
    char * addedString = malloc(sizeof(char) * mystrlen(origin) + mystrlen(addition));
    strcpy(addedString, origin);
    strcpy(addedString + mystrlen(origin), addition);
    return addedString;
}

int mystrfind(char *origin, char *substr){
    char* x = origin;
    char* y = substr;

    while(x < origin + mystrlen(origin)) {
        if(*x == *y) {
            if(y == substr + mystrlen(substr)-1) {
                return 1;
            }

            x += 1;
            y += 1;
        } else {
            x += 1;
            y = substr;
        }
    }

    return 0;   
}
