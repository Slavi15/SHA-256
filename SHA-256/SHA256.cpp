#include <iostream>
#include <fstream>

using namespace std;

constexpr size_t SAFE_MAX_SIZE = 10000; // maximum size of input string
constexpr size_t INITIAL_BITS = 512; // initial bits in the Message Block

constexpr size_t MAX_SIZE_MESSAGE_SCHEDULE = 64; // maximum size of Message Schedule Rows
constexpr size_t MAX_INPUT_CHUNK_LENGTH = 56; // maximum number of changing rows in Message Schedule when we work with > 512-bit words
constexpr size_t CHAR_SIZE_IN_BITS = 8;
constexpr size_t FRAGMENT_SIZE = 32; // length of every encoded line in Message Schedule

const char* INPUT_FILE_NAME = "sha256_input.txt"; // file path to the input file
const char* OUTPUT_FILE_NAME = "sha256_output.txt"; // file path to the output file

uint32_t H[CHAR_SIZE_IN_BITS] = { // Hash Values Constants
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

uint32_t K[MAX_SIZE_MESSAGE_SCHEDULE] = { // K Constants Array
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Input/Output File Operations
int fReadString(const char* fileName, char* inputMessage, size_t size, size_t line);
int fWriteString(const char* fileName, const char* outputMessage, size_t outputLength);

void getFinalMessage(char** messageBlock, size_t ROWS, char* outputMessage, char* inputMessage, size_t inputLength, size_t NIterations);
void SHA256Iterations(char** messageBlock, uint32_t workingVariables[], unsigned& iteration);
void calculateSHA256Message(uint32_t hexNumber, char* output, unsigned& arrayIndex, size_t messageLength, size_t fragmentLength, size_t symbolLength);

// Fill Message Block
char** generateMessageBlockRows(size_t ROWS, size_t COLUMNS);
void encodeInputMessage(char** messageBlock, size_t blockLength, char* input, size_t inputLength);
void appendCharactersBinary(char** messageBlock, size_t charSize, size_t i, char ch);
void appendBigEndianInteger(char** messageBlock, size_t charSize, size_t blockLength, size_t inputLength);
void deleteMessageBlock(char** messageBlock, size_t ROWS);

// Fill Message Schedule
void fillMessageSchedule(char** messageBlock, char W[][FRAGMENT_SIZE + 1], size_t blockLength, size_t fragmentLength, unsigned& iteration);
void getMessageBlockValues(char** messageBlock, char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t i, unsigned& iteration);
void calculateNextRows(char W[][FRAGMENT_SIZE + 1], size_t i, size_t fragmentLength, unsigned& firstIndex, unsigned& secondIndex);

// SHA-256 Calculations
uint32_t calculateTEMP1(const char W[][FRAGMENT_SIZE + 1], const uint32_t workingVariables[], size_t fragmentLength, size_t index);
uint32_t calculateTEMP2(const uint32_t workingVariables[], size_t fragmentLength);
uint32_t SSIGSUMMATION(const char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t SHIFTZEROS, size_t index);
uint32_t EPSUMMATION(uint32_t decimal, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t ROTRTHIRD);
uint32_t CHOICE(uint32_t e, uint32_t f, uint32_t g);
uint32_t MAJORITY(uint32_t a, uint32_t b, uint32_t c);
uint32_t ROTATERIGHT(uint32_t decimalNumber, size_t bits, size_t fragmentLength);

void updateInitialHashValues(uint32_t workingVariables[], size_t workingVariablesLength, size_t fragmentLength);
void updateWorkingVariables(const char W[][FRAGMENT_SIZE + 1], uint32_t workingVariables[], size_t fragmentLength, size_t index);

void stringCopy(char* dest, const char* source);
void mySubstring(char* dest, const char* source, size_t start, size_t length);

unsigned getInputLength(const char* str);

void decimalToKSystem(unsigned num, char to[], size_t fragmentLength, unsigned k);
uint32_t kSystemToDecimal(const char* binary, size_t fragmentLength, unsigned k);
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

	char** messageBlock = generateMessageBlockRows(ROWS, CHAR_SIZE_IN_BITS);

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

	uint32_t workingVariables[CHAR_SIZE_IN_BITS] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	unsigned iteration = 0;
	for (size_t i = 0; i < NIterations + 1; i++)
		SHA256Iterations(messageBlock, workingVariables, iteration);

	unsigned arrayIndex = 0;
	for (size_t i = 0; i < CHAR_SIZE_IN_BITS; i++)
		calculateSHA256Message(H[i], outputMessage, arrayIndex, CHAR_SIZE_IN_BITS, FRAGMENT_SIZE, CHAR_SIZE_IN_BITS / 2);
}

void SHA256Iterations(char** messageBlock, uint32_t workingVariables[], unsigned& iteration)
{
	if (!messageBlock)
		return;

	char W[MAX_SIZE_MESSAGE_SCHEDULE][FRAGMENT_SIZE + 1]; // Message Schedule Array
	fillMessageSchedule(messageBlock, W, MAX_SIZE_MESSAGE_SCHEDULE, FRAGMENT_SIZE, iteration);

	for (size_t index = 0; index < MAX_SIZE_MESSAGE_SCHEDULE; index++)
		updateWorkingVariables(W, workingVariables, FRAGMENT_SIZE, index);

	updateInitialHashValues(workingVariables, CHAR_SIZE_IN_BITS, FRAGMENT_SIZE);
}

void calculateSHA256Message(uint32_t hexNumber, char* output, unsigned& arrayIndex, size_t messageLength, size_t fragmentLength, size_t symbolLength)
{
	if (!output)
		return;

	char binaryNumber[FRAGMENT_SIZE + 1];
	binaryNumber[fragmentLength] = '\0';
	decimalToKSystem(hexNumber, binaryNumber, fragmentLength, 2);

	for (size_t i = 0, counter = 0; i < messageLength; i++, counter += symbolLength)
	{
		char binaryChar[CHAR_SIZE_IN_BITS / 2 + 1];
		mySubstring(binaryChar, binaryNumber, counter, symbolLength);

		output[arrayIndex++] = integerToChar(kSystemToDecimal(binaryChar, symbolLength, 2));
	}
}

void fillMessageSchedule(char** messageBlock, char W[][FRAGMENT_SIZE + 1], size_t blockLength, size_t fragmentLength, unsigned& iteration)
{
	if (!messageBlock)
		return;

	unsigned firstIndex = 0, secondIndex = 9;
	size_t iterationLengthMessageSchedule = blockLength / 4;

	for (size_t i = 0; i < blockLength; i++)
		i < iterationLengthMessageSchedule ? getMessageBlockValues(messageBlock, W, fragmentLength, i, iteration) : calculateNextRows(W, i, fragmentLength, firstIndex, secondIndex);
}

void getMessageBlockValues(char** messageBlock, char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t i, unsigned& iteration)
{
	if (!messageBlock)
		return;

	for (unsigned j = 0, counter = 0; j < fragmentLength; j++, (++counter) %= CHAR_SIZE_IN_BITS)
	{
		j != fragmentLength ? W[i][j] = messageBlock[iteration][counter] : W[i][j] = '\0';
		if (counter == CHAR_SIZE_IN_BITS - 1)
			iteration++;
	}
}

void calculateNextRows(char W[][FRAGMENT_SIZE + 1], size_t i, size_t fragmentLength, unsigned& firstIndex, unsigned& secondIndex)
{
	uint32_t summation = SSIGSUMMATION(W, fragmentLength, 7, 18, 3, firstIndex + 1) +
		SSIGSUMMATION(W, fragmentLength, 17, 19, 10, secondIndex + 5) +
		kSystemToDecimal(W[firstIndex], fragmentLength, 2) +
		kSystemToDecimal(W[secondIndex], fragmentLength, 2);

	char binaryArray[FRAGMENT_SIZE + 1];
	decimalToKSystem(summation, binaryArray, fragmentLength, 2);

	for (int j = fragmentLength; j >= 0; j--)
		j != fragmentLength ? W[i][j] = binaryArray[j] : W[i][j] = '\0';

	firstIndex++;
	secondIndex++;
}

void updateWorkingVariables(const char W[][FRAGMENT_SIZE + 1], uint32_t workingVariables[], size_t fragmentLength, size_t index)
{
	uint32_t TEMP1 = calculateTEMP1(W, workingVariables, fragmentLength, index);
	uint32_t TEMP2 = calculateTEMP2(workingVariables, fragmentLength);

	uint32_t firstSummation = workingVariables[3] + TEMP1;
	uint32_t secondSummation = TEMP1 + TEMP2;

	for (int i = CHAR_SIZE_IN_BITS - 1; i >= 0; i--)
		switch (i)
		{
		case 4:
			workingVariables[i] = firstSummation;
			break;
		case 0:
			workingVariables[i] = secondSummation;
			break;
		default:
			workingVariables[i] = workingVariables[i - 1];
			break;
		}
}

void updateInitialHashValues(uint32_t workingVariables[], size_t workingVariablesLength, size_t fragmentLength)
{
	for (size_t i = 0; i < workingVariablesLength; i++)
	{
		uint32_t updatedHashValue = workingVariables[i] + H[i];
		workingVariables[i] = updatedHashValue;
		H[i] = updatedHashValue;
	}
}

uint32_t calculateTEMP1(const char W[][FRAGMENT_SIZE + 1], const uint32_t workingVariables[], size_t fragmentLength, size_t index)
{
	uint32_t w = kSystemToDecimal(W[index], fragmentLength, 2);

	return w + K[index] +
		EPSUMMATION(workingVariables[4], fragmentLength, 6, 11, 25) +
		CHOICE(workingVariables[4], workingVariables[5], workingVariables[6]) + workingVariables[7]; // h
}

uint32_t calculateTEMP2(const uint32_t workingVariables[], size_t fragmentLength)
{
	return EPSUMMATION(workingVariables[0], fragmentLength, 2, 13, 22) +
		MAJORITY(workingVariables[0], workingVariables[1], workingVariables[2]);
}

uint32_t SSIGSUMMATION(const char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t SHIFTZEROS, size_t index)
{
	uint32_t decimal = kSystemToDecimal(W[index], fragmentLength, 2);

	return SHIFTZEROS == 3 ?
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 3) :
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 10);
}

