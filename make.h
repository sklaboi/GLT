#ifndef _MAKE_H_
#define _MAKE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "struct.h"

int make_words( int, int, int, struct_t*, char*, FILE* );
int make_paragraph( int, int, int, int, struct_t*, char*, FILE* );
void clean_struct_adds( struct_t*, int, int);
int losuj( int);

#endif
