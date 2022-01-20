//----------------------------------------------------
/*Required files*/
//----------------------------------------------------

#include <fcntl.h>
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


/* struct for movie information*/
typedef struct Movie
{
	char* title;
	char* languages;
	int   year;
	double rating;
	struct Movie* next;
} Movie;




#ifndef FILES_H_
#define FILES_H_ 

Movie* createNewMovieNode(char* currLinePointer);
void evaluateUserInput1(int userInput1);
void evaluateUserInput2(int userInput2);
void exitProgram();
int findEarliestYearInList(Movie* list);
void findLargestCSVFile(char* shortestFileName);
void findSmallestCSVFile(char* shortestFileName);
void findSizedCSVFile(char* shortestFileName, char sizeRequirement);
bool findSpecifiedFile(char* specifiedFileName);
void fileNotFound();
void getSpecifiedFile(char* fileName);
int getUserInput1();
int getUserInput2();
void generateFileName(char* fileName, int movieYear);
long int generateRandomNumber();
void generateTextFilesForReleaseYears(Movie* list);
void makeNewDir(char* directoryName);
Movie* processList(char* filePath);
int processSmallestFile();
int processLargestFile();
int processSpecifiedFile();
int promptUserInput1();
int promptUserInput2();
void printOptions1();
void printOptions2();


#endif // !FILES_H_
