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

#include "Constants.h"
#include "HelperFunctions.h"

void fReadString(const char* fileName, char* inputMessage, uint32_t size);

void fReadString(std::ifstream& ifs, char* inputMessage, uint32_t size);

void fWriteString(const char* fileName, const char* outputMessage, uint32_t outputLength);

void fWriteString(std::ofstream& ofs, const char* outputMessage, uint32_t outputLength);