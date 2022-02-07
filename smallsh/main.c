/*Project: Assignment 3
Class: CS 344 Winter 2022
Author: Elizabeth Mayer
Date: 06Feb2022

Description: Program that is a mini shell. Has build in functionality for "cd", "exit" and "status". Redirects everything
else through execvp. Can handle background and foreground processes. Ctrl+Z makes the background process request ignored.
Ctrl+C terminates any foreground process.
*/

//----------------------------------------------------
/*Files needed*/
//----------------------------------------------------

#include "main.h"

//----------------------------------------------------
/*Entry point
Entry point into program. Sets up the handlers. Asks user for input and evaluates it appropriately to ensure
appropriate processing*/
//----------------------------------------------------


int main()
{
	setHandlers(); // need to be set up once
	while (1)
	{
		getCommand();
	}
	return 0;
}



