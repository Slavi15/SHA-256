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
* Header: Definitions of File Helper Functions
*
*/

#include "FileHelpers.h"

int fReadString(const char* fileName, char* inputMessage, size_t size)
{
	if (!fileName || !inputMessage)
		return 0;

	ifstream myFile(fileName);

	if (!myFile.is_open())
		return 1;

	unsigned int index = 0;
	if (myFile.is_open())
		while (myFile.good())
			myFile.get(inputMessage[index++]);

	inputMessage[--index] = '\0';

	myFile.close();
	return 0;
}

int fWriteString(const char* fileName, const char* outputMessage, size_t outputLength)
{
	if (!fileName || !outputMessage)
		return 0;

	ofstream myFile(fileName, ios::out);

	if (!myFile.is_open())
		return 1;

	for (size_t i = 0; i < outputLength; i++)
		myFile << outputMessage[i];

	myFile.close();

	return 0;
}