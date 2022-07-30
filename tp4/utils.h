#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>


void raise_error(const char* message);
char** parse_line(char* line, int* nlines);
void print_lines(char** lines);
int parse_int(char* num);

typedef struct op_state {
	struct flock init_flock;
	struct flock final_flock;
	int status;
	int error;
	int executed_cmd;
} OperationState;

OperationState set_ctrl(int fd, int cmd, struct flock init);
void print_operation_state(OperationState op_state);
