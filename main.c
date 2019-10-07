#include "main.h"

int main()
{
    LOGFILE = stdout;
    start_logger(); //*/

    fprintf(LOGFILE, "Starting up\n");
    fprintf(LOGFILE, "Balance=%d\nTimeToBet=%d\nQuotient=%d\nNick=%s\nPlaystyle=%s\n", BALANCE, TIME_TO_BET, QUOTIENT, NICK, 1 PLAYSTYLE 0 ?"Spicy":"Salty");
    FLUSH

    setup();

    twirc_loop(s);

    fprintf(LOGFILE, "Shutting down\n");
    fflush(LOGFILE);
    fclose(LOGFILE);
    twirc_kill(s);
    return 0;
}

int fetch_amount(char* msg)
{
    int amount;
    err = sscanf(msg, "%*[^0123456789]%d%*[^0123456789]", &amount);
    ASSERT(err == 1);
    return amount;
}

int fetch_balance(char* msg)
{
    int amount;
    err = sscanf(msg, "%*[^.]%*[^0123456789]%d", &amount);
    ASSERT(err == 1);
    return amount;
}

void start_logger()
{
    /*
     char* logpath;
    logpath = malloc(7 + 8 + 1 + 3 + 1);
    ASSERT(logpath);

    err = snprintf(logpath, 7 + 8 + 1 + 3 + 1, "./logs/%lx.log", time(NULL));
    ASSERT(err == 7 + 8 + 1 + 3);
    struct stat st = {0};
    if(stat("./logs", &st) == -1)
        mkdir("./logs", 0777);
    //*/
    LOGFILE = fopen("log.log", "w");
    ASSERT(LOGFILE);
}

void setup()
{
    state.balance = BALANCE; // todo: proper solution needed
    state.phase = GAME;
    state.blue = 0;
    state.red = 0;
    err = pthread_mutex_init(&state.mut, NULL);
    ASSERT(err == 0);

    s = twirc_init();

    twirc_callbacks_t* cbs = twirc_get_callbacks(s);

//    cbs->connect         = handle_everything;
    cbs->welcome = handle_welcome;
    cbs->globaluserstate = handle_everything;
    cbs->capack = handle_everything;
    cbs->ping = handle_everything;
    cbs->join = handle_join;
    cbs->part = handle_everything;
    cbs->mode = handle_everything;
    cbs->names = handle_everything;
    cbs->privmsg = handle_message;
    cbs->whisper = handle_everything;
    cbs->action = handle_everything;
    cbs->notice = handle_everything;
    cbs->roomstate = handle_everything;
    cbs->usernotice = handle_everything;
    cbs->userstate = handle_everything;
    cbs->clearchat = handle_everything;
    cbs->clearmsg = handle_everything;
    cbs->hosttarget = handle_everything;
    cbs->reconnect = handle_everything;
    cbs->invalidcmd = handle_everything;
    cbs->other = handle_everything;
//    cbs->disconnect      = handle_everything;
    cbs->outbound = handle_everything;

    err = twirc_connect(s, "irc.chat.twitch.tv", "6667", NICK, TOKEN);
    LOG("logging into twitch.tv");
    ASSERT(err == 0)
    LOG("login successful!");
}

void handle_everything(twirc_state_t* _, twirc_event_t* evt)
{
    // fprintf(LOGFILE, "> %s\n", evt->raw);
}

void handle_welcome(twirc_state_t* _, twirc_event_t* evt)
{
    err = twirc_cmd_join(s, "#saltyteemo");
    LOG("joining saltyteemo");
    //todo: find out what ints shall be returned and check
}

void handle_join(twirc_state_t* _, twirc_event_t* evt)
{
    // fprintf(LOGFILE, "> %s\n", evt->raw);
}

