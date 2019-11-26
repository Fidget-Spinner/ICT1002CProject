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

 /* GLOBALS */

/* Just the hard coded base small talk responses.
*/
typedef struct {
	char* intent;
	char* responses[SMALLTALK_RESPONSE_NUM]; //To change amount of strings responses have
}record;

record SmallTalkBase[] = {
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
	},

	{"It's",
	{"Indeed it is", "Yes it is", "I agree"}
	},
	
	{"help",
	{
		"================================= HELP INSTRUCTIONS =================================					\n\
		WHAT [is/are]	- asks the chatbot a question									 						\n\
		RESET			- wipes any loaded/learnt memory (reset)												\n\
		LOAD [from]		- load .ini file into chatbot memory. (load [from] <FILENAME>)							\n\
		SAVE			- saves the entries in chatbot's memory to an .ini file Eg.(save as/to/at <FILENAME>),	\n\
		REDEFINE		- modify entries currently in memory Eg. (REDEFINE What is SIT?)						\n\
		EXIT			- exit the program. (exit/quit),														\n\
		====================================================================================="
	}
	}

};

/* INTERNAL HELPER FUNCTIONS PROTOTYPE DEFINITIONS*/

/*
* Helper function for answering questions. Required by chatbot_do_question.
*
* inv[0] contains the the question word.
* inv[1] may contain "is" or "are"; if so, it is skipped.
* The remainder of the words form the entity.
*
* This function retrieves a value from the hashmap corresponding to the questionEntityPtr (key).
*/
void chatbot_do_question_helper(int inc, char* inv[], char* response, int n, char questionEntityPtr[]);

/*
* Helper function for redefining entries. Required by chatbot_do_redefine
*
* inv[0] contains the the question word.
* inv[1] may contain "is" or "are"; if so, it is skipped.
* The remainder of the words form the entity.
*
* This function overwrites a value from the hashmap corresponding to questionEntityPtr (key).
*/
void chatbot_do_redefine_helper(int inc, char* inv[], char* response, int n, char questionEntityPtr[]);


/* MAIN FUNCTIONS */

