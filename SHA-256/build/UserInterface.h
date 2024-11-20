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

void SHA256();

void integrityCheck(uint32_t* messageBlock, uint32_t rows, char* inputMessage, uint32_t inputLength, uint32_t N);

void refreshScreen();