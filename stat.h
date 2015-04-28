#ifndef _STATS_H_
#define _STATS_H_

#include <stdio.h>
#include <string.h>
#include "struct.h"

int make_log(int, int, int, char *);
int count_words(FILE *);
void druk_strukt( int, int, struktura_t *);
int make_data ( int, int, FILE * [], char *);
int stat_wej( int, int, FILE * [], char *, struktura_t *, int);
int stat_wyj(FILE *, char *);

#endif
