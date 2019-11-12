/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 chatbot.
 */

#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>

 /* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    256

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 256

/* return codes for knowledge_get() and knowledge_put() */
#define KB_OK        0
#define KB_NOTFOUND -1
#define KB_INVALID  -2
#define KB_NOMEM    -3

/*Global storage for loaded ini content*/
typedef struct {
	char* entity;
	char* responses; //To change amount of strings responses have
}knowledgerecord;

/*This 3 struct arrays stores Loaded Ini File Inputs in this format:
{{entity},{response}}, {{entity},{response}}, {{entity},{response}}
Index 0					Index 1					Index 2

It has 255 individual struct arrays for individual entity and responses
*/
knowledgerecord LoadKnowledgeWhat[255];
knowledgerecord LoadKnowledgeWhere[255];
knowledgerecord LoadKnowledgeWho[255];

//Temp storage for loop to store ini variables 
char LoadedKnowledge[255][255];

/* functions defined in main.c */
int compare_token(const char* token1, const char* token2);
void prompt_user(char* buf, int n, const char* format, ...);

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

/* functions defined in knowledge.c */
int knowledge_get(const char* intent, const char* entity, char* response, int n);
int knowledge_put(const char* intent, const char* entity, const char* response);
void knowledge_reset();
int knowledge_read(FILE* f);
void knowledge_write(FILE* f);

#endif