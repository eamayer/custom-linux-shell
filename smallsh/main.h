#pragma once
//----------------------------------------------------
/*Required files*/
//----------------------------------------------------

#include <ctype.h>
#include <execinfo.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

//----------------------------------------------------
/*Structures*/
//----------------------------------------------------

/* struct for movie information*/
typedef struct Command
{
	char* argArray[512];
	char* inputFile;
	char* outputFile;
	char* commandCall;
	int  numOfArgs;
	bool background;
} Command;

//----------------------------------------------------
/*Function List*/
//----------------------------------------------------

#ifndef FILES_H_
#define FILES_H_ 

void checkBackgroundProcesses();
void changeDir(Command* parsedInt);
Command* createCommandStruc(char* input);
void exitCommand(Command* parsedInt);
void expandPID(char* userInput);
int externalProcess(Command* userInput);
void freeUp(Command* parsedInput);
int getCommand();
void handleSIGINT(int status);
void handleSTSTP(int sig);
void processCommand(Command* userInput);
void statusCheck();
void setHandlers();
void redirectionNeeded(Command* userInput);
bool validateInput(char* userInput);

#endif // !FILES_H_

//----------------------------------------------------
/*Constants*/
//----------------------------------------------------

//CONSTANTS
#define _GNU_SOURCE
#define __USE_GNU
#define MAX_INPUT_LENGTH 2048
#define MAX_NUM_ARGS 512
#define MAX_BACKGROUND_PROCCESES 25

//----------------------------------------------------
/*Global Variables*/
//----------------------------------------------------
pid_t processTracker[MAX_BACKGROUND_PROCCESES];  //keeps all the process PIDs
int numOfProcesses = 0;
volatile bool ignoreBackground = false;
int codeStatusGlobal = 0; 

//----------------------------------------------------
/*Signal Handlers*/
//----------------------------------------------------

/// <summary>
/// Outputs nothing. Purpose is to make having the children not inherit it due to needing it to interact with
/// ignoring the background.
/// </summary>
/// <param name="sig"></param>
void handleSIGINT(int sig) {
	write(STDOUT_FILENO, "", 1);
}

/// <summary>
/// switches the "ignoreBackground" flag and prints wether it is entering or exiting
/// </summary>
/// <param name="sig"></param>
void handleSTSTP(int sig) {

	if (ignoreBackground == false) {
		ignoreBackground = true;
		char* message = "\nEntering foreground-only mode (& is now ignored)\n";
		write(STDOUT_FILENO, message, 50);
	}
	else {
		ignoreBackground = false;
		char* exitMessage = "\nExiting foreground-only mode\n";
		write(STDOUT_FILENO, exitMessage, 30);
	}
}

//----------------------------------------------------
/*All other functions*/
//----------------------------------------------------

/// <summary>
/// At the end of every command input (except for exit), all background processes are checked to 
/// see if they have completed and print information on them.
/// </summary>
void checkBackgroundProcesses() {
	for (int argPos = 0; argPos <= MAX_BACKGROUND_PROCCESES; argPos++) {
		if (processTracker[argPos] != 0) {
			int childStatus;
			pid_t childPid = processTracker[argPos];

			if (waitpid(childPid, &childStatus, WNOHANG)) {
				printf("The background process %d completed: ", childPid);
				if (WIFEXITED(childStatus)) {
					printf("exit status %d\n", WEXITSTATUS(childStatus));
					fflush(stdout);
				}
				else if (WIFSIGNALED(childStatus)) {
					printf("terminated by signal %d\n", WTERMSIG(childStatus));
					fflush(stdout);
				}
				processTracker[argPos] = 0;
			}
		}
	}
}

/// <summary>
/// changes the directory. If none specified, it goes to the home directory.
/// </summary>
/// <param name="parsedInt"></param>
void changeDir(Command* parsedInt) {
	if (parsedInt->argArray[1] == NULL) {
		chdir(getenv("HOME"));			// moves to home directory of file
	}
	else {
		chdir(parsedInt->argArray[1]); 	//moves to the path of the user's input
	}  
}

