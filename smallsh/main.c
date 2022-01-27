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

//----------------------------------------------------
/*Entry point
Entry point into program. Asks user for input and evaluates it appropriately to ensure
appropriate processing*/
//----------------------------------------------------

int main()
{
	getCommand();
	return 0;
}

void externalProcess(char** argArray)
{

}

void getCommand()
{
	char* userInput = NULL;
	size_t len;

	do
	{
		printf(":");
		fflush(stdout);

		getline(&userInput, &len, stdin);
		userInput[strlen(userInput) - 1] = '\0'; // remove added '\n' by getline

		Command* input = createCommandStruc(userInput);


	} while (strlen(userInput) == 0 || userInput[0] == '#' || strlen(userInput) > MAX_INPUT_LENGTH);

	printf("my pid is %d\n", getpid());


}

//void getPID

//void variableExpansion(char* userInput, char* target, char* replacement)
//{
//
//}


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
	for (int i=1; i <= argTotal; i++)
	{
		if (strcmp(userCommand->argArray[i], ">") == 0 && i <= argTotal)
		{
			userCommand->inputFile = calloc(strlen(userCommand->argArray[i - 1]) + 1, sizeof(char));
			strcpy(userCommand->inputFile, userCommand->argArray[i - 1]);
			userCommand->outputFile = calloc(strlen(userCommand->argArray[i + 1]) + 1, sizeof(char));
			strcpy(userCommand->outputFile, userCommand->argArray[i + 1]);
		}

		else if (strcmp(userCommand->argArray[i], "<") == 0 && i <= argTotal)
		{
			userCommand->inputFile = calloc(strlen(userCommand->argArray[i + 1]) + 1, sizeof(char));
			strcpy(userCommand->inputFile, userCommand->argArray[i + 1]);
			userCommand->outputFile = calloc(strlen(userCommand->argArray[i - 1]) + 1, sizeof(char));
			strcpy(userCommand->outputFile, userCommand->argArray[i - 1]);
		}
	}

	if (strcmp(userCommand->argArray[argTotal], "&") == 0)
	{
		userCommand->isBackgroundFlag = true;
	}
	else
	{
		userCommand->isBackgroundFlag = true;
	}

	return userCommand;

}



int numOfArgs(char* input)
{
	int argCounter = 0;
	char character;

	for (int i = 0; i < strlen(input); i++)
	{
		character = input[i];

		if (isspace(character))
		{
			argCounter++;
		}
	}
	return argCounter;
}

void parseCommand(char* input, char** argArray, int totalArgs)
{
	//char bufferSize = MAX_NUM_ARGS;
	char* tokenptr;

	//argArray = malloc(bufferSize * sizeof(char*));  //create placeholder for array of pointers

	tokenptr = strtok(input, " ");

	for (int i = 0; i <= totalArgs; i++)
	{
		argArray[i] = tokenptr;
		tokenptr = strtok(NULL, " ");
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

void processCommand(char** argArray)
{
	char* firstArg = &argArray[0][0];
	
	if (strcmp(firstArg, "cd") == 0)
	{
		printf("hi");
	}
	else if (strcmp(firstArg, "status") == 0)
	{
		printf("status");
	}
	else if (strcmp(firstArg, "exit") == 0)
	{
		exit(0);
	}
	else
	{
		externalProcess(argArray);
	}

	getCommand();
}


