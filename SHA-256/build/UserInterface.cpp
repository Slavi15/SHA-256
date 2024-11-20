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
* Header: Main Menu
*
*/

#include "UserInterface.h"
#include "FileHelpers.h"
#include "HelperFunctions.h"
#include "Hash.h"

void SHA256()
{
	char inputMessage[SAFE_MAX_SIZE + 1]{ SENTINEL_CHARACTER };
	fReadString(INPUT_FILE_NAME, inputMessage, SAFE_MAX_SIZE);

	uint32_t inputLength = getInputLength(inputMessage);

	const uint32_t N = ((inputLength * CHAR_SIZE_IN_BITS) + MAX_SIZE_MESSAGE_SCHEDULE) / INITIAL_BITS;
	const uint32_t ROWS = ((N + 1) * INITIAL_BITS) / FRAGMENT_SIZE;

	uint32_t* messageBlock = new uint32_t[ROWS]{ 0 };
	fillMessageBlock(messageBlock, ROWS, inputMessage, inputLength);

	bool exitCommand = false;

	do
	{
		refreshScreen();
		std::cout << "Welcome to SHA-256!" << std::endl;
		std::cout << "1. Generate SHA-256 Message" << std::endl;
		std::cout << "2. SHA-256 Integrity Check" << std::endl;
		std::cout << "3. Exit" << std::endl;

		uint32_t menuOption = getOption();

		switch (menuOption)
		{
		case 1:
		{
			char* output = getFinalMessage(messageBlock, ROWS, inputMessage, inputLength, N);
			fWriteString(OUTPUT_FILE_NAME, output, MAX_SIZE_MESSAGE_SCHEDULE);
			exitCommand = true;
		}
		break;
		case 2:
		{
			integrityCheck(messageBlock, ROWS, inputMessage, inputLength, N);
			exitCommand = true;
		}
		break;
		case 3:
		{
			std::cout << "You have exited successfully!" << std::endl;
			exitCommand = true;
		}
		break;
		}
	} while (!exitCommand);

	delete[] messageBlock;
}

void integrityCheck(uint32_t* messageBlock, uint32_t rows, char* inputMessage, uint32_t inputLength, uint32_t N)
{
	if (!messageBlock || !inputMessage) return;

	char compareMessage[MAX_SIZE_MESSAGE_SCHEDULE + 1]{ SENTINEL_CHARACTER };

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), NEWLINE_CHARACTER);

	std::cout << "Your SHA-256 Message: ";
	std::cin.getline(compareMessage, MAX_SIZE_MESSAGE_SCHEDULE + 1, NEWLINE_CHARACTER);

	char* output = getFinalMessage(messageBlock, rows, inputMessage, inputLength, N);

	strCmp(compareMessage, output) == 0 ?
		std::cout << "SHA-256 Messages successfully matched!" << std::endl :
		std::cout << "SHA-256 Messages didn't match!" << std::endl;
}


void refreshScreen() {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}