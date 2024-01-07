#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

#include "common.h"
#include "mask.h"
#include "gogen.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which removes unprintable characters like carriage returns and newlines from strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
  char **board = allocate_2D_array(5, 6);
  ifstream input(filename);
  assert(input);
  char buffer[512];
  int lines = 0;
  input.getline(buffer, 512);
  while (input && lines < HEIGHT) {
    filter(buffer);
    if (strlen(buffer) != WIDTH)
      cout << "WARNING bad input = [" << buffer << "]" << endl;
    assert(strlen(buffer) == WIDTH);
    strcpy(board[lines], buffer);
    input.getline(buffer, 512);
    lines++;
  }
  input.close();
  return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
  ofstream out(filename); 
  if (!out)
    return false;
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      out << board[r][c];
    }
    out << endl;
  }
  bool result = out.good();
  out.close();
  return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
  char word[512];
  int count = 0;
  ifstream in(filename);
  while (in >> word)
    count++;
  in.close();
  return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
  int count = count_words(filename);
  ifstream in(filename);
  assert(in);
  int n=0;
  char **buffer = new char *[count+1]; // +1 because we NULL terminate 
  char word[512];
  for (; (in >> word) && n<count; n++) {
    buffer[n] = new char[strlen(word) + 1];
    strcpy(buffer[n], word);
  }
  buffer[n] = NULL;
  in.close();
  return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
  for (int r=0; r<HEIGHT; r++) {
    for (int c=0; c<WIDTH; c++) {
      cout << "[" << board[r][c] << "]";
      if (c < WIDTH-1)
	cout << "--";
    }
    cout <<endl;
    if (r < HEIGHT-1) {
      cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
      cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
    }
  }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
  for (int n=0; words[n]; n++) 
    cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) {
  deallocate_2D_array(board, HEIGHT);
}

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
  int count = 0;
  for (; words[count]; count++);
  deallocate_2D_array(words, count);
}

/* add your own function definitions here */

bool get_position(char** board, char ch, int& row, int& column){
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (board[i][j] == toupper(ch)) {
        row = i;
        column = j;
        return true;
      }
    }
  }
    row = -1;
    column = -1;
    return false;
  }

  bool valid_solution(char** board, char** words) {
    int i = 0, j = 1, row = 0, col = 0, prev_row = 0, prev_col = 0;
    while (words[i] != NULL) {
      if(!get_position(board, words[i][0], prev_row, prev_col))
        return false;
      while (words[i][j] != '\0') {
        if(!get_position(board, words[i][j], row, col) || abs(prev_row-row) > 1 || abs(prev_col-col) > 1) 
          return false;
        prev_row = row;
        prev_col = col;
        j++;
      }
      j=1;
      i++;
    }
    return true;
  }

  void update(char** board, char ch, Mask& m) {

    int row = 0, col = 0;
    if (get_position(board, ch, row, col)) {
      m.set_all(false);
      m[row][col] = true;
      return;
    }
    for (row = 0; row < HEIGHT; row++) {
      for (col = 0; col < WIDTH; col++) {
        if (board[row][col] != '.')
          m[row][col] = false;
      }
    }
    if (m.count() == 1) {
      m.get_position(true, row, col);
      board[row][col] = ch;
    }
  }

  void neighbourhood_intersect(Mask& one, Mask&  two) {

    Mask one_n1 = one.neighbourhood();
    Mask two_n1 = two.neighbourhood();
    one.intersect_with(two_n1);
    two.intersect_with(one_n1);
  }

  bool solve_board(char** board, char** words) {
    Mask m[25];
    for (char letter = 'A'; letter < 'Z'; letter++) {
      update(board, letter, m[letter - 65]);
    }
    if (solve_masks(board, words, m, 0))
      return true; 
    else
      return investigate_board(board, words);
  }

  bool solve_masks(char** board, char** words, Mask m[25], int filled_fields) {
    int filled_fields_count = count_filled(board);
    if (valid_solution(board, words))
      return true;
    if (filled_fields != filled_fields_count) {
      int i = 0, j = 1;
      while (words[i] != NULL) {
        while (words[i][j] != '\0') {
          char ch = words[i][j];
          char prev_ch = words[i][j-1]; 
          neighbourhood_intersect(m[ch - 65], m[prev_ch - 65]);
          update(board, ch, m[ch - 65]);
          update(board, prev_ch, m[prev_ch - 65]);
          j++;
        }
        j = 1;
        i++;
      }
      return solve_masks(board, words, m, filled_fields_count);
    }
    else 
      return false;
  }

int count_filled(char** board) {
  int count = 0;
  for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++) {
        if (board[i][j] != '.') {
          count++;
        }
      }
    }
    return count;
}

bool investigate_board(char** board, char** words) {
    if (valid_solution(board, words))
        return true;
    int row = 0, col = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == '.') {
                for (char ch = 'A'; ch <= 'Y'; ch++) {
                    if (!get_position(board, ch, row, col)) {
                        board[i][j] = ch;
                        if (investigate_board(board, words)) {
                            return true;
                        }
                    }
                }
                board[i][j] = '.';
                return false;
            }
        }
    }
    return false; 
}

