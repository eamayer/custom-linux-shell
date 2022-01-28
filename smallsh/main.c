/*Project: Assignment 3
Class: CS 344 Winter 2022
Author: Elizabeth Mayer
Date: 15Jan2022

Description: Program that asks user if they want to select a file to process. They then can choose
the file they want to process from smallest, largest or specific one they enter. It finds the correct file
and creates a new directory for it and then processes the file (must be in Movie format) by creating a .txt file
for movies released in each year. After completion asks user if they would like to process another file
*/

//----------------------------------------------------
/*Files needed*/
//----------------------------------------------------

#include "main.h"

#define MAX_INPUT_LENGTH 2048
#define MAX_NUM_ARGS 512
#define MAX_RUNS 2

//----------------------------------------------------
/*Entry point
Entry point into program. Asks user for input and evaluates it appropriately to ensure
appropriate processing*/
//----------------------------------------------------
int runTracker = 0;

int main()
{
	getCommand();
	return 0;
}

int externalProcess(int argv, Command* userInput, int* codeStatus)
{	
	char** argArray = userInput->argArray;
	printf("hello world (pid:%d)\n", (int)getpid());
	fflush(stdout);
	pid_t spawnPid = -5;
	spawnPid = fork();
	if (spawnPid < 0) {
		printf(stderr, "fork failed\n");
		fflush(stdout);
		exit(1);
	} 
	else if (spawnPid == 0){		//create child process
		printf("hello, I am child (pid:%d)\n", (int)getpid());
		fflush(stdout);
		if (userInput->inputFile || userInput->outputFile)
		{
			redirectionNeeded(userInput);
		}
		else
		{
			execvp(argArray[0], argArray);
			perror("execvp");
		}
		printf("this shouldn’t print out");
	}
	else
	{
		spawnPid = waitpid(spawnPid, codeStatus, WNOHANG);
		printf("hello, I am parent of %d (rc_wait:%d)\n",
			spawnPid, (int)getpid());
		fflush(stdout);
	}
	return 0;

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

	if (userCommand != NULL)
	{
		char* tokenptr;
		tokenptr = strtok(input, " ");

		while (tokenptr != NULL)
		{
			userCommand->argArray[argPosition] = calloc(strlen(tokenptr) + 1, sizeof(char*));
			strcpy(userCommand->argArray[argPosition], tokenptr);
			tokenptr = strtok(NULL, " ");
			argPosition++;
		}
	}
	else {
		printf("I'm sorry but we cannot create a userCommand");
	}

	//setting the command variable
	userCommand->commandCall = calloc(strlen(userCommand->argArray[0]) + 1, sizeof(char));
	strcpy(userCommand->commandCall, userCommand->argArray[0]);

	int argTotal = argPosition - 1;

	//finding input and output files
	for (int i = 1; i < argTotal; i++)
	{
		if (strcmp(userCommand->argArray[i], "<") == 0 && i < argTotal)
		{
			userCommand->inputFile = calloc(strlen(userCommand->argArray[i + 1]) + 1, sizeof(char));
			strcpy(userCommand->inputFile, userCommand->argArray[i + 1]);
		}

		else if (strcmp(userCommand->argArray[i], ">") == 0 && i < argTotal)
		{
			userCommand->outputFile = calloc(strlen(userCommand->argArray[i + 1]) + 1, sizeof(char));
			strcpy(userCommand->outputFile, userCommand->argArray[i + 1]);
		}
	}

	//whether it needs to be a background or foreground process
	if (strcmp(userCommand->argArray[argTotal], "&") == 0)
	{
		userCommand->isBackgroundFlag = true;
	}
	else
	{
		userCommand->isBackgroundFlag = true;
	}
	return userCommand;

	userCommand->numOfArgs = argTotal;
}

void exitCommand(Command* inputUser)
{
	freeUp(inputUser);
	exit(0);
}

void getCommand()
{
	int codeStatus;
	char* userInput = NULL;
	size_t len;
	Command* parsedInput;

	printf(":");
	fflush(stdout);

	getline(&userInput, &len, stdin);
	userInput[strlen(userInput) - 1] = '\0'; // remove added '\n' by getline

	if (strlen(userInput) == 0 || userInput[0] == '#' || strlen(userInput) > MAX_INPUT_LENGTH)
	{
		free(userInput);
		getCommand();
	}
	else
	{
		parsedInput = createCommandStruc(userInput);
		free(userInput);
		processCommand(4, parsedInput, &codeStatus);
		freeUp(parsedInput);
		runTracker++;
	}

	if (runTracker <= MAX_RUNS)
	{
		getCommand();
	}
	else 
	{
		exit(0);
	}

}

void freeUp(Command* parsedInput)
{
	for (int i = 0; i <= parsedInput->numOfArgs; i++) {
		free(parsedInput->argArray[i]);
	}

	free(parsedInput->commandCall);

	if (parsedInput->inputFile) { 
		free(parsedInput->inputFile); 
	};

	if (parsedInput->outputFile) {
		free(parsedInput->outputFile);
	};

	free(parsedInput);
}


//void variableExpansion(char* userInput, char* target, char* replacement)
//{
//
//}

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

void processCommand(int arg, Command* userInput, int* codeStatus)
{
	char** argArray = userInput->argArray;
	char* firstArg = &argArray[0][0];
	
	if (strcmp(firstArg, "cd") == 0) { 
		printf("hi");
	}
	else if (strcmp(firstArg, "status") == 0) {
		printf("status");
	}
	else if (strcmp(firstArg, "exit") == 0) {
		exit(0);
	}
	else {
		externalProcess(1, userInput, codeStatus);
	}
}

void redirectionNeeded(Command* userInput)
{
	char** argv = userInput->argArray;
	int sourceFD = open(userInput->inputFile, O_RDONLY);
	if (sourceFD == -1) {
		perror("source open()");
		exit(1);
	}
	// Written to terminal
	printf("sourceFD == %d\n", sourceFD);

	// Redirect stdin to source file
	int result = dup2(sourceFD, 0);
	if (result == -1) {
		perror("source dup2()");
		exit(2);
	}

	// Open target file
	int targetFD = open(userInput->outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (targetFD == -1) {
		perror("target open()");
		exit(1);
	}
	printf("targetFD == %d\n", targetFD); // Written to terminal

	// Redirect stdout to target file
	result = dup2(targetFD, 1);
	if (result == -1) {
		perror("target dup2()");
		exit(2);
	}
	// Run the command using execlp.
	// The stdin and stdout are pointing to files
	execlp(argv[0], argv[0], NULL);
	perror("execvp");
}

void status(int codeStatus)
{
	if (WIFEXITED(codeStatus)) {
		printf("Child exited normally with status %d\n", WEXITSTATUS(codeStatus));
	}
	else if (WTERMSIG(codeStatus)){
		printf("Child exited abnormally due to signal %d\n",  WTERMSIG(codeStatus));
	}
	else
	{
		printf("exit status 0");
	}
	fflush(stdout);
}