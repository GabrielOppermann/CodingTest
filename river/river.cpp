#include "river.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
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

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}

/* insert your functions here */

char** make_river_scene(const char left[7], const char boat[3]) {
  char** scene = new char*[SCENE_WIDTH];
  scene = create_scene();
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  
  int i = 0, colCannibals = 1, colMissionaries = 1, colBoat = 36, countMissionaries = 0, countCanibals = 0;
  char ch;
  while (left[i] != '\0') {
    ch = left[i]; 
    if (ch == 'M') {
      add_to_scene(scene, 2, colMissionaries, "missionary.txt");
      colMissionaries += 6;
      countMissionaries++;
    }
    if (ch == 'C') {
      add_to_scene(scene, 11, colCannibals, "cannibal.txt");
      colCannibals += 6;
      countCanibals++;
    }
     if (ch == 'B') {
      colBoat = 19;
    }
    i++;
  }
  
  add_to_scene(scene, 17, colBoat, "boat.txt");

  // fill boat
  i = 0;
  while (boat[i] != '\0') {
    ch = boat[i]; 
    if (ch == 'M') {
      add_to_scene(scene, 11, colBoat + 3, "missionary.txt");
      countMissionaries++;
    }
    if (ch == 'C') {
      add_to_scene(scene, 11, colBoat + 3, "cannibal.txt");
      countCanibals++;
    }
    i++;
    colBoat += 6;
  }

  colMissionaries = 54; 
  colCannibals = 54;
   for (;countCanibals < 3; countCanibals++) {
      add_to_scene(scene, 11, colCannibals, "cannibal.txt");
      colCannibals += 6;
   }
    for (; countMissionaries < 3; countMissionaries++) {
      add_to_scene(scene, 2, colMissionaries, "missionary.txt");
      colMissionaries += 6;
   }
  return scene;
}

int perform_crossing(char left[10], const char* targets) {
  int leftM = count(left, 'M'), leftC = count(left, 'C'), targetM = count(targets, 'M'), targetC = count(targets, 'C');
  bool boat_left = count(left, 'B');
  char** scene;

  if (strlen(targets) < 1 || strlen(targets) > 2)
    return -3;
  // Invalid Canibals / missionary numbers
  if ((boat_left && targetC > leftC) || (!boat_left && targetC > 3 - leftC))
    return -1;
  else if ((boat_left && targetM > leftM) || (!boat_left && targetM > 3 - leftM))
    return -2;

  //display scene a)
  char scence_left[10];
  if (boat_left) {
    leftM -= targetM;
    leftC -= targetC;
  }
  generate_string(scence_left, leftC, leftM, boat_left);
  cout << "Loading the boat..." << endl;
  scene = make_river_scene(scence_left, targets);
  print_scene(scene);

  //display scene b)
  boat_left = !boat_left;
  generate_string(scence_left, leftC, leftM, boat_left);
  cout << "Crossing the river..." << endl;
  scene = make_river_scene(scence_left, targets);
  print_scene(scene);

  if (boat_left) {
    leftM += targetM;
    leftC += targetC;
  }

  //display scene c)
  generate_string(scence_left, leftC, leftM, boat_left);
  cout << "Unloading the boat..." << endl;
  scene = make_river_scene(scence_left, "");
  print_scene(scene);
  
  if ((leftC > leftM && leftM != 0 )||(3 - leftC > 3 - leftM && leftM != 3)) {
    if (leftC > leftM && leftM)
      leftM = 0;
    generate_string(left, leftC, leftM, boat_left);
    return -4;
  }
  if (leftM == 0 && leftC == 0) {
    generate_string(left, leftC, leftM, boat_left);
    return 2;
  }
    generate_string(left, leftC, leftM, boat_left);
  return 1;
}

void generate_string(char* string, int c, int m, int b) {
  int stringIndex = 0; 
  for (int i = 0; i < c; i++) {
    string[stringIndex] = 'C'; 
    stringIndex++;
  }
  for (int i = 0; i < m; i++) {
    string[stringIndex] = 'M';
    stringIndex++;
  }
  for (int i = 0; i < b; i++) {
    string[stringIndex] = 'B';
    stringIndex++;
  }
  string[stringIndex] = '\0';
}

int count(const char* string, char letter) {
  int count = 0 ;
  for (size_t i = 0; i < strlen(string); i++) {
    if (toupper(string[i]) == toupper(letter))
      count++;
  }
  return count;
}

int play_game() {
    char** scene; 
    int status = 0;
    char left[8] = "MMMCCCB";
    scene = make_river_scene(left, "");
    cout << "This is your starting position:" << endl;
    print_scene(scene);
    while (status != 2 && status != -4) {
      char move[3];
      cout << "Type in who should cross! 'C' for cannibal, 'M' for missionary: ";
      cin.getline(move, 3);
      move[2] = '\0';
      status = perform_crossing(left, move);
    }
    return status;
}