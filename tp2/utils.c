#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>


unsigned char* hash_file(char *filename, int is_md5, int *len) {
    int length;

    if (is_md5 == 1) {
        length = MD5_DIGEST_LENGTH;
    } else {
        length = SHA_DIGEST_LENGTH;
    }
    *len = length;
    unsigned char* hash = malloc(sizeof(unsigned char) * length);

    FILE *file = fopen(filename, "rb");

    int bytes;
    unsigned char data[1024];

    if (file == NULL) {
        fprintf(stderr, "%s can't be opened.\n", filename);
        exit(EXIT_FAILURE);
    }

    if (is_md5 == 1) {
        MD5_CTX mdContext;
        MD5_Init (&mdContext);
        while ((bytes = fread(data, 1, 1024, file)) != 0)
            MD5_Update(&mdContext, data, bytes);
        MD5_Final(hash, &mdContext);
    } else {
        SHA_CTX mdContext;
        SHA1_Init (&mdContext);
        while ((bytes = fread(data, 1, 1024, file)) != 0)
            SHA1_Update(&mdContext, data, bytes);
        SHA1_Final(hash, &mdContext);
    }

    fclose(file);
    return hash;
}


char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

unsigned char* hash_line(char *line, int is_md5, int *len) {
    int length;
    unsigned char* res;
    unsigned char* hash;

    if (is_md5 == 1) {
        length = MD5_DIGEST_LENGTH;
        hash = malloc(sizeof(unsigned char) * length);
        res = MD5((unsigned char*)line, strlen(line), hash);
    } else {
        length = SHA_DIGEST_LENGTH;
        hash = malloc(sizeof(unsigned char) * length);
        res = SHA1((unsigned char*)line, strlen(line), hash);
    }
    *len = length;
    
    return res;
}
