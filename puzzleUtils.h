/*
 * puzzleUtils.h
 *
 *  Created on: Apr 1, 2014
 *      Author: papa
 */

#ifndef PUZZLEUTILS_H_
#define PUZZLEUTILS_H_

#include <stdint.h>

//
//  DESIGN
//
//  For each position, keep one unsigned 16-bit int (uint16_t) for each position:
//  bits   0-3: lower 4 bits are the value of the puzzle at that spot
//  bits  4-12: next 9 bits indicate whether or not that value is still possible here (1-9)
//  bits 13-15: unused

//
//  CONSTANTS
//
const int ROWS=9;  // array bound constants for puzzle
const int COLS=9;  //
const int POSS_VALS = 9;
const int SHIFT_FOR_POSS_VALS = 4;
const uint16_t SIXTEEN_ONES = 65535;  // 2^16 - 1

//
//  BITMASKS
//
const uint16_t PUZZ_VAL_MASK = 0x000F;   // 1st 4 bits only
const uint16_t POSS_VALS_MASK = 0x1FF0;    // bits 4-12 only

//
//  the position flag value = 2^(pos-1)
//
//  To get value out of the possible flags:
//     puzzleVal = puzzle[i][j]
//     puzzleVal && POSS_VALS_MASK
//     puzzleVal >> 4
//     if puzzleVal >> 4 is a power of 2, then you've found the puzzle value
//     puzzleVal = log2(puzzVal) + 1
//
//  example:
//     puzzleVal = 0x0000 0001 0010 0000  (0x0120)
//     shifted   = 0x0000 0000 0001 0010  (0x0012)
//  unset 2th bit= 0x0000 0000 0001 0000  (0x0002)
//    log2(16) + 1 = 4 + 1 = 5 is the puzzle value

//		0x0 == 0000
//		0x1 == 0001
//		0x2 == 0010
//		0x3 == 0011
//		0x4 == 0100
//		0x5 == 0101
//		0x6 == 0110
//		0x7 == 0111
//		0x8 == 1000
//		0x9 == 1001
//		0xA == 1010
//		0xB == 1011
//		0xC == 1100
//		0xD == 1101
//		0xE == 1110
//		0xF == 1111


//
//  TYPEDEFS
//
typedef uint16_t puzzleArray[ROWS][COLS];

//
// UTILITY FUNCTIONS
//
void printPuzzle(puzzleArray puzzleOut);
void printPossibleVals(puzzleArray puzzle);
bool isPowerOfTwo(uint16_t checkVal);

//
//  DEBUG FUNCTIONS
//
void printMask(uint16_t mask);


#endif /* PUZZLEUTILS_H_ */
