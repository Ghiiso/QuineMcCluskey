#include<stdlib.h>
#include<stdio.h>

typedef struct node {
    int data;
    int dontCare;
    short int essential;
    struct node* next;
    struct node* prev;
}node_t;



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

node_t* search(node_t** head, int data) {
    node_t* cursor = *head;
    while(cursor != NULL && cursor->data != data) {
        cursor = cursor->next;
    }
    return cursor;
}

short int contains(node_t* head, int data) {
    return search(&head,data) != NULL ? 1:0;
}

short int removeNode(node_t** head, node_t* node) {
    if(node == NULL) return 0;
    if(node->prev != NULL) node->prev->next = node->next;
    else *head = node->next;
    if(node->next != NULL) node->next->prev = node->prev;
    free(node); 
    return 1;
}

short int removeNodeFromData(node_t** head, int data) {
    node_t* cursor = search(head,data);
    return removeNode(head,cursor);
}

node_t* next(node_t* this) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return NULL;
    }
    return this->next;
}

node_t* prev(node_t* this) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return NULL;
    }
    return this->prev;
}

int getData(node_t* this) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return -1;
    }
    return this->data;
}

int isEssential(node_t* this) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return -1;
    }
    return this->essential;
}

int isDontCare(node_t* this) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return -1;
    }
    return this->dontCare;
}

short int setData(node_t* this, int value) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return 0;
    }
    this->data = value;
    return 1;
}

short int setEssential(node_t* this, int value) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return 0;
    }
    this->essential = value;
    return 1;
}

short int setDontCare(node_t* this, int value) {
    if (this == NULL) {
        perror("\nError: access to NULL pointer\n");
        return 0;
    }
    this->dontCare = value;
    return 1;
}