uint32_t ROTATERIGHT(uint32_t decimalNumber, size_t bits, size_t fragmentLength)
{
	return ((decimalNumber >> bits) | (decimalNumber << (fragmentLength - bits)));
}

uint32_t EPSUMMATION(uint32_t decimal, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t ROTRTHIRD)
{
	return ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ ROTATERIGHT(decimal, ROTRTHIRD, fragmentLength);
}

uint32_t CHOICE(uint32_t e, uint32_t f, uint32_t g)
{
	return (e & f) ^ (~(e) & g);
}

uint32_t MAJORITY(uint32_t a, uint32_t b, uint32_t c)
{
	return (a & b) ^ (a & c) ^ (b & c);
}

char** generateMessageBlockRows(size_t ROWS, size_t COLUMNS)
{
	char** mtx = new char* [ROWS];

	for (size_t i = 0; i < ROWS; i++)
	{
		mtx[i] = new char[COLUMNS + 1];

		for (size_t j = 0; j < COLUMNS; j++)
			mtx[i][j] = '0';
	}

	return mtx;
}

void encodeInputMessage(char** messageBlock, size_t blockLength, char* input, size_t inputLength)
{
	if (!messageBlock)
		return;

	for (size_t i = 0; i < inputLength; i++)
		appendCharactersBinary(messageBlock, CHAR_SIZE_IN_BITS, i, input[i]);

	for (size_t i = 0; i < CHAR_SIZE_IN_BITS; i++)
		i == 0 ? messageBlock[inputLength][i] = '1' : messageBlock[inputLength][i] = '0';

	appendBigEndianInteger(messageBlock, CHAR_SIZE_IN_BITS, blockLength, inputLength);
}

