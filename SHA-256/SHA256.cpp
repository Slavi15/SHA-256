#include <iostream>
#include <fstream>

using namespace std;

constexpr size_t SAFE_MAX_SIZE = 10000; // maximum size of input string
constexpr size_t INITIAL_BITS = 512; // initial bits in the Message Block

constexpr size_t MAX_SIZE_MESSAGE_SCHEDULE = 64; // maximum size of Message Schedule Rows

const char* INPUT_FILE_NAME = "sha256_input.txt"; // file path to the input file
const char* OUTPUT_FILE_NAME = "sha256_output.txt"; // file path to the output file

// Input/Output File Operations
int fReadString(const char* fileName, char* inputMessage, size_t size, size_t line);
int fWriteString(const char* fileName, const char* outputMessage, size_t outputLength);

int main()
{
	char inputMessage[SAFE_MAX_SIZE];
	fReadString(INPUT_FILE_NAME, inputMessage, SAFE_MAX_SIZE, 1);

	char outputMessage[MAX_SIZE_MESSAGE_SCHEDULE];

	fWriteString(OUTPUT_FILE_NAME, outputMessage, MAX_SIZE_MESSAGE_SCHEDULE);

	return 0;
}

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