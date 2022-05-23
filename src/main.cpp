#include "Decode.h"
#include <iostream>
#define BUF_SIZE 400*400

uint8_t buf[BUF_SIZE];

// demo test for the decoder
int main(){
	Decoder d;
	d.next_frame(buf);
	std::cout << d.frame_number() << std::endl;
	return 0;
}
