#ifndef RIVER_H
#define RIVER_H

const int SCENE_HEIGHT=24, SCENE_WIDTH=72;

enum Status {VALID_GOAL_STATE=2, VALID_NONGOAL_STATE=1,ERROR_INVALID_CANNIBAL_COUNT=-1, ERROR_INVALID_MISSIONARY_COUNT=-2, ERROR_INVALID_MOVE=-3, ERROR_MISSIONARIES_EATEN=-4, ERROR_INPUT_STREAM_ERROR=-5, ERROR_BONUS_NO_SOLUTION=-6};

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char* status_description(int code); 

bool add_to_scene(char **scene, int row, int col, const char *filename); 

void filter(char *line);

void print_scene(char **scene);

void destroy_scene(char **scene);

char **create_scene();

void deallocate_2D_array(char **m, int rows);

char **allocate_2D_array(int rows, int columns);

char** make_river_scene(const char left[7], const char boat[3]);

int perform_crossing(char left[10], const char* targets);

void generate_string(char* string, int m, int c, int b); 

int count(const char* string, char letter); 

int play_game(); 

#endif