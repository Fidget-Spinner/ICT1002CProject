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
char LoadedKnowledge[MAX_ENTITY + 1 + MAX_RESPONSE + 1];

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

	/* to be implemented */

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

	/* to be implemented */

	return KB_INVALID;

}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE* f) {
	FILE* fptr;
	int i = 0;
	int swap = 0;
	int indexofknowledge = -1;;

	char *key;
	char *value;
	char testBuf[MAX_ENTITY + 1 + MAX_RESPONSE + 1]; // only for testing purposes
	fptr = fopen(f, "r");
	if (fptr == NULL)
	{
		printf("Could not open the file\n");
		return 1;
	}
	while (fgets(LoadedKnowledge, MAX_ENTITY + 1 + MAX_RESPONSE + 1, fptr) != NULL) //Loop through ini file and store content to global knowledge
	{
		if (LoadedKnowledge[0] == '[')
		{
			if (compare_token(strtok(LoadedKnowledge, "[]"), "what") == 0)
			{
				swap = 0;
				indexofknowledge = -1;
			}
			else if (compare_token(strtok(LoadedKnowledge, "[]"), "who") == 0)
			{
				swap = 1;
				indexofknowledge = -1;
			}
			else if (compare_token(strtok(LoadedKnowledge, "[]"), "where") == 0)
			{
				swap = 2;
				indexofknowledge = -1;
			}
		}
		else if (LoadedKnowledge[0] == '\n' || LoadedKnowledge == "")
		{
			continue;
		}
		else if (strchr(LoadedKnowledge, '=') != NULL)
		{
			switch (swap) {
				case 0: // intent is "what"
					key = strtok(LoadedKnowledge, "=");
					value = strtok(NULL, "=");
					insertHashEntry(LoadKnowledgeWhatMap, str_upper(key), value); // what hashmap
					searchKeyGetValue(LoadKnowledgeWhatMap, key, testBuf); //Only for testing purposes
					printf("%s\n", testBuf); //Only for testing purposes
					break;
				case 1: // intent is "who"
					key = strtok(LoadedKnowledge, "=");
					value = strtok(NULL, "=");
					insertHashEntry(LoadKnowledgeWhoMap, str_upper(key), value); // who hashmap
					searchKeyGetValue(LoadKnowledgeWhoMap, key, testBuf); //Only for testing purposes
					printf("%s\n", testBuf); //Only for testing purposes
					break;
				case 2: // intent is "where"
					key = strtok(LoadedKnowledge, "=");
					value = strtok(NULL, "=");
					insertHashEntry(LoadKnowledgeWhereMap, str_upper(key), value); // where hashmap
					searchKeyGetValue(LoadKnowledgeWhereMap, key, testBuf); //Only for testing purposes
					printf("%s\n", testBuf); //Only for testing purposes
					break;			
			}
		}
		indexofknowledge++;
	}
	fclose(fptr);
	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	memset(LoadedKnowledge, '\0', sizeof(LoadedKnowledge));
	freeHashMap(LoadKnowledgeWhatMap);
	freeHashMap(LoadKnowledgeWhereMap);
	freeHashMap(LoadKnowledgeWhoMap);

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE* f) {

	/* to be implemented */

}