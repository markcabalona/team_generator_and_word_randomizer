#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "proj_utils.h"


int add(char** wordBank, int num_of_words);
void open_wb(char** wb, int n_words);
void search_wb(char** wb, int n_words);
void edit_wb(char** wb, int n_words);
void delete_wb(char** wb, int n_words);
void random_pick(char** wb, int n_words);
void random_generator(char** wb, int n_words);

char* clear_screen = NULL;// = (char*)malloc(5*sizeof(char));
int main(int argc, char* argv[]){
    
    //check the OS to set appropriate clear command
    //if on windows use cls, if on linux use clear
    if (argc > 1){
        if(strcasecmp(argv[1], "win")==0){
            clear_screen = strdup("cls");
        }
        else{
            clear_screen = strdup("clear");
        }
    }
    else{
        printf("Invalid number of arguments.\n\n");
        printf("If you're on linux, run the program like this: ");
        printf("./name_of_executable_file linux\n\n");
        printf("If you're on windows, run the program like this: ");
        printf("name_of_executable_file win\n");
        exit(EXIT_FAILURE);
    }
    system(clear_screen);
    intro();

    FILE *fp;//check if the "word_bank.txt" file is existing, if it is then use it, if not then create a new one
    fp= fopen("word_bank.txt", "a");
    fseek(fp,0,SEEK_END);
    int size = ftell(fp);
    if(size == 0)
    {
        fprintf(fp,"");
    }
    fclose(fp);

    int enter;
    int running = 1;//boolean
    char** wb = NULL;//this will hold the words from wordbank txt file
    int n_words;//number of words in word_bank
    char* buffer = (char*)malloc(STRING_MAX * sizeof(char));
    char* token = (char*)malloc(STRING_MAX * sizeof(char));

    do{//main program loop

        //collect all the words from wordBank textfile load it to an array  called wb
        fp = fopen("word_bank.txt", "r");
        if(!fp){
            perror("CANT OPEN WORD BANK");
            exit(1);
        }
        wb = (char**)malloc(0*sizeof(char*));
        n_words = 0;//initialized to 0
        while(fgets(buffer, STRING_MAX, fp)){
            // if this is executed then it means that wb is not empty
            n_words++;
            wb = (char**)realloc(wb, (n_words) * sizeof(char*));//allocate memory for each line in word bank
            token = strtok(buffer, "\n");//tokenize every line in word bank(word bank is newline seperated)
            while(token!=NULL){
                wb[n_words-1] = strdup(token);//load every word from word bank in to wb array
                token = strtok(NULL, "\n");
            }
        }
        fclose(fp);//close file

        //show the main menu
        system(clear_screen);//clear the terminal

        divider();//decorations (prints a line seperator design)
        printf("\t\t----------------------------------  MAIN MENU  ---------------------------------");
        divider();
        //print the main menu options
        printf("\n\t\t\t\t\t     [1] ADD A WORD");
        printf("\n\t\t\t\t\t     [2] OPEN WORD BANK");
        printf("\n\t\t\t\t\t     [3] SEARCH WORD BANK");
        printf("\n\t\t\t\t\t     [4] EDIT ");
        printf("\n\t\t\t\t\t     [5] DELETE");
        printf("\n\t\t\t\t\t     [6] WORD RANDOMIZER");
        printf("\n\t\t\t\t\t     [7] TEAM GENERATOR");
        printf("\n\t\t\t\t\t     [8] EXIT\n");
        divider();
        //get an integer response from user
        //get_int is defined in 'proj_utils.c' file
        enter = get_int("\n\t\t\t\t\t     Enter an option: ");

        switch(enter)
        {
            case 1:{
                //allocate memory for one more word in wb
                wb = (char**)realloc(wb, (n_words+1) * sizeof(char*));
                //add function defined below it modifies the wb arrray and returns the number of words/elements in wb
                n_words = add(wb, n_words);
                break;
            }
            case 2:{
                //defined below, it prints words from wb
                open_wb(wb, n_words);
                break;
            }
            case 3:{
                //search for existence of a word in wb
                //binary search is used to do this
                //function is defined below
                search_wb(wb, n_words);
                break;
            }
            case 4:{
                //modifies the wb array
                //function is defined below
                edit_wb(wb, n_words);
                break;
            }
            case 5:{
                //modifies the wb array
                //function is defined below
                delete_wb(wb, n_words);
                break;
            }
            case 6:{
                //randomly picks a word from word bank or from user-inputted set of words
                //function is defined below
                random_pick(wb, n_words);
                break;
            }
            case 7:{
                //creates teams based on user's desired num of teams
                //members may come from word bank or user-inputted set of strings
                //function is defined below
                random_generator(wb, n_words);
                break;
            }
            case 8:{
                running = 0;//set running to false
                break;
            }
            default:{
                //if user enters other number
                printf("\n\t\t\t\t INVALID INPUT. Please choose a valid option.\n");
                break;
            }
        }

        //sort wb array and update the word bank txt file each iteration of main loop
        qsort(wb,n_words,sizeof(wb[0]),string_compare);//defined in standard library of c

        //open for writing, write the content of wb array back to word bank file
        fp = fopen("word_bank.txt", "w");
        for (int i = 0; i < n_words; i++){
            if(strcmp(wb[i],"\n") == 0){//in delete function we replace the word with new line
                continue;
            }
            fprintf(fp,"%s\n", wb[i]);
        }

        fclose(fp);//close


        if (enter != 8){//if user wants to go back to main menu
            printf("\n\t\t\t\t    (Press any key to go to the main menu)");
            char* temp = (char*)malloc(STRING_MAX * sizeof(char));
            scanf("%[^\n]s",temp);
            fgets(temp,STRING_MAX,stdin);
            free(temp);
        }

    }while(running);

    for(int i = 0; i < n_words; i++){// free allocated memory
        if (wb[i] != NULL)
        {
            //printf("freed");
            free(wb[i]);
        }
    }
    if (wb != NULL){
        //printf("done");
        free(wb);
    }

    free(buffer);
    free(token);

    system(clear_screen);
	divider();
	printf("\t\t--------------------------------  EXIT PROGRAM  --------------------------------");
    divider();
    printf("\n\t\t\t\t\tThank you for using the program!\n");
	divider();
	printf("\n\t\t\t\t\t    Press enter key to exit...");
	char* temp = (char*)malloc(STRING_MAX * sizeof(char));
    scanf("%[^\n]s",temp);
    fgets(temp,STRING_MAX,stdin);
    free(temp);
	printf("\n");

    free(clear_screen);
}




