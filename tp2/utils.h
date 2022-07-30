#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>

unsigned char* hash_file(char *filename, int is_md5, int *len);
char* concat(const char *s1, const char *s2);
unsigned char* hash_line(char *line, int is_md5, int *len);
