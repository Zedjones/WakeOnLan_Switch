#include <stdlib.h>
#include <stdbool.h>

#include "alternatives.h"
#include "config.h"
#include "ini.h"

static int config_handler(void* user, const char* section, const char* name, const char* value){

    WolConfigs* config = (WolConfigs*)user;

    
}

WolConfigs* get_configs(){

    FILE* config_file;
    char* line = NULL;
    size_t len;

    config_file = fopen(CONFIG_FILE, "r");

    if (config_file == NULL){
        printf("Failed to open config file\n");
        return NULL;
    }

    WolConfigs* ret_list = create_config_list();

    char key[10];
    char value[16];
    bool in_config = false;

    WolConfig* new_config;
    char broadcast[17];
    char mac[18];
    char config_name[16];

    while(getline(&line, &len, config_file) != -1){
/*         if(in_config){
            sscanf(line, "%[^':']:%s", key, value);
            printf("key: %s, value: %s\n", key, value);
        }
        else{
            sscanf(line, "[%15[^]]]", value);
            printf("Entering configuration mode.\n");
            printf("config name: %s\n", value);
            in_config = true;
        }

        if(strcmp(key, "broadcast")){
            strcpy(broadcast, value);
        }
        else if(strcmp(key, "mac")){
            strcpy(mac, value);
        }
        else if(!in_config){
            strcpy(config_name, value);
        }
        else{
            printf("Error in configuration file.\n");
            return NULL;
        }
        */

    }
    printf("\n\n\n");

    free(line);

    return ret_list;
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

WolConfig* create_config(char* broadcast, char* mac, char* name, int port){
    WolConfig* config = malloc(sizeof(WolConfig));

    config->broadcast_address = broadcast;
    config->mac_address = mac;
    config->name = name;
    config->udp_port = port; 

    return config;
}

void add_config(WolConfigs* configs, char* broadcast, char* mac, char* name, int port){

    int current_capacity = configs->capacity;
    if(configs->size == configs->capacity-1){
        configs->configs = realloc(configs->configs, sizeof(WolConfig*) * 
                                   (current_capacity + STARTING_CAPACITY));
        configs->capacity += STARTING_CAPACITY;
    }

    configs->size++;
    WolConfig* new_config = create_config(broadcast, mac, name, port);

    configs->configs[configs->size] = new_config;
    
}

void destroy_config_list(WolConfigs* configs){
    for(int i = 0; i < configs->size; i++){
        free(configs->configs[i]->broadcast_address);
        free(configs->configs[i]->mac_address);
        free(configs->configs[i]->name);
        free(configs->configs[i]);
    }
    free(configs->configs);
    free(configs);
}