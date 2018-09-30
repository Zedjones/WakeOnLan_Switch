#include <stdio.h>
#include <stdbool.h> 
#include <string.h>

#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#define CONFIG_FILE "./configurations"
#define MAX_LINE 1024

typedef struct WoLConfig_t {
    char* broadcast_address;
    char* mac_address;
    int udp_port; 
} WolConfig; 

WolConfig** get_configs();

WolConfig* create_config(char* broadcast, char* mac, int port);

void destroy_config(WolConfig* config);

#endif