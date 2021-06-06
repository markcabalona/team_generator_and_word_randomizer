#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "proj_utils.h"

//this will be used in qsort function in standard library.
int string_compare (const void * a, const void * b ) {
    return strcasecmp(* (char * const *) a, * (char * const *) b);
}

void divider()
{
    printf("\n\t\t================================================================================\n"); //80 characters
}

void intro(){
    printf("\n\n\t\t\t\t____________________________________________");
    printf("\n\t\t\t\t|                                          |");
    printf("\n\t\t\t\t|                                          |");
    printf("\n\t\t\t\t|                 WELCOME !                |");
    printf("\n\t\t\t\t|                                          |");
    printf("\n\t\t\t\t|___     __________________________________|");
    printf("\n\t\t\t\t    |   /");
    printf("\n\t\t\t\t    |  /");
    printf("\n\t\t  _                 | /");
    printf("\n\t\t ( \\                |/");
    printf("\n\t\t  ) )");
    printf("\n\t\t ( (  .-''''-.  A.-.A      \t\t   _____________________________");
    printf("\n\t\t  \\ \\/        \\/ , , \\ \t\t\t ||                             ||");
    printf("\n\t\t   \\    \\     =;  o  /=  \t\t ||                             ||");
    printf("\n\t\t    \\  _/-|'''-',---'     \t\t ||   BS COMPUTER SCIENCE 1AB   ||");
    printf("\n\t\t   / //   | ||             \t\t ||_____________________________||");
    printf("\n\t\t  /_,))   |_,))\n\n\n\n\t\t\t\t (Press enter key to continue)");
    char* temp = (char*)malloc(STRING_MAX * sizeof(char));
    scanf("%[^\n]s",temp);
    fgets(temp,STRING_MAX,stdin);
    free(temp);
}


char yes_no(char* prompt){
    int invalid = 1; //true
    char* response;
    do{//only accept y/Y or n/N as input
        response = get_string(prompt);
        if(strcasecmp(response, "y") == 0 || strcasecmp(response, "n") == 0)
            invalid = 0;//set to false
    }while(invalid == 1);
    char res = response[0];
    free(response);//free memory allocated by get_string
    return res;
}

char* get_string(char* prompt){
    //whenever you call get_string you need to free the memory allocated by it
    char* temp = (char*)malloc(STRING_MAX * sizeof(char));
    char* buff = malloc(sizeof(char));
    printf("%s", prompt);
    scanf("%[^\n]",temp);
    fgets(buff, sizeof(buff), stdin);//clear the \n in stdin
    free(buff);
    return temp;
}

int get_int(char* promt){
    char* temp_str = NULL;
    
    do{//do this while user's input is not an int
        temp_str = get_string(promt);
        if (strcmp(temp_str, "0") == 0){//if the user enters the number 0
            break;
        }
    }while(atoi(temp_str) == 0);

    int num = atoi(temp_str);//convert the user's input to an int
    free(temp_str);// free the memory allocated by get_string
    return num;//return the int
}

float get_float(char* promt){
    //same logic as the get_int function above
    char* temp_str = NULL;
    
    do{
        temp_str = get_string(promt);
        if (strcmp(temp_str, "0") == 0){
            break;
        }
    }while(atof(temp_str) == 0);

    float num = atof(temp_str);
    free(temp_str);
    return num;
}


unsigned int binary_search(char** word_bank, char* target, unsigned int min,unsigned int max)
{
    unsigned int word_index = ((min + max) / 2); 
    //printf(">%d",word_index);
    //base case 
    // if word in word bank is equal to target or if target is not in worbank
    if (min >= max)//target is not found
        return -1;  
    if (strcasecmp(word_bank[word_index], target) == 0)//target found
        return word_index;


    //recursive     
    if (strcasecmp(word_bank[word_index], target) > 0)
    {
        //search the lower half
        binary_search(word_bank,target,min, word_index);
    }
    else{
        //search the upper half
        binary_search(word_bank,target,word_index+1,max);
    }
}


void team_generator(char** members,int num_of_members,team* teams, int num_of_teams){
    
    int rem = num_of_members % num_of_teams;//remaining members
    int member_per_team = num_of_members / num_of_teams;//min number of members per team
    
    if(num_of_members > 2)
        shuffle(members, num_of_members);//shuffle the array so that the memebers of teams will be random

    int counter = 0, team_n = 0;
    for (int i = 0; i < num_of_teams; i++){//fill the first num_of_teams teams with member_per_team members
        teams[i].num_of_members = 0;
        for(int j = 0; j < member_per_team; j++){//fll each team in teams with mem_per_team members
            strcpy(teams[i].member_list[j], members[counter]);
            teams[i].num_of_members++;
            counter++;
        }
    }

    for (int j = 0; j < rem; j++){
        //distribute the remaining members who are not grouped yet to the first rem teams
        strcpy(teams[j].member_list[member_per_team], members[counter]);
        teams[j].num_of_members++;
    }
}

void shuffle(char** array, int size_of_arr){
    srand(time(NULL));
    int index, i = 0;
    char* temp = (char*)malloc(STRING_MAX * sizeof(char));
    while(i <= size_of_arr/2){// we just need to swap half of the array to randomize it

        do{
            index = rand() % size_of_arr;
            //generate random index from current index+1 to size_of_arr
        }while(index < 0 || index >= size_of_arr || index <= i);
        //swap each element of the array with a random word in the array
        strcpy(temp, array[i]);
        strcpy(array[i],array[index]);
        strcpy(array[index],temp);
        i++;
    }
    
}


