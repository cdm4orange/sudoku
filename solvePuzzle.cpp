/*
 * solvePuzzle.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: C. Meacham
 */

#include <stdio.h>  //  for printf()
#include <math.h>   //  for pow()
#include <stdbool.h> //  for true, false

#include "solvePuzzle.h"
#include "puzzleUtils.h"

void initPossibleMatrix(puzzleArray puzzleIn)
{
	for (int row=0; row < ROWS; row++)
	{
		for (int col=0; col < COLS; col++)
		{
			if (puzzleIn[row][col] == 0)
			{
				// unknown puzzle value here - turn on all possible value
				//  flags.  For known values, flags will already be zero
				uint16_t tmpPuzzleVal = puzzleIn[row][col] | POSS_VALS_MASK;
				puzzleIn[row][col] = tmpPuzzleVal;
			}
		}
	}
}

//
//  FUNCTION:     turnOffBit
//  DESCRIPTION:  take in a 16-bit uint with 16 flags, and turn off the
//                n-th bit with xor.
//
void turnOffBit(uint16_t &bitSet, int whichBit)
{
	// first, shift the bits representing possible cell values
	// over to bits 0-8.
	bitSet = bitSet >> SHIFT_FOR_POSS_VALS;

	// what we're doing is flipping the nth bit to 0 if the
	// value "n" is found in this puzzle row/col/sector
	//   ex. the value 6 would change 1111 1111 1111 1111 to
	//                                1111 1111 1101 1111

	// this is such a mess because the pow() function only uses doubles
	uint16_t mask = (uint16_t)(pow(2.0, (double)(whichBit - 1)));

	// don't forget '^' is xor, not power.
	uint16_t newBitSet = bitSet ^ mask;

	bitSet = newBitSet << SHIFT_FOR_POSS_VALS;
}  // end turnOffBit()

//
//  FUNCTION:    setCellSolutionValue
//  DESCRIPTION: convert the bitset of eliminated values into an integer
//   			 value and return that integer
//
uint16_t getCellSolutionValue(uint16_t puzzleVal)
{
	puzzleVal = puzzleVal >> SHIFT_FOR_POSS_VALS;
	uint16_t whichBitWasSet = 1;

	// not going to check for error conditions for now,
	// because before this got called, isPowerOfTwo() was called to check
	// if only 1 bit was set
	while (!(puzzleVal & 1))
	{
		puzzleVal = puzzleVal >> 1;
		whichBitWasSet++;
	}
	return whichBitWasSet;
}  // end getCellSolutionValue()

//
//  FUNCTION:    findSolution
//  DESCRIPTION: use the bitset list of eliminated positions to find
//   			 the solution for a cell.  This function can be used
//               by any pass across the puzzle, horizontal, vertical
//               or sector.
//  RETURNS:   	 TRUE - if application of the eliminated value mask
//                      for this cell found a solution
//
bool findSolution( uint16_t &puzzleVal, uint16_t &elimVals )
{
	bool foundSolution(false);

	// - apply the eliminatedVals mask to any unknown
	//   position  (bit-wise AND)
	puzzleVal = puzzleVal & elimVals;

	// - If a cell's possible values bitset is a power of 2,
	//   then there's only one possible value left: you've
	//   found the solution for that cell
	if (isPowerOfTwo( puzzleVal ))
	{
		// - get the integer solution from the bitset of possible values
		puzzleVal = getCellSolutionValue(puzzleVal);

		// - eliminate value from being possible in other cells
		turnOffBit(elimVals, (int)puzzleVal);

		// - and start PASS 2 over for this column
		foundSolution = true;
	}
	return foundSolution;
}  // end findSolution()

//
//  FUNCTION:     horizontalPass
//  DESCRIPTION:  For every col in each row, if a value is present,
//				  eliminate it as possible for every other position in the
//	row. If there's only one value left that's possible, it logically must
//  be the solution for that cell, which then eliminates it as a possibility
//  for any other cell in the row.
//
bool horizontalPass( puzzleArray puzzle)
{
	bool done(true);
	for (int row=0; row < ROWS; row++)
	{
		//  PASS 1 - for each col in this row
		//    - find any known puzzle values, and create a bit mask from
		//      discovered values:  start with all 1's and knock bits out
		uint16_t eliminatedVals = SIXTEEN_ONES;

		for (int col=0; col < COLS; col++)
		{
			uint16_t puzzleVal = puzzle[row][col] & PUZZ_VAL_MASK;
			if (puzzleVal)
			{
				turnOffBit(eliminatedVals, puzzleVal);
			}
		}

		//  PASS 2 - for each cell in this row:
		//    - apply knowledge from PASS 1 to every unknown cell in the row
		int col = 0;
		do {
			//  if cell has no solution known yet,
			if (!(puzzle[row][col] & PUZZ_VAL_MASK))
			{
				done = false;
				if (findSolution(puzzle[row][col], eliminatedVals))
				{
					col = 0;
					continue;
				}
			}
			col++;
		} while (col < COLS);
	}
	return done;
}  // end horizontalPass