/*
* Utility function to give an appropiate reponse to KB errors
*  @param kb_status: Either KB_OK, KB_NOTFOUND, KB_INVALID or KB_NOMEM
*  @param response: the buffer to receive the response
*  @param n : the maximum number of characters to write to the response buffer
*  @param what_to_respond: the string to write to response buffer if KB_OK
*/
void respond_kb_errors(int kb_status, int inc, char * inv[], char* response, int n, char *what_to_respond, char questionEntityPtr[]) {
	char buf[MAX_LENGTH_USER_INPUT];
	switch (kb_status) {
	case KB_OK:
		snprintf(response, n, what_to_respond);
		break;
	case KB_INVALID:
		snprintf(response, n, "Invalid knowledge base");
		break;
	case KB_NOMEM:
		snprintf(response, n, "I've run out of memory");
		break;
	case KB_NOTFOUND: //if cant find it in the hashmap, then insert it in.
		prompt_user(buf, n, "%s%s", "I don't know. ", user_input);
		knowledge_put(inv[0], questionEntityPtr, buf);
		snprintf(response, n, "Thank you.");
		break;
	}
}

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
	else if (chatbot_is_redefine(inv[0]))
		return chatbot_do_redefine(inc, inv, response, n);
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
	knowledge_reset(); //reset knowledge first to free memory and prevent memory leaks
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
	int err_code;
	if (inv[1] == "" || inv[1] == NULL)
	{
		snprintf(response, n, "Error missing filename!");
	}
	else {
		int index = (compare_token(inv[1], "from") == 0) ? 2 : 1;  // check for the "from" and ignore it
		f = fopen(inv[index], "r");
		if (f) {
			err_code = knowledge_read(f);
			if (err_code) //Test for error reading
			{
				snprintf(response, n, "Successfully read %d response(s) from %s", err_code, inv[index]);
			}
			else if (err_code == 0)
			{
				snprintf(response, n, "Error reading file!");
			}
			fclose(f);
		}
		else {
			snprintf(response, n, "Error opening file! Check the name or the file permissions.");
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

	char questionEntityPtr[MAX_LENGTH_USER_INPUT] = ""; //Store entity of user input

	chatbot_do_question_helper(inc, inv, response, n, questionEntityPtr);
	return 0;

}

void chatbot_do_question_helper(int inc, char* inv[], char* response, int n, char questionEntityPtr[]) {
	DATA_NODE* hashMapToSearch = NULL;
	char responseBuf[MAX_LENGTH_USER_INPUT];
	int KB_STATUS;
	int startSearchIndex;
	if (inv[1] == NULL)
	{
		snprintf(response, n, "Please input a proper question!"); //If question is improper, break
		return;
	}
	startSearchIndex = (compare_token(inv[1], "are") == 0 || compare_token(inv[1], "is") == 0) ? 2 : 1; //check for is and are and ignore it
	for (startSearchIndex; startSearchIndex < inc; startSearchIndex++)
	{
		strcat(questionEntityPtr, str_upper(inv[startSearchIndex])); //store input entity 
		if (inv[startSearchIndex + 1] != NULL)
			strcat(questionEntityPtr, " "); //check for space and insert space
	}
	KB_STATUS = knowledge_get(inv[0], questionEntityPtr, responseBuf, n);
	respond_kb_errors(KB_STATUS, inc, inv, response, n, responseBuf, questionEntityPtr);

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

	knowledge_reset(); //reset knowledge to free memory and prevent memory leaks
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
 *  1, if the intent is "save"
 *  0, otherwise
 */
int chatbot_is_save(const char* intent) {

	return compare_token(intent, "save") == 0;

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Ignores "as" "to" or "at" at the second word
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char* inv[], char* response, int n) {
	int index;
	if (inv[1] == "" || inv[1] == NULL)
	{
		snprintf(response, n, "Error missing filename!");
	}
	else {
		index = (compare_token(inv[1], "as") == 0 || compare_token(inv[1], "to") == 0 || compare_token(inv[1], "at") == 0) ? 2 : 1; 
		FILE* f = fopen(inv[index], "wb");
		if (f) { // check for file errors
			knowledge_write(f);
		}
		else {
			snprintf(response, n, "%s %s", "Could not create/open file with the name", inv[index]);
			return 0;
		}
		
		if (ferror(f)) {
			snprintf(response, n, "%s %s", "Could not save to", inv[index]);
			return 0;
		}
		else {
			snprintf(response, n, "%s %s", "My knowledge has been saved to", inv[index]);
		}
		fclose(f);
	}

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

	return compare_token(intent, "hello") == 0 
		|| compare_token(intent, "morning") == 0 
		|| compare_token(intent, "afternoon") == 0 
		|| compare_token(intent, "nights") == 0 
		|| compare_token(intent, "how") == 0 
		|| compare_token(intent, "it's") == 0 
		|| compare_token(intent, "help") == 0;

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
	int r = rand() % SMALLTALK_RESPONSE_NUM;
	r = (compare_token(inv[0], "help") == 0) ? 0 : r; //check for "help" smalltalk special case which only has 1 output at index 0
	for (int i = 0; i < sizeof(SmallTalkBase) / sizeof(SmallTalkBase[0]); ++i) {
		if (compare_token(SmallTalkBase[i].intent, inv[0]) == 0) {
			smalltalkoutput = SmallTalkBase[i].responses[r];
			snprintf(response, n, smalltalkoutput);
		}
	}

	return 0;

}

/*
 * Determine which an intent is redefine.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the sredefine phrases
 *  0, otherwise
 */
int chatbot_is_redefine(const char* intent) {

	return compare_token(intent, "redefine") == 0;
}


/*
 * Respond to.
 *
 * Redefines an answer to a question in knowledge base.
 * 
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */

int chatbot_do_redefine(int inc, char* inv[], char* response, int n) {

	char questionEntityBuf[MAX_LENGTH_USER_INPUT] = ""; //Store entity of user input

	chatbot_do_redefine_helper(inc, inv, response, n, questionEntityBuf);
	return 0;

}

void chatbot_do_redefine_helper(int inc, char* inv[], char* response, int n, char questionEntityPtr[]) {
	DATA_NODE* hashMapToSearch = NULL;
	char buf[MAX_LENGTH_USER_INPUT] = ""; //Store entity of user input
	int startSearchIndex;
	if (inv[2] == NULL)
	{
		snprintf(response, n, "Please input a proper question!"); //If question is improper, break
		return;
	}
	startSearchIndex = (compare_token(inv[2], "is") || compare_token(inv[2], "are")) ? 3 : 2; //check for is/are and ignore

	for (int b = startSearchIndex; b < inc; b++)
	{
		strcat(questionEntityPtr, str_upper(inv[b])); //store input entity 
			if (inv[b + 1] != NULL)
				strcat(questionEntityPtr, " "); //check for space and insert space
	}
	prompt_user(buf, n, "%s", "Redefining...");
	knowledge_put(inv[1], questionEntityPtr, buf);
	snprintf(response, n, "Thank you.");

	return;
	

}
