
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

#define BG 0;
#define FG 1;

int redir_stdout = 0;
int redir_stdin = 0;
char descriptor_name[128];
struct CMD{
	char name[128];
	char path[1024];
};
typedef struct CMD CMD;

static CMD command_tab[10192];

static int max_i = 0 ;

void interpret_history_command(char * line);

void executeCommand(char * command_path, char ** argv, int state_bg);
void welcome_msg();
char * test_white (char * line);
int verify_command(char * command);
void executeCommand_pipe(char * command_path_1, char ** argv_1, char * command_path_2, char ** argv_2);
int compare( const void * word1, const void * word2);
void construct_command_tab();
void initialize_readline();
void init_shell();
void interpret_command (char * line);
void cd (char ** argv);
char * dumpstring (char *s);
