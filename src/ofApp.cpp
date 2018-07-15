#include "ofApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    fbo_out.allocate(320,240,GL_RGB);
    fig_detection.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    fig_detection.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(ofColor::white);
    fig_detection.draw(ofGetWidth()-320-20,20);

    fbo_out.begin();
        ofFill();
        ofClear(0);
        ofSetColor(ofColor::red);
        ofSetLineWidth(1);
        // fbo out here
    fbo_out.end();
    fbo_out.draw(ofGetWidth()-60,ofGetHeight()-64);
    fbo_out.draw(ofGetWidth()-60, ofGetHeight()-64-12);

    ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()) + "fps", 10, 20);
    artnet_communicator.sendFrame(fbo_out); 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}