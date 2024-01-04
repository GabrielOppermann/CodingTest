#include <iostream>
#include "correct.h"

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary);

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary);

void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

char binary_to_ascii(const char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */

void text_to_binary(const char* str, char* binary) {

    if (*str == '\0') {
        *binary = '\0';
        return;
    }
    else {
        ascii_to_binary(*str, binary); 
        text_to_binary(str + 1, binary + 8);
    }
}

void binary_to_text(const char* binary,char* str) {
    if (*binary == '\0') {
        *str = '\0';
        return;
    }
    else {
        *str = binary_to_ascii(binary);
        binary_to_text(binary + 8, str + 1);
    }
}

void add_error_correction(const char* data, char* corrected) {
    int j = 0;
    for (size_t i = 0; i < strlen(data); i+=4, j+=7) {
        char c1 = parity(data[i+0], data[i+1], data[i+3]);
        char c2 = parity(data[i+0], data[i+2], data[i+3]);
        char c3 = parity(data[i+1], data[i+2], data[i+3]);

        corrected[j+0] = c1; 
        corrected[j+1] = c2; 
        corrected[j+2] = data[i+0];
        corrected[j+3] = c3;
        corrected[j+4] = data[i+1];
        corrected[j+5] = data[i+2];
        corrected[j+6] = data[i+3];
    }
        corrected[j] = '\0';
}

char parity(char one, char two, char three) {
    if ((one + two + three - 3* 48) % 2 != 0)
        return '1';
    else 
        return '0';
}

int decode(const char* received, char* decoded) {
    int j = 0, count = 0, index = 0;
    for (size_t i = 0; i < strlen(received); i+=7, j+=4) {
        index = -1;
        char c1 = parity(received[i+3], received[i+4], received[i+5], received[i+6]);
        char c2 = parity(received[i+1], received[i+2], received[i+5], received[i+6]);
        char c3 = parity(received[i+0], received[i+2], received[i+4], received[i+6]);
        if (c1 == '1' || c2 == '1' || c3 == '1' ) {
            const char position[9] = {'0','0', '0','0','0', c1, c2, c3, '\0'};
            index = static_cast<int>(binary_to_ascii(position));
        }
        decoded[j+0] = received[i+2];
        decoded[j+1] = received[i+4];
        decoded[j+2] = received[i+5];
        decoded[j+3] = received[i+6];
        if (index != -1) {
            flip_bit(decoded + j, index);
            count++;
        }
        }
        decoded[j] = '\0';
        return count;
} 

char parity(char one, char two, char three, char four) {
     if ((one + two + three + four - 4* 48) % 2 == 0)
        return '0';
    else 
        return '1';
}

void flip_bit(char* decoded, int index) {
    if (index == 6)
        index = 2;
    else if (index == 7)
        index = 3;
    else if (index == 5)
        index = 1;
    else if (index == 3)
        index = 0;
    else 
        return;
    if (decoded[index] == '0')
        decoded[index] = '1';
    else 
        decoded[index] = '0';
}