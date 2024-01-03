#ifndef SOUNDEX_H
#define SOUNDEX_H
#include <cstring>
#include <iostream>
#include <cctype>

using namespace std;

void encode(const char* surname, char* soundex);

char return_soundex(char letter);

bool compare(const char* one, const char* two);

int count(const char* surname, const char* sentence);

#endif
