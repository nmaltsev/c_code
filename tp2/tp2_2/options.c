#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void
 parseOptions(int argc, char* argv[], int* type, int* mode, int* opt_ind) {
    int opt;
    *type = 0; // sha1
    *mode = 0; // hash from arguments
      
    while((opt = getopt(argc, argv, ":ft")) != -1) { 
        switch(opt) { 
            case 'f':
                *mode = 1; // file
                break;
            case 't': 
                *type = 1; // md5
                break; 
            // case '?': 
            //     printf("unknown option: %c\n", optopt);
            //     break; 
            default:
              fprintf(stderr, "Usage: %s [-f] [-t] names\n", argv[0]);
              exit(EXIT_FAILURE);
        } 
    }
    *opt_ind = optind;
}