int add(char** wordBank, int num_of_words)
{
    system(clear_screen);//clear the screen

    char* word_to_add = NULL;
    divider();
    printf("\t\t--------------------------------  ADD A STUDENT  -------------------------------");
    divider();

    word_to_add = get_string("\n\t\t\t\t\t     Enter a Word: ");//get a string input from user
    int in_wb;

    //search if the word is already existing in word bank, we dont want duplicates
    //binary_search returns -1 if word is not found, and returns the index of the word otherwise
    in_wb = binary_search(wordBank, word_to_add, 0 , num_of_words);

    if (in_wb == -1){
        num_of_words++;
        wordBank[num_of_words-1] = strdup(word_to_add);// append the word
        printf("\n\t\t\t\t\t     Word successfully added!\n");
    }
    else{//if the word is already in the word bank dont append it
        wordBank[num_of_words+1] = strdup("\n");//dont append anything in word bank
        printf("\n\t\t\t\t\t%s is already in the word bank.\n", word_to_add);
    }

    divider();
    free(word_to_add);
    return num_of_words;
}

void open_wb(char** wb, int n_words){
    system(clear_screen);
    
    divider();
    printf("\t\t----------------------------  WELCOME TO WORD BANK!  ---------------------------");
    divider();

    if(n_words == 0)//if word bank is empty display a message
        printf("\n\t\t\t\t\t**WORD BANK IS CURRENTLY EMPTY**\n");

    else if (n_words > 20){// we'll print just 20 random words from wordbank incase word bank has many words in it
        printf("\n\t\t\t\t     The word bank has too many words in it.\n"
        	   "\n\t\t\t\t          Printing you 20 Random Words.\n\n");

        //defined in proj_utils.c
        shuffle(wb,n_words);//shuffles the wb array to randomize the words that are going to be printed
        for (int i = 0; i < 20; i++)
            printf("\t\t\t\t\t\t%s\n", wb[i]);
    }
    else{//words in wb is <= 20
        printf("\n");
        for (int i = 0; i < n_words; i++){
            printf("\t\t\t\t\t\t%s\n",wb[i]);
        }
    }

    divider();
    return;

}

void search_wb(char** wb, int n_words){
    system(clear_screen);

    divider();
	printf("\t\t------------------------------  SEARCH WORD BANK  ------------------------------");
    divider();
    char* search_word;
    search_word = get_string("\n\t\t\t\t\tEnter a word to search: ");

    int in_wb = binary_search(wb,search_word,0,n_words);
    if (in_wb > -1){
        printf("\n\t\t\t\t     \"%s\" is number %d in the word bank\n", search_word, in_wb+1);
    }
    else{
        printf("\n\t\t\t\t  The word \"%s\" is not in the word bank.\n", search_word);
    }

    divider();
    free(search_word);
}

