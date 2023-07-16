#CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS =  myGit main

.PHONY:	all clean

all: $(PROGRAMS)

myGit: myGit.o bib_liste.o -lm
	$(CC) -o $@ $(CFLAGS) $^

main: main.o bib_liste.o -lm
	$(CC) -o $@ $(CFLAGS) $^

myGit.o: myGit.c
	gcc $(CFLAGS) -c  myGit.c

main.o: main.c
	gcc $(CFLAGS) -c  main.c

bib_liste.o: bib_liste.c
	gcc $(CFLAGS) -c bib_liste.c

#Ou plus simplement
#%.o:%.c %.h
#	$(CC)  $(GCC_FLAGS) -c  $<

clean:
	rm -f *.o *~ $(PROGRAMS)