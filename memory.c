//Daniel Arias Fachal | d.fachal@udc.es / daniel02af@outlook.com

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <linux/limits.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h> //memdump
#include <sys/wait.h> //memory
#include "base.h"
#include "memory.h"





	// * * * Implementation

// * Memory List

	// * Auxiliaries for memList implementation:
bool createNodeML(pML *p);
bool copyModeML(pML *q,char mode[]);
pML findPML(void *d, ML *list);


void createML(ML* list){
	return;
}


pML findItemML(void* memAddress, ML *list){
	pML p;
	if(*list==NULL)
		return NULL;
	for(p = *list;p->next != NULL; p = p->next){
		if(p->data == memAddress){
			return p;
		}
	}
	return NULL;
}


bool insertNodeML(ML * list, void* memAddress, int size, struct tm time, char mode[]){
	pML p,q;
	if(!createNodeML(&q))
		return false;
	if(!copyModeML(&q,mode)){
		free(q);
		return false;
	}
	//will create node q to insert
	q->data = memAddress;
	q->size = size;
	q->time = time;
	q->next = NULL;
	
	if((*list) == NULL){
		*list = q;
		printf("Inserted node: ");
		printNodeML(q);
		return true;
	}
	p = findPML(memAddress,list);
	if(p==NULL){ //insert at first position
		q->next = *list;
		*list = q;
		printf("Inserted node: ");
		printNodeML(q);
		return true;
	}
	q->next = p->next; //if it's not the last node, maintain the list after the insertion

	p->next = q;
	printf("Inserted node: ");
	printNodeML(q);
	return true;
}

void freeNode(pML p){
	free(p->data);
	free(p->mode);
	free(p);
}


bool deleteNodeML(void* memAddress, ML *list){
	pML p,q;
	q = *list;
	if(q==NULL)
		return false;
	if(q->data == memAddress){
		printf("Deleting: ");
		printNodeML(q);
		*list = q->next;
		freeNode(q);
	}
	else{
		while(q ->next != NULL && q->data < memAddress){
			p = q;
			q = q->next;
		}
		if(q->data == memAddress){
			printf("Deleting: ");
			printNodeML(q);
			p->next = q->next;
			freeNode(q);
		}
		else
			return false;
	}
	return true;
}

bool deleteBySizeML(int targetSize, ML *list){
	pML p,q;
	q = *list;
	if(q==NULL)
		return false;
	if(q->size == targetSize){
		printf("Deleting: ");
		printNodeML(q);
		*list = q->next;
		freeNode(q);
		return true;
	}
	else{
		while(q -> next != NULL){
			p = q;
			q = q->next;
			if(q->size == targetSize){
				printf("Deleting: ");
				printNodeML(q);
				p->next = q->next;
				freeNode(q);
				return true;
			}
		}
	}
	return false;
}


bool createNodeML(pML *p){ // Auxiliary for InsertNodeML
	*p = malloc(sizeof(struct nodeML));
	return (*p!=NULL);
}


//Can use this as model for mallocating fields of the struct
bool copyModeML(pML *q,char mode[]){ // Auxiliary for InsertNodeML
	(*q)->mode = malloc(sizeof(char[strlen(mode)]));
	if((*q)->mode == NULL)
		return false;
	strcpy((*q)->mode,mode);
	return true;
}


pML findPML(void *d, ML *list){ // Auxiliary for InsertNodeML
	//find position after which d should be inserted to maintain the order (NULL if it's before the first)
	//note: the list will be ordered by address, as it is the only unique identifier, and I do believe there's some value in keeping it ordered
	pML p = NULL;
	
	if(d < (*list)->data)
		return p;
	p = *list;
	while(p->next != NULL && d > (p->next->data))
		p = p->next;
		
	//if d is the biggest, it'll return the last position in the list
	return p;
}

void printNodeML(pML p){
	printf("A: %p | T: ",p->data);
	printTimeOnlyTM(p->time);
	printf("| S: %d | %s\n",p->size,p->mode);
}

void printML(ML *list){
	pML p = NULL;
	pid_t pid = getpid();
	printf("List of blocks assigned for the process %d:\n",pid);
	if(*list != NULL){
		for(p = *list;p != NULL; p = p -> next){
			printf("\t");
			printNodeML(p);
		}
	}
}

void emptyML(ML *list){
	pML p,q;
	p = *list;
	if(p!= NULL){
		q = p->next;Initialized
		free(p->mode);
		free(p->data);
		free(p);
		emptyML(&q);
	}
}



// * Memory

	// * Auxiliaries
