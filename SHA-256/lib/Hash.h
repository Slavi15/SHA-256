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

#pragma once
//#include <cstddef> // Needed for compilation with g++

constexpr size_t MAX_SIZE_MESSAGE_SCHEDULE = 64; // maximum size of Message Schedule Rows
constexpr size_t FRAGMENT_SIZE = 32; // length of every encoded line in Message Schedule
constexpr size_t CHAR_SIZE_IN_BITS = 8;

void fillMessageBlock(unsigned int* messageBlock, size_t ROWS, const char* input, unsigned inputLength);

void encodeMessageBlock(unsigned int* messageBlock, const char* input, size_t fragmentLength, int charSizeInBits);

void extractBinaryRepresentation(unsigned int* messageBlock, unsigned arrayIndex, unsigned int decimal, size_t& fragmentLength, int charSizeInBits);

void appendBigEndianInteger(unsigned int* messageBlock, size_t ROWS, size_t fragmentLength, unsigned int inputLength);

void fillMessageSchedule(unsigned int* messageBlock, unsigned int* W, size_t blockLength, size_t fragmentLength, unsigned& iteration);

void calculateNextRows(unsigned int* W, size_t index, size_t fragmentLength, unsigned& firstIndex, unsigned& secondIndex);

unsigned int calculateTEMP1(unsigned int decimal, const unsigned int* workingVariables, size_t fragmentLength, size_t index);

unsigned int calculateTEMP2(const unsigned int* workingVariables, size_t fragmentLength);

unsigned int SSIGSUMMATION(const unsigned int* W, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t SHIFTZEROS, unsigned int index);

unsigned int EPSUMMATION(unsigned int decimal, size_t fragmentLength, size_t ROTRFIRST, size_t ROTRSECOND, size_t ROTRTHIRD);

unsigned int CHOICE(unsigned int e, unsigned int f, unsigned int g);

unsigned int MAJORITY(unsigned int a, unsigned int b, unsigned int c);

unsigned int ROTATERIGHT(unsigned int decimalNumber, size_t bits, size_t fragmentLength);

void updateInitialHashValues(unsigned int* workingVariables, size_t workingVariablesLength, size_t fragmentLength);

void updateWorkingVariables(const unsigned int* W, unsigned int* workingVariables, size_t fragmentLength, size_t index);

char* getFinalMessage(unsigned int* messageBlock, size_t ROWS, char* inputMessage, size_t inputLength, size_t NIterations);

void SHA256Iterations(unsigned int* messageBlock, unsigned int* workingVariables, unsigned& iteration);

void calculateSHA256Message(unsigned int hexNumber, char* output, unsigned& arrayIndex, size_t messageLength);