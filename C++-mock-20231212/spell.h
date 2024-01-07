#ifndef SPELL_H
#define SPELL_H
/* you may assume a maximum word length of 512 characters*/
#define MAX_LENGTH 512
#include<fstream>

int frequency (const char*);

int edit_distance(const char*, const char*);

/* insert your function prototypes here */

bool spell_correct(const char*, char*);

#endif