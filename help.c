//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "base.h"
#include "help.h"




/* INSTRUCTIONS to add a command:
 * Create help%Command%() function at the bottom of this file.
 * Add help%Command%() to help.h
 * Add the strcmp to cHelp()
 * Add it into the printHelp()
 */
 
 	// * * * Implementation

// * Main

void cHelp(char *pieces[], int numP){
	//all available commands listed here
	//if new commands must be added, add them with their respective function
	if(numP==1){
		printHelp();
	}
	else if(numP>=2){
		printf("\n");
		// Main
		if(!strcmp(pieces[1],"authors"))
			helpAuthors();
		else if(!strcmp(pieces[1],"pid"))
			helpPid();
		
		else if(!strcmp(pieces[1],"ppid"))
			helpPPid();
		
		else if(!strcmp(pieces[1],"cd"))
			helpCd();
		
		else if(!strcmp(pieces[1],"date"))
			helpDate();
		
		else if(!strcmp(pieces[1],"historic"))
			helpHistoric();
		
		else if(!strcmp(pieces[1],"open"))
			helpOpen();
		
		else if(!strcmp(pieces[1],"close"))
			helpClose();
		
		else if(!strcmp(pieces[1],"dup"))
			helpDup();
		
		else if(!strcmp(pieces[1],"infosys"))
			helpInfoSys();
		
		else if(!strcmp(pieces[1],"help"))
			helpHelp();
		
		else if(!strcmp(pieces[1],"quit"))
			helpQuit();
		
		else if(!strcmp(pieces[1],"exit"))
			helpExit();
		
		else if(!strcmp(pieces[1],"bye"))
			helpBye();
			
		// FileSys
		else if(!strcmp(pieces[1],"makefile"))
			helpMakeFile();
			
		else if(!strcmp(pieces[1],"makedir"))
			helpMakeDir();

		else if(!strcmp(pieces[1],"listfile"))
			helpListFile();

		else if(!strcmp(pieces[1],"cwd"))
			helpCWD();

		else if(!strcmp(pieces[1],"listdir") || !strcmp(pieces[1],"ls"))
			helpListDir();

		else if(!strcmp(pieces[1],"reclist"))
			helpRecList();

		else if(!strcmp(pieces[1],"revlist"))
			helpRevList();

		else if(!strcmp(pieces[1],"erase"))
			helpErase();

		else if(!strcmp(pieces[1],"delrec"))
			helpDelRec();
			
		else if(!strcmp(pieces[1],"allocate"))
			helpAllocate();

		else if(!strcmp(pieces[1],"deallocate"))
			helpDeallocate();

		else if(!strcmp(pieces[1],"memfill"))
			helpMemFill();

		else if(!strcmp(pieces[1],"memdump"))
			helpMemDump();
		
		else if(!strcmp(pieces[1],"memory"))
			helpMemory();
			
		else if(!strcmp(pieces[1],"recurse"))
			helpRecurse();
		
		else
			errorUnknownArgument("");
		
	}
}

void printHelp(){
	//TODO: Make command list implementation
	int col, i;
	const char *allCommands[] = {
	"authors","pid","ppid","cd","date",
	"historic","open","close","dup","infoSys",
	"makefile","makedir","listfile","cwd","listdir",
	"ls","reclist","revlist","erase","delrec",
	//memory
	"allocate","deallocate","memfill","memdump","memory",
	"recurse",
	"!"
	};
	col = 0;
	//! marks the end of the list, and so must always be at the end
	printf("Comandos disponibles:\n");
	for(i = 0; strcmp(allCommands[i],"!"); i++){
		if (col >= 3){
			col = 0;
			printf("\n");
		}
		printf("%-15s", allCommands[i]);
		col++;
	}
	printf("\n");
}


// * * Help Commands

// * Base Help

void helpAuthors(){
	printf("\t|authors [-n][-l]\n");
	printf("Muestra los nombres y/o logins de los autores\n\n");
}


void helpPid(){
	printf("\t|pid [-p]\n");
	printf("Muestra el pid del shell o de su proceso padre\n\n");
}


void helpPPid(){
	printf("\t|ppid\n");
	printf("Muestra el pid del proceso padre del shell\n\n");
}


void helpCd(){
	printf("\t|cd [dir]\n");
	printf("Cambia (o muestra) el directorio actual del shell\n\n");
}


