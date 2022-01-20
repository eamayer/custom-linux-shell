/*Project: Assignment 2
Class: CS 344 Winter 2022
Author: Elizabeth Mayer
Date: 15Jan2022

Description: Movie information processing and returning desired information. Processes file given
when executing the program. Asks user if they want to see which movies were released in a given year,
if they want to see the highest rated movie for each year, show movies released in specific languages.
Displays desired information back to user.
*/

#define _GNU_SOURCE
#define SUFFIX ".csv"
#define PREFIX "movies_"
#define MAXYEAR 2021

#include "files.h"

/*
Entry point into program. Takes file provided, processes and retrung informaiton requested.
*/

int main()
{
	int userInput1 = NULL;

	//takes the movie list provided as a file, processes it into a linked list
	//Movie* list = processList(argv[1])

	do
	{
		userInput1 = promptUserInput1();
		evaluateUserInput1(userInput1);
		int userInput2 = promptUserInput2();
		evaluateUserInput2(userInput2);
	} while (userInput1 != 2);

	exitProgram();
	return 0;
}



// Purpose: creates a new Movie node
// 
// Citation for the following function:
//Date 10Jan2022
//Based on: OSU CS 344 Instructors Assignment 1 Students example
//Source URL: https://replit.com/@cs344/studentsc#main.c

Movie* createNewMovieNode(char* currLinePointer) {
	Movie* currMovie = malloc(sizeof(Movie));

	char* saveptr;
	char* tokenptr;

	if (currMovie != NULL)
	{
		tokenptr = strtok_r(currLinePointer, ",", &saveptr);
		//allocating the space
		currMovie->title = calloc(strlen(tokenptr) + 1, sizeof(char));
		strcpy(currMovie->title, tokenptr);

		tokenptr = strtok_r(NULL, ",", &saveptr);
		int year = atoi(tokenptr);
		currMovie->year = year;

		tokenptr = strtok_r(NULL, ",", &saveptr);
		currMovie->languages = calloc(strlen(tokenptr) + 1, sizeof(char));
		strcpy(currMovie->languages, tokenptr);

		tokenptr = strtok_r(NULL, ",", &saveptr);
		char* end;
		double rating = strtod(tokenptr, &end);
		currMovie->rating = rating;
	}
	else printf("Error, Can't create new movie node\n");

	currMovie->next = NULL;
	return currMovie;
}


void evaluateUserInput1(int userInput1)
{
	switch (userInput1) {
	case 1: //user wants to select a file to process
		
		break;

	case 2: //user wants to exit
		exitProgram();
		break;
	}
}

void evaluateUserInput2(int userInput2)
{
	switch (userInput2) {
	case 1: //process largest file
		processLargestFile();
		break;

	case 2: //process smallest file
		processSmallestFile();
		break;

	case 3: //specify file name to process
		processSpecifiedFile();
		break;

	}
}

void exitProgram()
{
	exit(0);
}

void fileNotFound()
{
	int userInput2;
	printf("Sorry, your file was not found, please pick again\n");
	userInput2 = promptUserInput2();
	evaluateUserInput2(userInput2);
}

int findEarliestYearInList(Movie* list)
{
	int earliestYear = list->year; /*set earliest year to first movie in list*/

	while (list != NULL)
	{
		int movieYear = list->year;

		if (movieYear < earliestYear)
		{
			earliestYear = movieYear;
		}
		list = list->next;
	}
	return earliestYear;
}

bool findSpecifiedFile(char* specifiedFileName)
{
	DIR* currDir = opendir(".");
	struct dirent* aDir;
	struct stat dirStat;
	char tempFileName[256];
	strcpy(tempFileName, specifiedFileName);
	bool fileFound = false;

	if (currDir == NULL)
	{
		printf("directory did not open");
	}

	//// Go through all the entries
	while ((aDir = readdir(currDir)) != NULL) {
		// Get meta-data for the current entry
		stat(aDir->d_name, &dirStat);

		if (strcmp(aDir->d_name, tempFileName) == 0)
		{
			fileFound = true;
			return true;
		}
	}

	if (fileFound == false)
	{
		fileNotFound();
	}
		
	closedir(currDir);
	return true;
}


