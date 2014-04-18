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
#include <dirent.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/wait.h>


#include <readline/readline.h>
#include <readline/history.h>


struct CMD{
	char name[128];
	char path[128];
};
typedef struct CMD CMD;


static CMD command_tab[2048];
static int list_index, len;
static int max_i = 0 ;



char * command_generator (const char * text, int state);
char ** completion (const char * text, int start, int end);

void interpret_history_command(char * line);


char * dumpstring (char *s)
{
  char *r;

  r = (char*) malloc (strlen (s) + 1);
  strcpy (r, s);
  return (r);
}



void executeCommand(char * command_path, char ** argv){
	execv(command_path, argv);
	pid_t pid_process;

	pid_process = fork();
	switch (pid_process){
	case -1:
		printf("Fork failure \n Exiting \n");
		exit(1);
		break;
	case 0:

		break;
	default:
		//wait(pid_process);
		break;
	}
}

void welcome_msg(){
	printf("------------------------------------------------------------------- \n");
	printf("------------------This is a shell project --------------------------\n");
	printf("------------------------------------------------------------------- \n");
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


int verify_command(char * command){
	//Verifies if the command is in the command_tab list.
	// if yes, returns its index, otherwise, returns -1
	int i;
	for(i = 0; i < max_i; i++){
		if(strcmp(command_tab[i].name, command) == 0 )
			return i;
	}
	return -1;
}
void interpret_command (char * line){
	int i;
	char * command_vect_tmp;
	char * command_vect;
	char * argv[128];
	char   command_path[256];
	int command_vect_index = 0;
	int exists = 0;

	if(line == NULL)
		return;

	//interpreting history command
	if(strncmp (line, "hist-", 5) == 0){
		interpret_history_command(line);
	}
	command_vect_tmp = strtok(line, " ");
	while(command_vect_tmp != NULL)
	  {
		if(command_vect_index == 0){
		command_vect = command_vect_tmp;
		printf ("command vect %d : %s\n",command_vect_index, command_vect);
		command_vect_index++;
	    command_vect_tmp = strtok (NULL, " ");
		}
		else{
			argv[command_vect_index - 1] = command_vect_tmp;
			printf ("arg index %d : %s\n",command_vect_index, argv[command_vect_index - 1]);
			command_vect_index++;
			command_vect_tmp = strtok (NULL, " ");
		}

	  }


		exists = verify_command(command_vect);
		if(exists == -1){
			printf("No such command. \n");
			return;
		}


		strcpy(command_path, command_tab[exists].path);
		strcat(command_path, command_tab[exists].name);
		 executeCommand(command_path,argv);



}

void interpret_history_command(char * line){
	/* Interpret history commands. Should begin with the string hist-.
	hist-save saves history list in history.txt,
	hist-read loads history.txt
	hist-list list history
	hist-clear clears history list
	 */
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
		printf("history cleared \n");
	}
	else
		printf("Unknown history command \n");
}

int compare( const void * word1, const void * word2) {
	CMD * w1 = (CMD *) word1;
	CMD * w2 = (CMD *) word2;
	return strcmp(w1->name, w2->name);
}
void construct_command_tab(){
	//prints the name of all the files in the directory in the static "command_tab" and puts it in alphabetical order
		DIR *dir;
		struct dirent *entry;


		dir = opendir ("/bin");

		if(dir != NULL){
			while((entry = readdir(dir)) != NULL){
				if (entry->d_type == DT_REG){
					strcpy(command_tab[max_i].name, entry->d_name);
					strcpy(command_tab[max_i].path, "/bin/");
					max_i++;
				}
			}
		}
		else{
			printf("Error opening /bin \n");
		}
		closedir(dir);

		dir = opendir ("/usr/bin");
		if(dir != NULL){
			while((entry = readdir(dir)) != NULL){
				if (entry->d_type == DT_REG){
					strcpy(command_tab[max_i].name, entry->d_name);
					strcpy(command_tab[max_i].path, "/usr/bin/");
					max_i++;
				}
			}
		}
		else{
			printf("Error opening /usr/bin \n");
		}
		closedir(dir);

		dir = opendir ("/sbin");
		if(dir != NULL){
			while((entry = readdir(dir)) != NULL){
				if (entry->d_type == DT_REG){
					strcpy(command_tab[max_i].name, entry->d_name);
					strcpy(command_tab[max_i].path, "/sbin/");
					max_i++;
				}
			}
		}
		else{
			printf("Error opening /sbin \n");
		}
		closedir(dir);

		qsort (command_tab, max_i, sizeof(CMD), compare);
}

void initialize_readline(){

	rl_readline_name = "Yaush";
	//rl_attempted_completion_function = completion;
}


char ** completion (const char * text, int start, int end){

	char **matches;
	  matches = (char **)NULL;
	  if (start == 0)
		    matches = rl_completion_matches (text, command_generator);


	  return (matches);

}

char * command_generator (const char * text, int state){

char  name[128];



  if (list_index == max_i)
    {
      list_index = 0;
      len = strlen (text);

    }

  /* Return the next name which partially matches from the command list. */
  while ( list_index < max_i)
      {
	  	strncpy(name, command_tab[list_index].name, len);


        if (strncmp (name, text, len) == 0){
        	return (dumpstring(command_tab[list_index].name));
        	printf("command_tab : %s;  name: %s; index = %d \n", command_tab[list_index].name, name, list_index);
        }
        list_index++;
      }

  printf("not found\n");
  return ((char *)NULL);
}


void init_shell(){
	using_history ();
	write_history ("history.txt");
	construct_command_tab();
	initialize_readline();
}



int main(int argc, char ** argv) {
	char *line, *history_tmp;


	init_shell();
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

		interpret_command(history_tmp);

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


