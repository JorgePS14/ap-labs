#include <stdio.h>

int mystrlen(char*);
char* mystradd(char*, char*);
int mystrfind(char*, char*);

int main(int argc, char** argv) {
    if(argc < 4) {
        printf("Please pass the 3 obligatory arguments.\n");
        return -1;
    }

    int len = mystrlen(argv[1]);
    char* added = mystradd(argv[1], argv[2]);
    char* isSub = mystrfind(added, argv[3]) ? "Yes, it is." : "No, it is not.";
    printf("Initial length:\t%d\n", len);
    printf("New String:\t%s\n", added);
    printf("Is it a substring?:\t%s\n", isSub);
    return 0;
}
