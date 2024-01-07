#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

bool valid_step(const char* current_word, const char* next_word) {
   int count_different_letters = 0, length = strlen(current_word);
   for (int i = 0; i <= length; i++) {
      if (current_word[i] != next_word[i])
         count_different_letters++;
   }
   if (count_different_letters != 1)
      return false;
   if (!dictionary_search(next_word))
      return false; 
   return true;
}

bool display_chain(const char *wheat_chain[], std::ostream& out) {
   int row_count = 0, string_index = 0;
   while (wheat_chain[row_count] != NULL) {
      if (row_count != 0 && wheat_chain[row_count + 1] != NULL) {
         while(wheat_chain[row_count][string_index] != '\0') {
            out.put(tolower(wheat_chain[row_count][string_index]));
            string_index++;
         }
      }
      else {
          while(wheat_chain[row_count][string_index] != '\0') {
            out.put(toupper(wheat_chain[row_count][string_index]));
            string_index++;
         }
      }
      string_index = 0;
      out.put('\n');
      row_count++;
      if (out.fail())
         return false;
   }
   return true;
}

bool valid_chain(const char *chain[]) {
   int element = 1;
   while (chain[element] != NULL) {
      if (!valid_step(chain[element-1], chain[element]) || already_there(chain, chain[element], element))
         return false;
      element++;  
   }
   return element >=2;
}

bool already_there(const char *chain[], const char* word, int end) {
   for (int i = 0; i < end; i++) {
      if (!strcmp(chain[i], word)) 
         return true;
   }
   return false;
}

bool find_chain(const char* start_word, const char* target_word, const char *chain[], int max_steps, int step) {
      if (step == 0) {
         char* tempStart = new char[strlen(start_word) + 1];
         strcpy(tempStart, start_word);
         chain[0] = tempStart;
      }
      if (step > max_steps)
         return false;
      else if (!strcmp(chain[step], target_word) && valid_chain(chain))
         return true;
      else {
         char* tempNew = new char[strlen(start_word) + 1];
         for (size_t i = 0; i < strlen(start_word); i++) {
            strcpy(tempNew, chain[step]);
            for (char ch = 'A'; ch <= 'Z'; ch++) {
               tempNew[i] = ch;
               if (valid_step(chain[step], tempNew)) {
                  chain[step+1] = tempNew;
                  chain[step+2] = NULL;
                  if (find_chain(start_word, target_word, chain, max_steps, step + 1))
                     return true;
               }
            }
         }
   }
   return false;
}