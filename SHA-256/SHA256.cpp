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

char H[CHAR_SIZE_IN_BITS][FRAGMENT_SIZE + 1] = { // Hash Values Constants
	{ '0', '1', '1', '0', '1', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '1', '1', '1', '0', '0', '1', '1', '0', '0', '1', '1', '0', '0', '1', '1', '1', '\0' },
	{ '1', '0', '1', '1', '1', '0', '1', '1', '0', '1', '1', '0', '0', '1', '1', '1', '1', '0', '1', '0', '1', '1', '1', '0', '1', '0', '0', '0', '0', '1', '0', '1', '\0' },
	{ '0', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '0', '1', '1', '1', '0', '1', '1', '1', '1', '0', '0', '1', '1', '0', '1', '1', '1', '0', '0', '1', '0', '\0' },
	{ '1', '0', '1', '0', '0', '1', '0', '1', '0', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '0', '1', '0', '1', '0', '0', '1', '1', '1', '0', '1', '0', '\0' },
	{ '0', '1', '0', '1', '0', '0', '0', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '1', '0', '1', '0', '0', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '\0' },
	{ '1', '0', '0', '1', '1', '0', '1', '1', '0', '0', '0', '0', '0', '1', '0', '1', '0', '1', '1', '0', '1', '0', '0', '0', '1', '0', '0', '0', '1', '1', '0', '0', '\0' },
	{ '0', '0', '0', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '1', '1', '1', '1', '0', '1', '1', '0', '0', '1', '1', '0', '1', '0', '1', '0', '1', '1', '\0' },
	{ '0', '1', '0', '1', '1', '0', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '1', '1', '0', '0', '1', '1', '0', '1', '0', '0', '0', '1', '1', '0', '0', '1', '\0' }
};

const char* K[MAX_SIZE_MESSAGE_SCHEDULE] = { // K Constants Array
	"01000010100010100010111110011000", "01110001001101110100010010010001", "10110101110000001111101111001111", "11101001101101011101101110100101",
	"00111001010101101100001001011011", "01011001111100010001000111110001", "10010010001111111000001010100100", "10101011000111000101111011010101",
	"11011000000001111010101010011000", "00010010100000110101101100000001", "00100100001100011000010110111110", "01010101000011000111110111000011",
	"01110010101111100101110101110100", "10000000110111101011000111111110", "10011011110111000000011010100111", "11000001100110111111000101110100",
	"11100100100110110110100111000001", "11101111101111100100011110000110", "00001111110000011001110111000110", "00100100000011001010000111001100",
	"00101101111010010010110001101111", "01001010011101001000010010101010", "01011100101100001010100111011100", "01110110111110011000100011011010",
	"10011000001111100101000101010010", "10101000001100011100011001101101", "10110000000000110010011111001000", "10111111010110010111111111000111",
	"11000110111000000000101111110011", "11010101101001111001000101000111", "00000110110010100110001101010001", "00010100001010010010100101100111",
	"00100111101101110000101010000101", "00101110000110110010000100111000", "01001101001011000110110111111100", "01010011001110000000110100010011",
	"01100101000010100111001101010100", "01110110011010100000101010111011", "10000001110000101100100100101110", "10010010011100100010110010000101",
	"10100010101111111110100010100001", "10101000000110100110011001001011", "11000010010010111000101101110000", "11000111011011000101000110100011",
	"11010001100100101110100000011001", "11010110100110010000011000100100", "11110100000011100011010110000101", "00010000011010101010000001110000",
	"00011001101001001100000100010110", "00011110001101110110110000001000", "00100111010010000111011101001100", "00110100101100001011110010110101",
	"00111001000111000000110010110011", "01001110110110001010101001001010", "01011011100111001100101001001111", "01101000001011100110111111110011",
	"01110100100011111000001011101110", "01111000101001010110001101101111", "10000100110010000111100000010100", "10001100110001110000001000001000",
	"10010000101111101111111111111010", "10100100010100000110110011101011", "10111110111110011010001111110111", "11000110011100010111100011110010"
};

