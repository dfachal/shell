//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "base.h"
#include "help.h"
#include "fileSys.h"
#include "memory.h"

#define CHMAX 255 //maximum characters for console input
#define ENMAX 100 //maximum number of individual words in console input

/*TODO (opt):
 * Figure out strcmp structure; use an ADT/switch?
 */

// * * * HEADERS
void cHist(char *pieces[], int numP,char *history[],FL *fileList,ML *memList,int *count);
bool readInput(int *count,char *history[], FL *fileList, ML *memList);
bool processInput(char *pieces[], int numP,char *history[],FL *fileList,ML *memList,int *count);



// * * * IMPLEMENTATION

void cHist(char *pieces[], int numP,char *history[],FL *fileList,ML *memList,int *count){
	int commandN, i;
	char* newCommand;
	if(numP > 1){
		//historic -c
		if(numP == 2 && !strcmp(pieces[1],"-c")){
		//note: this is NOT asked for in the Lab Assignment, but it seemed useful.
		//resets the history 
			emptyHistory(count,history);
		}
		else if(numP == 2){
			commandN = atoi(pieces[1]);
			if(commandN == 0 || abs(commandN) > *count-1){ 
				//input number must be between 1 and the total number of history entries
				errorAtoiFailed("historic",pieces[1]);
			}
			//historic -N
			else if (commandN < 0){
				//print the last -N commands after checking the number is valid and negative
				for (i = *count - abs(commandN) - 1; i < *count - 1; i++){
					if(i>=0)
						printf("%d: %s\n",(i+1),history[i]);
				}
			}
			//historic N
			else{
				commandN--;
				//execute the command at position N in the history
				newCommand = malloc(sizeof(history[commandN]));
				strcpy(newCommand,history[commandN]); //breakString could change the input string, must make a copy
				numP = breakString(newCommand,pieces);
				if(!strcmp(pieces[0],"historic")){
					if ((numP==1) || (pieces[1][0]) == '-'){ //case: historic command that does not execute
						processInput(pieces,numP,history,fileList,memList,count); //pieces already overwritten
					}
					else{ //case: historic command that does execute another command (prints error)
						errorHistoricSelfReference("historic");
					}
				}
				else{ //a command other than historic
					processInput(pieces,numP,history,fileList,memList,count); //pieces already overwritten
				}
				free(newCommand);
			}
		}
		else{
			errorUnknownArgument("historic");
		}
	}
	else if(numP == 1){
		//without arguments, just print them all
		for(int i = 0;i<(*count)-1;i++){
			printf("%d: %s \n",(i+1),history[i]);
		}
	}
}


bool processInput(char *pieces[], int numP, char *history[], FL *fileList,ML *memList, int *count){
	/*for (int i = 0; i < numP; i++){
		printf("Word %d: %s\n", i, pieces[i]); //COMMAND READ CHECK; FOR TESTING
	}*/
	
	if(!strcmp(pieces[0],"quit")|| !strcmp(pieces[0],"exit")|| !strcmp(pieces[0],"bye")){
		return true;
	}
	//p0 base
	else if(!strcmp(pieces[0],"authors")){
		cAuthors(pieces,numP);
	}
	else if(!strcmp(pieces[0],"pid")){
		cPid(pieces,numP);
	}
	else if(!strcmp(pieces[0],"ppid")){
		cPPid(pieces,numP);
	}
	else if(!strcmp(pieces[0],"cd")){
		cChDir(pieces,numP);
	}
	else if(!strcmp(pieces[0],"date")){
		cDate(pieces,numP);
	}
	else if(!strcmp(pieces[0],"historic")){
		cHist(pieces,numP,history,fileList,memList,count);
	}
	else if(!strcmp(pieces[0],"open")){
		cOpen(pieces,numP,fileList);
	}
	else if(!strcmp(pieces[0],"close")){
		cClose(pieces,numP,fileList);
	}
	else if(!strcmp(pieces[0],"dup")){
		cDup(pieces,numP,fileList);
	}
	else if(!strcmp(pieces[0],"infosys")){
		cInfoSys(pieces,numP);
	}
	else if(!strcmp(pieces[0],"help")){
		cHelp(pieces,numP);
	}
	
	//p1 filesys
	else if(!strcmp(pieces[0],"makefile")){
		cMakeFile(pieces,numP);
	}
	else if(!strcmp(pieces[0],"makedir")){
		cMakeDir(pieces,numP);
	}
	else if(!strcmp(pieces[0],"listfile")){
		cListFile(pieces,numP);
	}
	else if(!strcmp(pieces[0],"cwd")){
		cCWD(pieces,numP);
	}
	else if(!strcmp(pieces[0],"listdir")  || !strcmp(pieces[0],"ls")){
		cListDir(pieces,numP);
	}
	else if(!strcmp(pieces[0],"reclist")){
		cRecList(pieces,numP,true);
	}
	else if(!strcmp(pieces[0],"revlist")){
		cRecList(pieces,numP,false);
	}
	else if(!strcmp(pieces[0],"erase")){
		cErase(pieces,numP);
	}
	else if(!strcmp(pieces[0],"delrec")){
		cDelRec(pieces,numP);
	}
	else if(!strcmp(pieces[0],"trash")){
		cTrash(pieces,numP);
	}
	else if(!strcmp(pieces[0],"fsstats")){
		cFSStats(pieces,numP);
	}
	
	//p2 memory
	else if(!strcmp(pieces[0],"allocate")){
		cAllocate(pieces,numP,memList);
	}
	else if(!strcmp(pieces[0],"deallocate")){
		cDeallocate(pieces,numP,memList);
	}
	else if(!strcmp(pieces[0],"memfill")){
		cMemFill(pieces,numP,memList);
	}
	else if(!strcmp(pieces[0],"memdump")){
		cMemDump(pieces,numP,memList);
	}
	else if(!strcmp(pieces[0],"memory")){
		cMemory(pieces,numP,memList);
	}
	else{
		errorUnknownCommand(pieces[0]);
	}
	return false;
}


bool readInput(int *count,char *history[], FL *fileList, ML *memList){
	bool exit = false;
	char strInput[CHMAX];
	char *pieces[ENMAX];
	int numP = 0;
	char justInCase = 'a';
	fgets(strInput, CHMAX, stdin);
	if (strInput[strlen(strInput) - 1] != '\n'){
		do{
		justInCase = getchar();
		}
		while (justInCase != '\n' && justInCase!= EOF); //if our CHMAX was exceeded by the input, loop indefinitely while the input buffer isn't flushed
		printf("La entrada es demasiado larga. Inténtalo de nuevo...\n");
	}
	
	else if (strcmp(strInput, "\n")){
		//saving command to history and increasing count; command will be added even if it ends up not being valid or throwing an error
		/*if (!addToHistory(count,history,strInput)){
			historyFullWarning();
		}*/
		//break strInput and call for its processing
		numP = breakString(strInput, pieces);
		exit = processInput(pieces, numP,history,fileList,memList,count);
	}
	return exit;
}


int main(){
	bool exit = false;
	//history
	int count = 0;
	char *history[HISTMAX];
	//opened file list
	FL fileList;
	ML memList;
	createFL(&fileList);
	createML(&memList);
	
	//terminal
	while (!exit){
		printPrompt();
		exit = readInput(&count,history,&fileList,&memList);
	}
	emptyHistory(&count,history);
	emptyFL(&fileList);
	emptyML(&memList);
	return 0;
}
