#include "substring.h"

int substring_position(const char* str1, const char* str2, int index) {
    if (*str2 == '\0' && *str1 != '\0')
        return -1;
    else if (is_prefix(str1, str2))
        return index;
    else
        return substring_position(str1, str2 + 1, index + 1);
}

bool is_prefix(const char* str1, const char* str2) {
    if (*str1 == '\0')
        return true;    
    else if (*str1 != *str2)
        return false;
    else
        return is_prefix(str1 + 1, str2 + 1);
}