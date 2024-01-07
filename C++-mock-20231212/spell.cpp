#include <iostream>
#include<cstring>
#include<stdio.h>
#include<cmath>
#include "spell.h"

using namespace std;

/* insert your function definitions here */
int frequency (const char* word) {

  ifstream in("words.dat");
  char compare[MAX_LENGTH];
  int number; 

  if (in.fail()) {
    cerr << "fail in loading file!" << endl;
    return 0;
  }

  in >> number >> compare;
  while (!in.eof()) {
    if (strcmp(compare, word) == 0) {
      return number;
    }
    in >> number >> compare;
  }
  in.close();
  return 0;
}
				  
int edit_distance(const char* word1, const char* word2) {
int i = 0, j = 0, distance = 0; 
while (word1[i] != '\0' && word2[j] != '\0') {
  if (strcmp(word1+i, word2+j) == 0) 
    return distance;
  else if (word1[i] == word2[j]) {
    i++;
    j++;
  }
  else {
    distance++;
    if (word1[i] == word2[j+1] && word1[i+1] == word2[j]) {
      i += 2; 
      j += 2;
    }
    else if (word1[i+1] == word2[j]) 
      i++;
    else if (word1[i] == word2[j+1]) 
      j++;
    else {
      i++; 
      j++;
    }
  }
}
  return distance + strlen(word1) - i + strlen(word2) - j;
}

bool spell_correct(const char* word, char* fixed) {
  ifstream in("words.dat");
  char next_word[MAX_LENGTH];
  char max_ed_1[MAX_LENGTH];
  char max_ed_2[MAX_LENGTH];
  int number = 0, ed_1 = 0, ed_2 = 0;

  if (in.fail()) {
    cerr << "fail in loading file!" << endl;
    return 0;
  }

  in >> number >> next_word;
  while (!in.eof()) {
    if (strcmp(next_word, word) == 0) {
      strcpy(fixed, word);
      return false; 
    }
    else if (edit_distance(word, next_word) == 1 && number > ed_1) {
      strcpy(max_ed_1, next_word);
      ed_1 = number; 
    }
    else if (edit_distance(word, next_word) == 2 && number > ed_2) {
      strcpy(max_ed_2, next_word);
      ed_2 = number;
    }
    in >> number >> next_word;
  }
  in.close();

  if (ed_2 == 0 && ed_1 == 0) {
    strcpy(fixed, word);
    return false;
  }
  else if (ed_1 == 0)
    strcpy(fixed, max_ed_2);
  else
    strcpy(fixed, max_ed_1);
  return true;
}