// Input/Output File Operations
int fReadString(const char* fileName, char* inputMessage, size_t size, size_t line);
int fWriteString(const char* fileName, const char* outputMessage, size_t outputLength);

void getFinalMessage(char** messageBlock, size_t ROWS, char* outputMessage, char* inputMessage, size_t inputLength, size_t NIterations);
void SHA256Iterations(char** messageBlock, char workingVariables[][FRAGMENT_SIZE + 1], unsigned& iteration);
void calculateSHA256Message(const char* binaryNumber, char* output, unsigned& arrayIndex, size_t messageLength, size_t symbolLength);

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
uint32_t calculateTEMP1(const char W[][FRAGMENT_SIZE + 1], const char workingVariables[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t index);
uint32_t calculateTEMP2(const char workingVariables[][FRAGMENT_SIZE + 1], size_t fragmentLength);
uint32_t SSIGSUMMATION(char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t SHIFTZEROS, size_t index);
uint32_t EPSUMMATION(const char* binaryNumber, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t ROTRTHIRD);
uint32_t CHOICE(const char* e, const char* f, const char* g, size_t fragmentLength);
uint32_t MAJORITY(const char* a, const char* b, const char* c, size_t fragmentLength);
uint32_t ROTATERIGHT(unsigned decimalNumber, unsigned bits, size_t fragmentLength);

void updateInitialHashValues(char workingVariables[][FRAGMENT_SIZE + 1], size_t workingVariablesLength, size_t fragmentLength);
void updateWorkingVariables(const char W[][FRAGMENT_SIZE + 1], char workingVariables[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t index);

void stringCopy(char* dest, const char* source);
void mySubstring(char* dest, const char* source, unsigned start, unsigned length);

unsigned getInputLength(const char* str);

void decimalToBinary(unsigned num, char to[], size_t fragmentLength, unsigned k);
uint32_t binaryToDecimal(const char* binary, size_t fragmentLength, unsigned k);
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

	char workingVariables[CHAR_SIZE_IN_BITS][FRAGMENT_SIZE + 1] = {
		{ '0', '1', '1', '0', '1', '0', '1', '0', '0', '0', '0', '0', '1', '0', '0', '1', '1', '1', '1', '0', '0', '1', '1', '0', '0', '1', '1', '0', '0', '1', '1', '1', '\0' },
		{ '1', '0', '1', '1', '1', '0', '1', '1', '0', '1', '1', '0', '0', '1', '1', '1', '1', '0', '1', '0', '1', '1', '1', '0', '1', '0', '0', '0', '0', '1', '0', '1', '\0' },
		{ '0', '0', '1', '1', '1', '1', '0', '0', '0', '1', '1', '0', '1', '1', '1', '0', '1', '1', '1', '1', '0', '0', '1', '1', '0', '1', '1', '1', '0', '0', '1', '0', '\0' },
		{ '1', '0', '1', '0', '0', '1', '0', '1', '0', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '0', '1', '0', '1', '0', '0', '1', '1', '1', '0', '1', '0', '\0' },
		{ '0', '1', '0', '1', '0', '0', '0', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '1', '0', '1', '0', '0', '1', '0', '0', '1', '1', '1', '1', '1', '1', '1', '\0' },
		{ '1', '0', '0', '1', '1', '0', '1', '1', '0', '0', '0', '0', '0', '1', '0', '1', '0', '1', '1', '0', '1', '0', '0', '0', '1', '0', '0', '0', '1', '1', '0', '0', '\0' },
		{ '0', '0', '0', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '1', '1', '1', '1', '0', '1', '1', '0', '0', '1', '1', '0', '1', '0', '1', '0', '1', '1', '\0' },
		{ '0', '1', '0', '1', '1', '0', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '1', '1', '0', '0', '1', '1', '0', '1', '0', '0', '0', '1', '1', '0', '0', '1', '\0' }
	};

	unsigned iteration = 0;
	for (size_t i = 0; i < NIterations + 1; i++)
		SHA256Iterations(messageBlock, workingVariables, iteration);

	unsigned arrayIndex = 0;
	for (size_t i = 0; i < CHAR_SIZE_IN_BITS; i++)
		calculateSHA256Message(H[i], outputMessage, arrayIndex, CHAR_SIZE_IN_BITS, CHAR_SIZE_IN_BITS / 2);
}

void SHA256Iterations(char** messageBlock, char workingVariables[][FRAGMENT_SIZE + 1], unsigned& iteration)
{
	if (!messageBlock)
		return;

	char W[MAX_SIZE_MESSAGE_SCHEDULE][FRAGMENT_SIZE + 1];
	fillMessageSchedule(messageBlock, W, MAX_SIZE_MESSAGE_SCHEDULE, FRAGMENT_SIZE, iteration);

	for (size_t index = 0; index < MAX_SIZE_MESSAGE_SCHEDULE; index++)
		updateWorkingVariables(W, workingVariables, FRAGMENT_SIZE, index);

	updateInitialHashValues(workingVariables, CHAR_SIZE_IN_BITS, FRAGMENT_SIZE);
}

void calculateSHA256Message(const char* binaryNumber, char* output, unsigned& arrayIndex, size_t messageLength, size_t symbolLength)
{
	if (!binaryNumber || !output)
		return;

	for (size_t i = 0, counter = 0; i < messageLength; i++, counter += symbolLength)
	{
		char binaryArray[CHAR_SIZE_IN_BITS / 2 + 1];
		mySubstring(binaryArray, binaryNumber, counter, symbolLength);

		output[arrayIndex] = integerToChar(binaryToDecimal(binaryArray, symbolLength, 2));
		arrayIndex++;
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
		binaryToDecimal(W[firstIndex], fragmentLength, 2) +
		binaryToDecimal(W[secondIndex], fragmentLength, 2);

	char binaryArray[FRAGMENT_SIZE + 1];
	decimalToBinary(summation, binaryArray, fragmentLength, 2);

	for (int j = fragmentLength; j >= 0; j--)
		j != fragmentLength ? W[i][j] = binaryArray[j] : W[i][j] = '\0';

	firstIndex++;
	secondIndex++;
}

void updateWorkingVariables(const char W[][FRAGMENT_SIZE + 1], char workingVariables[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t index)
{
	uint32_t TEMP1 = calculateTEMP1(W, workingVariables, fragmentLength, index);
	uint32_t TEMP2 = calculateTEMP2(workingVariables, fragmentLength);

	uint32_t firstSummation = binaryToDecimal(workingVariables[3], fragmentLength, 2) + TEMP1;
	uint32_t secondSummation = TEMP1 + TEMP2;

	char firstSummationArray[FRAGMENT_SIZE + 1];
	decimalToBinary(firstSummation, firstSummationArray, fragmentLength, 2);

	char secondSummationArray[FRAGMENT_SIZE + 1];
	decimalToBinary(secondSummation, secondSummationArray, fragmentLength, 2);

	for (int i = CHAR_SIZE_IN_BITS - 1; i >= 0; i--)
		switch (i)
		{
		case 4:
			stringCopy(workingVariables[i], firstSummationArray);
			break;
		case 0:
			stringCopy(workingVariables[i], secondSummationArray);
			break;
		default:
			stringCopy(workingVariables[i], workingVariables[i - 1]);
			break;
		}
}

void updateInitialHashValues(char workingVariables[][FRAGMENT_SIZE + 1], size_t workingVariablesLength, size_t fragmentLength)
{
	for (size_t i = 0; i < workingVariablesLength; i++)
	{
		uint32_t updatedHashValue = binaryToDecimal(workingVariables[i], fragmentLength, 2) + binaryToDecimal(H[i], fragmentLength, 2);
		char binaryUpdatedValue[FRAGMENT_SIZE + 1];
		decimalToBinary(updatedHashValue, binaryUpdatedValue, fragmentLength, 2);

		stringCopy(workingVariables[i], binaryUpdatedValue);
		stringCopy(H[i], binaryUpdatedValue);
	}
}

uint32_t calculateTEMP1(const char W[][FRAGMENT_SIZE + 1], const char workingVariables[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t index)
{
	uint32_t w = binaryToDecimal(W[index], fragmentLength, 2);
	uint32_t k = binaryToDecimal(K[index], fragmentLength, 2);
	uint32_t decimalH = binaryToDecimal(workingVariables[7], fragmentLength, 2);

	return w + k +
		EPSUMMATION(workingVariables[4], fragmentLength, 6, 11, 25) +
		CHOICE(workingVariables[4], workingVariables[5], workingVariables[6], fragmentLength) + decimalH;
}

uint32_t calculateTEMP2(const char workingVariables[][FRAGMENT_SIZE + 1], size_t fragmentLength)
{
	return EPSUMMATION(workingVariables[0], fragmentLength, 2, 13, 22) + MAJORITY(workingVariables[0], workingVariables[1], workingVariables[2], fragmentLength);
}

uint32_t SSIGSUMMATION(char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t SHIFTZEROS, size_t index)
{
	unsigned decimal = binaryToDecimal(W[index], fragmentLength, 2);

	return SHIFTZEROS == 3 ?
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 3) :
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 10);
}

uint32_t ROTATERIGHT(unsigned decimalNumber, unsigned bits, size_t fragmentLength)
{
	return ((decimalNumber >> bits) | (decimalNumber << (fragmentLength - bits)));
}

uint32_t EPSUMMATION(const char* binaryNumber, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t ROTRTHIRD)
{
	if (!binaryNumber)
		return 0;

	unsigned decimal = binaryToDecimal(binaryNumber, fragmentLength, 2);

	return ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ ROTATERIGHT(decimal, ROTRTHIRD, fragmentLength);
}

uint32_t CHOICE(const char* e, const char* f, const char* g, size_t fragmentLength)
{
	if (!e || !f || !g)
		return 0;

	unsigned decimalE = binaryToDecimal(e, fragmentLength, 2);
	unsigned decimalF = binaryToDecimal(f, fragmentLength, 2);
	unsigned decimalG = binaryToDecimal(g, fragmentLength, 2);

	return (decimalE & decimalF) ^ (~(decimalE)&decimalG);
}

uint32_t MAJORITY(const char* a, const char* b, const char* c, size_t fragmentLength)
{
	if (!a || !b || !c)
		return 0;

	unsigned decimalA = binaryToDecimal(a, fragmentLength, 2);
	unsigned decimalB = binaryToDecimal(b, fragmentLength, 2);
	unsigned decimalC = binaryToDecimal(c, fragmentLength, 2);

	return (decimalA & decimalB) ^ (decimalA & decimalC) ^ (decimalB & decimalC);
}

char** generateMessageBlockRows(size_t ROWS, size_t COLUMNS)
{
	char** mtx = new char* [ROWS];
	for (size_t i = 0; i < ROWS; i++)
	{
		mtx[i] = new char[CHAR_SIZE_IN_BITS + 1];
		for (size_t j = 0; j < COLUMNS; j++)
			mtx[i][j] = '\0';
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
	decimalToBinary(ch, binaryArray, charSize, 2);
	stringCopy(messageBlock[i], binaryArray);
}

void appendBigEndianInteger(char** messageBlock, size_t charSize, size_t blockLength, size_t inputLength)
{
	if (!messageBlock)
		return;

	size_t num = inputLength * charSize;

	char binaryArray[MAX_SIZE_MESSAGE_SCHEDULE + 1];
	decimalToBinary(num, binaryArray, MAX_SIZE_MESSAGE_SCHEDULE, 2);

	for (int i = blockLength - charSize, counter = 0; i < blockLength; i++, counter += charSize)
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

void mySubstring(char* dest, const char* source, unsigned start, unsigned length)
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

uint32_t binaryToDecimal(const char* binary, size_t fragmentLength, unsigned k)
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
}

bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}