void edit_wb(char** wb, int n_words){
    system(clear_screen);

    divider();
	printf("\t\t--------------------------------- EDIT WORD BANK  ------------------------------");
    divider();

    char* word;
    word = get_string("\n\t\t\t\t\t Enter a word to edit: ");
    int in_wb = binary_search(wb,word,0, n_words);// search if the word to be edited is existing

    if (in_wb > -1){
        char* new_word;
        printf("\n\t\t\t\t\t Enter a new word to replace %s: ", word);
        new_word = get_string("");
        int in_wb_ = binary_search(wb, new_word,0, n_words);//check if the new word is already on the list, we dont want duplicates
        if (in_wb_ > -1){
            printf("\n\t\t\t\t\"%s\" is already in the list. \"%s\" will not be edited.\n", new_word, word);
        }
        else{
            wb[in_wb] = strdup(new_word);// replace the old word with new word
            printf("\n\t\t\t\t\t Word edited.\n");
        }
        free(new_word);//free
    }
    else{
        printf("\n\t\t\t\t  The word \"%s\" is not on the word bank.\n", word);
    }

    divider();
    free(word);//free memory allocated in get_string

}

void delete_wb(char** wb, int n_words){
    system(clear_screen);

    divider();
	printf("\t\t------------------------------- DELETE WORD BANK  -----------------------------");
    divider();

    char* word;
    word = get_string("\n\t\t\t\t\t Enter a word to delete: ");
    int in_wb = binary_search(wb,word,0, n_words);//check if the word to be deleted is in wb, we cant delete what's not existing

    if (in_wb > -1){
        wb[in_wb] = strdup("\n");//replace the word with newline, becase when we update the word bank we do not print newline
        printf("\n\t\t\t\t\t Word successfully deleted.\n");
    }
    else{
        printf("\n\t\t\t\t   The word \"%s\" is not on the word bank.\n", word);
    }

    divider();
    free(word);
}

void random_pick(char** wb, int n_words){
    system(clear_screen);

    divider();
	printf("\t\t---------------------------------  RANDOM PICK  --------------------------------");
    divider();

    srand(time(NULL));
    int random_index;
    char yn = yes_no("\n\t\t\t\tDo you want to use the words from word bank?[Y/N]: ");//this function return y or n character
    if(yn == 'y'){//use words from word bank
        if(n_words != 0){//if word bakn is not empty
            printf("\n\t\t\t\t\tChoosing random word from word bank...\n");
            random_index = rand() % n_words;//choose random index from 0 to words in word bank -1
            printf("\n\t\t\t\t\tThe random word is...");

            printf("\n\t\t\t\t\t____________________________________________");
		    printf("\n\t\t\t\t\t|                                          |");
		    printf("\n\t\t\t\t\t|          ======================          |");
		    printf("\n\t\t\t\t\t                   %s                      ", wb[random_index]);// print random word from word bank
		    printf("\n\t\t\t\t\t|          ======================          |");
		    printf("\n\t\t\t\t\t|___   ____________________________________|");
		    printf("\n\t\t\t\t\t    | /");
		    printf("\n\t\t\t\t\t    |/\n");
        }
        else//word bank is empty
            printf("\n\t\t\t\t\tSorry! but the word bank is currently empty.");
    }
    else{// user wants to enter his own set of words
        char** temp =NULL;
        int num_of_words = get_int("\n\t\t\t\tHow many words/sentences are you going to enter? ");
        temp = (char**)malloc(num_of_words*sizeof(char*));//allocate enough memory
        for (int i = 0; i < num_of_words; i++){
            temp[i] = (char*)malloc(STRING_MAX*sizeof(char));
            printf("\n\t\t\t\tEnter word number %d: ",i+1);
            temp[i] = get_string("");//get input form the user
        }
        random_index = rand() % num_of_words;//choose random index from 0 to (num_of_words entered by user)-1
        printf("\n\t\t\t\t\tThe random word is...");

        printf("\n\t\t\t\t\t____________________________________________");
	    printf("\n\t\t\t\t\t|                                          |");
	    printf("\n\t\t\t\t\t|          ======================          |");
	    printf("\n\t\t\t\t\t                   %s                      ", temp[random_index]);
	    printf("\n\t\t\t\t\t|          ======================          |");
	    printf("\n\t\t\t\t\t|___   ____________________________________|");
	    printf("\n\t\t\t\t\t    | /");
	    printf("\n\t\t\t\t\t    |/\n");
    }
}

