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
		exit();
		break;
	case 0:
	 //executer le code
		break;
	default:
		wait();
		break;
	}
}

void welcome_msg(){
	printf("----------------------- \n");
	printf("This is a shell project \n");
	printf("----------------------- \n");
}


int main(void) {

	welcome_msg();
	nextLine();

	while(1){
		input = getchar();
		if(input == '\n')
			nextLine();
			//Do nothing
		else {
			listenCommand(); //Do nothing
			executeCommand(); //Execute command
			nextLine(); //Do nothing
		}
		printf("Good Bye");
	}
	return EXIT_SUCCESS;
}
