#include "Utils.h"
#include "Function_utils.h"




void executeCommand_pipe(char * command_path_1, char ** argv_1, char * command_path_2, char ** argv_2){
	// execute program when | is typed
	pid_t pid_process;
	pid_t pid_process_2;
	pid_process = fork();
	int status1;
	int par[2], chd[2];
	pipe(par);
	pipe(chd);

	switch (pid_process){

	case -1:
		printf("Fork failure \n Exiting \n");
		exit(1);
		break;

	case 0:

		pid_process_2 = fork();
		switch (pid_process_2){

			case -1:
				printf("Fork failure \n Exiting \n");
				exit(1);
				break;

			case 0:
				dup2(par[0], STDIN_FILENO);
				close(par[0]);close(par[1]);
				close(chd[0]); close(chd[1]);
				printf("Ici grep \n");
				execv(command_path_2, argv_2);
				printf("Fin grep \n");
				exit(EXIT_SUCCESS);
				break;

			default:
				dup2(par[1], STDOUT_FILENO);
				close(par[0]);close(par[1]);
				close(chd[0]); close(chd[1]);
				printf("Ici ps \n");
				execv(command_path_1, argv_1);
				printf("Fin ps \n");
				break;
			}
		exit(EXIT_SUCCESS);
		break;

	default:

		waitpid(pid_process, &status1, WUNTRACED);

		break;

	}




}

void executeCommand(char * command_path, char ** argv, int state_bg){
	// executes the programm in command_path with arguments in argv.

	pid_t pid_process;
	pid_process = fork();
	int status;

	int devNull;


	switch (pid_process){

	case -1:
		printf("Fork failure \n Exiting \n");
		exit(1);
		break;

	case 0:
		if(redir_stdout == 1){
			devNull  = open(descriptor_name, O_CREAT | O_TRUNC | O_WRONLY, 0600);
			dup2(devNull, STDOUT_FILENO);
		}else{

			if(!state_bg){
				devNull = open("/dev/null", O_WRONLY);
				dup2(devNull, STDOUT_FILENO);
			}
		}

		execv(command_path, argv);
		exit(EXIT_SUCCESS);
		break;

	default:


		if(state_bg){
			waitpid(pid_process, &status, WUNTRACED);
		}
		else{
			printf("+ [%d] %s \n", (int) pid_process, command_path);
		}
		printf("Exit status: %d \n", status);
		break;


	}


}


char * test_white (char * line){
	//Remove white spaces in at the beginning or at the end of the line
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
	char * new_line =  (char*) malloc((up_index + 1) * sizeof(char));


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
	char * path_vect;
	char * path_buf;

	path_vect = getenv("PATH");

	path_buf = strtok (path_vect,":");
	while (path_buf != NULL)
	{
		dir = opendir (path_buf);
		if(dir != NULL){
			while((entry = readdir(dir)) != NULL){
				if (entry->d_type == DT_REG){
					strcpy(command_tab[max_i].name, entry->d_name);
					strcpy(command_tab[max_i].path, path_buf);
					strcat(command_tab[max_i].path, "/");
					max_i++;
				}
			}
		}
		else{
			printf("Error opening /bin \n");
		}
		closedir(dir);


		path_buf = strtok (NULL, ":");
	}


	qsort (command_tab, max_i, sizeof(CMD), compare);

}

void initialize_readline(){
	rl_readline_name = "Yaush";
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



		free(line);
		free(history_tmp);

	}
	write_history ("history.txt");
	return EXIT_SUCCESS;
}


