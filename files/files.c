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

#include "files.h"

/*
Entry point into program. Takes file provided, processes and retrung informaiton requested.
*/

int main()
{
	int userInput1 = NULL;
	int userInput2 = NULL;

	//takes the movie list provided as a file, processes it into a linked list
	//Movie* list = processList(argv[1]);

	do
	{
		int userInput1 = promptUserInput1();
		evaluateUserInput1(userInput1);
		int userInput2 = promptUserInput2();
		evaluateUserInput2(userInput2);
	} while (userInput1 != 2);

	exitProgram();
	return 0;
}


void evaluateUserInput1(int userInput1)
{
	switch (userInput1) {
	case 1: //user wants to select a file to process
		printf("userinput 1 1");
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
/// Try to figure out how to get this to work. sizes are swapped?
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


//if user picks largest file. The largest file with the extension CSV 
// in current directory is found. If multiple files have the same size, 
// pick the largest files that is a CSV and starts with "movies_"
int processLargestFile()
{
	// find largest file (if ties, print any fi
	// print name of largest file
	
	char largestFileName[256];
	//char largest = 'l';
	findLargestCSVFile(largestFileName);
	printf("Now processing the chosen file named %s", largestFileName);
	return 0;
}

int processSpecifiedFile()
{
	
	return 0;
}

// find smallest file (if ties, print any fi
// print name of largest file
int processSmallestFile()
{
	char smallestFileName[256];
	//char smallest = 's';
	findSmallestCSVFile(smallestFileName);
	printf("Now processing the chosen file named %s", smallestFileName);
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



