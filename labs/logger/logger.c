#include <stdarg.h>
#include <stdio.h>

#define RESET       0
#define BOLD        1
#define DIM         2
#define ITALICS     3
#define UNDERLINE   4
#define BLINK       5
#define REVERSE     7
#define HIDDEN      8
#define CROSSED     9

#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

void colour(int attr, int text, int background) {
    char cmd[13];
    sprintf(cmd, "%c[%d;%d;%dm", 0x1b, attr, text + 30, background + 40);
    printf("%s", cmd);
}

void infof(const char *f, ...) {
    va_list arg;
    va_start(arg, f);

    colour(UNDERLINE, CYAN, BLACK);
    vfprintf(stdout, f, arg);

    va_end(arg);
    colour(RESET, WHITE, BLACK);
}

void warnf(const char *f, ...) {
    va_list arg;
    va_start(arg, f);

    colour(BOLD, YELLOW, BLACK);
    vfprintf(stdout, f, arg);
    
    va_end(arg);
    colour(RESET, WHITE, BLACK);
}

void errorf(const char *f, ...) {
    va_list arg;
    va_start(arg, f);

    colour(BOLD, RED, BLACK);
    vfprintf(stdout, f, arg);

    va_end(arg);
    colour(RESET, WHITE, BLACK);
}

void panicf(const char *f, ...) {
    va_list arg;
    va_start(arg, f);
    
    colour(BOLD, WHITE, RED);
    vfprintf(stdout, f, arg);
    
    va_end(arg);
    colour(RESET, WHITE, BLACK);
}