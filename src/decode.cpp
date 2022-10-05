#include "decode.h"

#include <cstring>
Decoder::Decoder() {
    frame_count = 0;
    return;
}

void Decoder::next_frame(uint8_t* buffer) {
    memset(buffer, 0, 400 * 400);
    for (int r = 0; r < 400; r++) {
        for (int c = 0; c < 400; c++) {
            if (r / 20 == frame_count / 20 % 20 && c / 20 == frame_count % 20) {
                buffer[r * 400 + c] = 255;
            }
        }
    }
    frame_count++;
    return;
}

int Decoder::frame_number() { return frame_count; }
