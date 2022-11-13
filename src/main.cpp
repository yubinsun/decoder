/**
 * @file test.cpp
 * @author your name (you@domain.com)
 * @brief Recever Image Packets (see definition), and display image
 * @version 0.1
 * @date 2022-10-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>   //printf
#include <stdlib.h>  //exit(0);
#include <string.h>  //memset
#include <sys/socket.h>

#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

#include "Decode.h"
#define ROWS           240
#define COLS           320
#define BYTE_PER_PIXEL 2
#define BUF_SIZE       (ROWS * COLS * BYTE_PER_PIXEL)
using namespace cv;
using namespace std;
using namespace std::this_thread;      // sleep_for, sleep_until
using namespace std::chrono;           // nanoseconds, system_clock, seconds
using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.

uint8_t buf[BUF_SIZE];
uint8_t display_buf[BUF_SIZE];


/**
 * @brief Display an image. Size is defined in CV::mat
 * 
 * @param image 
 * @return int 
 */
int display_img(cv::Mat &image) {
    // Check for failure
    if (image.empty()) {
        cout << "Could not open or find the image." << endl;
        return -1;
    }

    namedWindow("Display Window");  // Create a window
    /* Also save the image as file*/
    imwrite("Image.jpg", image);

    imshow("Display Window",
           image);  // Show our image inside the created window.

    waitKey(1);
}

/**
 * @brief Error handling
 * 
 * @param s 
 */
void exit_(char *s) {
    perror(s);
    exit(1);
}

// demo test for the decoder
int main() {
    /**
     * @brief Create a socket and receive image
     *
     */
    struct sockaddr_in local, remote;
    int s, i, slen = sizeof(remote), recv_len;

    memset((char *)&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(9999);
    // local.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton(LOCAL_IP, &local.sin_addr);
    // try to receive some data, this is a blocking call
    printf("Waiting for data...");
    // create a UDP socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        exit_("socket");
    }
    // bind socket to port
    if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
        exit_("bind");
    }
    uint32_t size_total = 0;
    uint32_t packet_count = 0;
    long frame_count = -1;
    while (1) {
        // recv
        size_total = 0;
        packet_count = 0;
        frame_count = -1;
        while (1) {
            // Recv max size is set to BUFSIZE, but not actually receiving that
            // much
            if ((recv_len =
                     recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *)&remote,
                              (socklen_t *)&slen)) == -1) {
                exit_("recvfrom()");
            }
            /* Check for out of order package and ignore */
            if ((frame_count == -1 && buf[1] != 0) | (frame_count != -1 && frame_count != buf[0])){
                std::cout << "frame messed up, skip this frame 2" << std::endl;
                frame_count = -1;
                break;
            }
            else if (frame_count == -1) {
                frame_count = buf[0];
            } 
            /* Copy to display buf and stats*/
            memcpy(display_buf + size_total, buf + 12, recv_len - 12);
            size_total = size_total + recv_len - 12;
            packet_count++;

            /* Print info */
            printf(
                "Received packet from %s:%d\n",
                // network to ascii => prints network binary to dot IP notation
                inet_ntoa(remote.sin_addr),
                // network to host short
                ntohs(remote.sin_port));
            std::cout << std::dec << "size: " << recv_len << std::endl;
            std::cout << std::dec << "FC:" << (unsigned int)buf[0]
                      << "\tPC:" << (unsigned int)buf[1] << std::endl;
            /* A simple, unsafe check */
            if (size_total >= BUF_SIZE) {
                break;
            }
        }
        /* Check for out of order package and ignore */
        if (frame_count == -1){
            continue;
        }
        /* Print stats */
        std::cout << std::dec << "Frame size: " << size_total << std::endl;
        /* At this moment we dont need decoder, yet. Will be some this like
         *this:
         ** Decoder d;
         ** d.decode_frame(display_buf);
         */
        /* Display the image */
        /**
         * @brief Convert RGB565 to BGR888
         *
         */
        char BGR888[ROWS * COLS * 3];
        memset(BGR888, 0, ROWS * COLS * 3);
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                // red
                BGR888[(r * COLS + c) * 3 + 2] =
                    display_buf[(r * COLS + c) * 2] & 0xF8;
                // green
                BGR888[(r * COLS + c) * 3 + 1] =
                    (display_buf[(r * COLS + c) * 2] & 0x07) << 5 |
                    (display_buf[(r * COLS + c) * 2 + 1] & 0xE0) >> 3;
                // blue
                BGR888[(r * COLS + c) * 3 + 0] =
                    (display_buf[(r * COLS + c) * 2 + 1] & 0x1F) << 3;  // blue
            }
        }
        cv::Mat A(ROWS, COLS, CV_8UC3, BGR888);
        display_img(A);
    }
    return 0;
}
