#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include <switch.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "config.h"

#define MAGICPACKET_SIZE 102

void create_magic_packet(unsigned char* mac, unsigned char* packet){

    // pad first 6 bytes
    for(int i = 0; i < 6; i++){
        packet[i] = 0xFF;
    }

    // need to append to the packet 16 times 
    for(int i = 1; i <= 16; i++){
        memcpy(&packet[i * 6], mac, 6 * sizeof(char));
    }

}

bool validate_ip_address(char* ip_addr){
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip_addr, &(sa.sin_addr)) != 0;
}

int validate_mac_addr(char* mac) {
    int i = 0;
    int s = 0;

    while (*mac) {
        if (isxdigit(*mac)) {
            i++;
        } else if (*mac == ':' || *mac == '-') {
            if (i == 0 || i / 2 - 1 != s)
                break;
            ++s;
        }
        else {
            s = -1;
        }
        ++mac;
    }
    return (i == 12 && (s == 5 || s == 0));
}

int enter_exit_loop(){
    while (appletMainLoop()) {
        // Scans our controllers for any button presses since the last time this function was called
        hidScanInput();

        // Read the last button presses and store them in the kdown variable. CONTROLLER_P1_AUTO reads the values from
        // the currently used controller.
        u64 kdown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kdown & KEY_PLUS){
                socketExit();
                gfxExit();
                return 0;
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }
    return 0;
}

int main(int argc, char **argv)
{

    unsigned char packet[MAGICPACKET_SIZE];
    unsigned char mac[6] = {0x90, 0x2b, 0x34, 0x3d, 0x17, 0x8f};
    unsigned char** packets;

    gfxInitDefault();
    consoleInit(NULL);
    socketInitializeDefault();

    printf("Press A to send a magic packet.\n");

    WolConfigs* configs = get_configs();

    packets = malloc(sizeof(char*) * configs->size+1);

    for(int i = 0; i <= configs->size; i++){
        WolConfig* current_config = configs->configs[i];
        bool invalid = false;
        if(!validate_ip_address(current_config->broadcast_address)){
            printf("Invalid broadcast address entered. Press + to exit");
            invalid = true;
        }
        if(!validate_mac_addr(current_config->mac_address)){
            printf("Invalid MAC address entered. Press + to exit");
            invalid = true;
        }
        if(invalid){
            printf("Configuration %s contains invalid input.", current_config->name);
            return enter_exit_loop();
        }
        packets[i] = malloc(sizeof(char) * MAGICPACKET_SIZE);
    }

    create_magic_packet(mac, packet);

    int sock;
    struct sockaddr_in client, server; 
    int broadcast = 1;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1){
        perror("setsockopt(2)");
        return enter_exit_loop();
    }

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = 0;

    bind(sock, (struct sockaddr*)&client, sizeof(client));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.0.255");
    server.sin_port = htons(9);

    // Main loop
    while(appletMainLoop())
    {
        //Scan all the inputs. This should be done once for each frame
        hidScanInput();

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A){
            sendto(sock, packet, sizeof(unsigned char) * MAGICPACKET_SIZE, 0, 
                   (struct sockaddr*)&server, sizeof(server));
            printf("Sending magic packet to ");
            for(int i = 0; i < 6; i++){
                if(i == 5){
                    printf("%x\n", mac[i]);
                }
                else{
                    printf("%x:", mac[i]);      
                }
            }
        }

        if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    socketExit();
    gfxExit();
    return 0;
}

