#include "main.h"

int main()
{
    int err;

    char* logpath;
    logpath = malloc(7+8+1+3+1);
    ASSERT(logpath);

    err = snprintf(logpath, 7+8+1+3+1, "./logs/%lx.log", time(NULL));
    ASSERT(err==7+8+1+3);
    struct stat st = {0};
    if(stat("./logs", &st) == -1)
        mkdir("./logs", 0777);

    FILE* log = fopen(logpath, "w");
    ASSERT(log);

    fprintf(log, "Starting up\n");
    fflush(log);

    fclose(log);

    return 0;
}