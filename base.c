//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h> //cDate
#include <unistd.h> //chdir();read()
#include <math.h> //atoi() (cHist)
#include <linux/limits.h> //PATH_MAX (cChDir)
#include <sys/utsname.h> //uname() (cInfoSys)
#include <sys/types.h> //open()
#include <sys/stat.h> //open()
#include <fcntl.h> //open()
#include <errno.h>
#include "base.h"

/* TODO (opt)
 * printFL: Translate fcntl to input strings
 * cOpen: input file name as full path rather than input name. Use sysCall realpath()
 * (opt) link closer syscall and errorDeleteFL to make sure one doesn't execute without the other
 * (opt) make history list dynamic?
 */


/* Note: This file contains the implementation for the basic commands of the terminal as well as
 * the implementation of the functions related to the list of open files
 * which is done through an dynamic ordered list (ordered by fn value)
 */



	// * * * Auxiliary function headers

// * Auxiliaries for findItemFL:
bool createNodeFL(pFL *p);
bool insertNodeFL(int fileDesc, char fileName[], int mode, FL *list);
bool deleteNodeFL(int fileDesc, FL *list);
bool copyFileNameFL(pFL *q,char fileName[]);
pFL findPFL(int d,FL *list);


	// * * * Implementation

// * Auxiliaries

void printTime(time_t timeL){
	//prints the time_t passed as argument as "DD/MM/YY HH:MM:SS"; NO SPACES OR LINE BREAKS AT THE END
	char timeStr[20];
	struct tm *time;
	
	time = localtime(&timeL);
	strftime(timeStr, sizeof(timeStr), "%D %T", time);
	printf("%-18s", timeStr);
}

void printTimeOnlyTM(struct tm time){
	//prints the time_t passed as argument as "DD/MM/YY HH:MM:SS"; NO SPACES OR LINE BREAKS AT THE END
	char timeStr[20];
	strftime(timeStr, sizeof(timeStr), "%T", &time);
	printf("%-9s", timeStr);
}


// * Base


int breakString(char *string, char *pieces[]){
	int i=1;
		if((pieces[0] = strtok(string," \n\t"))==NULL)
			return 0;
		while((pieces[i]=strtok(NULL," \n\t"))!=NULL){
			i++;
		}
	return i;
}


void printPrompt(){
	printf("\n$");
}


void printCWD(){
	char cwd[PATH_MAX];
	getcwd(cwd,PATH_MAX);
	printf("%s\n", cwd);
}


// * History

bool addToHistory(int *count, char *history[],char *strInput){
	if(*count < (HISTMAX - 1)){
		history[*count] = malloc(sizeof(char [strlen(strInput)]));
		strcpy(history[*count], strInput);
		(*count)++;
		return true;
	}
	return false;
}


void emptyHistory(int *count, char*history[]){
	for(int i = 0;i<(*count);i++){
		//free the mallocs before resetting count
		free(history[i]);
	}
	*count = 0;
}


// * Commands

void cAuthors(char *pieces[], int numP){
	bool l = false;
	bool n = false;
	if (numP > 1){
		for (int i = 1; i<numP; i++){
			if(!strcmp(pieces[i],"-l")){
				l = true;
			}
			else if(!strcmp(pieces[i],"-n")){
				n = true;
			}
			else{
				errorUnknownArgument("authors");
				l = false;
				n = false;
				i = numP;
			}
			
		}
	}
	else{
		l = true; n = true;
	}
	if(n==true){
		printf("\n");
		printf("%28s","Daniel Arias Fachal");
	}
	if(l==true){
		printf("\n");
		printf("%28s","d.fachal");
	}
}


void cPid(char *pieces[], int numP){
	pid_t pid;
	if(numP > 1){
		if(numP == 2 && !strcmp(pieces[1],"-p")){
			pid = getppid();
			printf("Parent process ID: %d", pid);
		}
		else{
			errorUnknownArgument("pid");
		}
	}
	else{
		pid = getpid();
		printf("Process ID: %d", pid);
	}
	printf("\n");
}


void cPPid(char *pieces[], int numP){
	pid_t pid;
	if(numP == 1){
		pid = getppid();
		printf("Parent process ID: %d", pid);
	}
	else{
		errorUnknownArgument("pid");
	}
	printf("\n");
}


void cChDir(char *pieces[], int numP){
	//known limitation: can't open directories with spaces in the name
	if(numP <= 2){
		//char cwd[PATH_MAX];
		int check;
		if (numP == 2){
			check = chdir(pieces[1]);
			if (check != 0){
				errorSyscall("cd");
			}
			else
				printCWD();
		}
		else
			printCWD();
	}
	else{
		errorUnknownArgument("cd");
		tipFilenameSpaces();
	}
}


