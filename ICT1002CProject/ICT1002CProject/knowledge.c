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
#include<Windows.h>;


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
	/* to be implemented */
	int i = 0;
	int result;
	int swap = 0;
	int indexofknowledge = -1;;

	fptr = fopen(f, "r");
	if (fptr == NULL)
	{
		return result = 1;
	}

	while (fgets(LoadedKnowledge[i], 255, fptr) != NULL) //Loop through ini file and store content to global knowledge
	{
		if (LoadedKnowledge[i][0] == '[')
		{
			if (strcmp(strtok(LoadedKnowledge[i], "[]"), "what") == 0)
			{
				swap = 0;
				indexofknowledge = -1;
			}

			else if (strcmp(strtok(LoadedKnowledge[i], "[]"), "who") == 0)
			{
				swap = 1;
				indexofknowledge = -1;
			}

			else if (strcmp(strtok(LoadedKnowledge[i], "[]"), "where") == 0)
			{
				swap = 2;
				indexofknowledge = -1;
			}
		}
		else if (LoadedKnowledge[i][0] == '\n' || LoadedKnowledge[i] == "")
		{
			continue;
		}
		else if (strchr(LoadedKnowledge[i], '=') != NULL)
		{
			if (swap == 0) //what
			{
				LoadKnowledgeWhat[indexofknowledge].entity = strtok(LoadedKnowledge[i], "=");
				LoadKnowledgeWhat[indexofknowledge].responses = strtok(NULL, "=");
				//printf("%s\n", LoadKnowledgeWhat[indexofknowledge].entity); //Only for testing purposes
				//printf("%s\n", LoadKnowledgeWhat[indexofknowledge].responses); //Only for testing purposes

			}
			else if (swap == 1) //who
			{

				LoadKnowledgeWho[indexofknowledge].entity = strtok(LoadedKnowledge[i], "=");
				LoadKnowledgeWho[indexofknowledge].responses = strtok(NULL, "=");
				//printf("%s\n", LoadKnowledgeWho[indexofknowledge].entity); //Only for testing purposes
				//printf("%s\n", LoadKnowledgeWho[indexofknowledge].responses); //Only for testing purposes
			}
			else if (swap == 2) //where
			{
				LoadKnowledgeWhere[indexofknowledge].entity = strtok(LoadedKnowledge[i], "=");
				LoadKnowledgeWhere[indexofknowledge].responses = strtok(NULL, "=");
				//printf("%s\n", LoadKnowledgeWhere[indexofknowledge].entity); //Only for testing purposes
				//printf("%s\n", LoadKnowledgeWhere[indexofknowledge].responses); //Only for testing purposes
			}
		}
		i++;
		indexofknowledge++;
	}

	fclose(fptr);


	return result = 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	memset(LoadedKnowledge, '\0', sizeof(LoadedKnowledge));
	memset(LoadKnowledgeWhat, '\0', sizeof(LoadKnowledgeWhat));
	memset(LoadKnowledgeWhere, '\0', sizeof(LoadKnowledgeWhere));
	memset(LoadKnowledgeWho, '\0', sizeof(LoadKnowledgeWho));

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