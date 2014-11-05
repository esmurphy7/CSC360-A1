#include "shell.h"
#include <signal.h>

//======== Constants =========
static int const MAX_LINE_LENGTH = 100;
//============================

//===== Global Variables =====
process* headProcess = NULL; // The head of the bglist
//============================

int main(void)
{
	char user_input[MAX_LINE_LENGTH];
	
	signal(SIGCHLD, signalHandler);
	
	while(1)
	{
		printPrompt();
		fgets(user_input, MAX_LINE_LENGTH, stdin);
		// end the user's input with a newline so that it will be processed as end of cmd
		user_input[strlen(user_input)-1] = '\n';
		// append a null character to identify the end of the input
		user_input[strlen(user_input)] = '\0';
		//printf("user input: %s\n",user_input);
		
		char* input = user_input;
		char** cmdline = parse(input);
		if(cmdline[0] != NULL)
			executeCmdLine(cmdline);
	}
}

void printPrompt()
{
	char *rsi = "RSI";
	if(getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s: %s > ", rsi, cwd);
	else
		perror("getcwd() error");
}

void executeCmdLine(char** cmdline)
{
	char* firstCmd = cmdline[0];
	
	// identify the first cmd and execute accordingly
	if(strcmp(firstCmd, "cd") == 0)
	{
		char* targetDir=NULL;
		char* dir = cmdline[1];
		char* currentDir = getcwd(cwd, sizeof(cwd));
		if(currentDir == NULL)
			perror("NULL current directory ");
		
		// if there is no directory specified (cmdline[1] == NULL), default to home case
		if(dir == NULL)
		{
			dir = "~";
		}
		// if it's a home directory case (~)
		if(dir[0] == '~')
		{
			// strip the tilde character from dir
			char* withoutTilde = ++dir;
			//printf("without tilde: %s\n",newDir);
			char* homeDir = "/home";
			// append dir withoutTilde to homeDir
			targetDir = strAppend(homeDir, withoutTilde);
		}
		// if it's an "up one level" case (..)
		else if(dir[0] == '.' && dir[1] == '.')
		{
			// remove the ".." from dir from the beginning of dir
			++dir;
			++dir;
			char* argWithoutUOL = dir;
			// remove the current folder from the end of working dir path
			int i = strlen(currentDir);
			while(currentDir[i] != '/' && i >= 0)
			{
				currentDir[i] = '\0';
				i--;
			} 
			// strip the '/' character at the end
			currentDir[strlen(currentDir)-1] = '\0';
			char* pathWithoutcwd = currentDir;
			// append argWithoutUOL to pathWithoutcwd
			targetDir = strAppend(pathWithoutcwd, argWithoutUOL);
		}
		else
		{
			targetDir = dir;
		}
		// change directory to the targetDir
		if(targetDir)
		{
			if(chdir(targetDir) == 0)
				currentDir = getcwd(currentDir, sizeof(cwd));	
		}
	}
	else if(strcmp(firstCmd, "bg") == 0)
	{
		// strip the "bg" cmd from the front cmdLine
		++cmdline;
		firstCmd = cmdline[0];
		int childpid = fork();
		if(childpid >= 0)
		{
			if(childpid == 0)
			{
				if(execvp(firstCmd, cmdline) < 0)
				{
					perror("Error on execvp\n");
					exit(0);
				}
			}
			else
			{
				// add the child process to the bg list
				if(headProcess == NULL)
					headProcess = create_list(childpid, firstCmd);
				else
					add_to_list(childpid, firstCmd, true);
			}
		}
		else
		{
			perror("fork() error");
		}
	}
	else if(strcmp(firstCmd, "bglist") == 0)
	{
		print_list();
	}
	else if(strcmp(firstCmd, "kill") == 0)
	{
		char* pidCmd = cmdline[1];
		if(!pidCmd)
			printf("Please specify a pid\n");
		else
		{
			int pid = atoi(pidCmd);
			process* target = search_in_list(pid, &headProcess);
			if(target != NULL)
			{
				//printf("killing %d \n", target->pid);
				if(kill(target->pid, SIGTERM) >= 0){
					delete_from_list(pid);
				}else{
					perror("could not kill pid specified");
				}
			}
			else
			{
				printf("Please specify an active pid\nType \"bglist\" for list of active processes\n");
			}
		}
	}
	else if(strcmp(firstCmd, "q") == 0)
	{
		printf("Quitting...\n");
		kill_list();
		exit(0);
	}
	else // all other internal commands
	{
		int childpid = fork();
		if(childpid < 0)
			perror("fork() failed");
		else{
			if(childpid == 0)
			{
				if(execvp(firstCmd, cmdline) < 0)
					perror("Error on execvp\n");
			}
			else
			{
				//printf("Waiting for child process to finish...\n");
				int status;
				wait(&status);
			}
		}
		
	}
}

static void signalHandler (int sig)
{
	int pid;
	int status;

	pid = waitpid(-1, &status, WNOHANG);
	broadcastTermination(pid, status);
}

void broadcastTermination(int pid, int status)
{
	if (WIFEXITED(status)) {
		printf("exited, status=%d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("killed by signal %d\n", WTERMSIG(status));
	} else if (WIFSTOPPED(status)) {
		printf("stopped by signal %d\n", WSTOPSIG(status));
	} else if (WIFCONTINUED(status)) {
		printf("continued\n");
	}
	delete_from_list(pid);
}
