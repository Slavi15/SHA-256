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
* Header: Generate SHA-256 Message
*
*/

#include "Hash.h"
#include "HelperFunctions.h"

unsigned int workingVariables[CHAR_SIZE_IN_BITS] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

unsigned int H[CHAR_SIZE_IN_BITS] = { // Hash Values Constants
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

const unsigned int K[MAX_SIZE_MESSAGE_SCHEDULE] = { // K Constants Array
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

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

void fillMessageSchedule(char** messageBlock, char W[][FRAGMENT_SIZE + 1], size_t blockLength, size_t fragmentLength, unsigned& iteration)
{
	if (!messageBlock)
		return;

	unsigned firstIndex = 0, secondIndex = 9;
	size_t iterationLengthMessageSchedule = blockLength / 4;

	for (size_t i = 0; i < blockLength; i++)
		i < iterationLengthMessageSchedule ?
		getMessageBlockValues(messageBlock, W, fragmentLength, i, iteration) :
		calculateNextRows(W, i, fragmentLength, firstIndex, secondIndex);
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
	unsigned int summation = SSIGSUMMATION(W, fragmentLength, 7, 18, 3, firstIndex + 1) +
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

unsigned int calculateTEMP1(const char W[][FRAGMENT_SIZE + 1], const unsigned int* workingVariables, size_t fragmentLength, size_t index)
{
	if (!workingVariables)
		return 0;

	unsigned int w = kSystemToDecimal(W[index], fragmentLength, 2);

	return w + K[index] +
		EPSUMMATION(workingVariables[4], fragmentLength, 6, 11, 25) +
		CHOICE(workingVariables[4], workingVariables[5], workingVariables[6]) + workingVariables[7];
}

unsigned int calculateTEMP2(const unsigned int* workingVariables, size_t fragmentLength)
{
	if (!workingVariables)
		return 0;

	return EPSUMMATION(workingVariables[0], fragmentLength, 2, 13, 22) +
		MAJORITY(workingVariables[0], workingVariables[1], workingVariables[2]);
}

unsigned int SSIGSUMMATION(const char W[][FRAGMENT_SIZE + 1], size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t SHIFTZEROS, unsigned int index)
{
	unsigned int decimal = kSystemToDecimal(W[index], fragmentLength, 2);

	return SHIFTZEROS == 3 ?
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 3) :
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 10);
}

unsigned int EPSUMMATION(unsigned int decimal, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t ROTRTHIRD)
{
	return ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ ROTATERIGHT(decimal, ROTRTHIRD, fragmentLength);
}

unsigned int CHOICE(unsigned int e, unsigned int f, unsigned int g)
{
	return (e & f) ^ (~(e)&g);
}

unsigned int MAJORITY(unsigned int a, unsigned int b, unsigned int c)
{
	return (a & b) ^ (a & c) ^ (b & c);
}

unsigned int ROTATERIGHT(unsigned int decimalNumber, size_t bits, size_t fragmentLength)
{
	return ((decimalNumber >> bits) | (decimalNumber << (fragmentLength - bits)));
}

void updateWorkingVariables(const char W[][FRAGMENT_SIZE + 1], unsigned int* workingVariables, size_t fragmentLength, size_t index)
{
	if (!workingVariables)
		return;

	unsigned int TEMP1 = calculateTEMP1(W, workingVariables, fragmentLength, index);
	unsigned int TEMP2 = calculateTEMP2(workingVariables, fragmentLength);

	unsigned int firstSummation = workingVariables[3] + TEMP1;
	unsigned int secondSummation = TEMP1 + TEMP2;

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

void updateInitialHashValues(unsigned int* workingVariables, size_t workingVariablesLength, size_t fragmentLength)
{
	if (!workingVariables)
		return;

	for (size_t i = 0; i < workingVariablesLength; i++)
	{
		unsigned int updatedHashValue = workingVariables[i] + H[i];
		workingVariables[i] = updatedHashValue;
		H[i] = updatedHashValue;
	}
}

char* getFinalMessage(char** messageBlock, size_t ROWS, char* inputMessage, size_t inputLength, size_t NIterations)
{
	if (!messageBlock || !inputMessage)
		return nullptr;

	encodeInputMessage(messageBlock, ROWS, inputMessage, inputLength);

	static char outputMessage[MAX_SIZE_MESSAGE_SCHEDULE + 1];
	outputMessage[MAX_SIZE_MESSAGE_SCHEDULE] = '\0';

	unsigned iteration = 0;
	for (size_t i = 0; i < NIterations + 1; i++)
		SHA256Iterations(messageBlock, workingVariables, iteration);

	unsigned arrayIndex = 0;
	for (size_t i = 0; i < CHAR_SIZE_IN_BITS; i++)
		calculateSHA256Message(H[i], outputMessage, arrayIndex, CHAR_SIZE_IN_BITS);

	return outputMessage;
}

void SHA256Iterations(char** messageBlock, unsigned int* workingVariables, unsigned& iteration)
{
	if (!messageBlock || !workingVariables)
		return;

	char W[MAX_SIZE_MESSAGE_SCHEDULE][FRAGMENT_SIZE + 1]; // Message Schedule Array
	fillMessageSchedule(messageBlock, W, MAX_SIZE_MESSAGE_SCHEDULE, FRAGMENT_SIZE, iteration);

	for (size_t index = 0; index < MAX_SIZE_MESSAGE_SCHEDULE; index++)
		updateWorkingVariables(W, workingVariables, FRAGMENT_SIZE, index);

	updateInitialHashValues(workingVariables, CHAR_SIZE_IN_BITS, FRAGMENT_SIZE);
}

void calculateSHA256Message(unsigned int hexNumber, char* output, unsigned& arrayIndex, size_t messageLength)
{
	if (!output)
		return;

	unsigned int iterations = 0;
	while (iterations != messageLength)
	{
		output[arrayIndex + (messageLength - iterations - 1)] = integerToChar(hexNumber & 0xf);
		iterations++;
		hexNumber >>= 4;
	}

	arrayIndex += messageLength;
}