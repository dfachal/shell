//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <linux/limits.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/sendfile.h>
#include <mntent.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include "fileSys.h"
#include "base.h"



	// * * * Auxiliary function headers

char LetraTF (mode_t m);
char * ConvierteModo2 (mode_t m);

char* concat(const char *s1, const char *s2, bool *check);

void fileInfoHeader(bool acc, bool lng, bool lnk);
void fileInfo(char fileName[], char relPath[], bool acc, bool lng, bool lnk);

void readDir(char dirName[], bool hid, bool acc, bool lng, bool lnk);
void readDirRec(char dirName[], bool hid, bool acc, bool lng, bool lnk, bool after);

bool delRec(char path[]);

char* getTrashPath(void);
char* getTrashInfoPath(void);



	// * * * Implementation

//function provided in help code:
char LetraTF (mode_t m){
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}


//function provided in help code:
char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}


char* concat(const char *s1, const char *s2, bool *check){
    char *result = malloc(strlen(s1) + strlen(s2) + 2);
    if(result == NULL){
    	*check = false;
    }
    else{
		strcpy(result, s1);
		strcat(result, "/");
		strcat(result, s2);
		*check = true;
    }
    return result;
}


void fileInfoHeader(bool acc, bool lng, bool lnk){
	if(acc || lng){
		printf("%-18s|","LastAccessTime");
	}
	printf("%-8s|","Size(B)");
	if(lng){
		printf("%-8s|","Inode #");
		printf("%-12s|","Permissions");
		printf("%-7s|","UserID");
		printf("%-7s|","GroupID");
		printf("%-8s|","# HLinks"); //NUMBER OF HARD LINKS
	}
	printf("Name");
	if(lnk)
		printf(" -> link");
	printf("\n");
}


void fileInfo(char fileName[], char relPath[], bool acc, bool lng, bool lnk){
	//stat call
	struct stat buff;
	int check;
	//permissions
	char permissions[12];
	//symlinks
	char linkPath[PATH_MAX];
	ssize_t linkPathLen;
	
	if(lnk)
		check = lstat(relPath,&buff);
	else
		check = stat(relPath,&buff);
		
	if(check == -1){
		printf("\"%s\"", relPath);
		errorSyscall("listfile");
	}
		
	else{ //print info of the file
		strcpy(permissions,ConvierteModo2(buff.st_mode)); //populates permissions var
		if(acc || lng){
			printTime(buff.st_atime); //TIME
			printf(" ");
		}
		printf("%8lu ",buff.st_size); //SIZE
		if(lng){
			printf("%8lu ",buff.st_ino); //INODE NUMBER
			printf("%-12s ",permissions); //PERMISSIONS
			printf("%7u %7u ",buff.st_uid,buff.st_gid); //USER AND GROUP IDs
			printf("%8lu ",buff.st_nlink); //NUMBER OF HARD LINKS
		}
		printf("%-s", fileName); //FILE NAME
		if(permissions[0] == 'l' && lnk){
			printf(" -> ");
			linkPathLen = readlink(relPath, linkPath, sizeof(linkPath) - 1);
			if(linkPathLen == -1){
				printf("\n");
				errorSyscall("listfile {readlink()}");
			}
			else{
				linkPath[linkPathLen] = '\0'; //apparently linkPath doesn't null terminate the string
				printf("%s",linkPath);
			}
		}
		printf("\n");
	}
	
}


