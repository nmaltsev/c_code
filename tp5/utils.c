#include "utils.h"


void raise_error(const char* message){
	//printf("%s\n", message);
	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}

int parse_int(char* num) {
    if (num == NULL) return -1;

    int n = atoi(num);

    if (n == 0 && strcmp(num, "0") != 0) return -1;

    return n;
}

// 1.struct sockaddr_in - Structures for handling internet addresses
// 		https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
// 2. host == NULL -> ip addr = 0.0.0.0
struct sockaddr_in init_int_address(char* host, int port) {
	struct sockaddr_in address;
		
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	
	if (host == NULL) {
		address.sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		inet_pton(AF_INET, host, &address.sin_addr);
		//inet_aton("10.0.0.1", &antelope.sin_addr); // store IP in antelope
	}
	
	return address;
}


unsigned char* readFromSocket(int socket) {
    size_t bufLen = BUF_SIZE * sizeof(unsigned char);
    size_t nread;
    unsigned char* buf = malloc(bufLen);
    unsigned char* out = malloc(bufLen);
    // Read data into the buffer in chunks
    
    do {
		nread = read(socket, buf, BUF_SIZE);
		//// printf("READ %d `%s`\n", nread, buf);
		out = realloc(out, nread + BUF_SIZE);
		memcpy(out, buf, nread);
		// a small trick to clear the buffer
		memset(buf, 0, bufLen);
    } while (nread == BUF_SIZE);
    
    free(buf);
    buf = NULL;
    return out;
}

unsigned char* sendRequest(int serverSocket, unsigned char* requestBody, int mesLen, int* status){
    if (write(serverSocket, requestBody, mesLen) != mesLen) {
		*status = EXIT_FAILURE; 
		return NULL;
    } 
    else {
		unsigned char* responseBody = readFromSocket(serverSocket);
		*status = EXIT_SUCCESS;
		return responseBody;
    }
}

int sendReply(int clientSocket, unsigned char* answer, int answerLen) {
	return write(clientSocket, answer, answerLen) == answerLen ? EXIT_SUCCESS : EXIT_FAILURE; 
}

int rands(){
    int randomValue;
    FILE *fpointer;
    fpointer = fopen("/dev/urandom", "rb");
    fread(&randomValue, sizeof(int), 1, fpointer);  
    fclose(fpointer);
    if (randomValue < 0) randomValue = -randomValue;
    return randomValue;
}

// get a random number in the range limited by the minimum and maximum
int random_num(int min, int max) {
	if (min == max) return min;
	int r = rands();
	//// srand(time(NULL)); int r = rand();
	//// `rand() % 256` returns an integer number in range from 0 to 255
    return  min + (r % (max - min));
}

// Message sent by the server to clients
unsigned char* init_response(unsigned char min, unsigned char max) {
	unsigned char* body = malloc(2 * sizeof(unsigned char));
	body[0] = min;
	body[1] = max;
	return body;
}

// Returns a message that can be sent by the client to start the game
unsigned char* init_request() {
	unsigned char* body = malloc(1 * sizeof(unsigned char));
	body[0] = 'i'; // means init
	return body;
}

// Message sent by a client to the server
unsigned char* proposal_request(unsigned char clientNumber) {
	unsigned char* body = malloc(2 * sizeof(unsigned char));
	body[0] = 'p'; // means Proposal
	body[1] = clientNumber;
	return body;
}

unsigned char* proposal_response(unsigned char clientNumber, unsigned char serverNumber) {
	unsigned char* body = malloc(2 * sizeof(unsigned char));
	// Command id: w = win, h = high, l = low
	body[0] = clientNumber == serverNumber ? 'w' : (clientNumber > serverNumber ? 'h' : 'l');
	// A proposal number
	body[1] = clientNumber;
	return body;
}