bool getAddress(char str[], long long unsigned *addressLLU){
	*addressLLU = strtoull(str, NULL, 16);
	if(errno != 0){
		errorAddressNotValid("address conversion",str);
		return false;
	}
	return true;
}

	// * Commands
void cAllocate(char *pieces[], int numP, ML *memList){
	void *address = 0;
	int size = 0;
	//time is calculated before processing the command; won't really make a difference when we're working with such low execution times	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
		
	if(numP == 3){
		if (!strcmp(pieces[1],"-malloc")){
			size = atoi(pieces[2]);
			if(size <= 0 && errno!=0){
				errorSizeNotValid("allocate",pieces[2]);
				return;
			}
			

			address = malloc(size);
			if (address == NULL || address == 0){
				errorMallocFailed("allocate");
				return;
			}
			//address = (long)(address);//how tf do i do this
			insertNodeML(memList,address,size,tm,"malloc");
			return;
		}
		else if (!strcmp(pieces[1],"-mmap")){
			return;
		}
		else if (!strcmp(pieces[1],"-createshared")){
			return;
		}
		else if (!strcmp(pieces[1],"-shared")){
			return;
		}
	}
	if(numP==1)
		printML(memList);
	else
		errorUnknownArgument("allocate");
}

void cDeallocate(char *pieces[], int numP, ML *memList){ //return on success || error
	long long unsigned addressLLU;
	void* address;
	int size;
	if (numP == 3){
		if(!strcmp(pieces[1],"-malloc")){
			size = atoi(pieces[2]);
			if(size <= 0 && errno!=0){
				errorSizeNotValid("allocate",pieces[2]);
				return;
			}
			if(!deleteBySizeML(size,memList)){
				printf("\tdeallocate - Error: Could not find an entry with specified size");
				return;
			}
			else
				return; //successful
		}
	}
	if (numP == 2){
			addressLLU = strtoull(pieces[1], NULL, 16);
			address = (void*) addressLLU;
			if(errno == 0){
				if(deleteNodeML(address,memList))
					return;
				else{
					errorAddressNotFoundML("deallocate",pieces[1]);
					return;
				}
			}
		}
	if(numP==1){
		printML(memList);
		return;
	}
	errorUnknownArgument("deallocate");
}

void cMemFill(char *pieces[], int numP, ML *memList){
	int validBlock;
	pML current;
	char *endPtr;
	void* blockStart;
	void* blockEnd;
	if (numP == 1){ //TODO: Fix placement of error out of var declaration
		errorUnknownArgument("memfill");
		return;
	}
	long unsigned address = strtoul(pieces[1], &endPtr, 16); // Dirección de memoria en hexadecimal
	int cont = atoi(pieces[2]);  // Cantidad de bytes
	char ch = pieces[3][0]; // Caracter a intruducir


	if (numP < 4 || numP > 4) {
		errorUnknownArgument("memfill");
		return;
	}

	if (*endPtr != '\0') { // Comprobar que la dirección de memoria es válida
		errorAddressNotValid("memfill",pieces[1]);
		return;
	}

	if (cont <= 0) { // Comprobar el número de bytes
		errorSizeNotValid("memfill",pieces[2]);
		return;
	}

	// Validar si la dirección pertenece a un bloque de memoria en memList
	current = *memList;
	validBlock = 0;  // Flag para comprobar si encontramos el bloque
	while (current) {
		blockStart = current->data;
		blockEnd = blockStart + current->size;
		if ((void*) address >= blockStart && (void*) address + cont <= blockEnd) {
			validBlock = 1;  // La dirección es válida
			break;
		}
		current = current->next;
	}

	if (!validBlock) {
		fprintf(stderr, "Address %lx does not belong to any allocated block.\n", address);
		return;
	}

	// Llenar la memoria
	for (int i = 0; i < cont; i++) {
		*((char *)address + i) = ch;
	}

	return;
}


void cMemDump(char *pieces[], int numP, ML *memList){

	char *endPtr;
	unsigned char *memory;
	long unsigned address = strtoul(pieces[1], &endPtr, 16);
	int cont = atoi(pieces[2]);

	if (numP != 3) {
		errorUnknownArgument("memdump");
		return;
	}

	if (*endPtr != '\0') { // Comprobar que la dirección de memoria es válida
		errorAddressNotValid("memdump",pieces[1]);
		return;
	}

	if (cont <= 0) { // Comprobar el número de bytes
		errorSizeNotValid("memdump",pieces[2]);
		return;
	}

	memory = (unsigned char*) address;

	for (int i = 0; i < cont; i++) {
		if(isprint(memory[i]))
			printf("%2c", memory[i]);
		else
			printf ("%02x", memory[i]);
		if(i % 25 == 24)
			printf("\n");
		else
			printf(" ");
	}

	printf ("\n");

	return;
}

