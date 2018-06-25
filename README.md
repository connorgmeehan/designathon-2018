# Designathon 2018
Github repo for SUEDE's 2018 Lighting Display Designathon

## Layout
- `./designathon_rbpi/` openFrameworks app for raspberry pi
    - Interfaces with webcam, openCV, passes data to arduino using serial connection
    - Build with `make -j3` (for 3 core compilation)
    - May have to change `OF_PATH` in the `Makefile` to fit your openFrameworks 0.10.0 directory
    - `./src/ofApp.h` Header file for application
    - `./sirc/ofApp.cpp` Implementation of header functions
    - **dependencies:** `ofxOpenCv` + To be decided we need a serial interface
- `./designathon_ard/`
    - Contains code for arduino
`./designathon_rbpi/` contains the code for  the raspberry pi component of the code
   
## Features to be implemented

- [ ] Figure detection
    - [ ] Setup ofApp to get webcam input
    - [ ] Pass webcam input to openCV's `ofxCvHaarFinder`
    - [ ] Build our own `figures_cascades.xml` file
- [ ] Code display logic
    - [ ] Convert openCV classifier coordinates to realword coordinates
        - [ ] Get dimensions of space and mounting point + angle of webcam
    - [ ] Logic that renders to FBO with same dimensions as LED display
        - Using a shader?
        - Meta balls?
    - [ ] Convert FBO to data stream to be sent over serial
- [ ] Serial interface
    - [ ] Decide on which IO addon to use (ofxSerial+ofxIO || ofxGPIO)
    - [ ] Test send frames to arduino
- [ ] Decoding serial data to light up LEDS
    - [ ] Read array of RGBA data and then render it until more data is received
    - [ ] Preset dimensions
    - [ ] Avoid screen tearing issues with assynchronous input->draw cycle, possibly store last received `frame` to render in ping pong buffer and switch once all data is received?

