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
//#include <cstddef> // Needed for compilation with g++

unsigned getInputLength(const char* str);

unsigned int myStrCmp(const char* first, const char* second);

unsigned int powerOfTwo(int n);

bool checkBit(unsigned int n, unsigned index);

char integerToChar(int n);

int charToInteger(char ch);