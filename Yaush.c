/*
 ============================================================================
 Name        : Yaush.c
 Author      : JSG
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>

static char input = '\0';
static char ** commandArray;
static char ** toExecute;
static char *** options;
static int commandArrayIndex = 0;

void nextLine(){
	printf("Oui Maitre ? >>");
}

void listenCommand(){
	int i;
	int characterIndex = 0;

	//Destroy old command
	for(i = 0; i < commandArrayIndex; i++)
		commandArray[commandArrayIndex - 1 - i] = NULL;
	commandArrayIndex = 0;

	//Store new command in commandArray

	while(input != '\n'){
		//While enter is not pressed then store command in array.
		while(input != ' '){
			//While input
			commandArray[commandArrayIndex][characterIndex] = input;
			characterIndex ++;
			input = getchar();
		}
		characterIndex = 0;
		commandArrayIndex ++;
		input = getchar();
	}
}

void executeCommand(){
	pid_t pid_process;

	pid_process = fork();
	switch (pid_process){
	case -1:
		printf("Fork failure \n Exiting \n");
		exit(1);
		break;
	case 0:
	 //executer le code
		break;
	default:
		//wait();
		break;
	}
}

void welcome_msg(){
	printf("----------------------- \n");
	printf("This is a shell project \n");
	printf("----------------------- \n");
}

char * test_white (char * line){
	int length_line = strlen(line);
	int begining_line_index = 0;
	int i;

	while(line[begining_line_index] == ' '){
		begining_line_index++;

	}

	if(begining_line_index == length_line)
		return NULL;

	while(line[length_line - 1] == ' '){
		length_line --;

		if(length_line == 0)
			return NULL;
	}
	int up_index = (length_line - begining_line_index);
	char * new_line =  (char*) malloc(up_index * sizeof(char));


	for( i = 0; i < up_index; i++){
		new_line[i] = line[begining_line_index + i];
	}
	new_line[up_index] = '\0';

	return new_line;
}
int main(int argc, char ** argv) {
	char *line, *history_tmp;
	//initialize_readline();
	using_history ();
	welcome_msg();


	while(1){
		line = readline("Oui maitre >>>");

		if(!line)
			break;

		history_tmp = test_white (line);
		// removes begining and en spaces
		if(history_tmp != NULL){
			add_history(history_tmp);
		}

		if (strcmp (line, "exit") == 0)
			{printf("Good Bye");
			return EXIT_SUCCESS;
			}

		free(line);

	}

}