void readDir(char dirName[], bool hid, bool acc, bool lng, bool lnk){
	DIR *dir;
	struct dirent *entry;
	char directory[PATH_MAX];
	char* relPath;
	bool concatCheck;
	
	strcpy(directory,dirName);
	dir = opendir(directory);
	if (dir == NULL) {
		printf("\"%s\"",directory);
		errorSyscall("listfile");
	}
	else{
		printf("*DIRECTORY: %s\n",dirName);
		fileInfoHeader(acc,lng,lnk); //print header with columns
		while((entry = readdir(dir)) != NULL){ //iterate through all entries
			if((strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")) && (hid || entry->d_name[0] != '.')){ //discard . and .. as well as hidden files if necessary
				relPath = concat(dirName,entry->d_name,&concatCheck);
				if(!concatCheck)
					errorMallocFailed("readdir");
				else{
					fileInfo(entry->d_name,relPath,acc,lng,lnk);
					free(relPath);
				}
			}
		}
		closedir(dir); //must close it at the end
	}
}

void readDirRec(char dirName[], bool hid, bool acc, bool lng, bool lnk, bool after){
	DIR *dir;
	struct dirent *entry;
	char directory[PATH_MAX];
	char* relPath;
	bool concatCheck;
	bool first = true; //for printing header
	bool afterAux = after; //will need to keep after intact for recursive calls
	//for STAT to check if it's file or dir
	int check;
	char permissions[12];
	struct stat buff; 
	
	int i; //will iter two times
	strcpy(directory,dirName);
	
	for(i = 0; i < 2; i++){
		//open directory:
		dir = opendir(directory);
		
		if (dir == NULL) { //
			printf("\"%s\"",directory);
			errorSyscall("opendir");
		}
		else{
			if(!afterAux){
				printf("*DIRECTORY: %s\n",dirName);
				while((entry = readdir(dir)) != NULL){ //iterate through all entries
					if((strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")) && (hid || entry->d_name[0] != '.')){ //discard . and .. as well as hidden files if necessary
						relPath = concat(dirName,entry->d_name,&concatCheck);
						if(!concatCheck)
							errorMallocFailed("recursivelist {concat()}");
						else{
							if(first)
								fileInfoHeader(acc,lng,lnk);
							first = false;
							fileInfo(entry->d_name,relPath,acc,lng,lnk);
							free(relPath);
						}
					}
				}
				if(!first){
					first = true;
					printf("\n");
				}
			}
			else{ //afterAux == true
				while((entry = readdir(dir)) != NULL){ //iterate through all entries
					if((strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")) && (hid || entry->d_name[0] != '.')){ //discard . and .. as well as hidden files if necessary
						relPath = concat(dirName,entry->d_name,&concatCheck);
						if(!concatCheck)
							errorMallocFailed("readdir");
						else{
							check = lstat(relPath,&buff);
								
							if(check == -1){
								errorFileRead(relPath);
							}
							else{ //get permissions
								strcpy(permissions,ConvierteModo2(buff.st_mode));
								if(permissions[0] == 'd'){ //if it's a directory, recursive call
									readDirRec(relPath,hid,acc,lng,lnk,after);
								}
							}
							free(relPath);
						}
					}
				}
			}
			closedir(dir); //must close after each iteration
			afterAux = !after;
		}
	}
}


bool delRec(char path[]){
	DIR *dir;
	struct dirent *entry;
	char directory[PATH_MAX];
	char* relPath;
	bool concatCheck;
	bool success = true; //for printing header
	//for STAT to check if it's file or dir
	int check;
	char permissions[12];
	struct stat buff; 
	
	strcpy(directory,path);

	//open directory:
	dir = opendir(directory);
	
	if (dir == NULL) { //
		printf("\"%s\"",directory);
		errorSyscall("delrec");
		success = false;
	}
	else{
		while((entry = readdir(dir)) != NULL){ //iterate through all entries
			if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){ //discard . and ..
				relPath = concat(path,entry->d_name,&concatCheck);
				if(!concatCheck)
					errorMallocFailed("readdir");
				else{
					check = lstat(relPath,&buff);
						
					if(check == -1){
						errorFileRead(relPath);
					}
					else{ //get permissions
						strcpy(permissions,ConvierteModo2(buff.st_mode));
						if(permissions[0] == 'd'){ //if it's a directory, recursive call
							success = delRec(relPath);
						}
						else{
							if (remove(relPath) != -1){
								printf("File deleted %s\n",relPath);
							}
							else if(rmdir(relPath) != -1){
								printf("File deleted %s\n",relPath);
							}
							else{
								success = false;
								printf("%s",relPath);
								errorSyscall("delrec");
							}
						}
					}
					free(relPath);
				}
			}
		}
		closedir(dir);
		
		if(success){
			printf("Deleting contents of %s\n",path);
			if (remove(path) != -1){
				printf("Directory deleted %s\n",path);
			}
			else if(rmdir(path) != -1){
				printf("Directory deleted %s\n",path);
			}
			else{
				success = false;
				printf("%s",path);
				errorSyscall("delrec");
			}
		}
	}
	return success;
}


// * fileSys

void cMakeFile (char *pieces[], int numP){
	//reference shell calls file creation with permissions 0777 but the resulting file has 0775; the behavior is the same here 
	int fileDesc = 0;
	if(numP == 1)
		printCWD(); //prints current directory if called without the file name argument
		
	else if (numP == 2){
		
		fileDesc = open(pieces[1], O_APPEND);
		if(fileDesc != -1){
			close(fileDesc);
			errorFileAlreadyExists("makefile",pieces[1]);
		}
		else{
			fileDesc = creat(pieces[1], 0777);
			if(fileDesc == -1)
				errorSyscall("makefile");
			
			else if(close(fileDesc) != 0)
				errorSyscall("makefile {close()}");
		}
	}
	
	else{
		errorUnknownArgument("makefile");
		tipFilenameSpaces();
	}
}


void cMakeDir (char *pieces[], int numP){
	if (numP == 1)
		printCWD(); //prints current directory if called without the directory name argument

	else if (numP == 2) {
		if (mkdir(pieces[1], 0777) != 0){ //0777 es para dar permisos
			errorSyscall("makedir");
		}
	}
	else{
		errorUnknownArgument("makedir");
		tipFilenameSpaces();
	}
}


void cListFile (char *pieces[], int numP){
	int i;
	bool acc = false, lng = false, lnk = false, ctrl = false; //for attribute evaluation
	
	for(i = 1; i < numP && ctrl == false; i++){
		if(!strcmp(pieces[i],"-acc"))
			acc = true; //show accesstime
		else if(!strcmp(pieces[i],"-long"))
			lng = true; //long listing (verbose)
		else if(!strcmp(pieces[i],"-link"))
			lnk = true; //show path if it's a symlink
		else{
			ctrl = true; //exit for loop
		}
	}

	if(!ctrl)
		printCWD(); //no words in input were something other than attributes; this will print the cwd, as the reference shell does
	else{
		//individual file processing
		fileInfoHeader(acc,lng,lnk); //print header with columns
		for(--i; i < numP; i++){ //iterate through the words that are not attributes;
			fileInfo(pieces[i],pieces[i],acc,lng,lnk); //call for processing
		}
	}
}


void cCWD (char *pieces[], int numP){
	if (numP == 1)
		printCWD();
	else
		errorUnknownArgument("cwd");
}


void cListDir (char *pieces[], int numP){
	int i;
	bool hid = false, acc = false, lng = false, lnk = false, ctrl = false;

	//for attribute evaluation
	for(i = 1; i < numP && ctrl == false; i++){
		if(!strcmp(pieces[i],"-hid"))
			hid = true; //show accesstime
		else if(!strcmp(pieces[i],"-acc"))
			acc = true; //show accesstime
		else if(!strcmp(pieces[i],"-long"))
			lng = true; //long listing (verbose)
		else if(!strcmp(pieces[i],"-link"))
			lnk = true; //show path if it's a symlink
		else{
			ctrl = true; //exit for loop
		}
	}
	
	if(!ctrl){ //no directories listed, only attributes; will list "." in this case
		readDir(".",hid,acc,lng,lnk);
	}
	else{
		for(--i; i < numP; i++){ //iterate through the words that are not attributes
			readDir(pieces[i],hid,acc,lng,lnk); //call for processing
		}
	}
}


void cRecList (char *pieces[], int numP, bool after){
	int i;
	bool hid = false, acc = false, lng = false, lnk = false, ctrl = false;

	//for attribute evaluation
	for(i = 1; i < numP && ctrl == false; i++){
		if(!strcmp(pieces[i],"-hid"))
			hid = true; //show accesstime
		else if(!strcmp(pieces[i],"-acc"))
			acc = true; //show accesstime
		else if(!strcmp(pieces[i],"-long"))
			lng = true; //long listing (verbose)
		else if(!strcmp(pieces[i],"-link"))
			lnk = true; //show path if it's a symlink
		else{
			ctrl = true; //exit for loop
		}
	}
	if(!ctrl){ //no directories listed, only attributes; will list "." in this case
		readDirRec(".",hid,acc,lng,lnk,after);
	}
	else{
		for(--i; i < numP; i++){ //iterate through the words that are not attributes
			readDirRec(pieces[i],hid,acc,lng,lnk,after); //call for processing
		}
	}
}


void cErase (char *pieces[], int numP){
	int i;
	if (numP == 1)
		printCWD(); //prints current directory if called without the directory name argument

	else{
		for(i = 1; i < numP; i++){
			if (remove(pieces[i]) != -1){
				printf("Successfully deleted %s\n",pieces[i]);
			}
			else if(rmdir(pieces[i]) != -1){
				printf("Successfully deleted %s\n",pieces[i]);
			}
			else{
				printf("%s",pieces[i]);
				errorSyscall("erase");
			}
		}
	}
}


void cDelRec (char *pieces[], int numP){
	int i;
	if (numP == 1)
		printCWD(); //prints current directory if called without the directory name argument

	else
		for(i = 1; i < numP; i++)
			if(!delRec(pieces[i]))
				printf("Recursive deletion of %s failed",pieces[i]);
}

char* getTrashPath(void) { //AUX
	const char *suffix = "/.local/share/Trash/files";
	const char *home = getenv("HOME");
	char* path;
	size_t len;
	
	if (!home) {
		return NULL;	// HOME env variable not set
	}
	
	len = strlen(home) + strlen(suffix) + 1;

	path = malloc(len);
	if (!path) {
		return NULL;
	}

	snprintf(path, len, "%s%s", home, suffix);
	return path;
}

char* getTrashInfoPath(void) { //AUX
	const char *suffix = "/.local/share/Trash/info";
	const char *home = getenv("HOME");
	char *path;
	size_t len;
	
	if (!home) {
		return NULL;	// HOME env variable not set
	}
	
	len = strlen(home) + strlen(suffix) + 1;

	path = malloc(len);
	if (!path) {
		return NULL;
	}

	snprintf(path, len, "%s%s", home, suffix);
	return path;
}

void cTrash (char *pieces[], int numP){
	char* trashPath = getTrashPath();
	char* trashInfoPath = getTrashInfoPath();
	
	//path obtention error
	if (trashPath == NULL){
		errorTrashPath();
		if (trashInfoPath != NULL)
			free(trashInfoPath);
		return;
	}
	if (trashInfoPath == NULL){
		errorTrashPath();
		free(trashPath);
		return;
	}
	if (numP == 1){
		cListDir((char *[]){ "trash", trashPath}, 2);
	}
	else if (numP == 2 && (!strcmp(pieces[1],"-empty"))){
		delRec(trashPath);
		delRec(trashInfoPath);
	}
	
	
	free(trashPath);
	free(trashInfoPath);
	
	return;
}

void cFSStats (char *pieces[], int numP){
	FILE* mount;
	struct mntent* mountedFS; //changes every iteration to each new filesystem
	
	mount = setmntent("/proc/self/mounts","r");
	if(numP != 1){
		errorUnknownArgument("fsstats");
	}
	if(mount == NULL){
		printf("Mount file could not be opened\n");
		return;
	}
	while((mountedFS = getmntent(mount)) != NULL){ //getmntent reads mounts one at a time
		struct statvfs fsStats; //declared locally within the loop
		//CONDITIONS: Mount systems must be have over 8GB in total, with at least one byte of free space and not of type "tmpfs" (temporary filesystem)
		if(statvfs(mountedFS->mnt_dir,&fsStats) == 0){
			if((fsStats.f_blocks * fsStats.f_bsize > 8000000) && fsStats.f_bfree != 0 && (strcmp(mountedFS->mnt_fsname,"tmpfs"))){
				printf("Mount point: %s\n", mountedFS->mnt_dir);
				printf("Filesystem: %s\n", mountedFS->mnt_fsname);
				printf("\tTotal (B): %lu\n", fsStats.f_bsize*fsStats.f_blocks);
				printf("\tFree (B): %lu\n", fsStats.f_bsize*fsStats.f_bfree);
				printf("\tAvailable (B): %lu\n--\n", fsStats.f_bavail);
				printf("\n");
			}
		}
	}
}

void cCopy (char *pieces[], int numP){
	int fileDescSource = 0;
	int fileDescDest = 0;
	off_t remaining;
	off_t offset = 0;
	struct stat statSource;
	
	if (numP != 3){
		errorUnknownArgument("copy");
		return;
	}
	
	fileDescSource = open(pieces[1], O_APPEND);
	if (fileDescSource == -1){
		errorSyscall("copy");
		return;
	}
	fstat(fileDescSource,&statSource);
	
	fileDescDest = open(pieces[2], O_APPEND);
	if(fileDescDest != -1){
		close(fileDescSource);
		close(fileDescDest);
		errorFileAlreadyExists("copy",pieces[2]);
		return;
	}
	fileDescDest = open(pieces[2], O_WRONLY | O_CREAT | O_TRUNC, statSource.st_mode);
	if(fileDescDest == -1){
		errorSyscall("copy");
		close(fileDescSource);
		return;
	}
	//both files are open now
	
	
	remaining = statSource.st_size;
	
	while(remaining > 0){
		ssize_t sent = sendfile(fileDescDest, fileDescSource, &offset, remaining);
		if(sent < 0){
			printf("copy - Error: Unknown, copy failed");
		}
		remaining -= sent;
	}
	
	close(fileDescSource);
	close(fileDescDest);
	return;
}

	// * * * Errors

// * FileSys Errors

void errorFileAlreadyExists(char command[], char fileName[]){
	printf("\t%s - Error: Could not creat file \"%s\":\nIt is possible that the file already exists\n",command,fileName);
}

void errorFileRead(char relPath[]){
	printf("\tError %d: Could not read \"%s\":\n%s\n",errno,relPath,strerror(errno));
}

void errorTrashPath(){
	printf("\t Error: Trash folder path could not be obtained\n");
}
