#ifndef _MAIN
#define _MAIN
#define ONE 0b01
#define ZERO 0b10
#define DC 0b00
#define MAX_VARIABLES 15
#define MIN_VARIABLES 1

/**
 * Returns number of literals in n.
 * Requires n in positional cube representation
*/
int literals(int n);

/**
 * Prints passed matrix.
 * @param m pointer to matrix
 * @param row number of rows 
 * @param col number of columns
 * @param n number of variables considered 
*/
void printMatrix(int** m, int row, int col, int n);
#endif