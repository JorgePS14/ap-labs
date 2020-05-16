#include <stdlib.h>
#include "logger.h"
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define LEN sizeof(struct inotify_event) * 1024

int main(int argc, char** argv){
    if(argc < 2) {
        errorf("Please specify which directory you want to monitor.\n");
        infof("In order to run this program correctly, you must use the correct format: sudo ./monitor <directory_to_monitor>\n");
        return -1;
    }

    struct inotify_event *e;
    char *buff;
    int f, r;

    f = inotify_init1(O_NONBLOCK);
    inotify_add_watch(f, argv[1], IN_ALL_EVENTS);

    char *b = (char*)malloc(LEN);

    while(1) {
        r = read(f, b, LEN);
        buff = b;

        e = (struct inotify_event*)buff;

        for (buff = b; buff < b + r;) {
            e = (struct inotify_event*)buff;

            if(e -> mask & IN_CREATE)
                warnf("A subdirectory/file has been created.\n");
            else if(e -> mask & IN_OPEN)
                infof("A subdirectory/file has been opened.\n");
            else if(e -> mask & IN_MODIFY)
                warnf("A subdirectory/file has been modified.\n");
            else if(e -> mask & IN_ACCESS)
                infof("A subdirectory/file has been accessed.\n");
            else if(e -> mask & IN_DELETE)
                errorf("A subdirectory/file has been deleted.\n");

            buff += sizeof(struct inotify_event) + e->len;
        }
    }

    return 0;
}
