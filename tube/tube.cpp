#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <string>

using namespace std;

#include "tube.h"

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

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

bool get_symbol_position(char** map, int height, int width, char target,int& r, int& c) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (map[i][j] == target) {
        r = i; 
        c = j; 
        return true;
      }
    }
  }
  r = -1; 
  c = -1; 
  return false;
}

char get_symbol_for_station_or_line(const char* name) {
  char ch, space;
  char lookup[50];

  // check if station
  ifstream stations("stations.txt");
  if (stations.fail()) {
    cout << "failed opening stations file" << endl;
    return ' ';
  }
  stations >> ch; 
  stations.get(space);
  stations.getline(lookup, 50);
  while(!stations.eof()) {
    
    if (!strcmp(lookup, name))
      return ch;
    stations >> ch; 
    stations.get(space);
    stations.getline(lookup, 50);
  }
  stations.close();

  // check if line
  ifstream lines("lines.txt");
  if (lines.fail()) {
    cout << "failed opening lines file" << endl;
    return ' ';
  }
  lines >> ch; 
  lines.get(space);
  lines.getline(lookup, 50);
  while(!lines.eof()) {
    if (!strcmp(lookup, name))
      return ch;
    lines >> ch;
    lines.get(space);
    lines.getline(lookup, 50);
  }
  lines.close(); 
  
  return ' ';
}

int validate_route(char** map, int height, int width, const char* start_station, const char* route, char* destination) {
  // check start station
  char start_station_code = get_symbol_for_station_or_line(start_station);
  if (!isalnum(start_station_code))
    return -1;
  int count = 0;
  char this_line = ' ';
  char last_position = start_station_code;
  Direction previous_direction = INVALID_DIRECTION;

  int r = 0, c = 0;
  get_symbol_position(map, height, width, start_station_code, r, c);

  char next_move[3];
  int string_index = 0, move_index = 0;
  // make moves and check
  while (route[string_index] != '\0') {
    // read in next move
    while (!(route[string_index] == '\0' || route[string_index] == ',')) {
      next_move[move_index] = route[string_index];
      move_index++;
      string_index++;
    }
    next_move[move_index] = '\0';

    // Ceck for invalid direction
    Direction direction = string_to_direction(next_move);
    if (direction == INVALID_DIRECTION)
      return -5;
    
    // Change r and c
    go_to_next_r_c(direction, r, c); 

    // Check out of bounds
    if (r < 0 || r >= height || c < 0 || c >= width)
      return -7;
    
    if (previous_direction == INVALID_DIRECTION)
      previous_direction = direction;

    char this_position = map[r][c];
    // check if line hopping
    if (!isalnum(this_position) && !isalnum(last_position)) {
      if(this_position != last_position)
        return -3;
      else if (revese_move(direction, previous_direction))
        return -4;
    }

    if (this_line == ' ' && !isalnum(this_position))
      this_line = map[r][c];
    else if (!isalnum(this_position) && this_line != map[r][c]) {
      this_line = map[r][c];
      count++;
    }

    // prepare next move
    previous_direction = direction;
    last_position = this_position;
    if (route[string_index] != '\0')
      string_index++;
    move_index = 0;
    next_move[0] = '\0';
  }

  if (!isalnum(map[r][c]))
    return -2;

  set_destination(destination, map[r][c]);

  return count;
}

void go_to_next_r_c(Direction direction, int& r, int& c) {
  int vertical_offset = 0; 
  int horizontal_offset = 0;

  switch (direction) {
    case S: vertical_offset = 1; break;
    case N: vertical_offset = -1; break;
    case E: horizontal_offset = 1; break;
    case W: horizontal_offset = -1; break;
    case SW: horizontal_offset = -1; vertical_offset = 1; break;
    case SE: horizontal_offset = 1; vertical_offset = 1; break;
    case NW: horizontal_offset = -1; vertical_offset = -1; break;
    case NE: horizontal_offset = 1; vertical_offset = -1; break;
    default: vertical_offset = 0; break;
  }
  r += vertical_offset;
  c += horizontal_offset;
}

void set_destination(char* destination, char symbol) {
  char ch, space;
  char lookup[50];
  ifstream stations("stations.txt");
  if (stations.fail()) {
    cout << "failed opening stations file" << endl;
    return;
  }
  stations >> ch;
  stations.get(space);
  stations.getline(lookup, 50);
  while(!stations.eof()) {
    if (ch == symbol ) {
      strcpy(destination, lookup);
      return;
    }
    stations >> ch; 
    stations.get(space);
    stations.getline(lookup, 50);u
  }
  stations.close();
}

bool revese_move(Direction direction, Direction previous_direction) {
  if (direction <= 3 && previous_direction <= 3 && abs(previous_direction-direction) == 1)
    return true; 
  return false;
}

