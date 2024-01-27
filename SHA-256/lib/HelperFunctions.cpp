/**
*
* Solution to course project # 6
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Slavi Rezashki
* @idnumber 6MI0600339
* @compiler VC
*
* Header: SHA-256 Helper Functions
*
*/

#include "HelperFunctions.h"

unsigned getInputLength(const char* str)
{
	if (!str)
		return 0;

	int counter = 0;
	while (*str)
	{
		str++;
		counter++;
	}

	return counter;
}

bool isValidOption(unsigned int option)
{
	return option > 0 && option < 4;
}

unsigned int getOption()
{
	unsigned int option = 0, counter = 0;

	do
	{
		if (counter > 0)
		{
			cin.clear();
			cin.ignore(128, '\n');
			cout << "Invalid command! Please, enter (1-3)!" << endl;
		}

		cout << "Enter command: ";

		counter++;
	} while (!(cin >> option) && !isValidOption(option)); // Checks whether the input option is of correct datatype

	return option;
}

unsigned int myStrCmp(const char* first, const char* second)
{
	if (!first || !second)
		return 0;

	while (*first && *first == *second)
	{
		first++;
		second++;
	}

	return *first - *second;
}

unsigned int powerOfTwo(int n)
{
	if (n > 31)
		return 0;

	return 1 << n;
}

bool checkBit(unsigned int n, unsigned index)
{
	unsigned int mask = 1;
	mask <<= index;
	return (mask & n) == mask;
}

char integerToChar(int n)
{
	if (n >= 0 && n <= 9)
		return n + '0';
	else
		return (n - 10) + 'A';
}

int charToInteger(char ch)
{
	return (int)ch;
}