#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(char* fileName) {
    int file = open(fileName, O_RDONLY);

    if(file == -1) {
        printf("Problem opening file.\n");
        return;
    }

    off_t offset = lseek(file, (size_t) 0, SEEK_CUR);
    off_t size = lseek(file, (size_t) 0, SEEK_END); 

    lseek(file, offset, SEEK_SET);

    char* buffer = (char*) malloc(sizeof(char) * size);
    read(file, buffer, size);
    write(1, buffer, size);

    close(file);

    printf("\n");

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[]) {

    if(argc < 2) {
        printf("Please specify the file you want to print on console.\n");
    }

    filecopy(argv[1]);

    return 0;
}