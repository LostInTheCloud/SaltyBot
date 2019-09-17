#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sys/stat.h>
#include "libtwirc.h"

#include "settings.h"

#define ERROR(expr2) fprintf(stderr, "ERROR in function \"%s\" in %s:%d%s%s\n", __func__, __FILE__, __LINE__, (expr2)?"\n> ":"", (expr2)?(expr2):"");
#define ASSERT(expr) if(!(expr)){ERROR(NULL); exit(0);}

void setup();
void handle_all(twirc_state_t *s, twirc_event_t *evt);