void helpDate(){
	printf("\t|date [-t][-h]\n");
	printf("Muestra la fecha y/o la hora actual\n\n");
}


void helpHistoric(){
	printf("\t|historic [-c|N|-N]\n");
	printf("Muestra (o borra)el histórico de comandos");
	printf("\t-c: Borra el histórico\n");
	printf("\t-N: Muestra los N últimos comandos\n");
	printf("\tN: Ejecuta de nuevo el comando N de la lista \n\t\t(no permite ejecución de comandos historic)\n\n");
}


void helpOpen(){
	printf("\t|open [file] [mode]\n");
	printf("Abre el archivo \"file\" y lo añade a la lista de ficheros abiertos \nSin argumentos imprime la lista\n\t[mode] permite los siguientes valores de dos caracteres:\n");
	printf("\n\tcr  = O_CREAT");
	printf("\tap  = O_APPEND");
	printf("\n\tex  = O_EXCL");
	printf("\tro  = O_RDONLY");
	printf("\n\trw  = O_RDWR");
	printf("\two  = O_WRONLY");
	printf("\n\ttr  = O_TRUNC");
	printf("\n\n");
}


void helpClose(){
	printf("\t|close df\n");
	printf("Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n\n");
}


void helpDup(){
	printf("\t|dup df\n");
	printf("Duplica el descriptor de fichero \"df\" y anade una nueva entrada a la lista ficheros abiertos\n\n");
}


void helpInfoSys(){
	printf("\t|infosys\n");
	printf("Muestra informacion de la maquina donde corre el shell\n\n");
}


void helpHelp(){
	printf("\t|help [cmd]\n");
	printf("Muestra todos los comandos disponibles por pantalla\n\t[cmd] muestra información de un comando específico\n\n");
}


void helpQuit(){
	printf("\t|quit\n");
	printf("Termina la ejecucion del shell\n\n");
}


void helpExit(){
	printf("\t|exit\n");
	printf("Termina la ejecucion del shell\n\n");
}


void helpBye(){
	printf("\t|bye\n");
	printf("Termina la ejecucion del shell\n\n");
}


// * FileSys Help

void helpMakeFile(){
	printf("\t|makefile [name]\n");
	printf("Crea un fichero de nombre \"name\"\n");
	printf("Si no hay ningún nombre de archivo, muestra el directorio actual del shell\n\n");
}

void helpMakeDir(){
	printf("\t|makedir [name]\n");
	printf("Crea un directorio de nombre \"name\"\n");
	printf("Si no hay ningún nombre de archivo, muestra el directorio actual del shell\n\n");
}

void helpListFile(){
	printf("\t|listfile [-long] [-link] [-acc] [name1] [name2]... \n");
	printf("Lista ficheros con su información (tamaño y nombre)\n");
	printf("\t-long: Listado largo (último acceso;num.inodo;permisos;userID;groupID;num.hardlinks\n");
	printf("\t-acc: Lista la fecha y hora de último acceso\n");
	printf("\t-link: Si el nombre corresponde a un enlace simbólico, muestra el path contenido\n");
	printf("\tname: Puede haber más de un nombre de archivo a listar, pero deben estar siempre al final\n");
	printf("Si no hay ningún nombre de archivo, muestra el directorio actual del shell\n\n");
	
}

void helpCWD(){
	printf("\t|cwd\n");
	printf("Muestra el directorio actual del shell\n\n");
}

void helpListDir(){
	printf("\t|{listdir/ls} [-hid][-long][-acc][-link][dir1][dir2]\n");
	printf("Lista los ficheros mostrados dentro del directorio \"dir\" con su información)\n");
	printf("\t-hid: incluye los ficheros ocultos\n");
	printf("\t-long: Listado largo (último acceso;num.inodo;permisos;userID;groupID;num.hardlinks\n");
	printf("\t-acc: Lista la fecha y hora de último acceso\n");
	printf("\t-link: Si el nombre corresponde a un enlace simbólico, muestra el path contenido\n");
	printf("\tdir: Puede haber más de un nombre de archivo a listar, pero deben estar siempre al final\n");
	printf("Si no hay ningún nombre de directorio, se listarán los contenidos del directorio actual\n\n");
}

