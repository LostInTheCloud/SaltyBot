#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libtwirc.h"

#include "settings.h"

#define ERROR(expr2) fprintf(stderr, "ERROR in function \"%s\" in %s:%d%s%s\n", __func__, __FILE__, __LINE__, (expr2)?"\n> ":"", (expr2)?(expr2):"");
#define ASSERT(expr) if(!(expr)){ERROR(NULL); exit(0);}
#define LOG(expr) fprintf(LOGFILE, "%s\n", ((expr)));
#define LOGINT(expr) fprintf(LOGFILE, "%ld\n", ((expr)));

void start_logger();
void setup();
void handle_everything(twirc_state_t *s, twirc_event_t *evt);
void handle_welcome(twirc_state_t *s, twirc_event_t *evt);
void handle_join(twirc_state_t *s, twirc_event_t *evt);
void handle_message(twirc_state_t *s, twirc_event_t *evt);

FILE* LOGFILE;
int err;
twirc_state_t* s;