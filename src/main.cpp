#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>

#include "Decode.h"
#define BUF_SIZE 400 * 400
using namespace cv;
using namespace std;
using namespace std::this_thread;      // sleep_for, sleep_until
using namespace std::chrono;           // nanoseconds, system_clock, seconds
using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.

uint8_t buf[BUF_SIZE];

int display_img(cv::Mat& image) {
    // Check for failure
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        cin.get();  // wait for any key press
        return -1;
    }

    imshow("The Guitar", image);  // Show our image inside the created window.

    waitKey(10);  // Wait for any keystroke in the window

    // destroyWindow(windowName);  // destroy the created window
}

// demo test for the decoder
int main() {
    Decoder d;
    String windowName = "The Guitar";  // Name of the window

    namedWindow(windowName);  // Create a window
    while (1) {
        sleep_for(30ms);
        d.next_frame(buf);
        cv::Mat A(400, 400, CV_8U, buf);
        display_img(A);
        std::cout << d.frame_number() << std::endl;
    }

    return 0;
}
