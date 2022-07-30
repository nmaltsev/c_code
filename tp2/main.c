#include <stdio.h>
#include "utils.h"
#include "options.h"

int main(int argc, char* argv[]) {
  int type, mode, opt_ind;
  parseOptions(argc, argv, &type, &mode, &opt_ind);

  if (mode == 1) { // From file
    for(; opt_ind < argc; opt_ind++){
      int hash_length, i;
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

    int hash_length, i;
    unsigned char* hash = hash_line(line, type, &hash_length);
    // printf("Length %d\n", hash_length);
    for (i = 0; i < hash_length; i++)  printf("%02x", hash[i]);
    printf("\n");
  }

  return 0;
}