void handle_message(twirc_state_t* _, twirc_event_t* evt)
{
    int local_amount;

    if(strcmp(evt->message, "!squid") == 0)
    {
        twirc_cmd_privmsg(s, "#saltyteemo", "Squid1 Squid2 Squid3 Squid4 ");
        return;
    }

    if(strcmp(evt->origin, "xxsaltbotxx") != 0)
        return;

    if(strstr(evt->message, NICK))
    {
        // targeted at @me

        if(strstr(evt->message, "Betting has not opened yet."))
        {
            ERROR("Betting has not opened yet");
            // todo: ...
            return;
        }
        if(strstr(evt->message, "Betting has ended for this round"))
        {
            ERROR("Betting has ended for this round");
            // todo: ...
            return;
        }
        if(strstr(evt->message, "You do not have enough"))
        {
            ERROR("You do not have enough");
            twirc_cmd_privmsg(s, "#saltyteemo", "!forage");
            twirc_cmd_privmsg(s, "#saltyteemo", "!blue 100");
            return;
        }
        if(strstr(evt->message, "balance is"))
        {
            // todo: ...
            int new_balance = fetch_balance(evt->message);
            new_balance += (int) (state.balance / 10);

            pthread_mutex_lock(&state.mut);
            if(new_balance == state.balance)
            {
                LOG("match remade - refunded");
            }
            else
            {
                fprintf(LOGFILE, "Bet %s! %d shrooms\n",
                        new_balance > state.balance ? "WON" : "LOST",
                        new_balance - state.balance);
            }
            state.balance = new_balance;
            pthread_mutex_unlock(&state.mut);

            if(DEBUG)
                LOG("Placed bet successfully!");
            return;
        }
        if(DEBUG)
            fprintf(LOGFILE, "MSG: %s\n", evt->message);
    }
    else
    {
        // @others

        if(strstr(evt->message, "Bet complete"))
        {
            // betting time

            pthread_mutex_lock(&(state.mut));
            if(state.phase == GAME)
            {
                state.phase = BETTING;
                pthread_mutex_unlock(&(state.mut));

                pthread_t num;
                pthread_create(&num, NULL, handle_betting, NULL);
                pthread_detach(num);
            }
            if(!DEBUG && state.phase == BUFFER)
            {
                pthread_mutex_unlock(&(state.mut));
                return;
            }
            pthread_mutex_unlock(&(state.mut));

            if(strstr(evt->message, "BLUE"))
            {
                local_amount = fetch_amount(evt->message);
                pthread_mutex_lock(&(state.mut));
                state.blue += local_amount;
                pthread_mutex_unlock(&(state.mut));
                return;
            }
            if(strstr(evt->message, "RED"))
            {
                local_amount = fetch_amount(evt->message);
                pthread_mutex_lock(&(state.mut));
                state.red += local_amount;
                pthread_mutex_unlock(&(state.mut));
                return;
            }
            ERROR("what the fuck?");
            exit(0);
        }
        else
        {
            // disregard
            // todo: actually, lets check for time of closed bets
            return;
        }
    }
}

void* handle_betting(void* _)
{
    if(DEBUG)
        LOG("Betting is opened!");

    sleep(TIME_TO_BET);

    pthread_mutex_lock(&state.mut);
    char* msg = malloc(64);
    ASSERT(msg != NULL);
    snprintf(msg, 64, "!%s %d", state.blue PLAYSTYLE state.red ? "red" : "blue", (int) state.balance * QUOTIENT);
    twirc_cmd_privmsg(s, "#saltyteemo", msg);
    state.phase = BUFFER;
    if(DEBUG)
        fprintf(LOGFILE, "BET %d\n %d:%d\n", (int) state.balance * QUOTIENT, state.blue, state.red);
    pthread_mutex_unlock(&state.mut);

    sleep(100);

    pthread_mutex_lock(&state.mut);
    if(DEBUG)
        fprintf(LOGFILE, "final count:\t%d:%d\n", state.blue, state.red);
    state.phase = GAME;
    state.blue = 0;
    state.red = 0;
    pthread_mutex_unlock(&state.mut);

    return NULL;
}