#include "server.h"
#include "utils.h"

void run(int serverSock) {
    while(1) {
	struct sockaddr_in clientAddress;
	unsigned int clientLength = sizeof(clientAddress);
	int clientSocket = accept(
	    serverSock, 
	    (struct sockaddr *) &clientAddress, 
	    &clientLength
	);
	
	if (clientSocket < 0) {
	    printf("Failed to accept client connection\n");
	}
	printf("Client %d connecté avec l’ip %s\n", clientSocket, inet_ntoa(clientAddress.sin_addr));
	
	unsigned char number, min, max;
	// The counter of attempts (MAX_N_ATTEMPTS = 5)
	int counter = 0;
	// The server closes connections when the client finds the correct number or loses the game
	int continue_game = 0;
	
	do {
	    unsigned char* requestBody = readFromSocket(clientSocket);
	
	    if (requestBody[0] == 'i') {
		min = random_num(0, 127);
		max = random_num(128, 255);
		number = random_num(min, max);
		counter = 0;
		unsigned char* responseBody = init_response(min, max);
		printf("La valeur %d est choisie pour le client %d (min %d, max %d)\n", number, clientSocket, min, max);
		sendReply(clientSocket, responseBody, 2);
		free(responseBody);
	    }
	    else if (requestBody[0] == 'p') {
		unsigned char clientNumber = requestBody[1];
		counter++;
		printf("#%d Client %d propose %d\n", counter, clientSocket, clientNumber);
		
		unsigned char* responseBody = proposal_response(clientNumber, number);
		// the game status changes to LOSE as soon as the counter reaches the maximum number of attempts
		if (responseBody[0] != 'w' && counter == MAX_N_ATTEMPTS) {
		    responseBody[0] = 'f';
		}
		
		if (responseBody[0] == 'f' || responseBody[0] == 'w') {
		    // Stop the game and close connection
		    continue_game = 1;
		}
		sendReply(clientSocket, responseBody, 2);
		free(responseBody);
	    }
	    free(requestBody);
	} while(continue_game == 0);
	close(clientSocket);
    }
}


int main(int argc, char *argv[]) {
	if (argc != 2) {
	    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	    raise_error("Not enough arguments");
	}

	int port_number = parse_int(argv[1]);
	printf("PORT %d\n", port_number);
	
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
	    raise_error("Failed to create socket");
	}
	
	// the server will listen for connections on IP address 0.0.0.0
	struct sockaddr_in internet_address = init_int_address(NULL, port_number);
	//struct sockaddr_in internet_address = init_int_address("127.0.0.1", port_number);
	char* ip_addr = inet_ntoa(internet_address.sin_addr);
	printf("%zu %zu ip:%s\n", internet_address.sin_port, internet_address.sin_addr.s_addr, ip_addr);
	
	if (
	    bind(sock, (struct sockaddr*)&internet_address, sizeof(internet_address)) < 0
	) {
	    raise_error("Failed to bind the server socket. Please choose another port number");
	}
	if (listen(sock, MAX_PENDING) < 0) {
	    raise_error("Failed to listen on server socket");
	}
	
	printf("START\n");
	run(sock);

	return EXIT_SUCCESS;
}
