#include <stdlib.h>

#include "alternatives.h"
#include "config.h"

WolConfigs* get_configs(){

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

WolConfigs* create_config_list(){
    WolConfigs* configs = malloc(sizeof(WolConfigs));

    configs->configs = malloc(sizeof(WolConfig*) * STARTING_CAPACITY);
    configs->capacity = STARTING_CAPACITY;
    configs->size = 0;

    if(configs->configs == NULL){
        printf("Cannot allocate storage for configuration list.");
    }

    return configs;
}

WolConfig* create_config(char* broadcast, char* mac, int port){
    WolConfig* config = malloc(sizeof(WolConfig));

    config->broadcast_address = broadcast;
    config->mac_address = mac;
    config->udp_port = port; 

    return config;
}

void add_config(WolConfigs* configs, char* broadcast, char* mac, int port){

    int current_capacity = configs->capacity;
    if(configs->size == configs->capacity-1){
        configs->configs = realloc(configs->configs, sizeof(WolConfig*) * 
                                   (current_capacity + STARTING_CAPACITY));
        configs->capacity += STARTING_CAPACITY;
    }

    configs->size++;
    WolConfig* new_config = create_config(broadcast, mac, port);

    configs->configs[configs->size] = new_config;
    
}

void destroy_config_list(WolConfigs* configs){
    for(int i = 0; i < configs->size; i++){
        free(configs->configs[i]);
    }
    free(configs->configs);
    free(configs);
}