//CITE THE HW
void findLargestCSVFile(char* longestFileName)
{
	DIR* currDir = opendir(".");
	struct dirent* aDir;
	struct stat dirStat;
	long int longestFileLength = 0;

	if (currDir == NULL)
	{
		printf("directory did not open");
	}

	//// Go through all the entries
	while ((aDir = readdir(currDir)) != NULL) {

		int lengthOfName = strlen(aDir->d_name);
		char* fileType;

		if (lengthOfName >= 3)
		{
			fileType = &aDir->d_name[lengthOfName - 4];
		}

		if ((strcmp(SUFFIX, fileType) == 0) && (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0))
		{
			// Get meta-data for the current entry
			stat(aDir->d_name, &dirStat);

			if (dirStat.st_size > longestFileLength) {
				longestFileLength = dirStat.st_size;
				//memset(longestFileName, '\0', sizeof(aDir->d_name));
				strcpy(longestFileName, aDir->d_name);
			}
		}
	}
	closedir(currDir);
}


//CITE THE HW
void findSmallestCSVFile(char* shortestFileName)
{
	DIR* currDir = opendir(".");
	struct dirent* aDir;
	struct stat dirStat;
	long int shortestFileLength = 2147483647;

	if (currDir == NULL)
	{
		printf("directory did not open");
	}

	//// Go through all the entries
	while ((aDir = readdir(currDir)) != NULL) {

		int lengthOfName = strlen(aDir->d_name);
		char* fileType;

		if (lengthOfName >= 3)
		{
			fileType = &aDir->d_name[lengthOfName - 4];
		}

		if ((strcmp(SUFFIX, fileType) == 0) && (strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0))
		{
			// Get meta-data for the current entry
			stat(aDir->d_name, &dirStat);

			if (dirStat.st_size < shortestFileLength) {
				shortestFileLength = dirStat.st_size;
				strcpy(shortestFileName, aDir->d_name);
			}
		}
	}
	closedir(currDir);
}

/// <summary>
/// CITE HW
/// </summary>
/// <param name="fileName"></param>
/// <param name="sizeRequirement"></param>

void findSizedCSVFile(char* fileName, char sizeRequirement)
{
	DIR* currDir = opendir(".");
	struct dirent* aDir;
	struct stat dirStat;
	long int smallestFileSize = 2147483647;
	long int largestFileSize = 0;

	if (currDir == NULL)
	{
		printf("directory did not open");
	}

	//// Go through all the entries
	while ((aDir = readdir(currDir)) != NULL) {

		if ((strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) && ((strstr(SUFFIX, aDir->d_name) == 0)))
		{
			// Get meta-data for the current entry
			stat(aDir->d_name, &dirStat);

			if (dirStat.st_size < smallestFileSize && sizeRequirement == 's')
			{
				smallestFileSize = dirStat.st_size;
				strcpy(fileName, aDir->d_name);
			}

			else if (dirStat.st_size > largestFileSize && sizeRequirement == 'l')
			{
				largestFileSize = dirStat.st_size;
				strcpy(fileName, aDir->d_name);
			}
		}
	}
	closedir(currDir);
}


void getSpecifiedFile(char* fileName)
{
	printf("what file do you want processed?");
	scanf("%s", fileName);
}


int getUserInput1()
{
	int userInput1 = NULL;
	printf("Enter a choice 1 or 2:");
	scanf("%d", &userInput1);
	return userInput1;
}

int getUserInput2()
{
	int userInput2 = NULL;
	printf("Enter a choice 1 to 3:");
	scanf("%d", &userInput2);
	return userInput2;
}

void generateFileName(char* fileName, int movieYear)
{
	sprintf(fileName, "%d", movieYear);
	strcat(fileName, ".txt");
}

// returns the titles of all movies made in a given year
void generateTextFilesForReleaseYears(Movie* list) {

	while (list != NULL)
	{
		int file_descriptor;
		int movieYear = list->year;
		char fileName[9];
		char* movieTitle = calloc(strlen(list->title) + 2, sizeof(char));
		strcpy(movieTitle, list->title);
		strcat(movieTitle, "\n");
		generateFileName(fileName, movieYear);

		file_descriptor = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0640);
		if (file_descriptor == -1) {
			printf("open() failed on \"%s\"\n", fileName);
			perror("Error");
			exit(1);
		}
		write(file_descriptor, movieTitle, strlen(movieTitle));

		close(file_descriptor);
		free(movieTitle);
	
		list = list->next;
	}
}


/// <summary>
///  generates random number between 0 and 99999 (inclusive).
/// </summary>
/// <returns>randomNumber</returns>
///
/// Date: 19Jan2022
/// Adapted the use of srandom(time(NULL)) to use as a seed number
/// Source: https://www.delftstack.com/howto/c/c-generate-random-number/
long int generateRandomNumber()
{
	long int randomNumber;

	//generate random seed
	srandom(time(NULL));
	do
	{
		randomNumber = (random() / 42950); //makes number smaller but still allows max of 99999
		printf("%ld\n", randomNumber);
	} while (randomNumber < 0 || randomNumber > 99999);

	return randomNumber;
}

