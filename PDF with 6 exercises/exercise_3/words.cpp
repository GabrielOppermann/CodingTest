#include "words.h"

void reverse(const char* str1, char* str2) {
    for (int i = strlen(str1) - 1, j = 0; i >= 0; i--, j++) {
        str2[j] = str1[i];
    }
    str2[strlen(str1)] = '\0';
}

bool compare(const char* one, const char* two) {
    if (*one == '\0' && *two == '\0')
        return true;
    else if (!isalpha(*one) && *one != '\0')
        return compare(one + 1, two);
    else if (!isalpha(*two) && *two != '\0')
        return compare(one, two + 1);
    else if (toupper(*one) == toupper(*two))
        return compare(one + 1, two + 1);
    else
        return false;
}

bool palindrome(const char* sentence) {
    char reversed[100];
    reverse(sentence, reversed);
    return compare(sentence, reversed);   
}

bool anagram(const char* str1, const char* str2) {
    char copy2[100];
    char copy1[100];
    strcpy(copy2, str2);
    strcpy(copy1, str1);
    sort(copy2);
    sort(copy1);
    return compare(copy1, copy2);
}

void sort(char* str) {
    char temp;
    int min;
    for (size_t i = 0; i < strlen(str) -1; i++) {
        min = i + 1;
        for (size_t j = i + 1; j < strlen(str); j++) {
            if (toupper(str[j]) < toupper(str[min]))
                min = j;
        }
        if (toupper(str[min]) < toupper(str[i])) {
            temp = toupper(str[i]);
            str[i] = toupper(str[min]);
            str[min] = temp;
        }
        else 
            str[i] = toupper(str[i]);
    }
}