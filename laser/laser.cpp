#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "laser.h"

using namespace std;

/* pre-supplied helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **array = new char *[rows];
  assert(array);
  for (int r=0; r<rows; r++) {
    array[r] = new char[columns];
    assert(array[r]);
  }
  return array;
}

/* pre-supplied helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **array, int rows) {
  for (int r=0; r<rows; r++)
    delete [] array[r];
  delete [] array;
}

/* internal helper function which gets the dimensions of a board */
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

/* internal helper function which removes carriage returns and newlines from end of strings */
void filter (char *line) {
  while (*line) {
    if (!isprint(*line))
     *line = '\0';
    line++;
  }
}

/* pre-supplied function to load a board from a file*/
char **load_board(const char *filename, int &height, int &width) {

  bool success = get_board_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **board = allocate_2D_array(height, width + 1);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    filter(line);
    strcpy(board[r], line);
  }
  
  return board;
}

/* pre-supplied function to print a board */
void print_board(char **board, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << board[r][c];
    cout << endl;
  }
}

/* add your own function definitions here */


bool find_laser(char** board, int height, int width, int& row) {

  for (int i = 1; i < height; i++) {
    if (board[i][0] == '>') {
      row = i;
      return true;
    }
  }
  row = -1;
  return false;
}

char mirror_label(char** board, int height, int width, int row, int column) {
  if (!(row >= 0 && row < height && column >= 0 && column < width))
    return '\0';
  else if (!(board[row][column] == '/' || board[row][column] == '\\' || board[row][column] == '?'))
    return '\0';

   int moves[8] = {1,1, -1,-1, -1,1, 1,-1};
   int end = 8, start = 0;

  if (board[row][column] == '/') {
    start = 4;
    end = 7;
  }  
  else if (board[row][column] == '\\') {
    start = 0;
    end = 3;
  }

  for (int i = start; i<end; i+=2) {
    int next_row = row + moves[i];
    int next_col = column + moves[i+1];
    if (!(next_row >= 0 && next_row < height && next_col >= 0 && next_col < width))
      continue;
    if (isalpha(board[next_row][next_col]))
      return toupper(board[next_row][next_col]);
  }
  return '\0';
}

bool shoot(char** board, int height, int width, char* message, int& last_row, int& last_col) {
  char marker = '-';
  int direction[2] = {0,1};
  int row = 0, col = 1, index = 0;
  if (!find_laser(board, height, width, row)) {
    *message = '\0';
    return false;
  }

  while (!(board[row][col] == '#' || board[row][col] == '@' || board[row][col] == '?')) {
    char board_entry = board[row][col];
    if (board_entry == '/') {
      mirror_direction(direction, '/');
      mirror_marker(marker);
      message[index] = mirror_label(board, height, width, row, col);
      if (message[index] != '\0')
        index++;
    }
    else if (board_entry == '\\') {
      mirror_direction(direction, '\\');
      mirror_marker(marker);
      message[index] = mirror_label(board, height, width, row, col);
      if (message[index] != '\0')
        index++;
    }
    else if (!isalpha(board_entry)) {
      board[row][col] = marker;
    }
    row += direction[0];
    col += direction[1];
  }
  last_row = row; 
  last_col = col;
  message[index] = '\0';
  if (board[row][col] == '@')
    return true;
  return false;
}

void mirror_direction(int direction[2], char mirror) {
  char temp;
  if (mirror == '/') {
    direction[0] = -direction[0];
    direction[1] = -direction[1];
  }
  temp = direction[0];
  direction[0] = direction[1];
  direction[1] = temp;
}

void mirror_marker(char& marker) {
  if (marker == '-')
    marker = '|';
  else
    marker = '-';
}

bool solve(char** board, int height, int width, const char* target) {
  char message[100];
  int row = 0, col = 0;
  if (shoot(board, height, width, message, row, col) && !strcmp(target, message))
    return true;
  else if (board[row][col] == '?') {
    char moves[3] = {'/', '\\', ' '};
    // make move
    for (int k = 0; k < 3; k++) {
      board[row][col] = moves[k];
      if (solve(board, height, width, target))
        return true;
    }
    board[row][col] = '?';
  }
  return false;
}