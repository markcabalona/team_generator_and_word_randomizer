#ifndef PROJ_UTILS
#define PROJ_UTILS

#define STRING_MAX 1025//max char length of a string


typedef struct{
    int num_of_members;
    char** member_list;
}team;//data structure that holds an int and an array of strings


int string_compare(const void * a, const void * b );

void intro();
void divider();

char yes_no(char* prompt);//returns y or n
//returns a pointer to a string
char* get_string(char* prompt);
//returns an int
int get_int(char* promt);
//returns a float
float get_float(char* promt);

//randomize the members array and create a num_of_teams teams
void team_generator(char** members,int num_of_members,team* teams, int num_of_teams);
//shuffle an array
void shuffle(char** arr, int size_of_arr);

//returns -1 if target is not in word_bank, returns the index of target otherwise
unsigned int binary_search(char** word_bank, char* target, unsigned int min,unsigned int max);



#endif