#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

#define SIZE_OF_LABEL 30
#define SIZE_OF_LINE 80
#define ERROR -1
#define CODE 0 
#define DATA 1 
#define STR 2 
#define ENT 3
#define EXT 4
#define BASE_IC 100
#define BASE_DC 0
#define START_LINE 1
#define DATA_FILE "dataFile"
#define OB_FILE "obFile"
#define ENT_FILE "entFile"
#define EXT_FILE "extFile"
#define FINAL_FILE "finalFile"
#define MAX_FILE_NAME 100
#define DATA_STR ".data"
#define STR_STR ".string"
#define ENT_STR ".entry"
#define EXT_STR ".extern"
#define MAX_PARM 2
#define DONT_CARE -1
#define DIR_OFF 0
#define LAB_OFF 1
#define REL_OFF 2
#define REG_OFF 3




void readfile(FILE* file,char* filename);
char * getLine(FILE *file);
int isEOF(FILE *file);
int isLabelLegal(char *label);
char *hasLabel(char* line);
char *rmLabel(char *oldLine);
char *rmWhiteChars(char *oldLine);
int stateofline(char *line);
int getstate(char * command);
int handledata(char* line,FILE* datafile);
int hasonlywhite(char * line);
int handlecode(char * line);
int isreg(char * str);
int isnum(char * str);
int handlestring(char* line,FILE* datafile);
int * strtoint(char * str);
char * convert_to_address(int v);
char * convert_to_value(int num);
int handleextern(char* line,FILE* datafile);
cp addtolist(char * name, int address,int type, list* l);
list* printlist(list*);
void printcell(cp cellp);
cp exist(list* l,char * name,int type);
void freelist(list* l);
void freelistrec(cp head);
void addic(list* l,int v);
int contentInFirstCell();
int isjmp(char * str);
void addCell(int n);
void rmFirstCell();
int handlecode2(char * line, FILE * file, FILE * extfile);
int codeBitFieldToInt(codebitfield field);
int DataBitFieldToInt(databitfield field);
int isent(list *l, char * name);
void combineFiles(FILE *finalFile, FILE *firstFile, FILE *secondFile, int IC);
int handleentry(char* line,FILE* file);
void freeparm(char** prm);