void cDate(char *pieces[], int numP){
	int i;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	bool d = false;
	bool h = false;
	if(numP>1){
		for(i = 1;i<numP;i++){
			if(!strcmp(pieces[i],"-d"))
				d = true;
			else if(!strcmp(pieces[i],"-h"))
				h = true;
			else{
				d = false;
				h = false;
				errorUnknownArgument("date");
				i = numP;
			}
		}
	}
	else{
		d=true;
		h=true;
	}
	if(d){
		printf("%02d/%02d/%04d\n",tm.tm_mday,tm.tm_mon,tm.tm_year+1900);
	}
	if(h){
		printf("%02d:%02d:%02d\n",tm.tm_hour,tm.tm_min,tm.tm_sec);
	}
}


void cOpen(char *pieces[], int numP, FL *fileList){
	int i, mode = 0, fileDesc = -2;
	if(numP==1){
		printFL(fileList);
		return;
	}
	else if(numP==2){
		mode|=O_APPEND;
		fileDesc = open(pieces[1],mode); //default
	}
    else{
		for (i=2; i < numP; i++)
		  if (!strcmp(pieces[i],"cr")) mode|=O_CREAT;
		  else if (!strcmp(pieces[i],"ex")) mode|=O_EXCL;
		  else if (!strcmp(pieces[i],"ro")) mode|=O_RDONLY; 
		  else if (!strcmp(pieces[i],"wo")) mode|=O_WRONLY;
		  else if (!strcmp(pieces[i],"rw")) mode|=O_RDWR;
		  else if (!strcmp(pieces[i],"ap")) mode|=O_APPEND;
		  else if (!strcmp(pieces[i],"tr")) mode|=O_TRUNC; 
		  else{
			fileDesc = -10;
			errorUnknownArgument("open");
			tipFilenameSpaces();
		}
		if(fileDesc != -10){
			fileDesc = open(pieces[1],mode,0777);
		}
	}
	
	//fileDesc should have been populated if call was successfull, -1 if unsuccessful
	if(fileDesc >= 0){
		if(!insertNodeFL(fileDesc,pieces[1],mode,fileList))
			errorMallocFailed("open");
		else{
			printf("Archivo %s abierto (fd:%d)\n",pieces[1],fileDesc);
		}
		//TODO (opt): input file name as full path rather than input name. Use sysCall realpath()
	}
	else if(fileDesc == -1){
		errorSyscall("open");
	}
}


void cClose(char *pieces[], int numP, FL *fileList){
	int fileDesc = 0;
	if(numP == 2){
		fileDesc = atoi(pieces[1]);
		if(fileDesc < 3){
			errorCloseNotAllowed("close");
		}
		else{
			if(close(fileDesc) != 0)
				errorSyscall("close");
			else{
				if(!deleteNodeFL(fileDesc,fileList))
					errorDeleteFL("close");
				else
					printf("Cerrado archivo (fd:%d)\n",fileDesc);
			}
		}
	}
	else
		errorUnknownArgument("close");
}


void cDup(char *pieces[], int numP, FL *fileList){
	int fileDesc = 0;
	int newFileDesc;
	pFL posOld;
	if(numP == 2){
		errno=0;
		fileDesc = atoi(pieces[1]);
		if(fileDesc == 0 && errno!=0){
			errorAtoiFailed("dup",pieces[1]);
		}
		else{
			posOld = findItemFL(fileDesc, fileList);
			if(posOld==NULL){
				errorNotFoundFL("dup");
			}
			else{
				newFileDesc = dup(fileDesc);
				if(dup(fileDesc) == -1)
					errorSyscall("dup");
				else{
					if(!insertNodeFL(newFileDesc, posOld->strFileN, posOld->mode, fileList))
						errorMallocFailed("dup");
					else
						printf("Archivo (fd:%d) duplicado en fd:%d\n",fileDesc,newFileDesc);
				}
			}
		}
	}
	else
		errorUnknownArgument("close");
}


void cInfoSys(char *pieces[], int numP){
	int success = -1;
	if (numP>1){
		errorUnknownArgument("infoSys");
	}
	else{
		struct utsname sysInfo;
		success = uname(&sysInfo);
		if (success == 0){
			printf("Nombre del sistema: %s\n", sysInfo.sysname);
			printf("Nombre de nodo: %s\n", sysInfo.nodename);
			printf("Release del SO: %s\n", sysInfo.release);
			printf("Versión del SO: %s\n", sysInfo.version);
			printf("HWID: %s\n", sysInfo.machine);
		}
		else errorSyscall("infosys");
	}
}



// * FileList

void createFL(FL* list){
	insertNodeFL(0,"stdin",0,list);
	insertNodeFL(1,"stout",0,list);
	insertNodeFL(2,"sterr",0,list);
}


pFL findItemFL(int fileDesc, FL *list){ //used in cDup
	pFL p;
	if(*list==NULL)
		return NULL;
		
	for(p = *list; p->next != NULL; p = p->next){
		if(p->data == fileDesc){
			return p;
		}
	}
	return NULL;
}


