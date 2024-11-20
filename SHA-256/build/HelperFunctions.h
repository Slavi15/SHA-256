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

#include <iostream>

uint32_t getInputLength(const char* str);

uint32_t getOption();

bool isValidOption(uint32_t option);

uint32_t strLen(const char* str);

void strCpy(char* dest, const char* src);

void strCat(char* first, const char* second);

int strCmp(const char* first, const char* second);

uint32_t powerOfTwo(uint32_t n);

bool checkBit(uint32_t n, uint32_t index);

char integerToChar(int n);