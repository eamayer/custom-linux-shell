#pragma once
//----------------------------------------------------
/*Required files*/
//----------------------------------------------------


#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#include <errno.h>
#include <stdbool.h>
#include <unistd.h>



//----------------------------------------------------
/*Structures*/
//----------------------------------------------------

/* struct for movie information*/
typedef struct Command
{
	char* argArray[5];
	char* inputFile;
	char* outputFile;
	char* commandCall;
	int  numOfArgs;
	bool background;
} Command;

typedef void sigfunc(int);
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
int externalProcess(Command* userInput, int* codeStatus);
void freeUp(Command* parsedInput);
void getCommand();
void handleSigIntInChild();
void handleSigInt(int status);
void handleSigStp(int signo);
void printArray(char** argArray, int argumentsTotal);
void processCommand(Command* userInput, int* codeStatus);
void statusCheck(int codeStatus);
void redirectionNeeded(Command* userInput);

#endif // !FILES_H_


//CONSTANTS
#define MAX_INPUT_LENGTH 2048
#define MAX_NUM_ARGS 512
#define MAX_BACKGROUND_PROCCESES 25

//GLOBAL VARIABLES
pid_t processTracker[MAX_BACKGROUND_PROCCESES];  //keeps all the process PIDs
int numOfProcesses = 0;
bool ignoreBackground = false;
int numSigStpEntered = 0;
bool sigIntTriggered = false;

void func(int signum)
{
	wait(NULL);
}


void checkBackgroundProcesses()
{
	for (int argPos = 0; argPos <= MAX_BACKGROUND_PROCCESES; argPos++) 
	{
		if (processTracker[argPos] != 0)
		{
			int childStatus;
			pid_t childPid = processTracker[argPos];
			if (waitpid(childPid, &childStatus, WNOHANG))
			{
				printf("The background process %d completed: ", childPid);
				statusCheck(childStatus);
				processTracker[argPos] = 0;
			}
		}
	}
}

void changeDir(Command* parsedInt)
{
	if (parsedInt->argArray[1] == NULL) {
		chdir(getenv("HOME"));			// moves to home directory of file
	}
	else {
		chdir(parsedInt->argArray[1]);
	}  //moves to the path of the user's input

}

Command* createCommandStruc(char* input)
{
	Command* userCommand = malloc(sizeof(Command));
	int argPosition = 0;

	userCommand->inputFile = NULL;
	userCommand->outputFile = NULL;

	if (userCommand != NULL)
	{
		char* tokenptr;
		tokenptr = strtok(input, " ");

		//setting the command variable
		userCommand->commandCall = calloc(strlen(tokenptr) + 1, sizeof(char));
		strcpy(userCommand->commandCall, tokenptr);

		while (tokenptr != NULL)
		{
			if (strcmp(tokenptr, "<") == 0)
			{
				tokenptr = strtok(NULL, " ");
				userCommand->inputFile = calloc(strlen(tokenptr) + 1, sizeof(char));
				strcpy(userCommand->inputFile, tokenptr);
				tokenptr = strtok(NULL, " ");
			}
			else if (strcmp(tokenptr, ">") == 0)
			{
				tokenptr = strtok(NULL, " ");
				userCommand->outputFile = calloc(strlen(tokenptr) + 1, sizeof(char));
				strcpy(userCommand->outputFile, tokenptr);
				tokenptr = strtok(NULL, " ");
			}
			else
			{
				userCommand->argArray[argPosition] = calloc(strlen(tokenptr), sizeof(char*));
				strcpy(userCommand->argArray[argPosition], tokenptr);
				tokenptr = strtok(NULL, " ");
				argPosition++;
			}
		}
		userCommand->argArray[argPosition] = NULL;
	}
	else {
		printf("I'm sorry but we cannot create a userCommand");
	}

	//whether it needs to be a background or foreground process
	if (strcmp(userCommand->argArray[argPosition - 1], "&") == 0)
	{
		if (ignoreBackground == false)
		{
			userCommand->background = true;
		}
		else
		{
			userCommand->background = false;			// this handles if the program is in foreground mode only
		}
		userCommand->argArray[argPosition - 1] = NULL;  /// remove it from the list
		argPosition--;
	}
	else
	{
		userCommand->background = false;
	}

	userCommand->numOfArgs = argPosition;

	return userCommand;
}

void exitCommand(Command* inputUser)
{
	freeUp(inputUser);
	exit(0);
}

/// <summary>
/// CITATION: The idea to use sprintf along with "%d" came from an office hour with 
/// the instructor. 
/// </summary>
/// <param name="userInput"></param>
void expandPID(char* userInput)
{
	char userInputTemp[2048];
	int	 pidID = getpid();

	strcpy(userInputTemp, userInput);

	for (int i = 0; i < strlen(userInputTemp); i++)
	{
		if (userInputTemp[i] == '$' && userInputTemp[i + 1] == '$')
		{
			userInputTemp[i] = '%';
			userInputTemp[i + 1] = 'd';
		}
	}
	sprintf(userInput, userInputTemp, pidID);
}

