#include <stdio.h>
#include "logger.h"

int main(){

    // default logging
    infof("INFO Message %d\n", 1);
    warnf("WARN Message %d\n", 2);
    errorf("ERROR Message %d\n", 2);
    panicf("PANIC! %d\n", 3);

    // stdout logging
    logger_init("stdout");
    infof("INFO Message %d\n", 1);
    warnf("WARN Message %d\n", 2);
    errorf("ERROR Message %d\n", 2);
    panicf("PANIC! %d\n", 3);

    // syslog logging
    logger_init("syslog");
    infof("INFO Message %d\n", 1);
    warnf("WARN Message %d\n", 2);
    errorf("ERROR Message %d\n", 2);
    panicf("PANIC! %d\n", 3);

    return 0;
}
