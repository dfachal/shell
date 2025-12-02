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
		
		else if(!strcmp(pieces[1],"trash"))
			helpTrash();
			
		else if(!strcmp(pieces[1],"fsstats"))
			helpFSStats();
		
		else if(!strcmp(pieces[1],"copy"))
			helpCopy();
			
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
	"ls","reclist","revlist","erase","delrec","trash",
	"fsstats","copy"
	//memory
	"allocate","deallocate","memfill","memdump","memory",
	"recurse",
	
	"!"
	};
	col = 0;
	//! marks the end of the list, and so must always be at the end
	printf("Available commands:\n");
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
	printf("Show authors name and/or e-mail\n\n");
}


void helpPid(){
	printf("\t|pid [-p]\n");
	printf("Show shell's PID or the parent process's PID\n\n");
}


void helpPPid(){
	printf("\t|ppid\n");
	printf("Show shell's parent process's PID\n\n");
}


void helpCd(){
	printf("\t|cd [dir]\n");
	printf("Changes (or shows) current working directory\n\n");
}


void helpDate(){
	printf("\t|date [-d][-h]\n");
	printf("Shows current date [-d] and/or time [-t]\n\n");
}


void helpHistoric(){
	printf("\t|historic [-c|N|-N]\n");
	printf("Shows (or deletes) history of commands. Can also execute a command in the history");
	printf("\t-c: Borra el histórico\n");
	printf("\t-N: Muestra los N últimos comandos\n");
	printf("\tN: Executes again command number N in the list \n\t\t(does not allow executing a \"historic\" command from the list)\n\n");
}


void helpOpen(){
	printf("\t|open [file] [mode]\n");
	printf("Opens \"file\" and adds it to the list of open files\nWithout arguments it shows the list of open files\n\t[mode] allows the following open modes:\n");
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
	printf("Closes file with file descriptor \"df\" and eliminates the element from the open file list\n\n");
}


void helpDup(){
	printf("\t|dup df\n");
	printf("Duplicates file with descriptor \"df\" also adding a new entry to the open file list\n\n");
}


void helpInfoSys(){
	printf("\t|infosys\n");
	printf("Shows system information\n\n");
}


void helpHelp(){
	printf("\t|help [cmd]\n");
	printf("Shows all available commands.\n\t[cmd] shows information about a specific command and its parameters\n\n");
}


void helpQuit(){
	printf("\t|quit\n");
	printf("Ends shell execution\n\n");
}


void helpExit(){
	printf("\t|exit\n");
	printf("Ends shell execution\n\n");
}


void helpBye(){
	printf("\t|bye\n");
	printf("Ends shell execution\n\n");
}


// * FileSys Help

void helpMakeFile(){
	printf("\t|makefile [name]\n");
	printf("Creates a new file called \"name\"\n");
	printf("If no file name is provided, prints current working directory\n\n");
}

void helpMakeDir(){
	printf("\t|makedir [name]\n");
	printf("Creates a new directory called \"name\"\n");
	printf("If no file name is provided, prints current working directory\n\n");
}

void helpListFile(){
	printf("\t|listfile [-long] [-link] [-acc] [name1] [name2]... \n");
	printf("Lists files and their information (name and size)\n");
	printf("\t-long: Lost listing (last access; inode number; permissions; userID; groupID; number of hardlinks\n");
	printf("\t-acc: Lists date and time of last access\n");
	printf("\t-link: If the file is a symbolic link, shows contained path\n");
	printf("\tname: May contain more than one file name, but they must be at the end of the command and contain no spaces\n");
	printf("If no file name is provided, prints current working directory\n\n");
	
}

void helpCWD(){
	printf("\t|cwd\n");
	printf("Prints current working directory\n\n");
}

void helpListDir(){
	printf("\t|{listdir/ls} [-hid][-long][-acc][-link][dir1][dir2]\n");
	printf("Lists files withing the directory \"dir\" (or current directory if none is provided) with their information\n");
	printf("\t-hid: Show hidden files \n");
	printf("\t-long: Long listing (last access; inode number; permissions; userID; groupID; number of hardlinks\n");
	printf("\t-acc: Lists date and time of last access\n");
	printf("\t-link: If the file is a symbolic link, shows contained path\n");
	printf("\tname: May contain more than one file name, but they must be at the end of the command and contain no spaces\n");
	printf("If no file name is provided, prints current working directory\n\n");
}

