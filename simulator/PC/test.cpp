/*
        Simple udp server
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

    // local.sin_family = AF_INET;
    // local.sin_port = htons(8888);
    // local.sin_addr.s_addr = htonl(INADDR_ANY);
    // // inet_aton("192.168.1.80", &local.sin_addr);

    remote.sin_family = AF_INET;
    remote.sin_port = htons(TARGET_PORT);
    // remote.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton(TARGET_IP, &remote.sin_addr);

    // bind socket to port
    // if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
    //     die("bind");
    // }

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

        // // try to receive some data, this is a blocking call
        // if ((recv_len =
        //          recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&remote,
        //                   (socklen_t *)&slen)) == -1) {
        //     die("recvfrom()");
        // }

        // // print details of the client/peer and the data received
        // printf("Received packet from %s:%d\n", inet_ntoa(remote.sin_addr),
        //        ntohs(remote.sin_port));
        // printf("Data: %s\n", buf);

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

        // break;
    }

    // close(s);
    return 0;
}