#pragma once
//----------------------------------------------------
/*Required files*/
//----------------------------------------------------


#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
//#include <dirent.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <time.h>
#include <unistd.h>
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
	bool isBackgroundFlag;
} Command;


//----------------------------------------------------
/*Function List*/
//----------------------------------------------------

#ifndef FILES_H_
#define FILES_H_ 

void getCommand();
void externalProcess(argArray);
void printArray(char** argArray, int argumentsTotal);
Command* createCommandStruc(char* input);
//void commonFileProcesses(char* fileName);
//Movie* createNewMovieNode(char* currLinePointer);
//void evaluateUserInputMainMenu(int userInput1);
//void evaluateUserInputSubMenu(int userInput2);
//void exitProgram();
//void findSizedLargestOrSmallestFileSize(char* fileName, char sizeRequirement);
//bool findSpecifiedFile(char* specifiedFileName);
//void fileNotFound();
//void freeTheNodes(Movie* list);
//void generateFileName(char* fileName, int movieYear);
//int generateRandomNumber();
//void generateTextFilesForReleaseYears(Movie* list);
//int getUserInputMainMenu();
//int getUserInputSubMenu();
//void makeNewDir(char* directoryName);
//void printMainMenu();
//void printSubMenu();
//Movie* processList(char* filePath);
//void processLargestFile();
//void processSmallestFile();
//void processSpecifiedFile();
//int promptUserMainMenu();
//int promptUserSubMenu();

#endif // !FILES_H_


//----------------------------------------------------
/*Constant Values*/
//----------------------------------------------------

//#define MAX_DIGITS 12
//#define _GNU_SOURCE
//#define SUFFIX ".csv"
//#define PREFIX "movies_"
//#define MAXYEAR 2021
//#define MAXRANDOM 99999
//
//
//#include "files.h"