int global1, global2, global3;
int global1i = 0, global2i = 0, global3i = 0;

void printVars(int *ext1, int *ext1i, int *ext2, int *ext2i, int *ext3, int *ext3i){
	int x,y,z;
	int xi = 0, yi = 0, zi = 0;
	static int a,b,c;
	static int ai = 0, bi = 0, ci = 0;
	auto int d,e,f;
	printf("Local variables:\t%p, %p, %p\n", &x, &y, &z);
	printf("Initialized local variables:\t%p, %p, %p\n", &xi, &yi, &zi);
	printf("External variables:\t%p, %p, %p\n", ext1, ext2, ext3);
	printf("Initialized external variables:\t%p, %p, %p\n", ext1i, ext2i, ext3i);
	printf("Static variables:\t%p, %p, %p\n", &a, &b, &c);
	printf("Initialized static variables:\t%p, %p, %p\n", &ai, &bi, &ci);
	printf("Automatic variables:\t%p, %p, %p\n", &d, &e, &f);
	printf("Global variables:\t%p, %p, %p\n", &global1, &global2, &global3);
	printf("Initialized global variables:\t%p, %p, %p\n", &global1i, &global2i, &global3i);
	printf("\n");
}

void cMemory(char *pieces[], int numP, ML *memList){
	bool funcs = false, vars = false, blocks = false;
	int ext1, ext2, ext3;
	int ext1i, ext2i, ext3i;
		
	if (numP == 2){
		if(!strcmp(pieces[1], "-funcs"))
			funcs = true;
		else if(!strcmp(pieces[1], "-vars"))
			vars = true;
		else if(!strcmp(pieces[1], "-blocks"))
			blocks = true;
		else if(!strcmp(pieces[1], "-all")){
			funcs= true;
			vars = true;
			blocks = true;
		}
		else if (!strcmp(pieces[1], "-pmap")){
			do_pmap();
			return;
		}
		if (funcs){
			printf("Program functions:\t%p, %p, %p\n", cAuthors, cPid, cInfoSys);
			printf("Library functions:\t%p, %p, %p\n", malloc, printf, strcmp);
			printf("\n");
		}

		if (vars){
			printVars(&ext1,&ext1i,&ext2,&ext2i,&ext3,&ext3i);
		}

		if (blocks)
			printML(memList);
		
		return;
	}
	errorUnknownArgument("memory");
}

void cOpenFile(char *pieces[], int numP, ML *memList){
	return;
}

void cCloseFile(char *pieces[], int numP, ML *memList){
	return;
}


void cRecurse(char *pieces[], int numP){
	int n = 0;
	if(numP == 2){
		errno=0;
		n = atoi(pieces[1]);
		if(n == 0 && errno!=0)
			errorAtoiFailed("recursive",pieces[1]);
		else
			recursiva(n);
	}
	else
		errorUnknownArgument("recursive");
}


	// * PROVIDED CODE START
