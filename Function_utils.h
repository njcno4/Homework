char * dumpstring (char *s)
{
	char *r;

	r = (char*) malloc (strlen (s) + 1);
	strcpy (r, s);
	return (r);
}




void cd(char ** argv){
	//changes directory
	if (argv[1] == NULL)
		chdir(getenv("HOME"));

	else{
		if (chdir(argv[1]) == -1)
			printf(" %s: no such directory\n", argv[1]);

	}
}

void interpret_command (char * line){

	char * command_vect_tmp;
	char * command_vect;
	char * argv[128] ;
	char * argv_1[64];
	char * argv_2[64];
	char command_path[256];
	char command_path_1[256];
	char command_path_2[256];
	int command_vect_index = 0;
	int exists = 0;
	int exists_1 = 0;
	int exists_2 = 0;
	int state_bg = 1;
	int i;
	int j;
	redir_stdout = 0;
	redir_stdin  = 0;
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
			argv[0] = command_vect_tmp;
			command_vect_index++;
			command_vect_tmp = strtok (NULL, " ");
		}
		else{
			argv[command_vect_index] = command_vect_tmp;
			command_vect_index++;
			command_vect_tmp = strtok (NULL, " ");
		}

	}

	if (command_vect_index > 1){

		if(strcmp(argv[command_vect_index - 2],">>") == 0){
			redir_stdout = 1;
			strcpy(descriptor_name , argv[command_vect_index -1]);
			command_vect_index = command_vect_index - 2;
		}

		if(command_vect_index > 1){
			if(strcmp(argv[command_vect_index - 2],"<<") == 0){
				redir_stdin = 1;
				strcpy(descriptor_name , argv[command_vect_index -1]);
				command_vect_index = command_vect_index - 2;
			}

		}
		for(i = 0; i < command_vect_index; i++){

			if(strcmp(argv[i],"|") == 0){
				for(j = 0; j < i; j++){
					argv_1[j] = argv[j];
				}
				argv_1[i] = NULL;

				for(j = 0; j < command_vect_index - i - 1; j++){
					argv_2[j] = argv[j+i+1];
				}
				argv_2[command_vect_index - i - 1] = NULL;

				exists_1 = verify_command(argv_1[0]);

				if(exists_1 == -1){
					printf("No such command. \n");
					return;
				}

				strcpy(command_path_1, command_tab[exists_1].path);
				strcat(command_path_1, command_tab[exists_1].name);

				exists_2 = verify_command(argv_2[0]);

				if(exists_2 == -1){
					printf("No such command. \n");
					return;
				}

				strcpy(command_path_2, command_tab[exists_2].path);
				strcat(command_path_2, command_tab[exists_2].name);


				executeCommand_pipe(command_path_1,  argv_1,  command_path_2,  argv_2);

				return;
			}
		}
	}

	if(strcmp(argv[command_vect_index - 1],"bg") == 0){
		state_bg = 0;
		command_vect_index --;
	}

	//printf("%d \n", command_vect_index);

	argv[command_vect_index] = NULL;



	if (strcmp (line, "cd") == 0){
		cd(argv);
		return;
	}
	if (strcmp (line, "exit") == 0){
		printf("Good Bye \n");
		exit(EXIT_SUCCESS);
	}

	exists = verify_command(command_vect);

	if(exists == -1){
		printf("No such command. \n");
		return;
	}




	strcpy(command_path, command_tab[exists].path);
	strcat(command_path, command_tab[exists].name);
	executeCommand(command_path,argv, state_bg);


	return;
}

void welcome_msg(){
	printf("------------------------------------------------------------------- \n");
	printf("------------------This is a shell project--------------------------\n");
	printf("------------------------------------------------------------------- \n");

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
