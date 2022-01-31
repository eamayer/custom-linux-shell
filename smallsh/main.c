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



