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

#include "GlobalVariables.h"

#include "MathCalculations.h"

void fillMessageBlock(uint32_t* messageBlock, uint32_t ROWS, const char* input, uint32_t inputLength);

void encodeMessageBlock(uint32_t* messageBlock, const char* input, uint32_t fragmentLength, int charSizeInBits);

void extractBinaryRepresentation(uint32_t* messageBlock, uint32_t arrayIndex, uint32_t decimal, uint32_t& fragmentLength, int charSizeInBits);

void appendBigEndianInteger(uint32_t* messageBlock, uint32_t ROWS, uint32_t fragmentLength, uint32_t inputLength);

void fillMessageSchedule(uint32_t* messageBlock, uint32_t blockLength, uint32_t fragmentLength, uint32_t& iteration);

void calculateNextRows(uint32_t index, uint32_t fragmentLength, uint32_t& firstIndex, uint32_t& secondIndex);

void updateInitialHashValues(uint32_t workingVariablesLength, uint32_t fragmentLength);

void updateWorkingVariables(uint32_t fragmentLength, uint32_t index);

char* getFinalMessage(uint32_t* messageBlock, uint32_t ROWS, char* inputMessage, uint32_t inputLength, uint32_t NIterations);

void SHA256Iterations(uint32_t* messageBlock, uint32_t& iteration);

void calculateSHA256Message(uint32_t hexNumber, char* output, uint32_t& arrayIndex, uint32_t messageLength);