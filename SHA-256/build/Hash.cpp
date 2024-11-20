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

void fillMessageBlock(uint32_t* messageBlock, uint32_t ROWS, const char* input, uint32_t inputLength)
{
	if (!messageBlock || !input) return;

	encodeMessageBlock(messageBlock, input, FRAGMENT_SIZE - 1, CHAR_SIZE_IN_BITS);
	appendBigEndianInteger(messageBlock, ROWS, FRAGMENT_SIZE, inputLength);
}

void encodeMessageBlock(uint32_t* messageBlock, const char* input, uint32_t fragmentLength, int charSizeInBits)
{
	if (!messageBlock || !input) return;

	uint32_t arrayIndex = 0, inputIndex = 0;

	while (input[inputIndex] != '\0')
	{
		uint32_t decimal = input[inputIndex];
		extractBinaryRepresentation(messageBlock, arrayIndex, decimal, fragmentLength, charSizeInBits);

		inputIndex++;

		if (inputIndex % 4 == 0)
		{
			fragmentLength = FRAGMENT_SIZE - 1;
			arrayIndex++;
		}
	}

	messageBlock[arrayIndex] += powerOfTwo(fragmentLength);
}

void extractBinaryRepresentation(uint32_t* messageBlock, uint32_t arrayIndex, uint32_t decimal, uint32_t& fragmentLength, int charSizeInBits)
{
	if (!messageBlock) return;

	for (int i = charSizeInBits - 1; i >= 0; i--)
	{
		if (checkBit(decimal, i))
		{
			messageBlock[arrayIndex] += powerOfTwo(fragmentLength);
		}

		fragmentLength--;
	}
}

void appendBigEndianInteger(uint32_t* messageBlock, uint32_t ROWS, uint32_t fragmentLength, uint32_t inputLength)
{
	if (!messageBlock) return;

	uint32_t inputLengthBits = inputLength * CHAR_SIZE_IN_BITS;
	uint32_t bigEndianBitsPosition = 0;

	while (inputLengthBits != 0)
	{
		if (checkBit(inputLengthBits, 0))
		{
			messageBlock[ROWS - 1] += powerOfTwo(bigEndianBitsPosition);
		}

		inputLengthBits >>= 1;
		bigEndianBitsPosition++;

		if (bigEndianBitsPosition == fragmentLength - 1)
		{
			bigEndianBitsPosition = 0;
			ROWS--;
		}
	}
}

void fillMessageSchedule(uint32_t* messageBlock, uint32_t blockLength, uint32_t fragmentLength, uint32_t& iteration)
{
	if (!messageBlock) return;

	uint32_t firstIndex = 0, secondIndex = 9;
	uint32_t iterationLengthMessageSchedule = blockLength / 4;

	for (uint32_t index = 0; index < blockLength; index++)
	{
		if (index < iterationLengthMessageSchedule)
		{
			W[index] = messageBlock[iteration++];
		}
		else
		{
			calculateNextRows(index, fragmentLength, firstIndex, secondIndex);
		}
	}
}

void calculateNextRows(uint32_t index, uint32_t fragmentLength, uint32_t& firstIndex, uint32_t& secondIndex)
{
	uint32_t summation = SSIGSUMMATION(fragmentLength, 7, 18, 3, firstIndex + 1) +
		SSIGSUMMATION(fragmentLength, 17, 19, 10, secondIndex + 5) +
		W[firstIndex++] + W[secondIndex++];

	W[index] = summation;
}

void updateWorkingVariables(uint32_t fragmentLength, uint32_t index)
{
	uint32_t TEMP1 = calculateTEMP1(W[index], fragmentLength, index);
	uint32_t TEMP2 = calculateTEMP2(fragmentLength);

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

void updateInitialHashValues(uint32_t workingVariablesLength, uint32_t fragmentLength)
{
	for (uint32_t i = 0; i < workingVariablesLength; i++)
	{
		uint32_t updatedHashValue = workingVariables[i] + H[i];

		workingVariables[i] = updatedHashValue;

		H[i] = updatedHashValue;
	}
}

char* getFinalMessage(uint32_t* messageBlock, uint32_t ROWS, char* inputMessage, uint32_t inputLength, uint32_t NIterations)
{
	if (!messageBlock || !inputMessage) return nullptr;

	static char outputMessage[MAX_SIZE_MESSAGE_SCHEDULE + 1];
	outputMessage[MAX_SIZE_MESSAGE_SCHEDULE] = '\0';

	uint32_t iteration = 0;

	for (uint32_t i = 0; i < NIterations + 1; i++)
	{
		SHA256Iterations(messageBlock, iteration);
	}

	uint32_t arrayIndex = 0;

	for (uint32_t i = 0; i < CHAR_SIZE_IN_BITS; i++)
	{
		calculateSHA256Message(H[i], outputMessage, arrayIndex, CHAR_SIZE_IN_BITS);
	}

	return outputMessage;
}

void SHA256Iterations(uint32_t* messageBlock, uint32_t& iteration)
{
	if (!messageBlock) return;

	fillMessageSchedule(messageBlock, MAX_SIZE_MESSAGE_SCHEDULE, FRAGMENT_SIZE, iteration);

	for (uint32_t index = 0; index < MAX_SIZE_MESSAGE_SCHEDULE; index++)
	{
		updateWorkingVariables(FRAGMENT_SIZE, index);
	}

	updateInitialHashValues(CHAR_SIZE_IN_BITS, FRAGMENT_SIZE);
}

void calculateSHA256Message(uint32_t hexNumber, char* output, uint32_t& arrayIndex, uint32_t messageLength)
{
	if (!output) return;

	uint32_t iterations = 0;

	while (iterations != messageLength)
	{
		output[arrayIndex + (messageLength - iterations - 1)] = integerToChar(hexNumber & 0xf);
		iterations++;
		hexNumber >>= 4;
	}

	arrayIndex += messageLength;
}