/// <summary>
/// creates a new directory, prints a message with the name of the directory that 
/// has been created. New directory is named "mayerel.movies.randomNumber" where
/// randomNumber it between 0 to 99999
/// </summary>
void makeNewDir(char* directoryName)
{
	long int randomNumber;
	char stringRandomNumber[256];
	strcpy(directoryName, "mayerel.movies.");

	randomNumber = generateRandomNumber();
	sprintf(stringRandomNumber, "%ld", randomNumber);

	strcat(directoryName, stringRandomNumber);

	mkdir(directoryName, 0750);
}

//Citation for the following function:
//Date 10Jan2022
//Based on: OSU CS 344 Instructors Assignment 1 Students example
//Source URL: https://replit.com/@cs344/studentsc#main.c

Movie* processList(char* filePath)
{
	FILE* movieFile = fopen(filePath, "r");

	//create variables to read line by line
	int currLineNumber = -1;
	char* currLinePointer = NULL;
	size_t buffer_size = 0;

	//create linked list pointer
	Movie* head = NULL;
	Movie* tail = NULL;

	if (movieFile == 0) {
		printf("File will not open!\n");
		exit(EXIT_FAILURE);
	}

	while (getline(&currLinePointer, &buffer_size, movieFile) != -1) {
		if (currLineNumber != -1)  //checking if not header line
		{
			Movie* newMovieNode = createNewMovieNode(currLinePointer);

			if (head == NULL)
			{
				head = newMovieNode;
				tail = newMovieNode;
			}
			else
			{
				tail->next = newMovieNode;
				tail = newMovieNode;
			}
		}
		currLineNumber++;
	}
	free(currLinePointer);
	fclose(movieFile);
	printf("\n Processed file %s and parsed data for %d movies\n", filePath, currLineNumber);
	return head;
}

//if user picks largest file. The largest file with the extension CSV 
// in current directory is found. If multiple files have the same size, 
// pick the largest files that is a CSV and starts with "movies_"
int processLargestFile()
{
	// find largest file (if ties, print any fi
	// print name of largest file
	
	char largestFileName[256];
	char largest = 'l';
	char directoryName[256];

	findSizedCSVFile(largestFileName, largest);
	printf("Now processing the chosen file named %s", largestFileName);

	Movie* list = processList(largestFileName);

	makeNewDir(directoryName);

	chdir(directoryName);

	generateTextFilesForReleaseYears(list);

	chdir("..");

	free(list);

	return 0;
}

int processSpecifiedFile()
{
	char specificFileName[256];
	getSpecifiedFile(specificFileName);
	char directoryName[256];

	if (findSpecifiedFile(specificFileName))
	{
		printf("Now processing the chosen file named %s", specificFileName);
	}


	Movie* list = processList(specificFileName);

	makeNewDir(directoryName);

	chdir(directoryName);

	generateTextFilesForReleaseYears(list);

	chdir("..");

	free(list);

	return 0;
}

// find smallest file (if ties, print any fi
// print name of largest file
int processSmallestFile()
{
	char smallestFileName[256];
	char smallest = 's';
	char directoryName[256];

	findSizedCSVFile(smallestFileName, smallest);
	printf("Now processing the chosen file named %s", smallestFileName);

	Movie* list = processList(smallestFileName);

	makeNewDir(directoryName);

	chdir(directoryName);

	generateTextFilesForReleaseYears(list);

	chdir("..");

	free(list);

	return 0;
}

void printOptions1()
{
	printf("\n1. Select file to process\n");
	printf("2. Exit the program \n");
}

void printOptions2()
{
	printf("\n1. Enter 1 to pick the largest file\n");
	printf("2. Enter 2 to pick the smallest file\n");
	printf("3. Enter 3 to specify the name of the file\n");
}

int promptUserInput1()
{
	printOptions1();
	int userInput1 = getUserInput1();
	if (userInput1 < 1 || userInput1 > 2)
	{
		printf("You entered a number that is not a choice, please try again\n\n");
		promptUserInput1();
	}
	return userInput1;
}

// Prints options and pompts user to make selection. If optiuon outside of allowed options is chosen,
// prompts user to put in another number
int promptUserInput2()
{
	printOptions2();
	int userInput2 = getUserInput2();
	if (userInput2 < 1 || userInput2 > 3)
	{
		printf("You entered a number that is not a choice, please try again\n\n");
		promptUserInput2();
	}
	return userInput2;

}

