#ifndef _LINKED_LIST
#define _LINKED_LIST

typedef struct node {
    int data;
    int dontCare;
    short int essential;
    struct node* next;
    struct node* prev;
}node_t;

/**
 * Insert a new element inside head list with value data according to criteria function.
 * Returns the new element if it has been inserted successfully, NULL otherwise.
 * @param head reference to the head of a linked list
 * @param data value of new element
 * @param criteria pointer to a function to use as criteria to order list
*/
node_t* insertNode(node_t** head, int data, int (*criteria)(int, int));

/**
 * Removes passed node from the list it's inserted
 * @param node node to remove
*/
bool removeNode(node_t** head, node_t* node);

/**
 * Removes the first element with value data from passed list. 
 * Returns true if element has been removed successfully, false otherwise.
 * @param head head of the list to remove item from
 * @param data value of element to remove
*/
bool removeNodeFromData(node_t** head, int data);

/**
 * Returns true if list contains at least an element with value that equals data, false otherwise.
 * @param head head of the list to search
 * @param data value to search inside the list
*/
bool contains(node_t* head, int data);

/**
 * Returns the node following passed node
*/
node_t* next(node_t* this);

/**
 * Returns the node preceeding passed node
*/
node_t* prev(node_t* this);

/**
 * Returns data contained inside passed node
*/
int getData(node_t* this);

/**
 * Returns 1 if passed node is essential, 
 * 0 if it isn't
 * -1 if is NULL
*/
int isEssential(node_t* this);

/**
 * Returns 1 if passed node is part of the DC set, 
 * 0 if it isn't
 * -1 if is NULL
*/
int isDontCare(node_t* this);

/**
 * Sets current node's data to passed value
*/
bool setData(node_t* this, int value);

/**
 * Sets current node's isEssential field to passed value
*/
bool setEssential(node_t* this, int value);

/**
 * Sets current node's data to passed value
*/
bool setDontCare(node_t* this, int value);

/**
 * Returns length of passed list
*/
int length(node_t* this);

/**
 * function that returns true. Used as criteria to append
 * last element at the end of the list 
*/
int append(int a, int b);
#endif

