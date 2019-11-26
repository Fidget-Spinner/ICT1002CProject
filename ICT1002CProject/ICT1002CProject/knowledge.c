/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
#include "HashMap.h"

// definition of global variables
DATA_NODE* LoadKnowledgeWhatMap[SIZE_OF_HASHMAP] = { NULL };
DATA_NODE* LoadKnowledgeWhoMap[SIZE_OF_HASHMAP] = { NULL };
DATA_NODE* LoadKnowledgeWhereMap[SIZE_OF_HASHMAP] = { NULL };



/*
  * Get the response to a question.
  *
  * Input:
  *   intent   - the question word
  *   entity   - the entity
  *   response - a buffer to receive the response
  *   n        - the maximum number of characters to write to the response buffer
  *
  * Returns:
  *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
  *   KB_NOTFOUND, if no response could be found
  *   KB_INVALID, if 'intent' is not a recognised question word
  */
int knowledge_get(const char* intent, const char* entity, char* response, int n) {

	DATA_NODE ** hashMapToSearch = NULL;
	if (compare_token(intent, "who") == 0)
		hashMapToSearch = LoadKnowledgeWhoMap;
	else if (compare_token(intent, "what") == 0)
		hashMapToSearch = LoadKnowledgeWhatMap;
	else if (compare_token(intent, "where") == 0)
		hashMapToSearch = LoadKnowledgeWhereMap;
	if (hashMapToSearch == NULL) //Check for intent
		return KB_INVALID;
	if (entity == NULL)
	{
		return KB_INVALID;
	}
	if (searchKeyGetValue(hashMapToSearch, entity, response)) {
		return KB_OK;
	}
	return KB_NOTFOUND;

}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char* intent, const char* entity, const char* response) {
	DATA_NODE** hashMapToSearch = NULL;
	if (compare_token(intent, "who") == 0)
		hashMapToSearch = LoadKnowledgeWhoMap;
	else if (compare_token(intent, "what") == 0)
		hashMapToSearch = LoadKnowledgeWhatMap;
	else if (compare_token(intent, "where") == 0)
		hashMapToSearch = LoadKnowledgeWhereMap;
	if (hashMapToSearch == NULL) //Check for intent
		return KB_INVALID;

	if (insertHashEntry(hashMapToSearch, entity, response, 1))
		return KB_OK;
	else
		return KB_NOMEM;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entries read, 0 if failed
 */
int knowledge_read(FILE* f) {
	// just a temporary buffer to store a single line of file input
	char LoadedKnowledge[MAX_ENTITY + 1 + MAX_RESPONSE + 1];
	int swap = 0;
	int indexofknowledge = -1;
	char* entityType;

	int numOfEntries = 0; //counts the number of responses/entries inside the file, this is just for informing the user.

	char *key;
	char *value;
	if (f == NULL)
	{
		printf("Could not open the file\n");
		return 0;
	}
	while (fgets(LoadedKnowledge, MAX_ENTITY + 1 + MAX_RESPONSE + 1, f) != NULL) //Loop through ini file and store content to global knowledge
	{
		if (LoadedKnowledge[0] == '[')
		{
			entityType = strtok(LoadedKnowledge, "[]");
			// set swap if it finds one of these
			swap = compare_token(entityType, "what") == 0 ? 0 : swap;
			swap = compare_token(entityType, "who") == 0 ? 1 : swap;
			swap = compare_token(entityType, "where") == 0 ? 2 : swap;
			indexofknowledge = -1; //reset the index to read knowledge
		}
		else if (LoadedKnowledge[0] == '\n' || LoadedKnowledge == "")
		{
			continue; //skip new lines or empty string
		}
		else if (strchr(LoadedKnowledge, '=') != NULL)
		{
			key = strtok(LoadedKnowledge, "=");
			value = strtok(NULL, "=");
			removeTrailingNewLine(key);
			removeTrailingNewLine(value);
			switch (swap) {
				case 0: // intent is "what"
					insertHashEntry(LoadKnowledgeWhatMap, str_upper(key), value, 1); // what hashmap
					break;
				case 1: // intent is "who"
					insertHashEntry(LoadKnowledgeWhoMap, str_upper(key), value, 1); // who hashmap
					break;
				case 2: // intent is "where"
					insertHashEntry(LoadKnowledgeWhereMap, str_upper(key), value, 1); // where hashmap
					break;			
			}
			numOfEntries++; //add 1 entry to number of entries
		}
		indexofknowledge++;
	}
	return numOfEntries;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	freeHashMap(LoadKnowledgeWhatMap);
	freeHashMap(LoadKnowledgeWhereMap);
	freeHashMap(LoadKnowledgeWhoMap);
}

/* Internal helper function to knowledge_write
 * Write the knowledge base to a file.
 *
 * Input:
 *	 hashMap - the hashmap to write to the file
 *   f - the file
 */
void knowledge_write_helper(DATA_NODE* hashMap[], FILE* f) {
	DATA_NODE* currentNodePtr = NULL;
	for (int i = 0; i < SIZE_OF_HASHMAP; i++) {
		if (currentNodePtr = hashMap[i]) {  //if currentNodePtr is not pointing to an empty node
			// write the whole linked list out
			while (currentNodePtr != NULL) {
				fputs(currentNodePtr->key, f);
				fputc('=', f);
				fputs(currentNodePtr->value, f);
				fputc('\n', f);
				currentNodePtr = currentNodePtr->next;
			}
		}
	}
}

/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE* f) {
	fputs("[what]\n", f);
	knowledge_write_helper(LoadKnowledgeWhatMap, f);
	fputs("[where]\n", f);
	knowledge_write_helper(LoadKnowledgeWhereMap, f);
	fputs("[who]\n", f);
	knowledge_write_helper(LoadKnowledgeWhoMap, f);
}

