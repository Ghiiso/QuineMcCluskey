/**
 * PETRICK METHOD IMPLEMENTATION
 * Implements the Petrick method 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "main.h"
#include "petrick.h"

/**
 * Removes every row that doesn't cover any constraint and
 * returns the number of removed lines
*/
int removeEmptyImplicants(chart_t** implicantsChart) {
    int rowsRemoved = 0;
    int cols = (*implicantsChart)->columns;
    int rows = (*implicantsChart)->rows;
    int uncoveredCostraint = 0;

    for(int row = 1; row < rows; row++) {
        for(int col = 1; col <cols;col++) {
            if((*implicantsChart)->matrix[row][col]) {
                uncoveredCostraint++;
                if((*implicantsChart)->matrix[0][col] == -1)
                    uncoveredCostraint--;
            }
        }
        if(uncoveredCostraint == 0) {
            (*implicantsChart)->matrix[row][0] = -1;
            rowsRemoved++;
        }
    }
    return rowsRemoved;
}

/**
 * Finds all essential implicants, adds them to the essential list and 
 * returns the number of covered constraints
*/
int findEssentials(chart_t** implicantsChart, node_t** essentials) {
    int constrainstCovered = 0;
    bool count;
    int cols = (*implicantsChart)->columns;
    int rows = (*implicantsChart)->rows;
    int implicantRow = 0;

    for(int col = 1; col < cols; col++) {
        if((*implicantsChart)->matrix[0][col] == -1) 
            continue;
        count = false;
        implicantRow = 0;
        for(int row = 1; row < rows; row++) {
            if((*implicantsChart)->matrix[row][0] == -1) 
                continue;
            if((*implicantsChart)->matrix[row][col]) {
                implicantRow = row;
                count = !count;
                if(!count) break;
            }
        }
        if(count) { // is essential
            insertNode(essentials, (*implicantsChart)->matrix[implicantRow][0],append);
            (*implicantsChart)->matrix[implicantRow][0] = -1;
            for(int col2=1;col2<cols;col2++) {
                if((*implicantsChart)->matrix[implicantRow][col2]) {
                    constrainstCovered++;
                    (*implicantsChart)->matrix[0][col2] = -1;
                }
            }
            
        }
    }
    return constrainstCovered;
}

/**
 * Removes every dominated row and returns how many implicants it removed
*/
int rowDominance(chart_t** implicantsChart, node_t** essentials) {
    int rowsRemoved = 0;
    bool count;
    int cols = (*implicantsChart)->columns;
    int rows = (*implicantsChart)->rows;

    for(int row = 1; row < rows - 1; row++) {
        if((*implicantsChart)->matrix[row][0] == -1) 
            continue;
        for(int row2 = row + 1; row2 < rows; row2++) {
            int relativeCardinality[] = {0,0};
            if((*implicantsChart)->matrix[row2][0] == -1) 
                continue;
            for(int col = 1;col<cols;col++) {
                if((*implicantsChart)->matrix[0][col] == -1) 
                    continue;
                int rowContent1 = (*implicantsChart)->matrix[row][col];
                int rowContent2 = (*implicantsChart)->matrix[row2][col];
                if(rowContent1 ^ rowContent2)
                    relativeCardinality[rowContent1 < rowContent2]++;
            }

            switch (!!relativeCardinality[0] + !!relativeCardinality[1]) {
            case 0: // equal rows: depends on n. of literals
                if(literals((*implicantsChart)->matrix[row][0]) <= literals((*implicantsChart)->matrix[row2][0]))
                    (*implicantsChart)->matrix[row2][0] = -1;
                else
                    (*implicantsChart)->matrix[row][0] = -1;
                rowsRemoved++;
                break;
            case 1: // dominance
                if(!!relativeCardinality[0])
                    (*implicantsChart)->matrix[row2][0] = -1;
                else
                    (*implicantsChart)->matrix[row][0] = -1;
                rowsRemoved++;
                break;
            default:
                break;
            }
        }
    }
    return rowsRemoved;
}

