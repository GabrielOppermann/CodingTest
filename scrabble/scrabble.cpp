#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */


int tile_score(char tile) {
    tile = toupper(tile);
    if (tile == 'A' || tile == 'E' || tile == 'I' || tile == 'L' || tile == 'N' || tile == 'O' || (tile >= 'R' && tile <= 'U'))
        return 1;
    else if (tile == 'D' || tile == 'G')
        return 2;
    else if (tile == 'B' || tile == 'C' || tile == 'M' || tile == 'P')
        return 3;
    else if (tile == 'F' || tile == 'H' || tile == 'V' || tile == 'W' || tile == 'Y')
        return 4;
    else if (tile == 'K')
        return 5;
    else if (tile == 'J' || tile == 'X')
        return 8;
    else if (tile == 'Q' || tile == 'Z')
        return 10;
    else if (tile == ' ' || tile == '?')
        return 0;
    else
        return -1;
}   

int return_index(const char* tiles, char letter) {
    int len = strlen(tiles);
    for (int i = 0; i < len; i++) {
        if (tiles[i] == letter)
            return i;
    }
    return -1;
}

bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles) {
    if (*word == '\0') {
        *played_tiles = '\0';
        return true;
    }
    int index = return_index(tiles, *word);
    if (index == -1) {
        index = return_index(tiles, '?');
        if (index == -1)
            return false;
    }
    *played_tiles = tiles[index];
    char tiles_copy[strlen(tiles) +1];
    strcpy(tiles_copy, tiles);
    tiles_copy[index] = '*';
    return can_form_word_from_tiles(word + 1, tiles_copy, played_tiles + 1);
}

int compute_score(const char* played_tiles, ScoreModifier score_modifiers[7]) {
    int score = 0, tile_multiplicator = 1, word_multiplicator = 1, len = strlen(played_tiles), addition = 0;;
    for (int i = 0; i < len; i++) {
        if (score_modifiers[i] == DOUBLE_LETTER_SCORE) 
            tile_multiplicator = 2; 
        else if (score_modifiers[i] == TRIPLE_LETTER_SCORE) 
            tile_multiplicator = 3;
        else if (score_modifiers[i] == DOUBLE_WORD_SCORE) 
            word_multiplicator = 2;
        else if (score_modifiers[i] == TRIPLE_WORD_SCORE) 
            word_multiplicator = 3;
        score += tile_score(played_tiles[i]) * tile_multiplicator;
        tile_multiplicator = 1;
    }
    if (len == 7)
        addition = 50;
    return score * word_multiplicator + addition;
}

int highest_scoring_word_from_tiles(const char* tiles, ScoreModifier score_modifiers[7], char* result_word) {
    int score = -1, max_score = -1; 
    char word[100], played_tiles[8];
    ifstream in("words.txt");
    
    if (in.fail())
        return -1;
    in >> word;
    while (!in.eof()) {
        if (strlen(word) < 8 && can_form_word_from_tiles(word, tiles, played_tiles)) {
            score = compute_score(played_tiles, score_modifiers);
            if (score > max_score) {
                strcpy(result_word, word);
                max_score = score;
            }
        }
        in >> word;
    }
    in.close();
    return max_score;
}