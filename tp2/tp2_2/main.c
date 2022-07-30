#include <stdio.h>
#include "utils.h"
#include "options.h"
#define MD5_HASH "md5"
#define SHA1_HASH "sha1"

int main(int argc, char* argv[]) {
  int hash_code, mode, opt_ind;
  char* type;
  parseOptions(argc, argv, &hash_code, &mode, &opt_ind);
  
  if (hash_code == 0) {
      type = malloc((4 + 1) * sizeof(char));
      strcpy(type, SHA1_HASH);
  } else {
      type = malloc((3 + 1) * sizeof(char));
      strcpy(type, MD5_HASH);
  }

  if (mode == 1) { // From file
    for(; opt_ind < argc; opt_ind++){
      unsigned int hash_length, i;  
      unsigned char* hash = hash_file(argv[opt_ind], type, &hash_length);
      for(i = 0; i < hash_length; i++) printf("%02x", hash[i]);
      printf(" %s\n", argv[opt_ind]);
    }
  } else if (mode == 0) { // From arguments
    char* line = "";
    for(; opt_ind < argc; opt_ind++){
      line = concat(line, argv[opt_ind]);
      if (opt_ind + 1 < argc) {
        line = concat(line, " ");
      }
    }

    unsigned int hash_length, i;
    unsigned char* hash = hash_line(line, type, &hash_length);
    // printf("Length %d\n", hash_length);
    for (i = 0; i < hash_length; i++)  printf("%02x", hash[i]); 
    printf("\n");
  }

  return 0; 
}
