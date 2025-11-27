so: fullSO
	rm base.o help.o fileSys.o memory.o

fullSO: so.c base.o help.o fileSys.o memory.o
	gcc -Wall -lm base.o help.o fileSys.o memory.o so.c -o so

sodebug: so.c base.o help.o fileSys.o memory.o
	gcc -Wall -lm base.o help.o fileSys.o memory.o so.c -o so -g -O0
	rm base.o help.o fileSys.o memory.o

base.o: base.h base.c
	gcc -Wall -c base.c -o base.o
	
help.o: help.h help.c
	gcc -Wall -c help.c -o help.o

fileSys.o: fileSys.h fileSys.c base.o
	gcc -Wall -c fileSys.c -o fileSys.o

memory.o: memory.h memory.c base.o
	gcc -Wall -c memory.c -o memory.o

clean:
	rm base.o help.o fileSys.o memory.o
