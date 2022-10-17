/**
 * @file test.cpp
 * @author your name (you@domain.com)
 * @brief Simulation that sends out Image Packets (see definition)
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <arpa/inet.h>
#include <stdio.h>   //printf
#include <stdlib.h>  //exit(0);
#include <string.h>  //memset
#include <sys/socket.h>

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::this_thread;      // sleep_for, sleep_until
using namespace std::chrono;           // nanoseconds, system_clock, seconds
using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.

#define TARGET_PORT 9999  // The port on which to listen for incoming data
#define TARGET_IP   "192.168.1.80"
#define ROW         400
#define COL         400
#define DATA_SIZE   (ROW * COL)
#define PACKSIZE \
    65495  // 65535 - 28 (Header 20 for IP, 8 for UDP ) - 12(Image packet
           // headers)

#define PACKAGE_NUMBER \
    DATA_SIZE / PACKSIZE + (DATA_SIZE % PACKSIZE == 0 ? 0 : 1)

char send_buf[DATA_SIZE];

/**
 * @brief Error handling
 *
 * @param s
 */
void die(char *s) {
    perror(s);
    exit(1);
}

int main(void) {
    struct sockaddr_in local, remote;

    int s, i, slen = sizeof(remote), recv_len;
    int frame_count = 3;

    // create a UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        die("socket");
    }

    // zero out the structure
    memset((char *)&local, 0, sizeof(local));

    remote.sin_family = AF_INET;
    remote.sin_port = htons(TARGET_PORT);
    inet_aton(TARGET_IP, &remote.sin_addr);

    /**
     * @brief Loop 100 times: Create a image, break the image into N Image
     * packet, and send each image packet out.
     *
     *
     */
    for (int i = 0; i < 100; i++) {
        /* generate a image */
        memset(send_buf, 128, DATA_SIZE);
        for (int r = 0; r < ROW; r++) {
            for (int c = 0; c < COL; c++) {
                if (r / 20 == frame_count / 20 % 20 &&
                    c / 20 == frame_count % 20) {
                    send_buf[r * ROW + c] = 255;
                }
            }
        }
        frame_count++;
        //

        for (int i = 0; i < PACKAGE_NUMBER; i++) {
            /* Construct packet*/
            char packet[PACKSIZE + 12];
            packet[0] = frame_count;
            packet[1] = i;
            memcpy(packet + 12, send_buf + PACKSIZE * i,
                   DATA_SIZE - PACKSIZE * i > PACKSIZE
                       ? PACKSIZE
                       : DATA_SIZE - PACKSIZE * i);

            if (sendto(s, packet,
                       DATA_SIZE - PACKSIZE * i > PACKSIZE
                           ? PACKSIZE
                           : DATA_SIZE - PACKSIZE * i,
                       0, (struct sockaddr *)&remote, sizeof(remote)) == -1) {
                die("sendto()");
            }
            sleep_for(10ms);
        }
        sleep_for(1s);

        /**
         * @brief Add a break here can help debug using wireshark.
         *
         */
        // break;
    }

    // close(s);
    return 0;
}