/// <summary>
/// Creates the structure that contains all the information from the userInput in a manner that will allow for easy 
/// processing later. Does not add ">", "<" or the input/output file names to the argument array so they do not get passed
/// to execvp[] later. Checks to see if it is a background process by seeing if "&" is the last argument. After checking
/// it removes it from the argumentArray so it does not get passed into the execvp[].
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
Command* createCommandStruc(char* input) {
	Command* userCommand = malloc(sizeof(Command));
	int argPosition = 0;

	userCommand->inputFile = NULL;
	userCommand->outputFile = NULL;

	if (userCommand != NULL) {
		char* tokenptr;
		tokenptr = strtok(input, " ");

		//setting the command variable
		userCommand->commandCall = calloc(strlen(tokenptr) + 1, sizeof(char));
		strcpy(userCommand->commandCall, tokenptr);

		while (tokenptr != NULL) {
			// check for input and output files. 
			if (strcmp(tokenptr, "<") == 0) {
				tokenptr = strtok(NULL, " ");
				// need to verify that an input file is provided
				if (tokenptr == NULL)
				{
					printf("No input file provided\n");
					fflush(stdout);
					getCommand();
				}
				else {
					userCommand->inputFile = calloc(strlen(tokenptr) + 1, sizeof(char));
					strcpy(userCommand->inputFile, tokenptr);
					tokenptr = strtok(NULL, " ");
				}
			}
			else if (strcmp(tokenptr, ">") == 0) {
				tokenptr = strtok(NULL, " ");
				// need to verify that an output file is provided
				if (tokenptr == NULL)
				{
					printf("No output file provided\n");
					fflush(stdout);
					getCommand();
				}
				else {
					userCommand->outputFile = calloc(strlen(tokenptr) + 1, sizeof(char));
					strcpy(userCommand->outputFile, tokenptr);
					tokenptr = strtok(NULL, " ");
				}
			}
			// handle all non-file related arguments
			else {
				userCommand->argArray[argPosition] = calloc(strlen(tokenptr), sizeof(char*));
				strcpy(userCommand->argArray[argPosition], tokenptr);
				tokenptr = strtok(NULL, " ");
				argPosition++;
			}
		}
		// add final "NULL" spot at the end to allow for use of execvp[];
		userCommand->argArray[argPosition] = NULL;
	}
	else {
		printf("I'm sorry but I cannot create this userCommand");
	}

	//checking whether it needs to be a background or foreground process
	if (strcmp(userCommand->argArray[argPosition - 1], "&") == 0) {
		userCommand->background = true;
		userCommand->argArray[argPosition - 1] = NULL;  // remove it from the list
		argPosition--;
	}
	else {
		userCommand->background = false;
	}

	userCommand->numOfArgs = argPosition;

	return userCommand;
}

/// <summary>
/// Kills off all remaining background processes prior to exiting.
/// </summary>
/// <param name="inputUser"></param>
void exitCommand(Command* inputUser) {
	for (int argPos = 0; argPos <= MAX_BACKGROUND_PROCCESES; argPos++) {
		if (processTracker[argPos] != 0) {
			pid_t childPid = processTracker[argPos];
			kill(childPid, SIGTERM);
			processTracker[argPos] = 0;
			}
		}
	freeUp(inputUser);
	exit(0);
}

/// <summary>
/// Expands any "$$" to the pidID for the user input.
/// </summary>
/// <param name="userInput"></param>
/// /// CITATION: The idea to use sprintf along with "%d" came from an office hour with 
/// the instructor. 
void expandPID(char* userInput)
{
	char userInputTemp[2049];
	int	 pidID = getpid();

	// copy the userInput over to a new string that will serve as the "format"
	strcpy(userInputTemp, userInput);

	for (int i = 0; i < strlen(userInputTemp); i++)
	{
		// if the characters are "$$" then replace with "%d" to be able to use in sprintf as the "format"
		if (userInputTemp[i] == '$' && userInputTemp[i + 1] == '$')
		{
			userInputTemp[i] = '%';
			userInputTemp[i + 1] = 'd';
		}
	}
	//copies the userInputTemp into userInput while replacing the "%d" with the pidIDs.
	sprintf(userInput, userInputTemp, pidID);
}

