#include "utils.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Random numbers:
    int r_min = random_num(0, 127);
    int r_max = random_num(128, 255);
    int r_num = random_num(r_min, r_max);
    
    printf("TESTS [%d %d] %d\n", r_min, r_max, r_num);
    printf("R0 %d\n", random_num(r_min, r_max));
    printf("R0 %d\n", random_num(r_min, r_max));
    printf("R0 %d\n", random_num(r_min, r_max));
    printf("R1 %d\n", random_num(127, 128));
    printf("R2 %d\n", random_num(128, 128));
    
    printf("r: %d %d %d\n", rands(), rands(), rands());
    
    unsigned char* initRequestBody = init_request();
    unsigned char* initResponseBody = init_response(r_min, r_max);
    
    printf("initRequestBody `%s`\n", initRequestBody);
    printf("initResponseBody `%s`\n", initResponseBody);
    
   
    return EXIT_SUCCESS;
}
