#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <cstring>
#include <cctype>
#include <iostream>
#include <fstream>

using namespace std;

int findFirstVowel(const char* word);

bool isVowel(char letter, int position, int length);

void translateWord(const char* english, char* piglatin);

void rearrange(const char* english, char* piglatin, int first_vowel, int length);

void translateStream(std::istream& in, std::ostream& out);

#endif