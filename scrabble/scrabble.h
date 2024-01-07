#ifndef SCRABBLE_H
#define SCRABBLE_H

enum ScoreModifier { NONE, DOUBLE_LETTER_SCORE, TRIPLE_LETTER_SCORE, DOUBLE_WORD_SCORE, TRIPLE_WORD_SCORE };

/* insert your function prototypes here */

int tile_score(char tile);

bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles);

int return_index(const char* tiles, char letter);

int compute_score(const char* played_tiles, ScoreModifier score_modifiers[7]);

int highest_scoring_word_from_tiles(const char* tiles, ScoreModifier score_modifiers[7], char* result_word);
#endif