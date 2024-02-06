/**
 * maximum of 15 variables: every 2 bit represent a literal in positional cube representation:
 * x  01
 * x' 10
 * -  00
*/

#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_VARIABLES 15
#define MIN_VARIABLES 1

int cardinality(int);
int dcCount(int);
int criteria(int, int);
int compatible(int, int);
int fromPositionalCube(int);
void printBinary(int,int);
void printPositionalCube(int, int);
void printImplicantVariables(node_t*, int);
int toPositionalCube(int, int);
int fromPositionalCube(int);
int join(int, int);
void removeNonPrime(node_t**);
void removeDc(node_t**);
void printList(node_t*, int);
void executeQMC(node_t**,int);

/**
 * Counts number of 01 pairs (1) in n. Requires that n is in positional cube representation 
*/
int cardinality(int n) {
    int card = 0;
    while(n != 0) {
        card += n & 0b01;
        n >>= 2;
    }
    return card;
}

/**
 * Counts number of don't care (00) in n
*/
int dcCount(int n) {
    int dc = 0;
    while(n != 0) {
        dc += (~n & 0b11) == 0b11;
        n >>= 2;
    }
    return dc;
}

/**
 * Criteria to order implicants list. Returns 1 if
 * - a has less dc than b
 * - they have the same cardinality and a is less than b (in natural representation)
 * - cardinality of a is less than the cardinality of b
 * 0 otherwise.
*/
int criteria(int a, int b) {
    if(dcCount(a) != dcCount(b)) return dcCount(a) < dcCount(b);
    if(cardinality(a) == cardinality(b)) return fromPositionalCube(a) < fromPositionalCube(b);
    return cardinality(a) < cardinality(b);
}

/**
 * Returns 1 if a and b are compatible e.g. the don't care are in the same positions, 0 otherwise
*/
int compatible(int a, int b) {
    while (a != 0 || b != 0) {
        int a1 = (a >> 1) & 0b1;
        int a0 = a & 0b1;
        int b1 = (b >> 1) & 0b1;
        int b0 = b & 0b1;
        if(((~b1) & (~b0) & (a1 | a0)) | ((~a1) & (~a0) & (b1 | b0)) ) return 0;
        a >>= 2;
        b >>= 2;
    }
    return 1;
}

/**
 * Prints a number by replacing every pair of bits with its equivalent in positional cube representation
*/
void printPositionalCube(int n, int nOfVariables) {
    int c; 
    char symbols[] = {'-','1','0',0};
    int shamt = nOfVariables*2-2;
    while(nOfVariables-->0) {
        putchar(symbols[(n & 0b11<<shamt) >> shamt&0b11]);
        n <<= 2;
    }
}

/**
 * Print passed implicant using Boolean algebra notation
*/
void printImplicantVariables(node_t* implicant, int nOfVariables) {
    const char variables[] = {'x', 'y', 'z', 'w', 't', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};
    
    int n = getData(implicant);
    for(int i = 0;i < nOfVariables; i++) {
        switch(n & 0b11) {
            case 1:
                printf("%c",variables[i]);
                break;
            case 2:
                printf("%c'",variables[i]);
                break;
        };
        n >>= 2;
    }
}

/**
 * Function that turns a natural number into his positional cube representation.
 * Every 1 is replaced by 01, every 0 is replaced by 10 and every don't care is
 * replaced by 00.
*/
int toPositionalCube(int n, int nOfVariables) {
    int ps = 0;
    for(int i=0; i<nOfVariables && i<sizeof(int)*4; i++) {
        if(n>>i & 1) ps += (0b01 << (2*i));
        else ps += (0b10 << (2*i));
    }
    return ps;
}

/**
 * Returns b**e
*/
int pot(int b,int e) {
    if(e == 0) return 1;
    return b*pot(b,e-1);
}

/**
 * return the natural representation of n given its positional cube representation.
 * Requires n in positional cube representation.
*/
int fromPositionalCube(int n) {
    int i = 0, res = 0;
    while(n != 0) {
        res += pot(2,i)*(n&0b11%2);
        n >>= 2;
        i++;
    }
    return res;
}

