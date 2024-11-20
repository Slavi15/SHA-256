#include "GlobalVariables.h"

uint32_t workingVariables[CHAR_SIZE_IN_BITS] = {
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

uint32_t H[CHAR_SIZE_IN_BITS] = { // Hash Values Constants
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

uint32_t W[MAX_SIZE_MESSAGE_SCHEDULE]{ 0 }; // Message Schedule Array