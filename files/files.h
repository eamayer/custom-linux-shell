//----------------------------------------------------
/*Required files*/
//----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

//----------------------------------------------------
/*Constant Values*/
//----------------------------------------------------

const int maxYear = 2021;

#ifndef FILES_H_
#define FILES_H_ 

void evaluateUserInput1(int userInput1);
void evaluateUserInput2(int userInput2);
void exitProgram();
void findLargestCSVFile(char* nameholder);
void findSmallestCSVFile(char* nameholder);
void findSizedCSVFile(char* shortestFileName, char sizeRequirement);
bool findSpecifiedFile(char* specifiedFileName);
void getSpecifiedFile();
int getUserInput1();
int getUserInput2();
int processSmallestFile();
int processLargestFile();
int processSpecifiedFile();
int promptUserInput1();
int promptUserInput2();

void printOptions1();
void printOptions2();



#endif // !FILES_H_




//---------------------------------------------------
/*Getting and processing information from user*/
//----------------------------------------------------

//helper function to print the available options
void printOptions()
{
	printf("\n1. Show movies released in the specified year\n");
	printf("2. Show highest rated movie for each year\n");
	printf("3. Show the title and year of release of all movies in a specific language\n");
	printf("4. Exit from the program\n\n");
}

//helper function to ask user for input
int getUserInput() {
	int userInput = NULL;
	printf("Enter a choice from 1 to 4:");
	scanf("%d", &userInput);
	return userInput;
}


//----------------------------------------------------
/*Generating Movie Structure node for Linked List*/
//----------------------------------------------------

/* struct for movie information*/
typedef struct Movie
{
	char* title;
	char* languages;
	int   year;
	double rating;
	struct Movie* next;
} Movie;


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


//----------------------------------------------------
/*Printing information from movie list*/
//----------------------------------------------------

//Prints a movie title
void printMovieTitle(Movie* aMovie) {
	printf("%s\n", aMovie->title);
}

//Prints a movie title and year
void printMovieYearAndTitle(Movie* aMovie) {
	printf("%d %s\n", aMovie->year, aMovie->title);
}

//Prints movie title, year and rating
void printMovieYearRatingAndTitle(Movie* aMovie) {
	printf("%d %.1f %s\n", aMovie->year, aMovie->rating, aMovie->title);
}


//----------------------------------------------------
/*Creating Linked List from file given during exection*/
//----------------------------------------------------

//Purpose:Takes the file provided during program execution and creates linked list of Movie structures
//Prints the number of lines processed

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

//----------------------------------------------------
/*Finding and printing list of movies released in a specific year*/
//----------------------------------------------------

//Gets a year from the user
int getYearInputFromUser() {
	int year;
	printf("Enter the year for which you want to see movies:");
	scanf("%d", &year);
	return year;
}

// returns the titles of all movies made in a given year
void releaseYearSearch(Movie* list) {
	//get input from user
	int yearDesired = getYearInputFromUser();
	int moviePrintedTracker = 0;  //used to determine if any movies were printed at all

	while (list != NULL)
	{
		int movieYear = list->year;
		if (movieYear == yearDesired)
		{
			printMovieTitle(list);
			moviePrintedTracker++;
		}
		list = list->next;
	}
	if (moviePrintedTracker == 0)
	{
		printf("No data about movies released in year %d\n", yearDesired);
	}
}


//----------------------------------------------------
/*Finding and printing list of movies with highest rating in each year*/
//----------------------------------------------------

//prints the highest rated movie in a single year given the year and rating
int printHighestRatedMovieInYear(Movie* list, int year, double rating)
{
	while (list != NULL)
	{
		if (list->year == year && list->rating == rating)
		{
			printMovieYearRatingAndTitle(list);
			return 0;
		}
		list = list->next;
	}
	return 0;
}

//finds and returns the rating of the highest rated movie in given year
double findHighestRatedMovieInYear(Movie* list, int year)
{
	double rating = 0;
	while (list != NULL)
	{
		if (list->year == year && list->rating > rating)
		{
			rating = list->rating;
		}
		list = list->next;
	}
	return rating;
}

//finds the year that is the earliest released
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


//Finds earliest year in list and begins searching and then prints the highest rated movie 
// in every year from that point
void highestRatedMovieInEveryYear(Movie* list)
{
	int year = findEarliestYearInList(list);

	while (year <= maxYear)
	{
		double rating = findHighestRatedMovieInYear(list, year);
		if (rating != 0)
		{
			printHighestRatedMovieInYear(list, year, rating);
		}
		year++; //increment to check next year
	}
}


//----------------------------------------------------
/*Finding and printing list of movies released in a specific language*/
//----------------------------------------------------

//Citation for the following function:
//Date 10Jan2022
//Adapted the while loop for token from:
//Source URL: https://riptutorial.com/c/example/2557/tokenisation--strtok----strtok-r---and-strtok-s--

//Gets lanugage from user to search for. Creates copy of the movie language to ensure no changes
// are made to the linked list. Searches string for language. Prints movies. Tells user if no 
// movies were found in requested language.

void findMoviesInLanguage(Movie* list)
{
	int numberOfMoviesPrinted = 0;
	char desiredLanguage[22];
	memset(desiredLanguage, '\0', 22);

	printf("What language would you like?");
	scanf("%s", desiredLanguage);

	while (list != NULL) {

		const char delimiters[] = "[;]";
		char* movieLanguages;

		//allocate space for languages and then copy
		movieLanguages = (char*)calloc(strlen(list->languages) + 1, sizeof(char));
		strcpy(movieLanguages, list->languages);

		//set token at the start of the languages
		char* token = strtok(movieLanguages, delimiters);

		while (token != NULL)
		{
			if ((strcmp(token, desiredLanguage)) == 0)
			{
				numberOfMoviesPrinted++;
				printf("%d %s\n", list->year, list->title);
			}
			token = strtok(NULL, delimiters);
		}
		free(movieLanguages);
		list = list->next;
	}

	if (numberOfMoviesPrinted == 0)
	{
		printf("No movies were released in %s\n", desiredLanguage);
	}
}

//----------------------------------------------------
/*Exiting the program*/
//----------------------------------------------------

//----------------------------------------------------
/*Determine which of the menu options user has chosen and starts the
cascade of functions to return that information */
//----------------------------------------------------

void evaluateInput(int userInput, Movie* list)
{
	switch (userInput) {
	case 1: //show movies released in specified year
		releaseYearSearch(list);
		break;

	case 2: //show highest rated movie for each year
		highestRatedMovieInEveryYear(list);
		break;

	case 3: //show title and year of release of all movies in language
		findMoviesInLanguage(list);
		break;

	case 4:
		exitProgram();
		break;
	}
}
