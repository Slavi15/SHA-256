#pragma once

#include <cstdint>

#include "GlobalVariables.h"

uint32_t calculateTEMP1(uint32_t decimal, uint32_t fragmentLength, uint32_t index);

uint32_t calculateTEMP2(uint32_t fragmentLength);

uint32_t SSIGSUMMATION(uint32_t fragmentLength, uint32_t ROTRFIRST, uint32_t ROTRSECOND, uint32_t SHIFTZEROS, uint32_t index);

uint32_t EPSUMMATION(uint32_t decimal, uint32_t fragmentLength, uint32_t ROTRFIRST, uint32_t ROTRSECOND, uint32_t ROTRTHIRD);

uint32_t CHOICE(uint32_t e, uint32_t f, uint32_t g);

uint32_t MAJORITY(uint32_t a, uint32_t b, uint32_t c);

uint32_t ROTATERIGHT(uint32_t decimalNumber, uint32_t bits, uint32_t fragmentLength);