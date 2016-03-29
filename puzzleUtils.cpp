/*
 * puzzleUtils.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: papa
 */

#include <stdio.h>   // for printf
#include <string>

#include "puzzleUtils.h"

//
//  UTILITY FUNCTIONS
//
void printPuzzle(puzzleArray puzzleOut)
{
	printf("=======================================\n");
	for (int i=0; i < ROWS; i++)
	{
		for (int j=0; j < COLS; j++)
		{
			int puzzleVal = puzzleOut[i][j] & PUZZ_VAL_MASK;
			if (puzzleVal != 0)
			{
				printf("  %d", puzzleVal);
			}
			else
			{
				printf("  .");
			}
		}
		printf("\n");
	}
	printf("=======================================\n");
}

void printPossibleVals(puzzleArray puzzle)
{
	for (int i=0; i < ROWS; i++)
	{
		for (int j=0; j < COLS; j++)
		{
			int puzzleVal = (int)(puzzle[i][j] & PUZZ_VAL_MASK);
			char puzzleValCh = '?';
			std::string possValString = "";
			char valString[5]="\0";

			if (puzzleVal != 0)   // solved puzzle position - just print the value
			{
				// this position is already solved, get the val as a char
				sprintf(valString, "%d", puzzleVal);
				puzzleValCh = valString[0];
			}
			else  // unsolved puzzle position - print what values are still possible here
			{
				// else print out the possible values left for this position
				uint16_t possVals = (puzzle[i][j] & POSS_VALS_MASK) >> SHIFT_FOR_POSS_VALS;

				possValString = " poss: ";

				// loop over the bits in the value
				for (int possible = 0; possible < POSS_VALS; possible++)
				{
					//  if the 0th-bit is 1, then the current "possible-bit" is flagged as true
					if (possVals & 1)
					{
						sprintf(valString, "%d", possible+1);
						possValString += valString[0];
					}
					possVals = possVals >> 1;
				}
			}

			printf("[%d][%d]:{%c}%s\n", i, j, puzzleValCh, possValString.c_str());
		}
	}
}

bool isPowerOfTwo(uint16_t checkVal)
{
	const int NUM_OF_BITS = 16;
	int bitsOnCount = 0;

	// checkVal is a power of two if only one bit is flipped.
	for (int i=0; i < NUM_OF_BITS; i++)
	{
		bitsOnCount += (checkVal & 1);
		checkVal = checkVal >> 1;
	}

	return bitsOnCount == 1;
}

//
//   DEBUG FUNCTIONS
//
void printMask(uint16_t mask)
{
	for (int i=0; i< 16; i++)
	{
		if (mask & 0x8000)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
		mask = mask << 1;
	}
}