void recursiva (int n){
	char automatico[MEM_LIST_SIZE];
	static char estatico[MEM_LIST_SIZE];

	printf("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico,estatico);

	if (n>0)
		recursiva(n-1);
}


void memFill (void *p, size_t cont, unsigned char byte){
	unsigned char *arr=(unsigned char *) p;
	size_t i;

	for (i=0; i<cont;i++)
		arr[i]=byte;
}

/*
void * ObtenerMemoriaShmget (key_t clave, size_t tam){
	void * p;
	int aux,id,flags=0777;
	struct shmid_ds s;

	if (tam)	 //tam distito de 0 indica crear 
		flags=flags | IPC_CREAT | IPC_EXCL; //cuando no es crear pasamos de tamano 0
	if (clave==IPC_PRIVATE) //no nos vale
		{errno=EINVAL; return NULL;}
	if ((id=shmget(clave, tam, flags))==-1)
		return (NULL);
	if ((p=shmat(id,NULL,0))==(void*) -1){
		aux=errno;
		if (tam)
			 shmctl(id,IPC_RMID,NULL);
		errno=aux;
		return (NULL);
	}
	shmctl (id,IPC_STAT,&s); // si no es crear, necesitamos el tamano, que es s.shm_segsz
 // Guardar en la lista	InsertarNodoShared (&L, p, s.shm_segsz, clave); 
	return (p);
}


void do_AllocateCreateshared (char *tr[]){
	key_t cl;
	size_t tam;
	void *p;

	if (tr[0]==NULL || tr[1]==NULL) {
		ImprimirListaShared(&L);
		return;
	}
	
	cl=(key_t) strtoul(tr[0],NULL,10);
	tam=(size_t) strtoul(tr[1],NULL,10);
	if (tam==0) {
	printf ("No se asignan bloques de 0 bytes\n");
	return;
	}
	if ((p=ObtenerMemoriaShmget(cl,tam))!=NULL)
		printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
	else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}


void do_AllocateShared (char *tr[]){
	key_t cl;
	size_t tam;
	void *p;

	if (tr[0]==NULL) {
		ImprimirListaShared(&L);
		return;
	}
	
	cl=(key_t) strtoul(tr[0],NULL,10);

	if ((p=ObtenerMemoriaShmget(cl,0))!=NULL)
		printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
	else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}


void * MapearFichero (char * fichero, int protection){
	int df, map=MAP_PRIVATE,modo=O_RDONLY;
	struct stat s;
	void *p;

	if (protection&PROT_WRITE)
		modo=O_RDWR;
	if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
		return NULL;
	if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
		return NULL;
// Guardar en la lista	InsertarNodoMmap (&L,p, s.st_size,df,fichero); 
// Gurdas en la lista de descriptores usados df, fichero
	return p;
}


void do_AllocateMmap(char *arg[]){ 
	 char *perm;
	 void *p;
	 int protection=0;
	 
	 if (arg[0]==NULL)
			{ImprimirListaMmap(&L); return;}
	 if ((perm=arg[1])!=NULL && strlen(perm)<4) {
			if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
			if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
			if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
	 }
	 if ((p=MapearFichero(arg[0],protection))==NULL)
			 perror ("Imposible mapear fichero");
	 else
			 printf ("fichero %s mapeado en %p\n", arg[0], p);
}


void do_DeallocateDelkey (char *args[]){
	key_t clave;
	int id;
	char *key=args[0];

	if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
		printf ("      delkey necesita clave_valida\n");
		return;
	}
	if ((id=shmget(clave,0,0666))==-1){
		perror ("shmget: imposible obtener memoria compartida");
		return;
	}
	if (shmctl(id,IPC_RMID,NULL)==-1)
		perror ("shmctl: imposible eliminar memoria compartida\n");
}


ssize_t LeerFichero (char *f, void *p, size_t cont){
	struct stat s;
	ssize_t n;
	int df,aux;

	if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;	 
	if (cont==-1)	// si pasamos -1 como bytes a leer lo leemos entero
	cont=s.st_size;
	if ((n=read(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
	}
	close (df);
	return n;
}


void Cmd_ReadFile (char *ar[]){
	void *p;
	size_t cont=-1; //si no pasamos tamano se lee entero 
	ssize_t n;
	if (ar[0]==NULL || ar[1]==NULL){
	printf ("faltan parametros\n");
	return;
	}
	p=cadtop(ar[1]); //convertimos de cadena a puntero
	if (ar[2]!=NULL)
	cont=(size_t) atoll(ar[2]);

	if ((n=LeerFichero(ar[0],p,cont))==-1)
	perror ("Imposible leer fichero");
	else
	printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

*/
void do_pmap (void){ //sin argumentos
	pid_t pid;		//hace el pmap (o equivalente) del proceso actual
	char elpid[32];
	char *argv[4]={"pmap",elpid,NULL};
	
	sprintf (elpid,"%d", (int) getpid());
	if ((pid=fork())==-1){
		perror ("Couldn't fork process");
		return;
		}
	if (pid==0){
		if (execvp(argv[0],argv)==-1)
		 perror("cannot execute pmap (linux, solaris)");
		 
		argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;	
		if (execvp(argv[0],argv)==-1)//No hay pmap, probamos procstat FreeBSD 
		 perror("cannot execute procstat (FreeBSD)");
		 
		argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;	
			if (execvp(argv[0],argv)==-1) //probamos procmap OpenBSD
		 perror("cannot execute procmap (OpenBSD)");
		 
		argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
		if (execvp(argv[0],argv)==-1) //probamos vmmap Mac-OS
		 perror("cannot execute vmmap (Mac-OS)");
		exit(1);
	}
	waitpid (pid,NULL,0);
}

	// PROVIDED CODE FINISH
	

	// * * * Errors

// * Base errors

void errorAddressNotValid(char command[], char argument[]){
	printf("\t%s - Error: %s is not a valid address",command,argument);
}

void errorSizeNotValid(char command[], char argument[]){
	printf("\t%s - Error: %s is not a valid size",command,argument);
}


void errorAddressNotFoundML(char command[], char argument[]){
	printf("\t%s - Error: Address %s is not on the list",command,argument);
}