/// <summary>
/// Takes the userInput in structure for non-built in functions and processes it.
/// Redirects the files as needed. Foreground child process does not ignore ctrl+c.
/// </summary>
/// <param name="userInput"></param>
/// <returns></returns>
int externalProcess(Command* userInput) {
	char** argArray = userInput->argArray;
	pid_t spawnPid = -5;
	spawnPid = fork();

	if (spawnPid == -1) {
		fflush(stdout);
		exit(1);
	} 
	// child process
	else if (spawnPid == 0)	{

		// set up for all children to ignore SIGTSTP
		struct sigaction ignore_action = { { 0 } };
		ignore_action.sa_handler = SIG_IGN;
		sigaction(SIGTSTP, &ignore_action, NULL);

		// background children need to ignore SIGINT. Need to add as SIGINT was set up initially to have a handler
		// and it isn't inherited
		if (userInput->background == true && ignoreBackground == false) {
			sigaction(SIGINT, &ignore_action, NULL);
		}
		if (userInput->inputFile || userInput->outputFile) {
			redirectionNeeded(userInput);
		}

		execvp(argArray[0], argArray);
		printf("%s command is not found\n", argArray[0]);
		fflush(stdout);
		codeStatusGlobal = 1;
		exit(1);
	}
	else {
		// run on foreground, wait for child to process. If bad exit, then prints that it was
		// terminated.
		if (userInput->background != true || ignoreBackground == true) {
			spawnPid = waitpid(spawnPid, &codeStatusGlobal, 0);
			if (WIFSIGNALED(codeStatusGlobal)) {
				printf("teminated by signal %d\n", WTERMSIG(codeStatusGlobal));
				fflush(stdout);
			}
		}
		// runs on background so add to the background process tracker
		else { 
			processTracker[numOfProcesses] = spawnPid;
			printf("process started for child %d \n", spawnPid);
			fflush(stdout);
			numOfProcesses++;
		}
	}
	return 0;
}	

/// <summary>
/// Cleans up after command has been processed
/// </summary>
/// <param name="parsedInput"></param>
void freeUp(Command* parsedInput) {
	for (int i = 0; i < parsedInput->numOfArgs; i++) {
		free(parsedInput->argArray[i]);
	}
	free(parsedInput->commandCall);
	free(parsedInput->inputFile);
	free(parsedInput->outputFile);
	free(parsedInput);
}

/// <summary>
/// Gets the command from the user. Expands it for "$$" if needed. Creates a structure from it 
/// and processes it. Clears out everything after to start over.
/// </summary>
/// <returns></returns>
/// 
/// CITATIONS:
/// while (1) read error check
/// SOURCE: https://people.gnome.org/~federico/blog/rust-libstd-syscalls-and-errors.html
/// DATE: 06Feb2022
int getCommand() {
	char* userInput = malloc(MAX_INPUT_LENGTH * sizeof(char) + 1); //need to set extra length in case of pidExpansion
	

	checkBackgroundProcesses();
	//this is needed to avoid a infinite repeat of ":" when the foreground only process is invoked.
	while (1) {
		size_t len = 0;
		ssize_t read;

		printf(": ");
		fflush(stdin);

		// read gets automatically restarted upon return from a signal handles
		read = getline(&userInput, &len, stdin);
		if (read == -1) {
			if (errno == EINTR)
			{
				clearerr(stdin);
				free(userInput);
			}
			else {
				return -1;
			}
		}
		else
		{
			break;
		}
	}
	fflush(stdout);

	if (validateInput(userInput) == false) {
		free(userInput);
		getCommand();
	}
	else {
		userInput[strlen(userInput) - 1] = '\0'; // remove added '\n' by getline
		expandPID(userInput);
		Command* parsedInput = createCommandStruc(userInput);
		free(userInput);
		processCommand(parsedInput);

		freeUp(parsedInput);
	}
	return 0;
}


/// <summary>
/// processes commands given by user. Either uses built in functions for cd,
/// status and exit or redirects for external processes.
/// </summary>
/// <param name="userInput"></param>
void processCommand(Command* userInput) {
	char** argArray = userInput->argArray;
	char* firstArg = &argArray[0][0];

	if (strcmp(firstArg, "cd") == 0) {
		changeDir(userInput);
		codeStatusGlobal = 0;
	}
	else if (strcmp(firstArg, "status") == 0) {
		statusCheck();
		codeStatusGlobal = 0;
	}
	else if (strcmp(firstArg, "exit") == 0) {
		exitCommand(userInput);
	}
	else {
		externalProcess(userInput);
	}
}

