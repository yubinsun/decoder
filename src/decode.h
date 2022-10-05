#ifndef DECODE_HPP
#define DECODE_HPP

#include <iostream>

/*
 * Decoder class
 */
class Decoder {
   public:
    /*
     * Constructor
     */
    Decoder();

    /*
     * Return a frame into the buffer.
     * buffer: A pointer to an unit8 or unsigned char array of size 160,000
     * Returns: void
     */
    void next_frame(uint8_t* buffer);

    /*
     * Return how many frames has been generated.
     */
    int frame_number();

   private:
    unsigned int frame_count;
};

#endif /*DECODE_HPP*/
