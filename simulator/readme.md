# Intro
A sender program to simulate a ESP32 that is transmitting images out. The program will continue to send out *Image Packet* (see definition in doc and readme) until exit. 

# PC version:
Requirments:
- Linux(Ubuntu) for socket programming.

How to run:
First, change the program on IP to your computer's IP. And port if necessary. Change image size according to receiver's configuation.
Compile and run:
```
g++ main.cpp -o simulate
./simulate
```

# ESP32 version:
Requirments:
- ESP IDF enviroment 


Follow standard ESP32 program procedure.
```
idf.py menuconfig # You need to set your wifi and socket infomation here
idf.py build flash monitor
```

### 