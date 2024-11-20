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

void fReadString(const char* fileName, char* inputMessage, uint32_t size)
{
	if (!fileName || !inputMessage) return;

	std::ifstream ifs(fileName, std::ios::in);

	if (!ifs.is_open()) return;

	return fReadString(ifs, inputMessage, size);
}

void fReadString(std::ifstream& ifs, char* inputMessage, uint32_t size)
{
	while (!ifs.eof())
	{
		char buffer[SAFE_MAX_SIZE + 1]{ SENTINEL_CHARACTER };

		ifs.getline(buffer, SAFE_MAX_SIZE + 1, NEWLINE_CHARACTER);

		strCat(inputMessage, buffer);
	}

	ifs.close();
}

void fWriteString(const char* fileName, const char* outputMessage, uint32_t outputLength)
{
	if (!fileName || !outputMessage) return;

	std::ofstream ofs(fileName, std::ios::out);

	if (!ofs.is_open()) return;

	return fWriteString(ofs, outputMessage, outputLength);
}

void fWriteString(std::ofstream& ofs, const char* outputMessage, uint32_t outputLength)
{
	ofs << outputMessage;
	ofs.close();
}
