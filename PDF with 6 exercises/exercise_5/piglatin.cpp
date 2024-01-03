#include "piglatin.h"

int findFirstVowel(const char* word) {
    char letter;
    size_t length = strlen(word);
    for (size_t i = 0; i < length; i++) {
        letter = tolower(word[i]);
        if (isVowel(letter, i, length))    
            return i;
    }
    return -1;
}

bool isVowel(char letter, int position, int length) {
     if (letter == 'a' || letter == 'e' || letter == 'i' || letter == 'o' || 
            letter == 'u' || (letter == 'y' && position != 0 && position != length -1))
            return true;
    return false;
}

void translateWord(const char* english, char* piglatin) {
    if (isdigit(english[0])) {
        strcpy(piglatin, english); 
        return;
    }
    int length = strlen(english);
    if (isVowel(english[0], 0, length)) {
        strcpy(piglatin, english);
        piglatin[length] = 'w';
        length++;
    }
    else {
        int first_vowel = findFirstVowel(english);
        if (first_vowel != -1)
            rearrange(english, piglatin, first_vowel, length);
        else
            strcpy(piglatin, english);
    }
    strcpy(piglatin + length, "ay\0");
}

void rearrange(const char* english, char* piglatin, int first_vowel, int length) {
    int j = 0;
    for (int i = first_vowel; i < length; i++) {
        piglatin[j] = tolower(english[i]); 
        j++;
    }
    for (int i = 0; i < first_vowel; i++) {
        piglatin[j] = tolower(english[i]); 
        j++;
    }
    if (isupper(english[0]))
        piglatin[0] = toupper(piglatin[0]);
}

void translateStream(std::istream& in, std::ostream& out) {
    if (in.eof())
        return;
    char word[64], piglatin[64];
    char punct = 'A';
    if (isalnum(in.peek())) {
        in >> word;
        if(ispunct(word[strlen(word)-1])) {
            punct = word[strlen(word)-1];
            word[strlen(word)-1] = '\0';
        }
        translateWord(word, piglatin);
        out << piglatin;
        if (punct != 'A')
            out.put(punct);
    }
    else {
        in.get(punct);
        out.put(punct);
    }
    translateStream(in, out);
}