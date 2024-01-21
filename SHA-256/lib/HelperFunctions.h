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
* Header: SHA-256 Helper Functions
*
*/

#pragma once
#include <cstddef> // Needed for compilation with g++

unsigned getInputLength(const char* str);

void stringCopy(char* dest, const char* source);

unsigned int myStrCmp(const char* first, const char* second);

void mySubstring(char* dest, const char* source, size_t start, size_t length);

void decimalToKSystem(unsigned num, char* to, size_t fragmentLength, unsigned k);

unsigned int kSystemToDecimal(const char* binary, size_t fragmentLength, unsigned k);

char integerToChar(int n);

int charToInteger(char ch);

bool isDigit(char ch);