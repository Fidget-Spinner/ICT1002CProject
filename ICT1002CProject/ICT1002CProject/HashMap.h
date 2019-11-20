/* This hash map implementation uses an array index to correspond to hashes
*  The array elements itself are a doubly linked list. Since that will help account for
*  hash collisions.
*/

#ifndef HASHMAP_H
#define HASHMAP_H



/** definitions/prototypes **/
typedef struct Data_Node DATA_NODE;



/** IMPORTANT FUNCTIONS TO USE HASHMAP **/

/** Inserts a key value pair into a hashmap.
* @param hashMap The hashMap to insert into.
* @param key The key to insert.
* @param value The corresponding value to insert.
* returns 1 if successful, 0 if fail
*/
int insertHashEntry(DATA_NODE* hashMap[], char* key, char* value, int override);

/** Returns a pointer to the DATA_NODE in the hashmap if it finds the key, returns null if the key does not exist. Just like a python dictionary/JS object.
* @param hashMap The hashMap to search through 
* @param key The key to search for
* @param buf The buffer to write the found value (or null) to
* returns the pointer to the DATA_NODE if found, null otherwise
*/
DATA_NODE * searchKeyGetValue(DATA_NODE * hashMap[], char * key, char buf[]);

// frees all nodes inside the hashMap: resets it to NULL array
// call this to prevent memory leaks
void freeHashMap(DATA_NODE * hashMap[]);

/** LESS IMPORTANT FUNCTIONS, NOT ALWAYS NECCESARY **/

/** Deletes a node from the HashMap and frees it.
* @param hashMap The hashMap to delete from.
* @param key The key of the node to delete.
* returns 1 if successful, 0 if failed
*/
int freeNode(DATA_NODE * hashMap[], char * key);

/** Helper function to create a node given a key-value pair.
* @param key The key of the node.
* @param value The corresponding value of the node.
* returns: The new DATA_NODE created.
*/
DATA_NODE * createNode(char * key, char * value);

/** Helper function to hash a string. Based on the djb2 hashing algorithm.
* @param strToHash The string to hash.
* returns the hash value of the string
*/
unsigned long hash(const char * strToHash);



#endif