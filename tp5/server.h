#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define HOST "127.0.0.1"
#define PORT 8000

static const int MAX_N_ATTEMPTS = 7;
