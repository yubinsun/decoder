# Intro
This program is a simple PC version of the display. It receives the Image Packet and display the image.

** Simulation of sender (ESP32 and PC) are in simulator folder** Those are use to test this one.


# requirments 
Tested in these environments (but not limited to):
```
OpenCV  -> To display the images.
Linux, Ubuntu -> Socket programming. 
Cmake -> To build project 
```

# How to used
First, change the program on IP to your computer's IP. And port if necessary. Change image size according to receiver's configuation.
```
mkdir build
cmake ..
make 
./testDecoder
```

# Side notes
*Decoder* is not used right now. 