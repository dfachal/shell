//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#ifndef BASE_H
#define BASE_H
#include <time.h>

	// * * * Defines
#define MAX_FILES_OPEN 99 //fileList implementation
#define HISTMAX 4096 //maximum number of entries in the history


	// * * * Typedefs

// * Auxiliaries
void printTime(time_t timeL);
void printTimeOnlyTM(struct tm time);

// * FileList
typedef struct nodeFL* pFL;
struct nodeFL{
	int data;
	char *strFileN;
	int mode;
	pFL next;
}; //note: won't store open type on the struct. It'll be checked by syscall
typedef pFL FL;



	// * * * Headers


// * Base
int breakString(char *string, char *pieces[]);
void printPrompt();
void printCWD();


// * History
bool addToHistory(int *count, char *history[],char *strInput);
void emptyHistory(int *count, char*history[]);


// * Commands
void cAuthors(char *pieces[], int numP);
void cPid(char *pieces[], int numP);
void cPPid(char *pieces[], int numP);
void cChDir(char *pieces[], int numP);
void cDate(char *pieces[], int numP);
void cOpen(char *pieces[], int numP, FL *list);
void cClose(char *pieces[], int numP, FL *fileList);
void cDup(char *pieces[], int numP, FL *fileList);
void cInfoSys(char *pieces[], int numP);


// * FileList
void createFL(FL* list);
pFL findItemFL(int fileDesc, FL *list);
void printFL(FL* list);
void emptyFL(FL *list);



	// * * * Errors

// * Base errors
void errorUnknownCommand(char command[]);
void errorAtoiFailed(char command[], char argument[]);
void errorUnknownArgument(char command[]);
void errorFatal(char command[]); //this should NEVER pop up
void errorMallocFailed(char command[]);
void errorSyscall(char command[]);
void tipFilenameSpaces();


// * History errors
void historyFullWarning();
void errorHistoricSelfReference(char command[]);


// * FileList errors
void errorDeleteFL(char command[]);
void errorCloseNotAllowed(char command[]);
void errorNotFoundFL(char command[]);



#endif
