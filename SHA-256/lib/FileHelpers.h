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
* Header: Definitions of File Helper Functions
*
*/

#pragma once
#include <fstream>

using namespace std;

int fReadString(const char* fileName, char* inputMessage, size_t size, size_t line);

int fWriteString(const char* fileName, const char* outputMessage, size_t outputLength);