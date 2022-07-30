#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>


void raise_error(const char* message);
void print_lines(char** lines);

char** parse_command(char* line, int* n_tokens);
void cleanup(char*** lines, size_t len);
void handler_HUP(int signal);
void run_foreground_job(char** arguments);
pid_t run_background_job(char** arguments);
void setup();