/// <summary>
/// Takes the userInput and puts the input and/or output files in the correct sources.
/// For background processes, it redirects to "/dev/null"
/// </summary>
/// <param name="userInput"></param>
/// CITATION: Adapted the file handling from: 
/// SOURCE: OSU CS344 Module 5 Processes II, Exploration: Processes and I/O
/// DATE: 06Feb2022
void redirectionNeeded(Command* userInput) {
	bool background = userInput->background;

	if (userInput->inputFile || background == true) {
		int sourceFD;

		// Open target file
		if (userInput->inputFile) {
			sourceFD = open(userInput->inputFile, O_RDONLY);
			//error check
			if (sourceFD == -1) {
				perror("error opening input file");
				codeStatusGlobal = 1;
				exit(1);
			}
		}
		else if (background == true) { // run on background, must be rerouted since not specified by user
			sourceFD = open("/dev/null", O_RDONLY);
		}
	
		// Redirect stdin to source file
		int result = dup2(sourceFD, 0);
		if (result == -1) {
			perror("error redirecting to source file");
			codeStatusGlobal = 1;
			exit(1);
		}
	}

	if (userInput->outputFile || background == true) {
		int targetFD;
		// Open target file
		if (userInput->outputFile) {
			targetFD = open(userInput->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (targetFD == -1) {
				perror("error opening output file");
				codeStatusGlobal = 1;
				exit(1);
			}
			printf("\n"); // new line
		}
		else if (background == true) { // run on background, must be rerouted since not specified by user
			targetFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		// Redirect stdout to target file
		int result2 = dup2(targetFD, 1);
		if (result2 == -1) {
			perror("error redirecting to target file");
			codeStatusGlobal = 1;
			exit(1);
		}
	}
}

/// <summary>
/// Set signal handlers for Ctrl+Z and Ctrl+C to be handled via specified handlers. 
/// Ensures they are not inherited by the children.
/// </summary>
/// CITATION: Adapted how to set signals from: 
/// SOURCE: OSU CS344 Module 5 Processes II, Exploration: Signal Handling API
/// DATE: 06Feb2022
void setHandlers() {
	struct sigaction SIGINT_action = { { 0 } }, SIGTSTP_action = { { 0 } };

	//remove SIGINT from main process only by using a handler to ensure that it does not
	// get inherited by the childres
	SIGINT_action.sa_handler = handleSIGINT;
	/* block all catchables while it is running*/
	sigfillset(&SIGINT_action.sa_mask);
	SIGINT_action.sa_flags = 0;
	//install out signal handler
	sigaction(SIGINT, &SIGINT_action, NULL);

	//set parent process to handle sigstp appropriately;
	SIGTSTP_action.sa_handler = handleSTSTP;
	/* block all catchables while it is running*/
	sigfillset(&SIGTSTP_action.sa_mask);
	SIGINT_action.sa_flags = SA_RESTART;
	//install our signal handler
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}

/// <summary>
/// Prints how the last foreground function was completed.
/// </summary>
void statusCheck() {
	if (WIFEXITED(codeStatusGlobal)) {
		printf("exit status %d\n", WEXITSTATUS(codeStatusGlobal));
		fflush(stdout);
	}
	else if (WIFSIGNALED(codeStatusGlobal)) {
		printf("terminated by signal %d\n", WTERMSIG(codeStatusGlobal));
		fflush(stdout);
	}
}

/// <summary>
/// Ensures the userInput is not empty, longer than max length, or starts with a #
/// </summary>
/// <param name="userInput"></param>
/// <returns></returns>
bool validateInput(char* userInput) {
	if (strlen(userInput) == 0 || userInput[0] == '#' || strlen(userInput) > MAX_INPUT_LENGTH || userInput[0] == '\n') {
		printf("\n"); // new line
		return false;
	}
	else {
		return true;
	}
}