#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include "libtwirc.h"

#include "settings.h"

#define FLUSH fflush(LOGFILE);fsync(fileno(LOGFILE));
#define ERROR(expr2) fprintf(LOGFILE, "ERROR in function \"%s\" in %s:%d%s%s\n", __func__, __FILE__, __LINE__, (expr2)?"\n> ":"", (expr2)?(expr2):"");FLUSH
#define ASSERT(expr) if(!(expr)){ERROR(NULL); exit(0);}
#define LOG(expr) fprintf(LOGFILE, "%s\n", ((expr)));FLUSH
#define LOGINT(expr) fprintf(LOGFILE, "%ld\n", ((expr)));FLUSH

int fetch_amount(char* msg);

int fetch_balance(char* msg);

void start_logger();

void setup();

void handle_everything(twirc_state_t* s, twirc_event_t* evt);

void handle_welcome(twirc_state_t* s, twirc_event_t* evt);

void handle_join(twirc_state_t* s, twirc_event_t* evt);

void handle_message(twirc_state_t* s, twirc_event_t* evt);

void* handle_betting(void* _);

typedef enum phase{BETTING, BUFFER, GAME} PHASE;

typedef struct state
{
    pthread_mutex_t mut;
    PHASE phase;
    int balance;
    int blue;
    int red;
} STATE;

STATE state;

FILE* LOGFILE;
int err;
twirc_state_t* s;