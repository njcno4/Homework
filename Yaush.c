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
			execv(command_path_2, argv_2);
			exit(EXIT_SUCCESS);
			break;

		default:
			dup2(par[1], STDOUT_FILENO);
			close(par[0]);close(par[1]);
			close(chd[0]); close(chd[1]);
			execv(command_path_1, argv_1);
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
		}
		if(redir_stdin == 1){
			devNull  = open(descriptor_name, O_RDONLY, 0600);
			dup2(devNull, STDIN_FILENO);
		}

		if(!state_bg){
			devNull = open("/dev/null", O_WRONLY);
			dup2(devNull, STDOUT_FILENO);
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
		break;


	}


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


