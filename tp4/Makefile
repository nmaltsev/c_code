arhive_name := x.TP4.zip

CC = gcc
CFLAGS = -g -Wall
EXECUTABLE = lockcmd
OBJECTS = main.o utils.o


all: $(EXECUTABLE)

$(EXECUTABLE): main.o utils.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main.o utils.o

main.o: main.c main.h utils.h
	$(CC) $(CFLAGS) -c -o main.o main.c


utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o utils.o utils.c

clean:
	rm -rf *o $(EXECUTABLE)

zip:
	zip -r $(arhive_name) . -x $(EXECUTABLE) -x *.o -x *.swp -x *.zip -x *.git/* -x *.vscode/*

rebuild:
	make clean && clear && make
