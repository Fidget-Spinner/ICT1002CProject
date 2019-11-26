/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 chatbot.
 */

#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>
#include "HashMap.h"

 /* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    256

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 1024

/* return codes for knowledge_get() and knowledge_put() */
#define KB_OK        0
#define KB_NOTFOUND -1
#define KB_INVALID  -2
#define KB_NOMEM    -3

/** global hashmap settings **/

// the larger this value is, the faster the hashmap will be for extremely large number of inputs, however the tradeoff is that more RAM will be used
#define SIZE_OF_HASHMAP 676 
//max length of a key or value's chars
#define MAX_LENGTH_USER_INPUT MAX_ENTITY + 1 + MAX_RESPONSE + 1 

// singly linked list struct, used for the hash table elements
struct Data_Node {
	char key[MAX_LENGTH_USER_INPUT + 2];
	char value[MAX_LENGTH_USER_INPUT + 2];
	struct Data_Node* next;
};


// declaration of global variables
extern DATA_NODE* LoadKnowledgeWhatMap[SIZE_OF_HASHMAP];
extern DATA_NODE* LoadKnowledgeWhoMap[SIZE_OF_HASHMAP];
extern DATA_NODE* LoadKnowledgeWhereMap[SIZE_OF_HASHMAP];
/* global buffer for holding the unmodified user input */

extern char user_input[MAX_INPUT];      



/* functions defined in main.c */
int compare_token(const char* token1, const char* token2);
void prompt_user(char* buf, int n, const char* format, ...);
char* str_upper(char* str);
void removeTrailingNewLine(char buf[]);

/* functions defined in chatbot.c */
const char* chatbot_botname();
const char* chatbot_username();
int chatbot_main(int inc, char* inv[], char* response, int n);
int chatbot_is_exit(const char* intent);
int chatbot_do_exit(int inc, char* inv[], char* response, int n);
int chatbot_is_load(const char* intent);
int chatbot_do_load(int inc, char* inv[], char* response, int n);
int chatbot_is_question(const char* intent);
int chatbot_do_question(int inc, char* inv[], char* response, int n);
int chatbot_is_reset(const char* intent);
int chatbot_do_reset(int inc, char* inv[], char* response, int n);
int chatbot_is_save(const char* intent);
int chatbot_do_save(int inc, char* inv[], char* response, int n);
int chatbot_is_smalltalk(const char* intent);
int chatbot_do_smalltalk(int inc, char* inv[], char* resonse, int n);
void respond_kb_errors(int kb_status, int inc, char* inv[], char* response, int n, char* what_to_respond, char questionEntityPtr[]);
int chatbot_is_redefine(const char* intent);
int chatbot_do_redefine(int inc, char* inv[], char* response, int n);


/* functions defined in knowledge.c */
int knowledge_get(const char* intent, const char* entity, char* response, int n);
int knowledge_put(const char* intent, const char* entity, const char* response);
void knowledge_reset();
int knowledge_read(FILE* f);
void knowledge_write(FILE* f);

#endif