#ifndef _MAIN
#define _MAIN
#define ONE 0b01
#define ZERO 0b10
#define DC 0b00
#define MAX_VARIABLES 15
#define MIN_VARIABLES 1

/**
 * Prints a number by replacing every pair of bits with its equivalent in positional cube representation
*/
void printPositionalCube(int, int);

/**
 * Function that turns a natural number into his positional cube representation.
 * Every 1 is replaced by 01, every 0 is replaced by 10 and every don't care is
 * replaced by 00.
*/
int toPositionalCube(int, int);

/**
 * return the natural representation of n given its positional cube representation.
 * Requires n in positional cube representation.
*/
int fromPositionalCube(int);

/**
 * Returns number of literals in n.
 * Requires n in positional cube representation
*/
int literals(int);
#endif