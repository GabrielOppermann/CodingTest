#ifndef BRAILLE_H
#define BRAILLE_H
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

int encode_character(char ch, char* braille);

void encode(const char* plaintext,char* braille);

void print_braille(const char* plaintext, std::ostream& out);

#endif