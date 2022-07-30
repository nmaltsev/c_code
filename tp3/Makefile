#output_name := ultra-cp
arhive_name := x.TP3.zip

CC = gcc
CFLAGS = -g -Wall
EXECUTABLE = ultra-cp
OBJECTS = main.o utils.o

##$(EXECUTABLE):	$(OBJECTS)
##	$(CC) $(OBJECTS) -o $(EXECUTABLE)
##%.o: %.c:
##	$(CC) $(CFLAGS) $< -o $@

all: $(EXECUTABLE)

$(EXECUTABLE): main.o options.o utils.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main.o options.o utils.o

main.o: main.c main.h utils.h
	$(CC) $(CFLAGS) -c -o main.o main.c

options.o: options.c options.h
	$(CC) $(CFLAGS) -c -o options.o options.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o utils.o utils.c

clean:
	rm -rf *o $(EXECUTABLE)

zip:
	zip -r $(arhive_name) . -x $(EXECUTABLE) -x *.o -x *.swp -x *.zip -x *.git/* -x *.vscode/*
