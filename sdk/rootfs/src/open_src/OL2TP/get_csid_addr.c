#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc != 2 && argc != 3){
        printf("Usage:\n  get_csid_addr CSID_XXX_XXX [Number of shifts]\n");
        return -1;
    }
    if(argc == 2)
        argv[2] = "0";

    char* result = strtok( argv[1], "x" );
    result = strtok(NULL, "x");
    if(result == NULL) {
        printf("Invalid CSID address \n");
        return -1;
    }
    
    unsigned int input = (unsigned int)strtoul(result, NULL, 16);
    unsigned int idx   = (unsigned int)atol(argv[2]);
    printf("0x%x\n", input+idx);
    return 0;
}
