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
`./designathon_rbpi/` contains the code for the raspberry pi component of the code
   
## Features to be implemented

- [] Figure detection
    - [] Setup ofApp to get webcam input
    - [] Pass webcam input to openCV's `ofxCvHaarFinder`
    - [] Build our own `figures_cascades.xml` file
- [] Serial interface
- 

