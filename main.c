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

    setup();

    fprintf(log, "Shutting down\n");
    fflush(log);
    fclose(log);

    return 0;
}

void setup()
{
    twirc_state_t *s = twirc_init();

    twirc_callbacks_t *cbs = twirc_get_callbacks(s);
    cbs->welcome = handle_all;
    cbs->privmsg = handle_all;

}

void handle_all(twirc_state_t *s, twirc_event_t *evt)
{
    fprintf(stdout, "> %s\n", evt->raw);
}