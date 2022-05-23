#include "decode.h"
Decoder::Decoder(){
	frame_count = 0;
	return;
}

void Decoder::next_frame(uint8_t * buffer){

	for (int r = 0; r < 400; r++){
		for (int c = 0; c < 400; c++){
			if (r % 40 == frame_count % 40 && c % 40 == frame_count % 40){
				buffer[r * 400 + c] = 255;
			}
		}
	}
	frame_count ++;
	return;
}

int Decoder::frame_number(){
	return frame_count;
}
