#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#include <fstream>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */

int count_words(const char* line) {
  int count = 0, i = 0;
  while (line[i] != '\0') {
    count++;
    while (line[i+1] != '\0' && !(line[i] == ' ' && line[i+1] != ' '))
      i++;
    i++;
  }
  return count;
}

bool find_phonetic_ending(const char* word, char* phonetic_ending) {
  char entry[512];
  char result[512];
  char space;

  ifstream in("dictionary.txt");
  if (in.fail())
    return false;
  in >> entry;
  in.get(space);
  in.get(space);
  in.getline(result, 512);
  while (!in.eof()) {
    if (!strcmp(word, entry)) {
      in.close();  
      strcpy(phonetic_ending, result + index_last_vowel(result));
      trim_string(phonetic_ending);
      return true;
    }
    in >> entry;
    in.get(space);
    in.get(space);
    in.getline(result, 512);
  }
  in.close();
  return false;
}

int index_last_vowel(const char* phonetic_ending) {
  int vowel_index = -1, i = 0;
  while (phonetic_ending[i] != '\0') {
    if (phonetic_ending[i] == 'A' || phonetic_ending[i] == 'E' || phonetic_ending[i] == 'I' || phonetic_ending[i] == 'O' || phonetic_ending[i] == 'U')
      vowel_index = i;
    i++;
  }
  return vowel_index;
}

void trim_string(char* phonetic_ending) {
  int i = 0;
  while (phonetic_ending[i] != '\0') {
    if (phonetic_ending[i] == ' ')
      strcpy(phonetic_ending + i, phonetic_ending + i + 1);
    i++;
  }
}

bool find_rhyme_scheme(const char* filename, char* scheme) {
  char line[512];
  char last_word[512];
  char phonetic_ending[512];
  int i = 0;
  rhyming_letter(RESET);

  ifstream in(filename);
  if (in.fail())
    return false;
  in.getline(line, 512);
  while (!in.eof()) {
    get_word(line, count_words(line), last_word);
    if (!isalpha(last_word[strlen(last_word)-1]))
      last_word[strlen(last_word)-1] = '\0';
    find_phonetic_ending(last_word, phonetic_ending);
    scheme[i] = rhyming_letter(phonetic_ending);
    i++;
    in.getline(line, 512);
  }
  in.close();
  scheme[i] = '\0';
  return true;
}

string identify_sonnet(const char* filename) {
  char scheme[512];

  if (!find_rhyme_scheme(filename, scheme))
    return "failed analysing";

  else if (!strcmp(scheme, "ababcdcdefefgg"))
    return "Shakespearean";

  else if (!strcmp(scheme, "abbaabbacdcdcd"))
    return "Petrarchian";

  else if (!strcmp(scheme, "ababbcbccdcdee"))
    return "Spenserian";
  
  else  
    return "unknown";
}
