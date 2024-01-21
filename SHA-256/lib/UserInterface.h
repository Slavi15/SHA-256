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
* Header: Main Menu
*
*/

#pragma once
#include <iostream>

using namespace std;

constexpr size_t SAFE_MAX_SIZE = 10000; // maximum size of input string
const size_t INITIAL_BITS = 512; // initial bits in the Message Block

const size_t MAX_INPUT_CHUNK_LENGTH = 56; // maximum number of changing rows in Message Schedule when we work with > 512-bit words

const char* const INPUT_FILE_NAME = "sha256_input.txt"; // file path to the input file
const char* const OUTPUT_FILE_NAME = "sha256_output.txt"; // file path to the output file

void menuLoop();

void refreshScreen();