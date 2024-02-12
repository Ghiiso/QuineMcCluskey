/**
 * LINKED LIST IMPLEMENTATION
 * Implements all necessary methods to handle a linked list
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "linkedlist.h"

node_t* createNode(int data) {
    node_t* newNode = (node_t*) malloc(sizeof(node_t));
    if(newNode == NULL) return newNode;
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->essential = 1;
    newNode->dontCare = 0;
    return newNode;
}

void error(char* str, int errorMsg) {
    errno = errorMsg;
    perror(str);
}

node_t* insertNode(node_t** head, int data, int (*criteria)(int,int)) {
    node_t* newNode = createNode(data);
    if(newNode == NULL) return NULL;
    node_t* cursor = *head;
    node_t* prevCursor = NULL;
    while(cursor != NULL && (*criteria)(cursor->data,data)) {
        prevCursor = cursor;
        cursor = cursor->next;
    }
    newNode->next = cursor;
    newNode->prev = prevCursor;
    if(cursor != NULL) cursor->prev = newNode;
    if(prevCursor != NULL) prevCursor->next = newNode;
    else *head = newNode;
    return newNode;
}

node_t* search(node_t* cursor, int data) {
    while(cursor != NULL && cursor->data != data) {
        cursor = cursor->next;
    }
    return cursor;
}

bool contains(node_t* head, int data) {
    return (bool) (search(head, data) != NULL);
}

bool removeNode(node_t** head, node_t* node) {
    if(node == NULL) return false;
    if(node->prev != NULL) node->prev->next = node->next;
    else *head = node->next;
    if(node->next != NULL) node->next->prev = node->prev;
    free(node); 
    return true;
}

bool removeNodeFromData(node_t** head, int data) {
    node_t* cursor = search(*head,data);
    return removeNode(head,cursor);
}

node_t* next(node_t* this) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function next",EFAULT);
        return NULL;
    }
    return this->next;
}

node_t* prev(node_t* this) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function prev",EFAULT);
        return NULL;
    }
    return this->prev;
}

int getData(node_t* this) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function getData",EFAULT);
        return -1;
    }
    return this->data;
}

int isEssential(node_t* this) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function isEssential",EFAULT);
        return -1;
    }
    return this->essential;
}

int isDontCare(node_t* this) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function isDontCare",EFAULT);
        return -1;
    }
    return this->dontCare;
}

bool setData(node_t* this, int value) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function setData",EFAULT);
        return false;
    }
    this->data = value;
    return true;
}

bool setEssential(node_t* this, int value) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function setEssential",EFAULT);
        return false;
    }
    this->essential = value;
    return true;
}

bool setDontCare(node_t* this, int value) {
    if (this == NULL) {
        error("Error: access to NULL pointer in function setDontCare",EFAULT);
        return false;
    }
    this->dontCare = value;
    return true;
}

int length(node_t* this) {
    int c = 0;
    while(this != NULL) {
        this = this->next;
        c++;
    }
    return c;
}

void removeList(node_t** this) {
    node_t* temp;
    while(*this != NULL) {
        temp = *this;
        *this = (*this)->next;
        free(temp);
    }
}

int append(int a, int b) {
    return true;
}

