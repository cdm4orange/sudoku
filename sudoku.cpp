//============================================================================
// Name        : sudoku.c
// Author      : C. Meacham
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <stdio.h>  // printf

#include "solvePuzzle.h"
#include "puzzleUtils.h"


int main(int argc, char** argv)
{
	printf("Sudoku Solver\n\n");

	puzzleArray puzzle =
	{
		{ 5, 0, 0, 0, 0, 1, 0, 0, 0},
		{ 0, 0, 0, 3, 4, 5, 0, 7, 0},
	    { 4, 0, 1, 0, 2, 0, 0, 0, 6},
	    { 0, 2, 8, 0, 5, 7, 3, 4, 0},
	    { 0, 6, 0, 0, 0, 0, 0, 5, 0},
	    { 0, 5, 3, 9, 8, 0, 1, 6, 0},
	    { 2, 0, 0, 0, 7, 0, 6, 0, 3},
	    { 0, 9, 0, 5, 3, 6, 0, 0, 0},
	    { 0, 0, 0, 1, 0, 0, 0, 0, 5}
	};
	printf("Original Puzzle\n");
	printPuzzle(puzzle);

	solvePuzzle(puzzle);

	printf("Solved Puzzle\n");
	printPuzzle(puzzle);
}
