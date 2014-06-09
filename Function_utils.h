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
		else
			redir_stdout = 0;

		if(strcmp(argv[command_vect_index - 2],"<<") == 0){
			redir_stdin = 1;
			strcpy(descriptor_name , argv[command_vect_index -1]);
			command_vect_index = command_vect_index - 2;
		}
		else
			redir_stdin = 0;

		for(i = 0; i < command_vect_index; i++){

			if(strcmp(argv[i],"|") == 0){
				for(j = 0; j < i; j++){
					argv_1[j] = argv[j];
				}
				argv_1[i] = NULL;

				for(j = 0; j < command_vect_index - i - 1; j++){
					printf("argv[%d] = %s \n", j + i + 1, argv[j+i+1]);
					argv_2[j] = argv[j+i+1];

					printf("argv_2[%d] = %s \n", j, argv_2[j]);
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
