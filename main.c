/**
 * MAIN FILE
 * maximum of 15 variables: every 2 bit represent a literal in positional cube representation:
 * x  01
 * x' 10
 * -  00
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "main.h"
#include "petrick.h"

static const char variables[] = {'x', 'y', 'z', 'w', 't', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};


/**
 * Joins a and b by putting a DC in place of the different digit. 
 * Requires that a and b are in positional cube representation
 * and that they are compatible
*/
int join(int a, int b);

/**
 * Returns 1 if a and b are compatible e.g. the don't care are in the same positions, 0 otherwise
*/
int compatible(int, int);

/**
 * Criteria to order implicants list. Returns 1 if
 * - a has less dc than b
 * - they have the same cardinality and a is less than b (in natural representation)
 * - cardinality of a is less than the cardinality of b
 * 0 otherwise.
*/
int criteria(int, int);

/**
 * Print passed implicant using Boolean algebra notation
*/
void printImplicantVariables(int, int);

/**
 * Counts number of don't care (00) in n
*/
int dcCount(int);

/**
 * Counts number of 01 pairs (1) in n. Requires that n is in positional cube representation 
*/
int cardinality(int);

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
 * return the natural representation of passed number given its positional cube representation.
 * Requires n in positional cube representation.
*/
int fromPositionalCube(int);

void printMatrix(int** m, int row, int col, int n) {
    bool skip = false;
    for(int r=0;r<row;r++) {
        for(int c=0;c<col;c++) {
            if(r==0 && c==0) {
                for(int i=0;i<n;i++) 
                    printf(" ");
                    printf("\t");
                continue;
            }
            if(c == 0) {
                if(m[r][c] == -1) {
                    skip = true;
                    break;
                }
                
                printPositionalCube(m[r][c],n);
                printf("\t");
                continue;
            }

            if(m[0][c] == -1)
                continue;
            
            if(r == 0) {
                printf("%d\t",fromPositionalCube(m[r][c]));
                continue;
            }
            printf("%c\t",m[r][c] == 1 ? 'X':' ');
        }
        printf(skip?"":"\n");
        skip = false;
    }
}

int cardinality(int n) {
    int card = 0;
    while(n != 0) {
        card += n & ONE;
        n >>= 2;
    }
    return card;
}

int dcCount(int n) {
    int dc = 0;
    for(int i=0;i<(sizeof(int)*2);i++) {
        dc += (((~n) & 0b11) == 0b11);
        n >>= 2;
    }
    return dc;
}

int criteria(int a, int b) {
    if(dcCount(a) != dcCount(b)) return dcCount(a) < dcCount(b);
    if(cardinality(a) == cardinality(b)) return fromPositionalCube(a) < fromPositionalCube(b);
    return cardinality(a) < cardinality(b);
}

int compatible(int a, int b) {
    while (a != 0 || b != 0) {
        int a1 = (a >> 1) & 0b1; // single bit
        int a0 = a & 0b1;
        int b1 = (b >> 1) & 0b1;
        int b0 = b & 0b1;
        if(((~b1) & (~b0) & (a1 | a0)) | ((~a1) & (~a0) & (b1 | b0)) ) return 0;
        a >>= 2;
        b >>= 2;
    }
    return 1;
}

void printPositionalCube(int n, int nOfVariables) {
    if(n < 0) return;
    int c; 
    char symbols[] = {'-','1','0',0};
    int shamt = nOfVariables*2-2;
    while(nOfVariables-->0) {
        putchar(symbols[(n & 0b11<<shamt) >> shamt&0b11]);
        n <<= 2;
    }
}

void printImplicantVariables(int n, int nOfVariables) {
    int var = 0;
    for(int i = nOfVariables-1;i >= 0; i--) {
        switch((n>>(2*i)) & 0b11) {
            case 1:
                printf("%c",variables[var]);
                break;
            case 2:
                printf("%c'",variables[var]);
                break;
        };
        var++;
    }
}

int toPositionalCube(int n, int nOfVariables) {
    int ps = 0;
    int i;
    for(i=0; i<nOfVariables; i++) {
        if(n>>i & 1) ps += (ONE << (2*i));
        else ps += (ZERO << (2*i));
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

int fromPositionalCube(int n) {
    int i = 0, res = 0;
    while(n != 0) {
        res += pot(2,i)*(n & 0b11 % 2);
        n >>= 2;
        i++;
    }
    return res;
}

int literals(int n) {
    int res = 0;
    while(n != 0) {
        res += !!(n & 0b11);
        n >>= 2;
    }
    return res;
}

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
        printPositionalCube(getData(l), n);
        printf("\t");
        printImplicantVariables(getData(l), n);
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
                if(!contains(*implicants, joinedValue)) 
                    current = insertNode(implicants, joinedValue, &criteria);
                if(isDontCare(cursor) && isDontCare(nextImplicant)) 
                    setDontCare(current, 1);
                setEssential(cursor, 0);
                setEssential(nextImplicant, 0);
            }
            nextImplicant = next(nextImplicant);
        }
        cursor = next(cursor);
    }
    removeFromList(implicants, &isEssential, 0);
    removeFromList(implicants, &isDontCare, 1);
    return;
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
    node_t* implicants = NULL; // list of prime implicants 
    node_t* constraint = NULL; // list of ON set minterms
    node_t* essentials = NULL; // list of essentials or partial reduntant implicants
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
    getMintermsFromFile(&constraint, nOfVariables, on_filename, 0);
    getMintermsFromFile(&implicants, nOfVariables, dc_filename, 1);
    executeQMC(&implicants, nOfVariables);
    essentials = petrick(constraint, implicants, nOfVariables);
    printf("Essential implicants:\n");
    printList(essentials,nOfVariables);
    printf("\nPoS form:\n");
    while(essentials != NULL) {
        printImplicantVariables(getData(essentials), nOfVariables);
        if((essentials = next(essentials)) != NULL) 
            printf(nOfVariables<8 ? " + " : " +\n");
    }
}