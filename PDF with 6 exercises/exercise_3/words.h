#ifndef WORDS_H
#define WORDS_H

#include <iostream>
#include <cctype>
#include <cstring>

using namespace std;

void reverse(const char* str1, char* str2);

bool compare(const char* one, const char* two);

bool palindrome(const char* sentence);

bool anagram(const char* str1, const char* str2);

void sort(char* str);

#endif