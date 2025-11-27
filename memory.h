//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#ifndef MEM_H
#define MEM_H


	// * * * Defines
#define MEM_LIST_SIZE 2048


	// * * * Typedefs
	
// * FileList
typedef struct nodeML* pML;
struct nodeML{
	void* data; //POINTER
	int size; //in bytes
	struct tm time;
	char* mode;
	pML next;
};
typedef pML ML;



	// * * * Headers

// * List implementation
void createML(ML* list);
pML findItemML(void* memAddress, ML *list);
bool insertNodeML(ML * list, void* memAddress, int size, struct tm time, char mode[]);
void freeNode(pML p);
bool deleteNodeML(void* memAddress, ML *list);
void printNodeML(pML p);
void printML(ML *list);
void emptyML(ML *list);

// * Commands
void cAllocate(char *pieces[], int numP, ML *memList);
void cDeallocate(char *pieces[], int numP, ML *memList);
void cMemFill(char *pieces[], int numP, ML *memList);
void cMemDump(char *pieces[], int numP, ML *memList);
void cMemory(char *pieces[], int numP, ML *memList);

void cRecurse(char *pieces[], int numP);

// * Provided code
void recursiva (int n);
void memFill (void *p, size_t cont, unsigned char byte);
void * ObtenerMemoriaShmget (key_t clave, size_t tam);
void do_AllocateCreateshared (char *tr[]);
void do_AllocateShared (char *tr[]);
void * MapearFichero (char * fichero, int protection);
void do_AllocateMmap(char *arg[]);
void do_DeallocateDelkey (char *args[]);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void Cmd_ReadFile (char *ar[]);
void do_pmap (void);

// * Errors
void errorAddressNotValid(char command[], char argument[]);
void errorSizeNotValid(char command[], char argument[]);
void errorAddressNotFoundML(char command[], char argument[]);

#endif