int externalProcess(Command* userInput, int* status)
{
	char** argArray = userInput->argArray;
	pid_t spawnPid = -5;
	spawnPid = fork();

	if (spawnPid == -1)
	{
		fflush(stdout);
		exit(1);
	}
	else if (spawnPid == 0)	// child processes
	{
		int childPid = getpid();
		
		//as the sigstp is not inherited, must set again
		signal(SIGTSTP, SIG_IGN);

		if (userInput->inputFile || userInput->outputFile)
		{
			redirectionNeeded(userInput);
		}

		if (userInput->background == false)
		{
			signal(SIGINT, handleSigIntInChild);		// reset the signal handler for sig int to allow for ctrl+c to work on foreground processes
			
			if (sigIntTriggered == true)
			{
				kill(childPid, SIGTERM);
				handleSigInt(*status);
			}

		}

		fflush(stdout);

		execvp(argArray[0], argArray);
		printf("%s command is not found\n", argArray[0]);
		fflush(stdout);


		exit(2);
	}
	else
	{
		signal(SIGTSTP, handleSigStp);

		if (userInput->background != true)  // run on foreground, wait for child to process
		{
			spawnPid = waitpid(spawnPid, status, 0);

			if (sigIntTriggered == true)
			{
				handleSigInt(*status);
			}
		}
		else  // runs on background
		{
			processTracker[numOfProcesses] = spawnPid;
			printf("process started for child %d \n", spawnPid);
			numOfProcesses++;
		}
	}
	return 0;
}

void freeUp(Command* parsedInput)
{
	for (int i = 0; i < parsedInput->numOfArgs; i++) {
		free(parsedInput->argArray[i]);
	}

	free(parsedInput->commandCall);
	free(parsedInput->inputFile);
	free(parsedInput->outputFile);
	free(parsedInput);
}

void getCommand()
{
	static int codeStatus = 0;
	char* userInput = malloc(MAX_INPUT_LENGTH * sizeof(char)); //need to set extra length in case of pidExpansion
	size_t len = 0;
	signal(SIGINT, SIG_IGN);  // ignore ctrl+c for all procceses
	signal(SIGTSTP, handleSigStp);  // ignore ctrl+z for all procceses


	//struc sigaction SIGSTP_action = { 0 };  // initialize the SIGSTP to be empty

	printf(":");
	fflush(stdout);

	getline(&userInput, &len, stdin);
	userInput[strlen(userInput) - 1] = '\0'; // remove added '\n' by getline

	expandPID(userInput);

	if (strlen(userInput) == 0 || userInput[0] == '#' || strlen(userInput) > MAX_INPUT_LENGTH)
	{
		free(userInput);
		getCommand();
	}
	else
	{
		Command* parsedInput = createCommandStruc(userInput);
		free(userInput);
		processCommand(parsedInput, &codeStatus);
		freeUp(parsedInput);
	}
	checkBackgroundProcesses();
	getCommand();
}

void handleSigInt(int status)
{
	statusCheck(status);
	sigIntTriggered = false;
}

void handleSigIntInChild()
{
	sigIntTriggered = true;
}

void handleSigStp(int signo)
{
	if (numSigStpEntered == 0)
	{
		char* enterMessage = "Entering foreground-only mode (& is now ignored)\n";
		write(STDOUT_FILENO, enterMessage, 50);
		ignoreBackground = true;
		numSigStpEntered++;
	}
	else
	{
		char* exitMessage = "Exiting foreground-only mode\n";
		write(STDOUT_FILENO, exitMessage, 29);
		ignoreBackground = false;
		numSigStpEntered--;
	}
}

void printArray(char** argArray, int argumentsTotal)
{
	for (int i = 0; i < argumentsTotal; i++) {
		// inner loop for column
		for (int j = 0; j < 1; j++) {
			printf("%s ", &argArray[i][j]);
		}
		printf("\n"); // new line
	}
}

void processCommand(Command* userInput, int* status)
{
	char** argArray = userInput->argArray;
	char* firstArg = &argArray[0][0];

	if (strcmp(firstArg, "cd") == 0) {
		changeDir(userInput);
	}
	else if (strcmp(firstArg, "status") == 0) {
		statusCheck(*status);
	}
	else if (strcmp(firstArg, "exit") == 0) {
		exit(0);
	}
	else {
		externalProcess(userInput, status);
	}
}

void redirectionNeeded(Command* userInput)
{
	bool background = userInput->background;

	if (userInput->inputFile || background == true)
	{
		int sourceFD;

		// Open target file
		if (userInput->inputFile)
		{
			sourceFD = open(userInput->inputFile, O_RDONLY);

			//error check
			if (sourceFD == -1) {
				perror("source open()");
				exit(1);
			}

		}
		else // run on background, must be rerouted since not specified by user
		{
			sourceFD = open("/dev/null", O_RDONLY);
		}
		// Written to terminal
		//printf("sourceFD == %d\n", sourceFD);

		// Redirect stdin to source file
		int result = dup2(sourceFD, 0);
		if (result == -1) {
			perror("source dup2()");
			exit(2);
		}
	}

	if (userInput->outputFile || background == true)
	{
		int targetFD;

		// Open target file
		if (userInput->outputFile) {
			targetFD = open(userInput->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (targetFD == -1) {
				perror("target open()");
				exit(1);
			}

		}
		else // run on background, must be rerouted since not specified by user
		{
			targetFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		//printf("targetFD == %d\n", targetFD); // Written to terminal

		// Redirect stdout to target file
		int result2 = dup2(targetFD, 1);
		if (result2 == -1) {
			perror("target dup2()");
			exit(2);
		}
	}

}

void statusCheck(int status)
{
	if (WIFEXITED(status)) {
		printf("exit status %d\n", WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status)) {
		printf("terminated by signal %d\n", WTERMSIG(status));
	}
	else
	{
		printf("exit status %d", status);
	}
	fflush(stdout);
}