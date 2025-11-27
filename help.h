//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#ifndef HELPCOM_H
#define HELPCOM_H


	// * * * Headers

// * Main
void cHelp(char *pieces[], int numP);
void printHelp();


// * * Help Commands

// * Base Help
void helpAuthors();
void helpPid();
void helpPPid();
void helpCd();
void helpDate();
void helpHistoric();
void helpOpen();
void helpClose();
void helpDup();
void helpInfoSys();
void helpHelp();
void helpQuit();
void helpExit();
void helpBye();

// * Filesys Help
void helpMakeFile();
void helpMakeDir();
void helpListFile();
void helpCWD();
void helpListDir();
void helpRecList();
void helpRevList();
void helpErase();
void helpDelRec();

// * Memory Help
void helpAllocate();
void helpDeallocate();
void helpMemFill();
void helpMemDump();
void helpMemory();
void helpRecurse();

#endif
