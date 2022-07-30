#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define MAX_PENDING 256
#define BUF_SIZE 4

unsigned char* init_response(unsigned char min, unsigned char max);
unsigned char* init_request();
unsigned char * proposal_request(unsigned char clientNumber);
unsigned char* proposal_response(unsigned char clientNumber, unsigned char serverNumber);

int random_num(int min, int max);
int rands();

void raise_error(const char* message);
int parse_int(char* num);

struct sockaddr_in init_int_address(char* host, int port);
unsigned char* readFromSocket(int socket);
unsigned char* sendRequest(int serverSocket, unsigned char* requestBody, int mesLen, int* status);
int sendReply(int clientSocket, unsigned char* answer, int answerLen);