void helpRecList(){
	printf("\t|reclist [-hid][-long][-acc][-link][dir1][dir2]\n");
	printf("Lists files withing the directory \"dir\" (or current directory if none is provided) with their information\n");
	printf("Additionally, it will print recursively all files contained within each directory afterwards\n");
	printf("\t-hid: Show hidden files \n");
	printf("\t-long: Long listing (last access; inode number; permissions; userID; groupID; number of hardlinks\n");
	printf("\t-acc: Lists date and time of last access\n");
	printf("\t-link: If the file is a symbolic link, shows contained path\n");
	printf("\tname: May contain more than one file name, but they must be at the end of the command and contain no spaces\n");
	printf("If no file name is provided, prints current working directory\n\n");
}

void helpRevList(){
	printf("\t|revlist [-hid][-long][-acc][-link][dir1][dir2]\n");
	printf("Lists files withing the directory \"dir\" (or current directory if none is provided) with their information\n");
	printf("Additionally, it will print recursively all files contained within each directory beforehand\n");
	printf("\t-hid: Show hidden files \n");
	printf("\t-long: Long listing (last access; inode number; permissions; userID; groupID; number of hardlinks\n");
	printf("\t-acc: Lists date and time of last access\n");
	printf("\t-link: If the file is a symbolic link, shows contained path\n");
	printf("\tname: May contain more than one file name, but they must be at the end of the command and contain no spaces\n");
	printf("If no file name is provided, prints current working directory\n\n");
}

void helpErase(){
	printf("\t|erase [name1][name2]\n");
	printf("Deletes files or empty directories\n");
	printf("May contain more than one name of file or directory\n");
	printf("If no file or directory name is provided, prints current working directory\n\n");
}

void helpDelRec(){
	printf("\t|delrec [name1][name2]\n");
	printf("Deletes a file or a directory. It will delete recursively all contents of the directory\n");
	printf("May contain more than one name of file or directory\n");
	printf("If no file or directory name is provided, prints current working directory\n\n");
}

void helpTrash(){
	printf("\t|trash [-empty]\n");
	printf("Shows or deletes trash contents\n");
	printf("\t-empty: Empties trash\n\n");
}

void helpFSStats(){
	printf("\t|fsstats\n");
	printf("Shows mounted file systems statistics, including free and occupied space (in bytes)\n\n");
}

void helpCopy(){
	printf("\t|copy [file1] [file2]\n");
	printf("\tCopies 'file1' creating 'file2' with the same contents\n\n");
}


// * Memory Help

void helpAllocate(){
	printf("\t|allocate [-malloc] [N]\n");
	printf("\t-malloc N: Assigns a space in memory of size N\n");
	printf("Running without arguments will print the table of assigned memory spaces\n\n");
}

void helpDeallocate(){
	printf("\t|deallocate <address>/[-malloc] [N]\n");
	printf("\t<address>: Deassigns the space in memory of <address>\n");
	printf("\t-malloc N: Deassigns a space in memory of size N (only one if there are several)\n");
	printf("Running without arguments will print the table of assigned memory spaces\n\n");
}

void helpMemFill(){
	printf("\t|memfill <address> <size> <char>\n");
	printf("Fills <size> bytes in memory from <address> with the character <char>\n\n");
}

void helpMemDump(){
	printf("\t|memdump <address> <size>\n");
	printf("Shows content in memory of <size> bytes from <address> to <address> + <size> bytes\n\n");
}

void helpMemory(){
	printf("\t|memory [-funcs/-vars/-blocks/-all/-pmap]\n");
	printf("\t[-funcs]: Prints memory address of three program functions and three library functions\n");
	printf("\t[-vars]: Prints addresses of variables of different types\n");
	printf("\t[-blocks]: Prints all memory blocks assigned with allocate command\n");
	printf("\t[-all]: Prints all all three -funcs -vars and -blocks\n");
	printf("\t[-pmap]: Show pmap of the shell's process\n");
}

void helpRecurse(){
	printf("\t|recurse [N] [NOT IMPLEMENTED]\n");
	printf("Recursive function that stores two arrays of 2048 bytes:\nOne's automatic, the other one's static\nShows addresses of both before calling itself\n\n");
	//keeping old Spanish description in case the translation must be corrected later TODO: Confirm translation and delete comment
	//printf("Función recursiva que almacena dos arrays de 2048 bytes:\nUno automático y otro estático\nMuestra también las direcciones de ambos antes de llamarse a sí misma\n\n");
}
