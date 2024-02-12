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

node_t* next(node_t* this);
node_t* prev(node_t* this);
int getData(node_t* this);
int isEssential(node_t* this);
int isDontCare(node_t* this);
bool setData(node_t* this, int value);
bool setEssential(node_t* this, int value);
bool setDontCare(node_t* this, int value);
int length(node_t* this);
int append(int,int);
#endif

