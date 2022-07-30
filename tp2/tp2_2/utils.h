#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>


unsigned char* hash_file(char *filename, char* type, unsigned int* hash_length);
char* concat(const char *s1, const char *s2);
unsigned char* hash_line(char *line, char* type, unsigned int* hash_length);
