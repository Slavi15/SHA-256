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

uint32_t getInputLength(const char* str)
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

bool isValidOption(uint32_t option)
{
	return option > 0 && option < 4;
}

uint32_t getOption()
{
	uint32_t option = 0, counter = 0;

	do
	{
		if (counter > 0)
		{
			std::cin.clear();
			std::cin.ignore(128, '\n');
			std::cout << "Invalid command! Please, enter (1-3)!" << std::endl;
		}

		std::cout << "Enter command: ";

		counter++;
	} while (!(std::cin >> option) && !isValidOption(option)); // Checks whether the input option is of correct datatype

	return option;
}

uint32_t strLen(const char* str)
{
	if (!str) return 0;

	uint32_t result = 0;
	while (*str++ && ++result);
	return result;
}

void strCpy(char* dest, const char* src)
{
	if (!dest || !src) return;

	while (*dest++ = *src++);
}

void strCat(char* first, const char* second)
{
	if (!first || !second) return;

	first += strlen(first);
	strCpy(first, second);
}

int strCmp(const char* first, const char* second)
{
	if (!first || !second) return 0;

	while (*first && *first++ == *second++);
	return *first - *second;
}

uint32_t powerOfTwo(uint32_t n)
{
	return n < 32 ? (1 << n) : 0;
}

bool checkBit(uint32_t n, uint32_t index)
{
	uint32_t mask = 1;
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