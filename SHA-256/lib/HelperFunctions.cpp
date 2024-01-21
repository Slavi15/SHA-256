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

void stringCopy(char* dest, const char* source)
{
	if (!dest || !source)
		return;

	while (*source)
	{
		*dest = *source;
		dest++;
		source++;
	}
	*dest = '\0';
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

void mySubstring(char* dest, const char* source, size_t start, size_t length)
{
	if (!dest || !source)
		return;

	for (size_t i = 0; i < length; i++)
	{
		*dest = source[start];
		start++;
		dest++;
	}
	*dest = '\0';
}

void decimalToKSystem(unsigned num, char* to, size_t fragmentLength, unsigned k)
{
	if (!to)
		return;

	to[fragmentLength] = '\0';

	for (int i = fragmentLength - 1; i >= 0; i--)
	{
		int mod = num % k;
		to[i] = integerToChar(mod);
		num /= k;
	}
}

unsigned int kSystemToDecimal(const char* binary, size_t fragmentLength, unsigned k)
{
	if (!binary)
		return 0;

	unsigned result = 0;

	for (int i = fragmentLength - 1, mult = 1; i >= 0; i--, mult *= k)
		if (binary[i] != '\0')
			result += (charToInteger(binary[i]) * mult);

	return result;
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
	if (isDigit(ch))
		return ch - '0';

	return -1;
}

bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}