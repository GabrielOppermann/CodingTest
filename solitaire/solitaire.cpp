#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "solitaire.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a board */
bool get_board_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  cout << "Loading puzzle board from '" << filename << "'...";
  
  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }

  cout << " done (height = " << height << ", width = " << width << ")." << endl;
    
  return m;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << static_cast<char>('A' + c);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your function definitions below */

bool are_identical(char ** first, char** second, int height, int width) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (first[i][j] != second[i][j])
        return false;
    }
  }
  return true;
}


bool make_move(char** board, const char* move, int height, int width) {
    // valid string
    if (strlen(move) != 3 || move[0] < 'A' || move[0] > 'G' || move[1] < '0' || move[1] > '6' || !(move[2] == 'N' || move[2] == 'S' || move[2] == 'E' || move[2] == 'W')) 
      return false; 
    int vertical_offset = 0, horizontal_offset = 0, target_row = 0, target_col = 0, origin_row = 0, origin_col = 0;
    if (move[2] == 'N')
      vertical_offset = -2;
    else if (move[2] == 'S')
      vertical_offset = 2;
    else if (move[2] == 'W')
      horizontal_offset = -2;
    else 
      horizontal_offset = 2;
    origin_row = move[1] - 48;
    origin_col = move[0] - 65;
    target_row = origin_row + vertical_offset;
    target_col = origin_col + horizontal_offset;

    if (target_row < 0 || target_row >= height || target_col < 0 || target_col >= width)
      return false;
    else if (board[origin_row][origin_col] != 'O')
      return false;
    else if (board[target_row][target_col] != '_')
      return false;
    else if (board[target_row - vertical_offset/2][target_col - horizontal_offset/2] != 'O')
      return false;
    else {
      board[origin_row][origin_col] = '_';
      board[target_row][target_col] = 'O';
      board[target_row - vertical_offset/2][target_col - horizontal_offset/2] = '_';
      return true;
  }
}

bool find_solution(char** begin, char** end, int height, int width, char* solution, int count) {

  if (count > 6)
    return false;
  else if (are_identical(begin, end, height, width)) {
    *(solution-1) = '\0';
    return true;
  }
  else {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (begin[i][j] == 'O') {
          char move[4] = {static_cast<char>(j+65), static_cast<char>(i+48),'N','\0'};
          char directions[4] = {'N', 'S', 'E', 'W'}; 
          for (int index = 0; index < 4; index++) {
            move[2] = directions[index];
            if(make_move(begin, move, height, width)) {
              strcpy(solution, move);
              *(solution + 3) = ',';
              if (find_solution(begin, end, height, width, solution + 4, count + 1)) {
                return true;
              }
              else
                undo_move(begin, move);
            }
          }
      }
    }
  }
  return false;
}
}

void undo_move(char** begin, const char* move) {
    int vertical_offset = 0, horizontal_offset = 0, target_row = 0, target_col = 0, origin_row = 0, origin_col = 0;
    if (move[2] == 'N')
      vertical_offset = -2;
    else if (move[2] == 'S')
      vertical_offset = 2;
    else if (move[2] == 'W')
      horizontal_offset = -2;
    else 
      horizontal_offset = 2;
    target_row = move[1] - 48;
    target_col = move[0] - 65;
    origin_row =  target_row + vertical_offset;
    origin_col = target_col + horizontal_offset;

    begin[origin_row][origin_col] = '_';
    begin[target_row][target_col] = 'O';
    begin[origin_row - vertical_offset/2][origin_col - horizontal_offset/2] = 'O';
}