bool insertNodeFL(int fileDesc, char fileName[], int modeO, FL *list){
	pFL p,q;
	if(!createNodeFL(&q))
		return false;
	if(!copyFileNameFL(&q,fileName)){
		free(q);
		return false;
	}
	//will create node q to insert
	q->data = fileDesc;
	q->next = NULL;
	q->mode = modeO;
	if((*list) == NULL){
		*list = q;
		return true;
	}
	p = findPFL(fileDesc,list);
	if(p==NULL){ //insert at first position
		q->next = *list;
		*list = q;
		return true;
	}
	if(!(p->next == NULL)){
		q->next = p->next; //if it's not the last node, maintain the list after the insertion
	}
	p->next = q;
	return true;
}


bool deleteNodeFL(int fileDesc, FL *list){ // Auxiliary for InsertNodeFL
	pFL p,q;
	q = *list;
	if(q==NULL)
		return false;
	if(q->data == fileDesc){
		*list = q->next;
		free(q->strFileN);
		free(q);
	}
	else{
		while(q ->next != NULL && q->data < fileDesc){
			p = q;
			q = q->next;
		}
		if(q->data == fileDesc){
			p->next = q->next;
			free(q->strFileN);
			free(q);
		}
	}
	return true;
}


bool createNodeFL(pFL *p){ // Auxiliary for InsertNodeFL
	*p = malloc(sizeof(struct nodeFL));
	return (*p!=NULL);
}


bool copyFileNameFL(pFL *q,char fileName[]){ // Auxiliary for InsertNodeFL
	(*q)->strFileN = malloc(sizeof(char[strlen(fileName)]));
	if((*q)->strFileN == NULL)
		return false;
	strcpy((*q)->strFileN,fileName);
	return true;
}


pFL findPFL(int d,FL *list){ // Auxiliary for InsertNodeFL
	//find position after which d should be inserted to maintain the order (NULL if it's before the first)
	pFL p = NULL;
	if(d < (*list)->data)
		return p;
	p = *list;
	while(p->next != NULL && d > (p->next->data)){
		p = p->next;
	}
	//if d is the biggest, it'll return the last position in the list
	return p;
}


void printFL(FL *list){
	pFL p = NULL;
	if(*list != NULL){
		for(p = *list;p -> next != NULL; p = p -> next){
			printf("%d | Mode: %3o | Filename: %s\n",p->data,p->mode,p->strFileN);
		}
		printf("%d | Mode: %3o | Filename: %s\n",p->data,p->mode,p->strFileN);
	}
}

void emptyFL(FL *list){
	pFL p,q;
	for(p = *list; p->next != NULL; p = q){
		q = p->next;
		free(p->strFileN);
		free(p);
	}
	free(p->strFileN);
	free(p);
}



	// * * * Errors

// * Base errors

void errorUnknownCommand(char command[]){
	printf("Comando desconocido: %s\nIntroduce 'help' para obtener una lista completa de los comandos disponibles\n",command);
}


void errorAtoiFailed(char command[], char argument[]){ //created for "history" command, might be useful later again
	printf("\t%s - Error: %s no es un número válido",command,argument);
}


void errorUnknownArgument(char command[]){ //generic, an argument for a specific command wasn't recognized
	printf("\t%s - Error: Argumento(s) no válido(s)\n\tIntroduce 'help %s' para vez la lista de argumentos aceptados\n",command,command);
}


void errorFatal(char command[]){ //this should NEVER pop up; it means I fucked up
	printf("\t%s - Error: Fatal error",command);
}


void errorMallocFailed(char command[]){
	printf("\t%s - Error %d durante la asignación de memoria:\n%s\n",command,errno,strerror(errno));
}


void errorSyscall(char command[]){ //MUST be called after a system call's returned an error value
	printf("\t%s - Error %d:\n%s\n",command,errno,strerror(errno));
	//Errno will be populated with the pertinent error number
}


void tipFilenameSpaces(){ //can be called if a command that requires a filename or path has too many arguments
	printf("Verifica que el path no contenga espacios\n");
}



// * FileList errors

void errorDeleteFL(char command[]){
	printf("\t%s - Error: La llamada ha sistema ha podido cerrar el archivo pero debido a un error desconocido este no se ha eliminado de la lista interna.\n",command);
}


void errorCloseNotAllowed(char command[]){
	printf("\t%s - Error: No se permite cerrar procesos con un descriptor de 2 o menor",command);
}


void errorNotFoundFL(char command[]){
	printf("\t%s - Error: No se ha encontrado un archivo abierto con el file descriptor especificado\n",command);
}



// * History errors

void historyFullWarning(){
	printf("Warning: El historial está lleno. \nLa línea de comandos seguirá funcionando, pero no se registrarán más comandos en el historial.\nPara reiniciarlo, introduce 'historic -c'\n\n");
}

void errorHistoricSelfReference(char command[]){
	printf("\t%s - Error: No se permite la llamada a otro comando historic N\n\tIntroduce 'help %s' para más información",command,command);
}
