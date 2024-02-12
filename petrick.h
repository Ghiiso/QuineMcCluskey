#ifndef _PETRICK
#define _PETRICK
typedef struct chart {
    int** matrix;
    int rows;
    int columns;
} chart_t;

node_t* petrick(node_t* constraints, node_t* implicants, int nOfVariables);
#endif