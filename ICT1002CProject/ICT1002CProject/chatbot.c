/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chat1002.h"
#include "HashMap.h"


 /*
  * Helper function for answering questions. Required by chatbot_do_question.
  *
  * inv[0] contains the the question word.
  * inv[1] may contain "is" or "are"; if so, it is skipped.
  * The remainder of the words form the entity.
  *
  * This function retrieves a value from the hashmap corresponding to the intent.
  */
void chatbot_do_question_helper(int inc, char* inv[], char* response, int n, char questionEntityPtr[]);

 /*
  * Get the name of the chatbot.
  *
  * Returns: the name of the chatbot as a null-terminated string
  */
const char* chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char* chatbot_username() {

	return "User";

}

/* Just the hard coded base small talk responses. 
*/
typedef struct {
	char* intent;
	char* responses[3]; //To change amount of strings responses have
}record;

record KnowledgeBase[] = {
	{"hello",
	{"Greetings!", "Hey there!", "It's good to see you!"}
	},

	{"how",
	{"I'm doing great!", "T_T Not so good ...", "^_^ Great!"}
	},

	{"morning",
	{"It's a beautiful morning!", "Rise and Shine!!", "Good morning!"}
	},

	{"afternoon",
	{"Good afternoon!", "Lunchtime!!", "Time for lunch!!"}
	},

	{"nights",
	{"Goodnight!", "I'm getting sleepy ...", "Sweet dreams!"}
	}
};



/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char* inv[], char* response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char* intent) {
	int result = compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
	return result;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char* inv[], char* response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char* intent) {

	return compare_token(intent, "load") == 0;

}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char* inv[], char* response, int n) {
	FILE* f;
	if (inv[1] == "" || inv[1] == NULL)
	{
		snprintf(response, n, "Error missing filename!");
	}
	else {
		f = fopen(inv[1], "r");
		if (knowledge_read(f) == 0) //Test for error reading
		{
			snprintf(response, n, "Successfully loaded!");
		}
		else if (knowledge_read(f) == 1)
		{
			snprintf(response, n, "Error opening file!");
		}
	}

	return 0;

}

/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char* intent) {
	return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;

}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char* inv[], char* response, int n) {

	char questionEntityPtr[255] = ""; //Store entity of user input

	chatbot_do_question_helper(inc, inv, response, n, questionEntityPtr);
	return 0;

}

void chatbot_do_question_helper(int inc, char* inv[], char* response, int n, char questionEntityPtr[]) {
	DATA_NODE* hashMapToSearch = NULL;
	char responseBuf[MAX_LENGTH_USER_INPUT];
	int KB_STATUS;
		if (inv[1] == NULL)
		{
			snprintf(response, n, "Please input a proper question!"); //If question is improper, break
			return;
		}
		if (compare_token(inv[1], "are") == 0 || compare_token(inv[1], "is") == 0) //check for is and are
		{

			for (int b = 2; b < inc; b++)
			{
				if (inv[b + 1] != NULL)
				{
					strcat(questionEntityPtr, inv[b]); //store input entity 
					strcat(questionEntityPtr, " "); //check for space and insert space
				}
				else
				{
					strcat(questionEntityPtr, inv[b]); //store input entity 
				}
			}
			KB_STATUS = knowledge_get(inv[0], questionEntityPtr, responseBuf, n);
			switch (KB_STATUS) {
				case KB_OK:
					snprintf(response, n, responseBuf);
					break;
				case KB_NOTFOUND:
					snprintf(response, n, "Question not found! Please tell me a response!"); // Question Not Found, Run write Function here
					break;
				case KB_INVALID:
					printf("uwu");
					snprintf(response, n, "Please input a proper question!"); //If question is inproper, break
					break;
			}
		}
		else {
			snprintf(response, n, "Please input a proper question!"); //If question is inproper, break
		}
	}

/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char* intent) {

	return compare_token(intent, "reset") == 0;

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char* inv[], char* response, int n) {

	knowledge_reset();
	snprintf(response, n, "Chatbot reset!");

	return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char* intent) {

	/* to be implemented */

	return 0;

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char* inv[], char* response, int n) {

	/* to be implemented */

	return 0;

}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char* intent) {

	return compare_token(intent, "hello") == 0 || compare_token(intent, "morning") == 0 || compare_token(intent, "afternoon") == 0 || compare_token(intent, "nights") == 0 || compare_token(intent, "how") == 0;;

}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char* inv[], char* response, int n) {
	char* smalltalkoutput;
	int r = rand() % 3;
	for (int i = 0; i < sizeof(KnowledgeBase) / sizeof(KnowledgeBase[0]); ++i) {
		if (compare_token(KnowledgeBase[i].intent, inv[0]) == 0) {
			int r = rand() % 3; //Random number from 0-2 for random response.
			smalltalkoutput = KnowledgeBase[i].responses[r];
			snprintf(response, n, smalltalkoutput);
		}
	}
	return 0;

}