/**
 * Removes every dominated column and returns how many constraint it removed
*/
int colDominance(chart_t** implicantsChart, node_t** essentials) {
    int colsRemoved = 0;
    bool count;
    int cols = (*implicantsChart)->columns;
    int rows = (*implicantsChart)->rows;

    for(int col = 1; col < cols - 1; col++) {
        if((*implicantsChart)->matrix[0][col] == -1) 
            continue;
        for(int col2 = col + 1; col2 < cols; col2++) {
            int relativeCardinality[] = {0,0};
            if((*implicantsChart)->matrix[0][col2] == -1) 
                continue;
            for(int row = 1;row<rows;row++) {
                if((*implicantsChart)->matrix[row][0] == -1) 
                    continue;
                int colContent1 = (*implicantsChart)->matrix[row][col];
                int colContent2 = (*implicantsChart)->matrix[row][col2];
                if(colContent1 ^ colContent2)
                    relativeCardinality[colContent1 < colContent2]++;
            }

            switch (!!relativeCardinality[0] + !!relativeCardinality[1]) {
            case 0: // equal rows: depends on n. of literals
                (*implicantsChart)->matrix[0][col2] = -1;
                colsRemoved++;
                break;
            case 1: // dominance
                if(!!relativeCardinality[0])
                    (*implicantsChart)->matrix[0][col] = -1;
                else
                    (*implicantsChart)->matrix[0][col2] = -1;
                    colsRemoved++;
                break;
            default:
                break;
            }
        }
    }
    return colsRemoved;
}

/**
 * Returns true if implicant covers the constraint e.g. every non
 * dc digit is equal, false otherwise.
 * Requires implicant and constraint in positional cube representation
*/
bool covers(int implicant, int constraint, int nOfVariables) {
    for(int i = 0; i < nOfVariables; i++) {
        if(!!(implicant & 0b11) && !!(constraint & 0b11) && (implicant & 0b11) != (constraint & 0b11)) return false; 
        implicant >>=2;
        constraint >>=2;
    }
    return true;
}

/**
 * Returns a int matrix representing the prime implicant chart.
 * Every row holds an implicant and every column a constraint: every
 * cell tells if the row implicant covers the column constraint
 * int** holds rows, int* holds columns
*/
chart_t* createChart(node_t* constraints, node_t* implicants, int nOfVariables) {
    int rows = length(implicants)+1;
    int cols = length(constraints)+1;
    int col = 0;
    node_t* cursor = constraints;
    chart_t* implicantsChart = malloc(sizeof(chart_t));

    implicantsChart->rows = rows;
    implicantsChart->columns = cols;
    implicantsChart->matrix = (int**) malloc((size_t) (sizeof(int)*rows));

    for(int idx = 0; idx < rows; idx++) {
        implicantsChart->matrix[idx] = (int*) malloc((size_t) (sizeof(int)*cols));
        if(idx == 0) {
            implicantsChart->matrix[idx][0] = 0;
            col = 1;
            cursor = constraints;
            while(cursor != NULL) {
                implicantsChart->matrix[idx][col++] = getData(cursor);
                cursor = next(cursor);
            }
        } else {
            implicantsChart->matrix[idx][0] = getData(implicants);
            col = 0;
            while(++col < cols) {
                implicantsChart->matrix[idx][col] = (int) covers(implicantsChart->matrix[idx][0], implicantsChart->matrix[0][col],nOfVariables);
            }
            implicants = next(implicants);
        }
    }
    return implicantsChart;
}

node_t* petrick(node_t* constraints, node_t* implicants, int nOfVariables) {
    int totalConstraints = length(constraints);
    int totalImplicants = length(implicants);
    bool noSimplification = false;
    node_t *essentials = NULL;
    chart_t* implicantsChart = createChart(constraints, implicants, nOfVariables);
    while(totalConstraints > 0 && totalImplicants > 0 && !noSimplification) {
        int ret = 0;
        if(!!(ret = findEssentials(&implicantsChart,&essentials))) {
            totalConstraints -= ret;
            totalImplicants -= removeEmptyImplicants(&implicantsChart);
            continue;
        }
        if(!!(ret = rowDominance(&implicantsChart, &essentials))) {
            totalImplicants -= ret;
            continue;
        }
        if(!!(ret = colDominance(&implicantsChart,&essentials))) {
            totalConstraints -= ret;
            totalImplicants -= removeEmptyImplicants(&implicantsChart);
            continue;
        }
        noSimplification = true; // no simplification occurred
    }    
    if(noSimplification) {
        printf("Cyclic table:\n"); 
        printMatrix(implicantsChart->matrix,implicantsChart->rows,implicantsChart->columns, nOfVariables);
    }
    return essentials;
}
