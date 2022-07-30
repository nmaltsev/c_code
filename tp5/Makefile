arhive_name := TP5.zip

CC = gcc
CFLAGS = -g -Wall
EXECUTABLE_server = server
EXECUTABLE_client = client
EXECUTABLE_test = test



all: $(EXECUTABLE_client) $(EXECUTABLE_server)

$(EXECUTABLE_server): server.o utils.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE_server) server.o utils.o

server.o: server.c server.h utils.h
	$(CC) $(CFLAGS) -c -o server.o server.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c -o utils.o utils.c

$(EXECUTABLE_client): client.o utils.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE_client) client.o utils.o

client.o: client.c client.h utils.h
	$(CC) $(CFLAGS) -c -o client.o client.c

test.o: test.c utils.h
	$(CC) $(CFLAGS) -c -o test.o test.c

$(EXECUTABLE_test): test.o utils.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE_test) test.o utils.o


clean:
	rm -rf *o $(EXECUTABLE_server)
	rm -rf *o $(EXECUTABLE_client)
	rm -rf *o $(EXECUTABLE_test)

zip:
	zip -r $(arhive_name) . -x $(EXECUTABLE_client) -x $(EXECUTABLE_server) -x $(EXECUTABLE_test) -x *.o -x *.swp -x *.zip -x *.git/* -x *.vscode/*
	
rebuild:
	make clean && clear && make
