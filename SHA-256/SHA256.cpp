#include <iostream>
#include <fstream>

using namespace std;

constexpr size_t SAFE_MAX_SIZE = 10000; // maximum size of input string
constexpr size_t INITIAL_BITS = 512; // initial bits in the Message Block

constexpr size_t MAX_SIZE_MESSAGE_SCHEDULE = 64; // maximum size of Message Schedule Rows
constexpr size_t MAX_INPUT_CHUNK_LENGTH = 56; // maximum number of changing rows in Message Schedule when we work with > 512-bit words
constexpr size_t CHAR_SIZE_IN_BITS = 8;

const char* INPUT_FILE_NAME = "sha256_input.txt"; // file path to the input file
const char* OUTPUT_FILE_NAME = "sha256_output.txt"; // file path to the output file

// Input/Output File Operations
int fReadString(const char* fileName, char* inputMessage, size_t size, size_t line);
int fWriteString(const char* fileName, const char* outputMessage, size_t outputLength);

void getFinalMessage(char** messageBlock, size_t ROWS, char* outputMessage, char* inputMessage, size_t inputLength, size_t NIterations);

// Fill Message Block
void generateMessageBlockRows(char** messageBlock, size_t ROWS, size_t COLUMNS);
void encodeInputMessage(char** messageBlock, size_t blockLength, char* input, size_t inputLength);
void appendCharactersBinary(char** messageBlock, size_t charSize, size_t i, char ch);
void appendBigEndianInteger(char** messageBlock, size_t charSize, size_t blockLength, size_t inputLength);
void deleteMessageBlock(char** messageBlock, size_t ROWS);

void stringCopy(char* dest, const char* source);
void mySubstring(char* dest, const char* source, unsigned start, unsigned length);

unsigned getInputLength(const char* str);

void decimalToBinary(unsigned num, char to[], size_t fragmentLength, unsigned k);
char integerToChar(int n);
int charToInteger(char ch);

bool isDigit(char ch);

int main()
{
	char inputMessage[SAFE_MAX_SIZE];
	fReadString(INPUT_FILE_NAME, inputMessage, SAFE_MAX_SIZE, 1);

	char outputMessage[MAX_SIZE_MESSAGE_SCHEDULE];
	unsigned inputLength = getInputLength(inputMessage);

	const size_t N = ((inputLength * CHAR_SIZE_IN_BITS) + MAX_SIZE_MESSAGE_SCHEDULE) / INITIAL_BITS;
	const size_t ROWS = MAX_SIZE_MESSAGE_SCHEDULE * ((inputLength / MAX_INPUT_CHUNK_LENGTH) + 1);

	char** messageBlock = new char* [ROWS];

	generateMessageBlockRows(messageBlock, ROWS, CHAR_SIZE_IN_BITS);

	getFinalMessage(messageBlock, ROWS, outputMessage, inputMessage, inputLength, N);

	deleteMessageBlock(messageBlock, ROWS);

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

void getFinalMessage(char** messageBlock, size_t ROWS, char* outputMessage, char* inputMessage, size_t inputLength, size_t NIterations)
{
	if (!messageBlock || !outputMessage || !inputMessage)
		return;

	encodeInputMessage(messageBlock, ROWS, inputMessage, inputLength);
}

void generateMessageBlockRows(char** messageBlock, size_t ROWS, size_t COLUMNS)
{
	for (size_t i = 0; i < ROWS; i++)
	{
		messageBlock[i] = new char[CHAR_SIZE_IN_BITS + 1];
		for (size_t j = 0; j < COLUMNS; j++)
			messageBlock[i][j] = '0';
	}
}

void encodeInputMessage(char** messageBlock, size_t blockLength, char* input, size_t inputLength)
{
	for (size_t i = 0; i < inputLength; i++)
		appendCharactersBinary(messageBlock, CHAR_SIZE_IN_BITS, i, input[i]);

	for (size_t i = 0; i < CHAR_SIZE_IN_BITS; i++)
		i == 0 ? messageBlock[inputLength][i] = '1' : messageBlock[inputLength][i] = '0';

	appendBigEndianInteger(messageBlock, CHAR_SIZE_IN_BITS, blockLength, inputLength);
}

void appendCharactersBinary(char** messageBlock, size_t charSize, size_t i, char ch)
{
	char binaryArray[CHAR_SIZE_IN_BITS + 1];
	decimalToBinary(ch, binaryArray, charSize, 2);
	stringCopy(messageBlock[i], binaryArray);
}

void appendBigEndianInteger(char** messageBlock, size_t charSize, size_t blockLength, size_t inputLength)
{
	size_t num = inputLength * charSize;

	char binaryArray[MAX_SIZE_MESSAGE_SCHEDULE + 1];
	decimalToBinary(num, binaryArray, MAX_SIZE_MESSAGE_SCHEDULE, 2);

	for (int i = blockLength - CHAR_SIZE_IN_BITS, counter = 0; i < blockLength; i++, counter += 8)
	{
		char part[CHAR_SIZE_IN_BITS + 1];
		// fix my substring with '\0'
		mySubstring(part, binaryArray, counter, CHAR_SIZE_IN_BITS);
		// stringCopy(messageBlock[i], part);
		// TODO: add array length as parameter and make it a function
		for (size_t j = 0; j <= charSize; j++)
			messageBlock[i][j] = part[j];
	}
}

void deleteMessageBlock(char** messageBlock, size_t ROWS)
{
	for (size_t i = 0; i < ROWS; i++)
		delete[] messageBlock[i];

	delete[] messageBlock;
}

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

void mySubstring(char* dest, const char* source, unsigned start, unsigned length)
{
	if (!dest || !source)
		return;

	for (size_t i = 0; i < length; i++)
		if (source[start] != '\0')
		{
			dest[i] = source[start];
			start++;
		}
}

void stringCopy(char* dest, const char* source)
{
	if (!dest || !source)
		return;

	while (*source != '\0')
	{
		*dest = *source;
		dest++;
		source++;
	}
	*dest = '\0';
}

void decimalToBinary(unsigned num, char to[], size_t fragmentLength, unsigned k)
{
	for (int i = fragmentLength; i >= 0; i--)
	{
		if (i == fragmentLength)
		{
			to[i] = '\0';
			continue;
		}

		int mod = num % k;
		to[i] = integerToChar(mod);
		num /= k;
	}
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
}

bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}