void random_generator(char** wb, int n_words){
    system(clear_screen);

    team* teams = NULL;
    char** temp = NULL;

    divider();
	printf("\t\t------------------------------  RANDOM GENERATOR  ------------------------------");
    divider();
    char yn = yes_no("\n\t\t\t\tDo you want to use the words from word bank?[Y/N]: ");
    int pop, num_of_teams,mem_per_team,rem;
    if(yn == 'y'){
        pop = get_int("\n\t\t\t\tHow many words do you want to use?: ");
        if( pop > n_words){
            printf("\n\t\t\t\tInputted number exceeds the number of words in the word bank.\n");
            printf("\n\t\t\t\tOur system will be using all the words (%d words) in our word bank.\n",n_words);
            pop = n_words;
        }
        temp = (char**)malloc(pop*sizeof(char*));
        shuffle(wb,n_words);//shuffle the word bank to get random words
        for (int i = 0; i < pop; i++){
            temp[i] = strdup(wb[i]);//fill the temp array with random words from word bank
        }
    }
    else{
        pop = get_int("\n\t\t\t\tEnter how many words you are going to enter: ");
        temp = (char**)malloc(pop*sizeof(char*));//allocate enough memory
        for (int i = 0; i < pop; i++){
            temp[i] = (char*)malloc(STRING_MAX*sizeof(char));
            printf("\n\t\t\t\tMember number %d: ", i+1);
            temp[i] = get_string("");//get pop input form user
        }
    }
    int invalid;
    do{
        invalid = 0;//false
        num_of_teams = get_int("\n\t\t\t\tEnter number of teams: ");
        if (num_of_teams > pop){
            printf("\n\t\t\t\tWe cannot group %d member/s to %d teams. The last %d team/s would be empty.\n",pop,num_of_teams,num_of_teams - pop);
            printf("\n\t\t\t\tPlease enter a valid team number.\n");
            invalid = 1;//true
        }
    }while(invalid);
    mem_per_team = pop/num_of_teams;//get the min members per team
    rem = pop % num_of_teams;


    //allocate memory for team struct. it holds the number of members of a team, and the list of members of a team
    teams = (team*)malloc(pop*sizeof(team));
    for (int i = 0; i < pop; i++){
        if (i < rem){
            teams[i].member_list = (char**)malloc((mem_per_team+1)*sizeof(char*));
            for (int j = 0; j < mem_per_team+1; j++){
                teams[i].member_list[j] = (char*)malloc(STRING_MAX*sizeof(char));
            }
        }
        else{
            teams[i].member_list = (char**)malloc((mem_per_team)*sizeof(char*));
            for (int j = 0; j < mem_per_team; j++){
                teams[i].member_list[j] = (char*)malloc(STRING_MAX*sizeof(char));
            }
        }
    }

    divider();
    /*this randomly group the temp array and fill each team in teams with atleast mem_per_team members */
    team_generator(temp,pop,teams,num_of_teams);
    FILE *fp;
    fp = fopen("generated_teams.txt", "a");
    //save the generated teams in the generated_teams.txt file
    for (int i = 0; i < num_of_teams; i++){
        printf("\n\t\t\t\tTeam %d has %d members\n", i+1, teams[i].num_of_members);
        fprintf(fp,"Team %d has %d members\n", i+1, teams[i].num_of_members);
        for (int j = 0; j < teams[i].num_of_members; j++){
            printf("\n\t\t\t\tMember %d: %s\n",j+1, teams[i].member_list[j]);
            fprintf(fp,"Member %d: %s\n",j+1, teams[i].member_list[j]);
        }
        printf("\n");
        fprintf(fp,"\n");
    }
    fprintf(fp,"**************************************************\n\n");
    fclose(fp);

    divider();
    yn = yes_no("\n\t\t\t\tDo you want to save a copy of the generated teams?[Y/N]: ");
    if (yn == 'y'){//if user wants a seperate txt file of his teams
        char* file_name = (char*)malloc(69 * sizeof(char));
        int invalid = 1;
        do{
            snprintf(file_name,68*sizeof(char), "%s.txt", get_string("\n\t\t\t\tEnter file name: "));//ask for unique file name
            if (strlen(file_name) < 69){
                fp = fopen(file_name,"w");
                for (int i = 0; i < num_of_teams; i++){
                    fprintf(fp,"Team %d has %d members\n", i+1, teams[i].num_of_members);
                    for (int j = 0; j < teams[i].num_of_members; j++){
                        fprintf(fp,"Member %d: %s\n",j+1, teams[i].member_list[j]);
                    }
                    fprintf(fp,"\n");
                }
                fclose(fp);
                invalid = 0;
                printf("\n\t\t\t\tTeams successfully saved.\n");
            }
            else{
                printf("\n\t\t\t\tFile name is too long. Maximum length is 64 charaters.\n");
            }
        }while(invalid);
        free(file_name);
    }
}
