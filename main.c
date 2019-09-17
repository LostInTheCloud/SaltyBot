#include "main.h"

int main()
{
    LOGFILE = stdout; /*
    start_logger(); //*/

    fprintf(LOGFILE, "Starting up\n");

    setup();

    twirc_loop(s);

    fprintf(LOGFILE, "Shutting down\n");
    fflush(LOGFILE);
    fclose(LOGFILE);
    twirc_kill(s);
    return 0;
}

void start_logger()
{
    char* logpath;
    logpath = malloc(7+8+1+3+1);
    ASSERT(logpath);

    err = snprintf(logpath, 7+8+1+3+1, "./logs/%lx.log", time(NULL));
    ASSERT(err==7+8+1+3);
    struct stat st = {0};
    if(stat("./logs", &st) == -1)
        mkdir("./logs", 0777);

    LOGFILE = fopen(logpath, "w");
    ASSERT(LOGFILE);
}

void setup()
{
    s = twirc_init();

    twirc_callbacks_t *cbs = twirc_get_callbacks(s);

//    cbs->connect         = handle_everything;
    cbs->welcome         = handle_welcome;
    cbs->globaluserstate = handle_everything;
    cbs->capack          = handle_everything;
    cbs->ping            = handle_everything;
    cbs->join            = handle_join;
    cbs->part            = handle_everything;
    cbs->mode            = handle_everything;
    cbs->names           = handle_everything;
    cbs->privmsg         = handle_message;
    cbs->whisper         = handle_everything;
    cbs->action          = handle_everything;
    cbs->notice          = handle_everything;
    cbs->roomstate       = handle_everything;
    cbs->usernotice      = handle_everything;
    cbs->userstate       = handle_everything;
    cbs->clearchat       = handle_everything;
    cbs->clearmsg        = handle_everything;
    cbs->hosttarget      = handle_everything;
    cbs->reconnect       = handle_everything;
    cbs->invalidcmd      = handle_everything;
    cbs->other           = handle_everything;
//    cbs->disconnect      = handle_everything;
    cbs->outbound        = handle_everything;

    err = twirc_connect(s, "irc.chat.twitch.tv", "6667", NICK, TOKEN);
    LOG("logging into twitch.tv");
    ASSERT(err==0)
    LOG("login successful!");
}

void handle_everything(twirc_state_t *s, twirc_event_t *evt)
{
    fprintf(LOGFILE, "> %s\n", evt->raw);
}

void handle_welcome(twirc_state_t *x, twirc_event_t *evt)
{
    fprintf(LOGFILE, "> %s\n", evt->raw);

    err = twirc_cmd_join(s, "#grafseegurke");
    LOG("joining grafseegurke");
    printf("%d\n", err);
}

void handle_join(twirc_state_t *s, twirc_event_t *evt)
{
    fprintf(LOGFILE, "> %s\n", evt->raw);
    // Check if 'origin' (the name of the user who triggered the event)
    // was us and the channel is the one we are interested in
    if (strcmp(evt->channel, "#grafseegurke") == 0 && strcmp(evt->origin, NICK) == 0)
    {
        twirc_cmd_privmsg(s, "#grafseegurke", "Hello world!");
    }
}

void handle_message(twirc_state_t *s, twirc_event_t *evt)
{
    if(strcmp(evt->message, "!bwuah")==0)
    {
        sleep(1);
        LOG("its a bwuah! sending bwuah");
        err = twirc_cmd_privmsg(s, "#grafseegurke", "BWUAAAAH!");
        LOGINT(err);
    }
    if(strcmp(evt->message, "!pog")==0)
    {
        sleep(1);
        LOG("its a pog! sending grafsePog");
        err = twirc_cmd_privmsg(s, "#grafseegurke", "grafsePog!");
        LOGINT(err);
    }
}