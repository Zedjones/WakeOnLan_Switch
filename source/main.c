/**
 *  @file    main.cpp
 *  @author  Nicholas Jones
 *  @date    2018-09-28
 *  @version 0.0.1
 *
 *  This is the main file for WoL_Switch.
 *  Original example file by WerWolv.
 */

#include <switch.h>     //The nxlib header file. It includes the functions which allow you to talk to the switch software/hardware
#include <stdio.h>      //Used for printf
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAGICPACKET_SIZE 102

unsigned char* create_magic_packet(unsigned char* mac, unsigned char* packet){

    // pad first 6 bytes
    for(int i = 0; i < 6; i++){
        packet[i] = 0xFF;
    }

    // need to append to the packet 16 times 
    for(int i = 1; i <= 16; i++){
        memcpy(&packet[i * 6], &mac, sizeof(char));
    }

    return packet;
}

//The main entry point
int main(int argc, char **argv) {
    // We save the current button press state in there.
    u32 kdown = 0x00000000;

    unsigned char packet[MAGICPACKET_SIZE];
    unsigned char mac[6];

    // Setup the console
    gfxInitDefault();
    consoleInit(NULL);

    // Sets up the network sockets for nxlink
    socketInitializeDefault();

    unsigned char* magic_packet = create_magic_packet(mac, packet);

    int sock;
    struct sockaddr_in client, server; 
    int broadcast = 1;

    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) == -1){
        printf("Error setting up broadcast");
        return -1;
    }

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = 0;

    bind(sock, (struct sockaddr*)&client, sizeof(client));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.1.255");
    server.sin_port = htons(9);

    // Our main loop. As long as the program shouldn't close, keep executing our code
    while (appletMainLoop()) {
        // Scans our controllers for any button presses since the last time this function was called
        hidScanInput();

        // Read the last button presses and store them in the kdown variable. CONTROLLER_P1_AUTO reads the values from
        // the currently used controller.
        kdown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kdown & KEY_A){
            sendto(sock, &magic_packet, sizeof(unsigned char) * MAGICPACKET_SIZE, 0, 
                   (struct sockaddr*)&server, sizeof(server));
        }

        // This isn't a convention but just for consistency. If the Plus button gets pressed, close the program.
        // Most homebrews do that.
        if (kdown & KEY_PLUS)
            break;
    }

    //Clean up after we're done and close our sockets.
    socketExit();

    //Terminate SUCCESSFULLY
    return 0;
}
