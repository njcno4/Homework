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
#include <curses.h>
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

void interpret_history_command(char * line);
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

void interpret_command (char * line){

	//interpreting history command
	if(strncmp (line, "hist-", 5) == 0){
		interpret_history_command(line);
	}

	//action = check_for_action(line);

}

void interpret_history_command(char * line){

	if(strcmp(line, "hist-save") == 0){
		write_history ("history.txt");
		printf("history saved in \"history.txt\" \n");
	}
	else if (strcmp(line, "hist-read") == 0){
		read_history("history.txt");
		printf("history read in \"history.txt\" \n");
	}
	else if (strcmp (line, "hist-list") == 0)
	{
		HIST_ENTRY ** hst_list;
		int i;

		hst_list = history_list();

		if(hst_list != NULL)
			for (i = 0; hst_list[i]; i++)
				printf ("%d: %s\n", i + history_base, hst_list[i]->line);
	}

	else if (strncmp (line, "hist-clear", 10) == 0)
	{
		char answer[1024];
		printf("Are you sure to clear the history ? [Y/N]\n");
		scanf("%s", answer);
		if(strncmp(answer, "Y", 1) == 0){
			clear_history();
			write_history ("history.txt");
		}
		printf("history cleared \n", strncmp(answer, "Y", 1));
	}
	else
		printf("Unknown history command \n");
}

int main(int argc, char ** argv) {
	char *line, *history_tmp;
	//initialize_readline();
	using_history ();
	write_history ("history.txt");
	welcome_msg();


	while(1){
		line = readline("Oui maitre >>>");
		if(!line)
			break;

		history_tmp = test_white (line);
		// removes beginning and end spaces
		if(history_tmp != NULL){
			add_history(history_tmp);
			append_history (1, "history.txt");
		}

		interpret_command(line);

		if (strcmp (line, "exit") == 0)
		{
			printf("Good Bye \n");
			return EXIT_SUCCESS;
		}

		free(line);
		free(history_tmp);

	}
	write_history ("history.txt");
	return EXIT_SUCCESS;
}


