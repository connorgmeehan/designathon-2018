#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofFboSettings settings;
    settings.height = 5;
    settings.width = 50;
    settings.internalformat = GL_RGB;
    fbo.allocate(settings);

    fbo.begin();
    ofClear(0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(ofColor::red);
    ofNoFill();
    ofDrawRectangle(0,0,50,5);

    fbo.begin();
    ofClear(0);

    ofSetColor(ofColor::green);
    ofFill();

    for(int i = 0; i < 17; i++){
        ofSetColor(ofColor::fromHsb( 50/5*i, 255, 255 ));
        glm::vec2 pos(0 + (((ofGetElapsedTimeMillis() / 100) + 50/16*i) % 50), 0 );
        ofDrawRectangle( pos ,50/16,5);
    }

    if(bDoStrobe && ofGetFrameNum() % 2 == 0){
        ofSetColor(ofColor::white);
        ofDrawRectangle(0,0,50,5);
    }

    fbo.end();

    ofSetColor(ofColor::white);

    fbo.draw(0,20, 400, 40);

    artnet_communicator.sendFrame(fbo); 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    bDoStrobe = !bDoStrobe;
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