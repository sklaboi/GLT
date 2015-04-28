CFLAGS = -Wall -ansi -ggdb

program: main.o flagi.o generator.o stat.o make.o
	$(CC) $(CFLAG) main.o flagi.o generator.o stat.o make.o

main.o: main.c flagi.h
# flagi.o: flagi.c flagi.h
generator.o: generator.c generator.h stat.h
stat.o: stat.c stat.h
make.o: make.c make.h
