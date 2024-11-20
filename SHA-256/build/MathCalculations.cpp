#include "MathCalculations.h"

uint32_t calculateTEMP1(uint32_t decimal, uint32_t fragmentLength, uint32_t index)
{
	if (!workingVariables)
		return 0;

	return decimal + K[index] +
		EPSUMMATION(workingVariables[4], fragmentLength, 6, 11, 25) +
		CHOICE(workingVariables[4], workingVariables[5], workingVariables[6]) + workingVariables[7];
}

uint32_t calculateTEMP2(uint32_t fragmentLength)
{
	if (!workingVariables)
		return 0;

	return EPSUMMATION(workingVariables[0], fragmentLength, 2, 13, 22) +
		MAJORITY(workingVariables[0], workingVariables[1], workingVariables[2]);
}

uint32_t SSIGSUMMATION(uint32_t fragmentLength, uint32_t ROTRFIRST, uint32_t ROTRSECOND, uint32_t SHIFTZEROS, uint32_t index)
{
	if (!W)
		return 0;

	uint32_t decimal = W[index];

	return SHIFTZEROS == 3 ?
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 3) :
		ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ (decimal >> 10);
}

uint32_t EPSUMMATION(uint32_t decimal, uint32_t fragmentLength, uint32_t ROTRFIRST, uint32_t ROTRSECOND, uint32_t ROTRTHIRD)
{
	return ROTATERIGHT(decimal, ROTRFIRST, fragmentLength) ^ ROTATERIGHT(decimal, ROTRSECOND, fragmentLength) ^ ROTATERIGHT(decimal, ROTRTHIRD, fragmentLength);
}

uint32_t CHOICE(uint32_t e, uint32_t f, uint32_t g)
{
	return (e & f) ^ (~(e)&g);
}

uint32_t MAJORITY(uint32_t a, uint32_t b, uint32_t c)
{
	return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t ROTATERIGHT(uint32_t decimalNumber, uint32_t bits, uint32_t fragmentLength)
{
	return ((decimalNumber >> bits) | (decimalNumber << (fragmentLength - bits)));
}