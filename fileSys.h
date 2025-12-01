//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#ifndef FILESYS_H
#define FILESYS_H


	// * * * Headers


// * FileSys
void cMakeFile (char *pieces[], int numP);
void cMakeDir (char *pieces[], int numP);
void cListFile (char *pieces[], int numP);
void cCWD (char *pieces[], int numP);
void cListDir (char *pieces[], int numP);
void cRecList (char *pieces[], int numP, bool after);
void cErase (char *pieces[], int numP);
void cDelRec (char *pieces[], int numP);
char *getTrashPath(void); //AUX
char *getTrashInfoPath(void); //AUX
void cTrash (char *pieces[], int numP);
void cFSStats (char *pieces[], int numP);



	// * * * Errors

// * FileSys Errors
void errorFileAlreadyExists(char command[], char fileName[]);
void errorFileRead(char relPath[]);
void errorTrashPath();


#endif
