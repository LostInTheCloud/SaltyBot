#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "libtwirc.h"

int main()
{
    char* logpath;
    logpath = malloc(7+8+1+3+1);
    snprintf(logpath, 7+8+1+3+1, "./logs/%lx.log", time(NULL));
    FILE* log = fopen(logpath, "w");

    printf("%s\nlog=%x\n", logpath, log);

    fprintf(log, "Starting up\n");
    fflush(log);

    fclose(log);

    return 0;
}