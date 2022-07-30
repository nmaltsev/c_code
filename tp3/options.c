#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void parseOptions(int argc, char* argv[], int* opt_ind, int* flag_a, int* flag_f){
	int opt;
	*flag_a = 0;
	*flag_f = 0;

	while((opt = getopt(argc, argv, ":af")) != -1) {
      switch(opt) {
          case 'a':
              *flag_a = 1;
              break;
          case 'f':
              *flag_f = 1;
              break;
          case '?':
               printf("unknown option: %c\n", optopt);
               break;
          default:
            fprintf(stderr, "Usage: %s [-a] [-a] ..files\n", argv[0]);
            exit(EXIT_FAILURE);
      }
  }
  *opt_ind = optind;
}
