#include <stdio.h>
#include <stdbool.h> 
#include <string.h>

#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#define CONFIG_FILE "/switch/WakeOnLan_Switch/configurations"
#define MAX_LINE 1024
#define STARTING_CAPACITY 5

typedef struct WoLConfig_t {
    char* broadcast_address;
    char* mac_address;
    char* name;
    int udp_port; 
} WolConfig; 

typedef struct WolConfigs_t {
    WolConfig** configs;
    int capacity;
    int size;
} WolConfigs;

void destroy_config(WolConfig* config);

WolConfigs* get_configs();

WolConfigs* create_config_list();

void add_config(WolConfigs* configs, char* broadcast, char* mac, char* name, int port);

void destroy_config_list(WolConfigs* configs);

#endif