/**
 * Function that joins two implicants into a single one by putting a don't care (00) on the first different pair of bits.
 * Requires that a and b have a Hamming distance of 1. If a and b have a Hamming distance different than 1, function
 * behaviour is undetermined.
*/
int join(int a, int b) {
    int i = 0;
    while((a >> 2*i) != 0) {
        if((((a >> 2*i) & 0b11) ^ ((b >> 2*i) & 0b11)) == 0b11) {
            a &= ~(0b11 << 2*i);
        }
        else i++;
    }
    return a;
}

/**
 * Remove all elements from structure that match or don't match a criteria:
 * if criteriaTrue = 1 every node that match the criteria is removed, else every node that doesn't match the criteria is removed
 * @param implicants head of the structure to access
 * @param criteria function pointer to use as a criteria
 * @param criteriaTrue boolean value to determine if node have to match o don't match criteria function to be removed
*/
void removeFromList(node_t** implicants, int (*criteria)(node_t*), int criteriaTrue) {
    node_t* cursor = *implicants;
    node_t* nextCursor = cursor;
    while(cursor != NULL) {
        nextCursor = next(cursor);
        if(criteriaTrue ? (*criteria)(cursor) : !(*criteria)(cursor)) removeNode(implicants, cursor);
        cursor = nextCursor;
    }
}

void printList(node_t* l, int n) {
    while(l != NULL) {
        printPositionalCube(l->data, n);
        printf("\t");
        printImplicantVariables(l, n);
        printf("\n");
        l = next(l);
    }
}

/**
 * Executes QuineMcCluskey method given a list of implicants and a number of variables. Passed list is modified to hold 
 * only prime implicants that are made with at least one minterm from the on set
 * @param implicants head of implicants (minterms) structure
 * @param nOfVariables number of considered variables (up to 16)
*/
void executeQMC(node_t** implicants, int nOfVariables) {
    node_t* cursor = *implicants;
    node_t* current;
    while(cursor != NULL) {
        node_t* nextImplicant = next(cursor);
        while(nextImplicant != NULL) {
            if (cardinality(getData(cursor) ^ getData(nextImplicant)) == 1 && compatible(getData(cursor), getData(nextImplicant))) {
                int joinedValue = join(getData(cursor), getData(nextImplicant));
                if(!contains(*implicants, joinedValue)) current = insertNode(implicants, joinedValue, &criteria);
                if(isDontCare(cursor) && isDontCare(nextImplicant)) setDontCare(current, 1);
                setEssential(cursor, 0);
                setEssential(nextImplicant, 0);
            }
            nextImplicant = next(nextImplicant);
        }
        cursor = next(cursor);
    }
    removeFromList(implicants, &isEssential, 0);
    removeFromList(implicants, &isDontCare, 1);
}

/**
 * Function that gets minterms from passed file. Set the new elements to dc if isDC is 1.
 * @param minterms head of structure to hold the minterms
 * @param nOfVariables number of considered variables (up to 16) 
 * @param isDC 1 if filename contains the don't care set, 0 otherwise
*/
void getMintermsFromFile(node_t** minterms, int nOfVariables, char* filename, int isDC) {
    FILE *fp;
    int minterm;
    node_t* current;

    fp = fopen(filename,"r");
    if(fp == NULL) {
        perror("Error while opening file");
        exit(1);
    }
    while(fscanf(fp, "%d", &minterm) == 1) {
        minterm = toPositionalCube(minterm, nOfVariables);
        if(contains(*minterms, minterm)) continue;
        if((current = insertNode(minterms, minterm, &criteria)) == NULL) {
            perror("Error while adding minterms\n");
            exit(1);    
        }
        if(isDC) setDontCare(current,1);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    int nOfVariables = 0;
    node_t* implicants = NULL;
    char* on_filename;
    char* dc_filename;
    if(argc != 4) {
        errno = EPERM;
        perror("Usage: quineMcCluskey on_set_filename dc_set_filename nOfVariables");
        exit(1);
    } else {
        on_filename = argv[1];
        dc_filename = argv[2];
        nOfVariables = atoi(argv[3]);
        if(nOfVariables > MAX_VARIABLES || nOfVariables < MIN_VARIABLES) {
            errno = EPERM;
            perror("Number of variables must be between 1 and 15 (included)");
            exit(1);
        }
    }
    getMintermsFromFile(&implicants, nOfVariables, on_filename, 0);
    getMintermsFromFile(&implicants, nOfVariables, dc_filename, 1);
    executeQMC(&implicants, nOfVariables);
    printList(implicants, nOfVariables);
}