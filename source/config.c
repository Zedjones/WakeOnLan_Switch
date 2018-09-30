#include <stdlib.h>

#include "alternatives.h"
#include "config.h"

WolConfig** get_configs(){

    FILE* config_file;
    char* line = NULL;
    size_t len;

    config_file = fopen(CONFIG_FILE, "r");

    if (config_file != NULL){
        return NULL;
    }

    while(getline(&line, &len, config_file) != -1){
        printf("%s\n", line);
    }

    free(line);

    return NULL;
}