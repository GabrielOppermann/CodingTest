#ifndef CORRECT_H
#define CORRECT_H

#include<cstring>

void ascii_to_binary(char letter, char *output);
char binary_to_ascii(const char *binary);

void text_to_binary(const char* str, char* binary);

void binary_to_text(const char* binary,char* str);

void add_error_correction(const char* data, char* corrected);

char parity(char one, char two, char three); 

int decode(const char* received, char* decoded);

char parity(char one, char two, char three, char four); 

void flip_bit(char* decoded, int index);

#endif