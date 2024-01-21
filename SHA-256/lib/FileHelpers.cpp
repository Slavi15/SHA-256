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

int fReadString(const char* fileName, char* inputMessage, size_t size, size_t line)
{
	if (!fileName || !inputMessage)
		return 0;

	ifstream myFile(fileName);

	if (!myFile.is_open())
		return 1;

	size_t lineNumber = 1;
	while ((lineNumber++ <= line) && (myFile.getline(inputMessage, size)));

	if (myFile.eof() && (line + 2 != lineNumber))
		return 2;

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