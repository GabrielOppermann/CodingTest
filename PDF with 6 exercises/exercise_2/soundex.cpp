#include "soundex.h"

void encode(const char* surname, char* soundex) {
    
    int surname_index = 1, soundex_index = 1;
    soundex[0] = toupper(surname[0]);
    soundex[4] = '\0';

    while (soundex_index <= 3) {
        // add letter to
        if (return_soundex(tolower(surname[surname_index])) != '7' && return_soundex(tolower(surname[surname_index])) != return_soundex(tolower(surname[surname_index-1]))) {
            soundex[soundex_index] = return_soundex(tolower(surname[surname_index]));
            soundex_index++;
        }
        if (surname[surname_index] != '\0')
            surname_index++;
    }
}

char return_soundex(char letter) {
    if (letter == 'b' || letter == 'f'  || letter == 'p'  || letter == 'v')
        return '1';
    else if (letter == 'c' || letter == 'g'  || letter == 'j'  || letter == 'k' || letter == 'q' || letter == 's' || letter == 'x' || letter == 'z')
        return '2';
    else if (letter == 'd' || letter == 't')
        return '3';
    else if (letter == 'l')
        return '4';
    else if (letter == 'm' || letter == 'n')
        return '5';
    else if (letter == 'r')
        return '6';
    else if (letter == '\0')
        return '0';
    else 
        return '7';
}

bool compare(const char* one, const char* two) {
    if (*one != *two)
        return false; 
    else if (*one == '\0')
        return true; 
    else 
        return compare(one + 1, two + 1);
}

int count(const char* surname, const char* sentence) {
    char copy [100], soundex_sentence[5], soundex_surname[5];
    encode(surname, soundex_surname);

    strcpy(copy, sentence);
    int count = 0, start = 0, end = 0, length = strlen(sentence);

    while (start < length) {
        while (copy[start] != '\0' && !isalpha(copy[start]))
            start++;
        end = start;
        while (isalpha(copy[end]))
            end++;
        copy[end] = '\0';
        encode(copy + start, soundex_sentence);
        if (compare(soundex_sentence,soundex_surname))
            count++;
        start = ++end;
    }
    return count;
}