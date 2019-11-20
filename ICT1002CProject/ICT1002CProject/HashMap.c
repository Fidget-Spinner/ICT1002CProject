/* This hash map implementation uses an array index to correspond to hashes
*  The array elements itself are a doubly linked list. Since that will help account for
*  hash collisions.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"
#include "chat1002.h"


// doubly linked list struct, used for the hash table elements
struct Data_Node{
  char key[MAX_LENGTH_USER_INPUT + 2];
  char value[MAX_LENGTH_USER_INPUT + 2]; 
  struct Data_Node *next;
  struct Data_Node *prev;
} ;

/** Returns a value (str pointer) in the hashmap given a key to search, returns null if the key does not exist.
* @param hashMap The hashMap to search through 
* @param key The key to search for
* @param buf The buffer to write the found value (or empty string) to
* returns the pointer to the DATA_NODE if found, null otherwise
*/
DATA_NODE * searchKeyGetValue(DATA_NODE * hashMap[], char * key, char buf[MAX_LENGTH_USER_INPUT + 2]){
  unsigned long index = hash(key);
  DATA_NODE *tableEntry = hashMap[index];
  while (tableEntry != NULL){
    // if the keys dont match, just go down the linked list and search
    if (strcmp(tableEntry->key, key) != 0){
      tableEntry = tableEntry->next;
    } else {
      // once found, return the value
      printf("[DEBUG]HashMap: Found Value: '%s', Key:'%s'\n", tableEntry->value, key);
      strncpy(buf, tableEntry->value, MAX_LENGTH_USER_INPUT);
      return tableEntry;
    }
  }
  //if after exhausting the list and nothing found, return null
  printf("[DEBUG]HashMap: Key does not exist in HashMap\n");
  strncpy(buf, "", 2);
  return NULL;
}

/** Inserts a key value pair into a hashmap.
* @param hashMap The hashMap to insert into.
* @param key The key to insert.
* @param value The corresponding value to insert.
* returns 1 if successful, 0 if failed
*/
int insertHashEntry(DATA_NODE * hashMap[], char * key, char * value){
  unsigned long index = hash(key);
  DATA_NODE *tableEntry = hashMap[index];
  DATA_NODE *newNode = createNode(key, value);
  // check if out of memory
  if (newNode == NULL){
    return 0;
  }
  // if the index is empty
  if (tableEntry == NULL){
    hashMap[index] = newNode;
  } else {
    printf("[DEBUG]HashMap: Hash Collision (this is usually normal)\n");
    // if something is there, then a collision might have occurred
    // checks if same key, if it is, then reject it since hashmaps cant have duplicate keys
    if (strcmp(tableEntry->key, key) == 0 ){
      printf("[DEBUG]HashMap: No duplicate keys allowed\n");
      return 0;
      // if an actual hash collision has occured
    } else {
      // if tableEntry->next == NULL, that means that is the last element
      while (tableEntry->next != NULL)
        tableEntry = tableEntry->next; // increment to last elmeent
      // append to last element
      tableEntry->next = newNode;
      newNode->prev = tableEntry;
    } 
  }
  return 1;
}

/** Deletes a node from the HashMap and frees it.
* @param hashMap The hashMap to delete from.
* @param key The key of the node to delete.
* returns 1 if successful, 0 if failed
*/
int freeNode(DATA_NODE * hashMap[], char * key){
  char uselessBuf[MAX_LENGTH_USER_INPUT + 2];
  DATA_NODE * foundNode = searchKeyGetValue(hashMap, key, uselessBuf);
  DATA_NODE * prevNode;
  if (foundNode){
    if (foundNode->prev != NULL){  // if the node found is not the first node
      prevNode = foundNode->prev;
      prevNode->next = foundNode->next;
      if (foundNode->next) // if there is another node after foundNode
        foundNode->next->prev = prevNode;
    } else { // if the node found is the first node
      hashMap[hash(key)] = foundNode->next;
    }
    free(foundNode);
    return 1;
  } else {
    printf("[DEBUG]:HashMap The key-value pair was not deleted because it does not exist.\n");
    return 0;
  }
}

// frees all nodes inside the hashMap: resets it to NULL array
void freeHashMap(DATA_NODE * hashMap[]){
  DATA_NODE * currentNode;
  DATA_NODE * tmp;
  for (int i=0; i<SIZE_OF_HASHMAP; i++){
    currentNode = hashMap[i];
    if (currentNode){
      // sets that entry in the hash table to null
      hashMap[i] = NULL;
      // loops through and frees the whole linkedlist
      while (currentNode != NULL){
        tmp = currentNode;
        currentNode = currentNode->next;
        free(tmp);
      }
    }
  }
}

/** Helper function to create a node given a key-value pair.
* @param key The key of the node.
* @param value The corresponding value of the node.
* returns: The new DATA_NODE created.
*/
DATA_NODE * createNode(char * key, char * value){
  DATA_NODE * newNodePtr = (DATA_NODE *)malloc(sizeof(DATA_NODE));
  if (newNodePtr){
    strncpy(newNodePtr->value, value, MAX_LENGTH_USER_INPUT);
    strncpy(newNodePtr->key, key, MAX_LENGTH_USER_INPUT);
    newNodePtr->next = NULL;
    newNodePtr->prev = NULL;
  } else {
    printf("Out of memory, cannot create new key-value pairs");
  }
  return newNodePtr;
}

/** Helper function to hash a string. Based on the djb2 hashing algorithm.
* @param strToHash The string to hash.
* returns the hash value of the string
*/
unsigned long hash(const char * strToHash){
  unsigned long hash = 5381;
  int c;
  while ((c = *strToHash++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash % SIZE_OF_HASHMAP; //make the hash be an index of the hash table
}

