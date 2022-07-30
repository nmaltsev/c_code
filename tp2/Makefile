output_name := hash
arhive_name := x.TP2.zip

all: main.c
	gcc main.c utils.c options.c -g -Wall -o $(output_name) -lssl -lcrypto

zip:
	zip -r $(arhive_name) . -x hash -x *.git/* -x *.vscode/*