//
//  FUNCTION:     verticalPass
//  DESCRIPTION:  For every row in each col, if a value is present,
//				  eliminate it as possible for every other position in the
//	column. If there's only one value left that's possible, it logically must
//  be the solution for that cell, which then eliminates it as a possibility
//  for any other cell in the column.
//
bool verticalPass( puzzleArray puzzle )
{
	bool done(true);
	for (int col=0; col < COLS; col++)
	{
		//  PASS 1 - for each row in this column
		//    - find any known puzzle values, and create a bit mask from
		//      discovered values:  start with all 1's and knock bits out
		uint16_t eliminatedVals = SIXTEEN_ONES;

		for (int row=0; row < ROWS; row++)
		{
			uint16_t puzzleVal = puzzle[row][col] & PUZZ_VAL_MASK;
			if (puzzleVal)
			{
				turnOffBit(eliminatedVals, puzzleVal);
			}
		}

		//  PASS 2 - for each cell in this col:
		//    - apply knowledge from PASS 1 to every unknown cell in the col
		int row = 0;
		do {
			//  if cell has no solution known yet,
			if (!(puzzle[row][col] & PUZZ_VAL_MASK))
			{
				done = false;
				// if by applying the eliminated values, we find a solution,
				// then start PASS 2 over again
				if (findSolution(puzzle[row][col], eliminatedVals))
				{
					row = 0;
					continue;
				}
			}
			row++;
		} while (row < ROWS);
	}
	return done;
}  // end verticalPass

//
//  FUNCTION:     sectorlPass
//  DESCRIPTION:  For every 3x3 sector in the puzzle, if a value is present,
//				  eliminate it as possible for every other position in the
//	sector. If there's only one value left that's possible, it logically must
//  be the solution for that cell, which then eliminates it as a possibility
//  for any other cell in the sector.
//
bool sectorPass( puzzleArray puzzle )
{
	bool done(true);
	for (int squareOneRow=0; squareOneRow < ROWS; squareOneRow +=3 )
	{
		for (int squareOneCol=0; squareOneCol < COLS; squareOneCol +=3)
		{
			//  PASS 1 - for each cell in this sector
			//    - find any known puzzle values, and create a bit mask from
			//      discovered values:  start with all 1's and knock bits out
			uint16_t eliminatedVals = SIXTEEN_ONES;
			for (int row=squareOneRow; row < squareOneRow+3; row++)
			{
				for (int col=squareOneCol; col < squareOneCol+3; col++)
				{
					uint16_t puzzleVal = puzzle[row][col] & PUZZ_VAL_MASK;
					if (puzzleVal)
					{
						turnOffBit(eliminatedVals, puzzleVal);
					}
				}  // col loop
			}  // row loop

			//  PASS 2 - for each cell in this sector:
			//    - apply knowledge from PASS 1 to every unknown cell in the sector
			for (int row=squareOneRow; row < squareOneRow+3; row++)
			{
				for (int col=squareOneCol; col < squareOneCol+3; col++)
				{
					//  if cell has no solution known yet,
					if (!(puzzle[row][col] & PUZZ_VAL_MASK))
					{
						done = false;
						// if by applying the eliminated values, we find a solution,
						// then start PASS 2 over again
						if (findSolution(puzzle[row][col], eliminatedVals))
						{
							row = squareOneRow;
							col = squareOneCol;
							break;
						}
					}
				}
			}
		} // end squareOneCol loop
	} // end squareOneRow loop
	return done;
}  // end sectorPass

void solvePuzzle( puzzleArray puzzle)
{
	initPossibleMatrix(puzzle);
	bool done(false);
	int passCount(0);

	while (!done && passCount < 10)
	{
		passCount++;
		if (horizontalPass(puzzle) ||
			verticalPass(puzzle) ||
			sectorPass(puzzle))
		{
			done = true;
		}
	}
	if (done)
	{
		printf("Done in %d passes.\n\n", passCount);
	}
	else
	{
		printf("NOT DONE AFTER 10 PASSES.\n");
	}
}
