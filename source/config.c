#include <stdlib.h>
#include <stdbool.h>

#include "alternatives.h"
#include "config.h"
#include "ini.h"

void check_for_resize(WolConfigs* configs){

    int current_capacity = configs->capacity;
    if(configs->size == configs->capacity-1){
        configs->configs = realloc(configs->configs, sizeof(WolConfig*) * 
                                   (current_capacity + STARTING_CAPACITY));
        configs->capacity += STARTING_CAPACITY;
    }
    for(int i = current_capacity; i < configs->capacity; i++){
        configs->configs[i] = malloc(sizeof(WolConfig));
    }
}

static int config_handler(void* user, const char* section, const char* name, const char* value){

    WolConfigs* configs = (WolConfigs*)user;
    WolConfig* current_config = configs->configs[configs->size];

    if(strcmp(section, "") == 0){
        //no config name defined
        printf("Malformed configuration file\n");
        return 0;
    }
    else{
        if(strcmp(current_config->name, "0") == 0){
            current_config->name = strdup(section);
        }
        else if(strcmp(current_config->name, section) != 0){
            check_for_resize(configs);
            configs->size++;
            current_config = configs->configs[configs->size];
            current_config->name = strdup(section);
        }
    }

    if(strcmp(BROADCAST_KEY, name) == 0){
        current_config->broadcast_address = strdup(value);
    }
    else if(strcmp(MAC_KEY, name) == 0){
        current_config->mac_address = strdup(value);
    }
    else if(strcmp(PORT_KEY, name) == 0){
        int port = strtol(value, NULL, 10);
        if(!(port == 7 || port == 9)){
            printf("Please enter either port 7 or 9\n");
            return 0;
        }
        current_config->udp_port = port;
    }
    else{
        printf("Unknown key specified\n");
        return 0;
    }

    return 1;

}

WolConfigs* get_configs(){

    WolConfigs* ret_list = create_config_list();

    if(ini_parse(CONFIG_FILE, config_handler, ret_list) < 0){
        printf("Failed to parse configuration\n");
        return NULL;
    }
    else{
        printf("Successfully parsed configuration file!\n");
    }

    printf("final size: %d\n", ret_list->size+1);

    for(int i = 0; i < ret_list->size+1; i++){
        WolConfig* config = ret_list->configs[i];
        printf("name: %s, broadcast: %s, mac: %s, port: %d\n", 
               config->name, config->broadcast_address, config->mac_address, config->udp_port);
    }

    printf("\n\n\n");

    return ret_list;
}

WolConfigs* create_config_list(){
    WolConfigs* configs = malloc(sizeof(WolConfigs));

    configs->configs = malloc(sizeof(WolConfig*) * STARTING_CAPACITY);
    configs->capacity = STARTING_CAPACITY;
    configs->size = 0;

    for(int i = 0; i < configs->capacity; i++){
        configs->configs[i] = malloc(sizeof(WolConfig));
        configs->configs[i]->name = "0";
    }

    if(configs->configs == NULL){
        printf("Cannot allocate storage for configuration list.");
    }

    return configs;
}

WolConfig* create_config(char* broadcast, char* mac, char* name, int port){
    WolConfig* config = malloc(sizeof(WolConfig*));

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