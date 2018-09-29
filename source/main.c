#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <switch.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAGICPACKET_SIZE 102

unsigned char* create_magic_packet(unsigned char* mac, unsigned char* packet){

    unsigned char test[6];

    // pad first 6 bytes
    for(int i = 0; i < 6; i++){
        packet[i] = 0xFF;
    }

    // need to append to the packet 16 times 
    for(int i = 1; i <= 16; i++){
        memcpy(&packet[i * 6], &mac, 6 * sizeof(char));
        printf("\n");
    }

    for(int i = 0; i < 6; i++){
        if(i == 5){
            printf("%x\n", mac[i]);
        }
        else{
            printf("%x:", mac[i]);      
        }
    }

    for(int i = 0; i < MAGICPACKET_SIZE; i += sizeof(char)){
        printf("%x:", packet[i]);      
    }
    printf("\n");

    return packet;
}

int main(int argc, char **argv)
{

    unsigned char packet[MAGICPACKET_SIZE];
    unsigned char mac[6] = {0x90, 0x2b, 0x34, 0x3d, 0x17, 0x8f};

    gfxInitDefault();
    consoleInit(NULL);
    socketInitializeDefault();

    printf("Hello World!\n");

    unsigned char* magic_packet = create_magic_packet(mac, packet);

    int sock;
    struct sockaddr_in client, server; 
    int broadcast = 1;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1){
        perror("setsockopt(2)");
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

        // Your code goes here

        //hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A){
            sendto(sock, &magic_packet, sizeof(unsigned char) * MAGICPACKET_SIZE, 0, 
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