//----------------------------------------------------
/*Entry point
Entry point into program. Asks user for input and evaluates it appropriately to ensure
appropriate processing*/
////----------------------------------------------------
//
//
////----------------------------------------------------
///*Functions*/
////----------------------------------------------------
//
///// <summary>
///// Processes the file by creating a linked list, creating a new directory,
///// changing directories, and writing a .txt file for every release year which contains
///// the titles of all movies released in that year. Moves back to home directory.
///// Frees the entire linked list after done processing it.
///// </summary>
///// <param name="fileName">file to be processed</param>
//void commonFileProcesses(char* fileName)
//{
//	char directoryName[256];
//
//	Movie* list = processList(fileName);
//
//	makeNewDir(directoryName);
//
//	printf("\nCreated directory with name %s", directoryName);
//
//	chdir(directoryName);
//
//	generateTextFilesForReleaseYears(list);
//
//	chdir("..");
//
//	freeTheNodes(list);
//}
//
///// <summary>
///// Creates a new Movie node
///// </summary>
///// <param name="currLinePointer"></param>
///// <returns>the newly created node</returns>
///// 
///// Citation for the following function:
///// Date 10Jan2022
///// Based on: OSU CS 344 Instructors Assignment 1 Students example
///// Source URL: https://replit.com/@cs344/studentsc#main.c
//Movie* createNewMovieNode(char* currLinePointer) {
//	Movie* currMovie = malloc(sizeof(Movie));
//	char* saveptr;
//
//	if (currMovie != NULL)
//	{
//		char* tokenptr = strtok_r(currLinePointer, ",", &saveptr);
//		//allocating the space
//		currMovie->title = calloc(strlen(tokenptr) + 1, sizeof(char));
//		strcpy(currMovie->title, tokenptr);
//
//		tokenptr = strtok_r(NULL, ",", &saveptr);
//		int year = atoi(tokenptr);
//		currMovie->year = year;
//
//		tokenptr = strtok_r(NULL, ",", &saveptr);
//		currMovie->languages = calloc(strlen(tokenptr) + 1, sizeof(char));
//		strcpy(currMovie->languages, tokenptr);
//
//		tokenptr = strtok_r(NULL, ",", &saveptr);
//		char* end;
//		double rating = strtod(tokenptr, &end);
//		currMovie->rating = rating;
//	}
//	else printf("Error, Can't create new movie node\n");
//
//	currMovie->next = NULL;
//
//	return currMovie;
//}
//
//
///// <summary>
///// Processes the given list of movies to create a linked list
///// Allocates space on the heap for the movie file and the linepointer.
///// </summary>
///// <param name="filePath">Name of file to be processed</param>
///// <returns>head of the linked list</returns>
///// 
///// Citation for the following function:
///// Date 10Jan2022
///// Based on: OSU CS 344 Instructors Assignment 1 Students example
///// Source URL: https://replit.com/@cs344/studentsc#main.c
//
//Movie* processList(char* filePath)
//{
//	FILE* movieFile = fopen(filePath, "r");
//
//	//create variables to read line by line
//	int currLineNumber = -1;
//	char* currLinePointer = NULL;
//	size_t buffer_size = 0;
//	ssize_t nread;
//
//	//create linked list pointer
//	Movie* head = NULL;
//	Movie* tail = NULL;
//
//	if (movieFile == 0) {
//		printf("File will not open!\n");
//		exit(EXIT_FAILURE);
//	}
//
//	while ((nread = getline(&currLinePointer, &buffer_size, movieFile)) != -1) {
//		if (currLineNumber != -1)  //checking if not header line
//		{
//			Movie* newMovieNode = createNewMovieNode(currLinePointer);
//
//			if (head == NULL)
//			{
//				head = newMovieNode;
//				tail = newMovieNode;
//			}
//			else
//			{
//				tail->next = newMovieNode;
//				tail = newMovieNode;
//			}
//		}
//		currLineNumber++;
//	}
//
//	free(currLinePointer);
//	fclose(movieFile);
//	return head;
//}
//
///// <summary>
///// Determines whether the user wants to process a file or exit
///// </summary>
///// <param name="userInputMainMenu"></param>
//void evaluateUserInputMainMenu(int userInputMainMenu)
//{
//	switch (userInputMainMenu) {
//	case 1:; //user wants to select a file to process
//		break;
//
//	case 2: //user wants to exit
//		exitProgram();
//		break;
//	}
//}
//
///// <summary>
///// evaluating whether the user wants to use the largest file, smallest or 
///// find an exact one. Processes appropriately.
///// </summary>
///// <param name="userInputSubMenu"></param>
//void evaluateUserInputSubMenu(int userInputSubMenu)
//{
//	switch (userInputSubMenu) {
//	case 1:
//		processLargestFile();
//		break;
//	case 2:
//		processSmallestFile();
//		break;
//	case 3:
//		processSpecifiedFile();
//		break;
//	}
//}
//
//void exitProgram()
//{
//	exit(0);
//}
//
///// <summary>
///// File matching the user's inputed desired file is not found. Repeats the submenu and
///// asks them to choose again.
///// </summary>
//void fileNotFound()
//{
//	int userInputSubMenu;
//	printf("Sorry, your file was not found, please pick again\n");
//	userInputSubMenu = promptUserSubMenu();
//	evaluateUserInputSubMenu(userInputSubMenu);
//}
//
///// <summary>
///// Searches the current directory for a file with the name provided by user.
///// Allocates space on the heap for the directory.
///// </summary>
///// <param name="specifiedFileName"></param>
///// <returns>Boolean</returns>
//bool findSpecifiedFile(char* specifiedFileName)
//{
//	DIR* currDir = opendir(".");
//	struct dirent* aDir;
//	struct stat dirStat;
//	bool fileFound = false;
//
//	if (currDir == NULL)
//	{
//		printf("directory did not open");
//	}
//
//	//// Go through all the entries
//	while ((aDir = readdir(currDir)) != NULL) {
//		// Get meta-data for the current entry
//		stat(aDir->d_name, &dirStat);
//		if (strcmp(aDir->d_name, specifiedFileName) == 0)
//		{
//			fileFound = true;
//		}
//	}
//
//	if (!fileFound)
//	{
//		fileNotFound();
//	}
//
//	closedir(currDir);
//
//	return true;
//}
//
//
///// <summary>
//// finds the smallest or largest file size in the directory, saves the files name.
///// </summary>
///// <param name="fileName"></param>
///// <param name="sizeRequirement">specifies whether it should search for smallest or largest</param>
///// 
///// CITATION FOR THE FOLLOWING FUNCTION:
///// function is based on:
///// SOURCE: https://canvas.oregonstate.edu/courses/1884946/pages/exploration-files?module_item_id=21835963
///// DATE: 23Jan2022
//
//void findSizedLargestOrSmallestFileSize(char* fileName, char sizeRequirement)
//{
//	DIR* currDir = opendir(".");
//	struct dirent* aDir;
//	struct stat dirStat;
//	long int smallestFileSize = 2147483647;
//	long int largestFileSize = 0;
//
//	if (currDir == NULL)
//	{
//		printf("directory did not open");
//	}
//
//	//// Go through all the entries
//	while ((aDir = readdir(currDir)) != NULL)
//	{
//		char* compareForSufffix = strrchr(aDir->d_name, '.');;
//
//		if (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0 && strcmp(SUFFIX, compareForSufffix) == 0)
//		{
//			// Get meta-data for the current entry
//			stat(aDir->d_name, &dirStat);
//
//			if (dirStat.st_size < smallestFileSize && sizeRequirement == 's')
//			{
//				smallestFileSize = dirStat.st_size;
//				strcpy(fileName, aDir->d_name);
//			}
//
//			else if (dirStat.st_size > largestFileSize && sizeRequirement == 'l')
//			{
//				largestFileSize = dirStat.st_size;
//				strcpy(fileName, aDir->d_name);
//			}
//		}
//	}
//	closedir(currDir);
//}
//
///// <summary>
///// Goes through linked list of Movies and frees each node
///// </summary>
///// <param name="list"></param>
///// CODE CITATION: Code is based on: 
///// SITE: https://codeforwin.org/2015/09/c-program-to-delete-all-nodes-of-singly-linked-list.html
///// AUTHOR: Pankaj
///// DATE: 20Jan2022
//
//void freeTheNodes(Movie* list)
//{
//	Movie* temp;
//
//	while (list != NULL)
//	{
//		temp = list;
//		list = list->next;
//		free(temp->languages);
//		free(temp->title);
//		free(temp);
//	}
//	list = NULL;
//}
//
///// <summary>
///// creates a file name of "YYYY.txt" for passed pointer where YYYY is the year 
///// the movie was released
///// </summary>
///// <param name="fileName">name of specific file</param>
///// <param name="movieYear"></param>
//void generateFileName(char* fileName, int movieYear) {
//	sprintf(fileName, "%d", movieYear);
//	strcat(fileName, ".txt");
//}
//
///// <summary>
///// creates a text file for year release (YYYY.txt)and adds the name of the movies
///// released in that year to it.
///// </summary>
///// <param name="list">Movie list</param>
//void generateTextFilesForReleaseYears(Movie* list) {
//
//	while (list != NULL)
//	{
//		int file_descriptor;
//		int movieYear = list->year;
//		char fileName[10];
//		char* movieTitle = calloc(strlen(list->title) + 2, sizeof(char));
//
//		strcpy(movieTitle, list->title);
//		strcat(movieTitle, "\n");
//
//		generateFileName(fileName, movieYear);
//
//		file_descriptor = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0640);
//		if (file_descriptor == -1) {
//			printf("open() failed on \"%s\"\n", fileName);
//			perror("Error");
//			exit(1);
//		}
//
//		write(file_descriptor, movieTitle, strlen(movieTitle));
//
//		close(file_descriptor);
//
//		free(movieTitle);
//
//		list = list->next;
//	}
//}
//
///// <summary>
/////  generates random number between 0 and 99999 (inclusive).
///// </summary>
///// <returns>randomNumber</returns>
/////
///// Date: 19Jan2022
///// Adapted function below from 2 sources
///// Source for using time for seed: https://www.delftstack.com/howto/c/c-generate-random-number/
///// Source for everything else: https://www.programmingsimplified.com/c-program-generate-random-numbers
//int generateRandomNumber()
//{
//	int randomNumber;
//
//	//generate random seed
//	srand(time(NULL));
//
//	randomNumber = (rand() % MAXRANDOM); //for setting max of 99999
//
//	return randomNumber;
//}
//
//int getUserInputMainMenu() {
//	int userInputMainMenu = NULL;
//
//	printf("Enter a choice 1 or 2:");
//	scanf("%d%*c", &userInputMainMenu);
//
//	return userInputMainMenu;
//}
//
//int getUserInputSubMenu() {
//	int userInputSubMenu = NULL;
//
//	printf("Enter a choice 1 to 3:");
//	scanf("%d%*c", &userInputSubMenu);
//
//	return userInputSubMenu;
//}
//
///// <summary>
///// Creates a new directory, prints a message with the name of the directory that 
///// has been created. New directory is named "mayerel.movies.randomNumber" where
///// randomNumber it between 0 to 99999 inclusive.
///// </summary>
///// <parameters=directoryName></parameters>
//void makeNewDir(char* directoryName)
//{
//	int randomNumber;
//	char stringRandomNumber[256];
//
//	strcpy(directoryName, "mayerel.movies.");
//
//	randomNumber = generateRandomNumber();
//
//	sprintf(stringRandomNumber, "%d", randomNumber);
//
//	strcat(directoryName, stringRandomNumber);
//
//	mkdir(directoryName, 0750);
//}
//
//void printMainMenu()
//{
//	printf("\n1. Select file to process\n");
//	printf("2. Exit the program \n");
//}
//
//void printSubMenu()
//{
//	printf("\n1. Enter 1 to pick the largest file\n");
//	printf("2. Enter 2 to pick the smallest file\n");
//	printf("3. Enter 3 to specify the name of the file\n");
//}
//
//
//// <summary>
//// If user picks largest file. The largest file with the extension CSV 
//// in current directory is found. If multiple files have the same size, 
//// pick the largest files that is a CSV and starts with "movies_"
//// </summary>
//void processLargestFile()
//{
//	char largestFileName[256];
//	char largest = 'l';
//
//	findSizedLargestOrSmallestFileSize(largestFileName, largest);
//	printf("Now processing the chosen file named %s\n", largestFileName);
//
//	commonFileProcesses(largestFileName);
//}
//
//// <summary>
//// If user picks smallest file. The smallest file with the extension CSV 
//// in current directory is found. If multiple files have the same size, 
//// pick the smallest files that is a CSV and starts with "movies_"
//// </summary>
//void processSmallestFile()
//{
//	char smallestFileName[256];
//	char smallest = 's';
//
//	findSizedLargestOrSmallestFileSize(smallestFileName, smallest);
//	printf("Now processing the chosen file named %s\n", smallestFileName);
//
//	commonFileProcesses(smallestFileName);
//}
//
///// <summary>
///// Gets and processes a user entered file name. Allocates space on heap during
///// creation of specificFileName
///// </summary>
//void processSpecifiedFile()
//{
//	printf("what file do you want processed?");
//
//	char* specificFileName = NULL;
//	size_t len;
//
//	getline(&specificFileName, &len, stdin);
//	specificFileName[strlen(specificFileName) - 1] = '\0'; // remove added '\n' by getline
//
//	if (findSpecifiedFile(specificFileName))
//	{
//		printf("Now processing the chosen file named %s\n", specificFileName);
//	}
//
//	commonFileProcesses(specificFileName);
//	free(specificFileName);
//}
//
///// <summary>
///// shows the main menu to the user and calls helper function to have them make a choice
///// Validates the input and if incorrect, shows error message and repeats menu.
///// </summary>
//int promptUserMainMenu()
//{
//	printMainMenu();
//	int userInputMainMenu = getUserInputMainMenu();
//	if (userInputMainMenu < 1 || userInputMainMenu > 2)
//	{
//		printf("You entered a number that is not a choice, please try again\n\n");
//		promptUserMainMenu();
//	}
//	return userInputMainMenu;
//}
//
///// <summary>
/////  Prints options for the submenu of which files to process and pompts user to make selection. 
/////  If option outside of allowed options is chosen, prompts user to put in another number
///// </summary>
//int promptUserSubMenu()
//{
//	printSubMenu();
//	int userInputSubMenu = getUserInputSubMenu();
//	if (userInputSubMenu < 1 || userInputSubMenu > 3)
//	{
//		printf("You entered a number that is not a choice, please try again\n\n");
//		promptUserSubMenu();
//	}
//	return userInputSubMenu;
//}

