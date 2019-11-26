/* This hash map implementation uses an array index to correspond to hashes
*  The array elements itself are a doubly linked list. Since that will help account for
*  hash collisions.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"
#include "chat1002.h"



/** Returns a value (str pointer) in the hashmap given a key to search, returns null if the key does not exist.
* @param hashMap The hashMap to search through 
* @param key The key to search for
* @param buf The buffer to write the found value (or empty string) to
* returns the pointer to the DATA_NODE if found, null otherwise
*/
DATA_NODE * searchKeyGetValue(DATA_NODE * hashMap[], const char * key, char buf[MAX_LENGTH_USER_INPUT + 2]){
  unsigned long index = hash(key);
  char tempBuf[MAX_LENGTH_USER_INPUT];
  strcpy(tempBuf, key); // use tempBuf to store the key since key is const
  DATA_NODE *tableEntry = hashMap[index];
  while (tableEntry != NULL){
    // if the keys dont match, just go down the linked list and search
    if (strcmp(tableEntry->key, str_upper(tempBuf)) != 0){
      tableEntry = tableEntry->next;
    } else {
      // once found, return the value
      //printf("[DEBUG]HashMap: Found Value: '%s', Key:'%s'\n", tableEntry->value, key);
      strncpy(buf, tableEntry->value, MAX_LENGTH_USER_INPUT);
      return tableEntry;
    }
  }
  //if after exhausting the list and nothing found, return null
  //printf("[DEBUG]HashMap: Key does not exist in HashMap\n");
  //strncpy(buf, "", 2);
  return NULL;
}

/** Inserts a key value pair into a hashmap.
* @param hashMap The hashMap to insert into.
* @param key The key to insert.
* @param value The corresponding value to insert.
* @param override Whether to override the current key-value pair 1 is true, 0 is false.
* returns 1 if successful, 0 if failed
*/
int insertHashEntry(DATA_NODE * hashMap[], const char * key, const char * value, int override){
  unsigned long index = hash(key);
  char tempBuf[MAX_LENGTH_USER_INPUT];
  strcpy(tempBuf, key); // use tempBuf to store the key since key is const
  char tempValueBuf[MAX_LENGTH_USER_INPUT];
  strcpy(tempValueBuf, value); // use tempBuf to store the value since value is const
  DATA_NODE *tableEntry = hashMap[index];
  DATA_NODE *newNode = createNode(str_upper(tempBuf), tempValueBuf);
  //printf("[DEBUG]HashMap: %s %s\n", key, value);
  // check if out of memory
  if (newNode == NULL){
    return 0;
  }
  // if the index is empty
  if (tableEntry == NULL){
    hashMap[index] = newNode;
  } else {
    //printf("[DEBUG]HashMap: Hash Collision (this is usually normal)\n");
    // if something is there, then a collision might have occurred
    // checks if same key, if it is, then reject it since hashmaps cant have duplicate keys
		// if override=1, then override that value
    if (strcmp(tableEntry->key, str_upper(tempBuf)) == 0 ){
			if (!override) {
				//printf("[DEBUG]HashMap: No duplicate keys allowed\n");
				//dont need the newNode anymore
				free(newNode);
				return 0;
			}
			else if (override) {
				strcpy(tableEntry->value, tempValueBuf);
				//printf("[DEBUG]HashMap: Overrode\n");
				//dont need the newNode anymore
				free(newNode);
				return 1;
			}
			// if not same key, then a hash collision actually occured, so extend the singly linked list
    } else {
      // if tableEntry->next == NULL, that means that is the last element
      while (tableEntry->next != NULL)
        tableEntry = tableEntry->next; // increment to last elmeent
      // append to last element
      tableEntry->next = newNode;
    } 
  }
  return 1;
}


// frees all nodes inside the hashMap: resets it to NULL array
void freeHashMap(DATA_NODE * hashMap[]){
  DATA_NODE * currentNode;
  DATA_NODE * tmp;
  for (int i=0; i<SIZE_OF_HASHMAP; i++){
    currentNode = hashMap[i];
    if (currentNode){
      // loops through and frees the whole linkedlist
      while (currentNode != NULL){
        tmp = currentNode;
        currentNode = currentNode->next;
        free(tmp);
      }
	  // sets that entry in the hash table to null
	  hashMap[i] = NULL;
    }
  }
}

/** Helper function to create a node given a key-value pair.
* @param key The key of the node.
* @param value The corresponding value of the node.
* returns: The new DATA_NODE created.
*/
DATA_NODE * createNode(const char * key, const char * value){
  DATA_NODE * newNodePtr = (DATA_NODE *)malloc(sizeof(DATA_NODE));
  if (newNodePtr){
    strncpy(newNodePtr->value, value, MAX_LENGTH_USER_INPUT);
    strncpy(newNodePtr->key, key, MAX_LENGTH_USER_INPUT);
    newNodePtr->next = NULL;
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

