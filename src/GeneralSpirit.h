#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class GeneralSpirit {
    private:
        ofColor color;
    public:
        virtual void setup();
        virtual void update();
        virtual void draw();
        virtual void kill();
}