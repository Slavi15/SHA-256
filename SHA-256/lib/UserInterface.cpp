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

void menuLoop()
{
	char inputMessage[SAFE_MAX_SIZE];
	fReadString(INPUT_FILE_NAME, inputMessage, SAFE_MAX_SIZE, 1);

	unsigned inputLength = getInputLength(inputMessage);

	const size_t N = ((inputLength * CHAR_SIZE_IN_BITS) + MAX_SIZE_MESSAGE_SCHEDULE) / INITIAL_BITS;
	const size_t ROWS = (FRAGMENT_SIZE / 2) * ((inputLength / MAX_INPUT_CHUNK_LENGTH) + 1);

	unsigned int* messageBlock = new unsigned int[ROWS] {0};
	fillMessageBlock(messageBlock, ROWS, inputMessage, inputLength);

	bool exitCommand = false;

	while (!exitCommand)
	{
		refreshScreen();
		cout << "Welcome to SHA-256!" << endl;
		cout << "1. Generate SHA-256 Message" << endl;
		cout << "2. SHA-256 Integrity Check" << endl;
		cout << "3. Exit" << endl;

		unsigned int menuOption = 0;
		cout << "Enter command: ";
		cin >> menuOption;

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
			char compareMessage[MAX_SIZE_MESSAGE_SCHEDULE + 1];
			cin.ignore(128, '\n');
			cout << "Your SHA-256 Message: ";
			cin.getline(compareMessage, MAX_SIZE_MESSAGE_SCHEDULE + 1);

			char* output = getFinalMessage(messageBlock, ROWS, inputMessage, inputLength, N);

			if (myStrCmp(compareMessage, output) == 0)
				cout << "SHA-256 Messages successfully matched!" << endl;
			else
				cout << "SHA-256 Messages didn't match!" << endl;

			exitCommand = true;
		}
		break;
		case 3:
		{
			cout << "You have exited successfully!" << endl;
			exitCommand = true;
		}
		break;
		default:
		{
			refreshScreen();
			cout << "Invalid command! Please, enter (1-3)!" << endl;
		}
		break;
		}
	}

	delete[] messageBlock;
}


void refreshScreen() {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}