void appendCharactersBinary(char** messageBlock, size_t charSize, size_t i, char ch)
{
	if (!messageBlock)
		return;

	char binaryArray[CHAR_SIZE_IN_BITS + 1];
	decimalToKSystem(ch, binaryArray, charSize, 2);
	stringCopy(messageBlock[i], binaryArray);
}

void appendBigEndianInteger(char** messageBlock, size_t charSize, size_t blockLength, size_t inputLength)
{
	if (!messageBlock)
		return;

	unsigned num = inputLength * charSize;

	char binaryArray[MAX_SIZE_MESSAGE_SCHEDULE + 1];
	decimalToKSystem(num, binaryArray, MAX_SIZE_MESSAGE_SCHEDULE, 2);

	for (size_t i = blockLength - charSize, counter = 0; i < blockLength; i++, counter += charSize)
	{
		char part[CHAR_SIZE_IN_BITS + 1];
		mySubstring(part, binaryArray, counter, charSize);
		stringCopy(messageBlock[i], part);
	}
}

void deleteMessageBlock(char** messageBlock, size_t ROWS)
{
	if (!messageBlock)
		return;

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

void decimalToKSystem(unsigned num, char to[], size_t fragmentLength, unsigned k)
{
	to[fragmentLength] = '\0';

	for (int i = fragmentLength - 1; i >= 0; i--)
	{
		int mod = num % k;
		to[i] = integerToChar(mod);
		num /= k;
	}
}

uint32_t kSystemToDecimal(const char* binary, size_t fragmentLength, unsigned k)
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