void helpRecList(){
	printf("\t|reclist [-hid][-long][-acc][-link][dir1][dir2]\n");
	printf("Lista los ficheros mostrados dentro del directorio \"dir\" con su información)\n");
	printf("Adicionalmente, mostrará después recursivamente los archivos contenidos dentro de cada directorio\n");
	printf("\t-hid: incluye los ficheros ocultos\n");
	printf("\t-long: Listado largo (último acceso;num.inodo;permisos;userID;groupID;num.hardlinks\n");
	printf("\t-acc: Lista la fecha y hora de último acceso\n");
	printf("\t-link: Si el nombre corresponde a un enlace simbólico, muestra el path contenido\n");
	printf("\tdir: Puede haber más de un nombre de archivo a listar, pero deben estar siempre al final\n");
	printf("Si no hay ningún nombre de directorio, muestra el directorio actual del shell\n\n");
}

void helpRevList(){
	printf("\t|revlist [-hid][-long][-acc][-link][dir1][dir2]\n");
	printf("Lista los ficheros mostrados dentro del directorio \"dir\" con su información)\n");
	printf("Adicionalmente, mostrará antes recursivamente los archivos contenidos dentro de cada directorio\n");
	printf("\t-hid: incluye los ficheros ocultos\n");
	printf("\t-long: Listado largo (último acceso;num.inodo;permisos;userID;groupID;num.hardlinks\n");
	printf("\t-acc: Lista la fecha y hora de último acceso\n");
	printf("\t-link: Si el nombre corresponde a un enlace simbólico, muestra el path contenido\n");
	printf("\tdir: Puede haber más de un nombre de archivo a listar, pero deben estar siempre al final\n");
	printf("Si no hay ningún nombre de directorio, muestra el directorio actual del shell\n\n");
}

void helpErase(){
	printf("\t|erase [name1][name2]\n");
	printf("Borra ficheros o directorios vacíos.\n");
	printf("Puede haber más de un nombre de archivo o directorio\n");
	printf("Si no hay ningún nombre de directorio, muestra el directorio actual del shell\n\n");
}

void helpDelRec(){
	printf("\t|delrec [name1][name2]\n");
	printf("Borra ficheros o directorios no vacíos recursivamente.\n");
	printf("Puede haber más de un nombre de archivo o directorio\n");
	printf("Si no hay ningún nombre de directorio, muestra el directorio actual del shell\n\n");
}

void helpAllocate(){
	printf("\t|allocate [-malloc] [N]\n");
	printf("\t-malloc N: Asigna un espacio en memoria de tamaño N\n");
	printf("Sin argumentos muestra la tabla de espacio asignado\n\n");
}

void helpDeallocate(){
	printf("\t|deallocate <address>/[-malloc] [N]\n");
	printf("\t<address>: Desasigna el espacio en memoria en la dirección <address>\n");
	printf("\t-malloc N: Desasigna un espacio en memoria asignado con tamaño N\n");
	printf("Sin argumentos muestra la tabla de espacio asignado\n\n");
}

void helpMemFill(){
	printf("\t|memfill <address> <size> <char>\n");
	printf("Llena <size> bytes en memoria desde <address> con el caracter <char>\n\n");
}

void helpMemDump(){
	printf("\t|memdump <address> <size>\n");
	printf("Muestra el contenido en memoria de <size> bytes a partir desde <address> a <address> + <size> bytes con el caracter <char>\n\n");
}

void helpMemory(){
	printf("\t|memory [-funcs/-vars/-blocks/-all/-pmap]\n");
	printf("\t[-funcs]: Muestra en pantalla la dirección en memoria de tres funciones del programa y tres functiones de librería\n");
	printf("\t[-vars]: Muestra en pantalla las direcciones de tres variables de distintos tipos\n");
	printf("\t[-blocks]: Muestra en pantalla los bloques de memoria asignador con el commando allocate\n");
	printf("\t[-all]: Muestra en pantalla las tres opciones anteriores\n");
	printf("\t[-pmap]: Muestra en pantalla el pmap del proceso del shell\n");
}

void helpRecurse(){
	printf("\t|recurse [N]\n");
	printf("Función recursiva que almacena dos arrays de 2048 bytes:\nUno automático y otro estático\nMuestra también las direcciones de ambos antes de llamarse a sí misma\n\n");
}
