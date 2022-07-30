#include "client.h"
#include "utils.h"

void play_game(struct sockaddr_in address) {
    unsigned char* requestBody;
    unsigned char* responseBody;

    // The expected size of messages is 2 bytes
    // (Client) Init request = "i", // client initiates the game
    // (Server) Init response = "<min><max>" where min and max := [0, 255]
    // (Client) Proposal request = "p<num>" where num := [0, 255]
    // (Server) Proposal response = "<status><num>" where <status>:={w "Win", f "Fail", h "High", l "Low"} 
    requestBody = malloc(2 * sizeof(unsigned char));
    responseBody = malloc(2 * sizeof(unsigned char));
    //// AF_INET = Address Format, Internet = IP Addresses
    //// PF_INET = Packet Format, Internet = IP, TCP/IP or UDP/IP
    //// Meaning, AF_INET refers to addresses from the internet, IP addresses specifically. PF_INET refers to anything in the protocol, usually sockets/ports.
    //// https://stackoverflow.com/questions/6729366/what-is-the-difference-between-af-inet-and-pf-inet-in-socket-programming
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
	raise_error("Failed to create socket");
    }
    
    if (
	connect(serverSocket, (struct sockaddr *) &address, sizeof(address)) < 0
    ) {
	raise_error("Failed to connect with server");
    }
    enum GameStatus gameStatus = NOT_STARTED;
    // Unsigned char is used to store integers from 0 to 255
    // In some environments regular chars can only store numbers from -127 to 128 only. 
    unsigned char min, max, number;
    int requestStatus;
    unsigned char clientNumber;

    do {
	// To start a new game the client sends "i"   
	if (gameStatus == NOT_STARTED) {
	    // Request min and max bounds of the range
	    requestBody = init_request();
	    // The client expects in a response bounds of the ramge min and max values
	    responseBody = sendRequest(serverSocket, requestBody, 2, &requestStatus);
	    min = responseBody[0];
	    max = responseBody[1];
	    printf("[Game started] min: %d max %d\n", min, max);
	    gameStatus = STARTED;
	} 
	else if (gameStatus == STARTED) {
	    number = random_num(min, max);
	    printf("Proposition envoyée: %d\n", number);
	    requestBody = proposal_request(number);
	    responseBody = sendRequest(serverSocket, requestBody, 2, &requestStatus);
	    clientNumber = responseBody[1];
	    
	    printf("La valeur réelle est: %d %c\n", clientNumber, responseBody[0]);
    
	    if (responseBody[0] == 'w') {
		gameStatus = GAME_WIN;
		printf("[Game win] number %d\n", clientNumber);
	    }
	    else if (responseBody[0] == 'f') {
		gameStatus = GAME_LOSE;
		printf("[Game lose]\n");
	    }
	    else if (responseBody[0] == 'h') {
		max = clientNumber;
	    }
	    else if (responseBody[0] == 'l') {
		min = clientNumber;
	    }
	    else {
		printf("something wrong with the server\n");
		continue;
	    }
	}
	memset(requestBody, 0, 2);
	memset(responseBody, 0, 2);
    } while ((gameStatus != GAME_WIN && gameStatus != GAME_LOSE));
    close(serverSocket);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
	fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
	raise_error("Not enough arguments");
    }

    char* host = argv[1];
    int port_number = parse_int(argv[2]);
    
    printf("HOST %s PORT %d\n", host, port_number);
    
    struct sockaddr_in address = init_int_address(host, port_number);
    
    char* ip_addr = inet_ntoa(address.sin_addr);
    printf("%zu %zu ip:%s\n", address.sin_port, address.sin_addr.s_addr, ip_addr);
    
    play_game(address);
    return EXIT_SUCCESS;
}
