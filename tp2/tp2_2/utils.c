#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/evp.h>


unsigned char* hash_file(char *filename, char* type, unsigned int *hash_length) {
    FILE *file = fopen(filename, "rb");
    
    int bytes;
    unsigned char data[1024];

    if (file == NULL) {
        fprintf(stderr, "%s can't be opened.\n", filename);
        exit(EXIT_FAILURE);
    }
    
    OpenSSL_add_all_digests();
    const EVP_MD* digest = EVP_get_digestbyname(type);
    EVP_MD_CTX* digest_context = EVP_MD_CTX_create();
    EVP_DigestInit_ex(digest_context, digest, NULL);
    
    unsigned char* hash_value = malloc(EVP_MAX_MD_SIZE * sizeof(unsigned char));
    
    while ((bytes = fread(data, 1, 1024, file)) != 0)
        EVP_DigestUpdate(digest_context, data, bytes);
	
    EVP_DigestFinal_ex(digest_context, hash_value, hash_length);
	EVP_MD_CTX_destroy(digest_context);

    fclose(file);
    return hash_value;
}


char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

unsigned char* hash_line(char* line, char* type, unsigned int *hash_length) {
    OpenSSL_add_all_digests();
    const EVP_MD* digest = EVP_get_digestbyname(type);
    EVP_MD_CTX* digest_context = EVP_MD_CTX_create();
    EVP_DigestInit_ex(digest_context, digest, NULL);
    EVP_DigestUpdate(digest_context, line, strlen(line));
	unsigned char* hash_value = malloc(EVP_MAX_MD_SIZE * sizeof(unsigned char));
    EVP_DigestFinal_ex(digest_context, hash_value, hash_length);
	EVP_MD_CTX_destroy(digest_context);
    return hash_value;
}
