#include "braille.h"

int encode_character(char ch, char* braille) {
    int i = 0;
    if (isupper(ch)) {
        strcpy(braille, ".....0"); 
        i = 6;
    }
    else if (isdigit(ch)) {
        strcpy(braille, "..0000"); 
        i = 6;
        if (ch != '0')
            ch += 65-49;
        else
            ch = 'j';
    }
    strcpy(braille + i, "......");
    ch = tolower(ch);

    if ((ch >= 'a' && ch <= 'h') || (ch >= 'k' && ch <= 'r') || (ch >= 'u' && ch <= 'v') || (ch >= 'x' && ch <= 'z'))
        braille[i] = '0';
    if (ch == 'b' || (ch >= 'f' && ch <= 'j') || ch == 'l' || (ch >= 'p' && ch <= 't') || (ch >= 'v' && ch <= 'w') || ch == '.' || ch == ',' || ch == ';' || ch == '!' || ch == '?' || ch == '(' || ch == ')')
        braille[i+1] = '0';
    if ((ch >= 'k' && ch <= 'v') || (ch >= 'x' && ch <= 'z') || ch == ';' || ch == '_' || ch == '!' || ch == '?' || ch == '(' || ch == ')')
        braille[i+2] = '0';
    if ((ch >= 'c' && ch <= 'd') || (ch >= 'f' && ch <= 'g') || (ch >= 'i' && ch <= 'j') || (ch >= 'm' && ch <= 'n') || (ch >= 'p' && ch <= 'q') || (ch >= 's' && ch <= 't') || (ch >= 'w' && ch <= 'y') || ch == '(' || ch == ')')
        braille[i+3] = '0';
    if ((ch >= 'd' && ch <= 'e') || (ch >= 'g' && ch <= 'h') || ch == 'j' || (ch >= 'n' && ch <= 'o') || (ch >= 'q' && ch <= 'r') || ch == 't' || ch == 'w' || (ch >= 'y' && ch <= 'z') || ch == '.' || ch == '!' || ch == '(' || ch == ')')
        braille[i+4] = '0';
    if ((ch >= 'u' && ch <= 'z') || ch == '.' || ch == '_' || ch == '?' || ch == '(' || ch == ')')
        braille[i+5] = '0';
    
    braille[i+6] = '\0';
    return i+6;
}


void encode(const char* plaintext,char* braille) {
    if (*plaintext != '\0')
        encode(plaintext + 1, braille + encode_character(*plaintext, braille));
    else
        *braille = '\0';
}

void print_braille(const char* plaintext, std::ostream& out) {
    char braille[100];
    encode(plaintext, braille);
    for (int j = 0; j < 3; j++) {
        for (size_t i = j; i < strlen(braille); i+=3) {
            out << braille[i] << " ";
        }
        out << endl;
    }
    out << endl << plaintext << endl;
}