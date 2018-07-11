#include "ofApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(120);

    cam.setup(width,height);
    blur_shader.setup(width,height);
    flow_fbo.allocate(320,240,GL_RGB);
    grabbed.allocate(width,height,OF_IMAGE_COLOR);
    fbo_out.allocate(50,64, GL_RGB);

    contourFinder.setFindHoles(false);

    // GUI
    gui.setup();
	gui.add(fbPyrScale.set("fbPyrScale", .5, 0, .99));
	gui.add(fbLevels.set("fbLevels", 4, 1, 8));
    gui.add(fbIterations.set("fbIterations", 2, 1, 8));
    gui.add(fbPolyN.set("fbPolyN", 7, 5, 10));
    gui.add(fbPolySigma.set("fbPolySigma", 1.5, 1.1, 2));
    gui.add(fbUseGaussian.set("fbUseGaussian", false));
    gui.add(fbWinSize.set("winSize", 32, 4, 64));
    gui.add(fbStepSize.set("stepSize", 4, 1, 8));
    gui.add(flowColor.set("flow_color", ofColor(255,255,255,255)));
    gui.add(flowLineWidth.set("flow_line_width", 1, 1, 100));
    gui.add(flowLineScale.set("flow_line_scale", 1, 0.5, 2));
    gui.add(blurFlow.set("flow_blur", true));
    gui.add(blurAmount.set("flow_blur_amount", 1, 0, 128));
    gui.add(blurIterations.set("flow_blur_itterations", 1, 0, 16));
    gui.add(contoursActive.set("contours_active", false));
    gui.add(contourShowLabels.set("contour_show_labels", true));
    gui.add(contourThreshold.set("contour_threshold", 15, 1, 100));
    gui.add(contourMinArea.set("contour_min_area", 5.0f, 1.0f, 40.0f));
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()){

        fb.setPyramidScale(fbPyrScale);
        fb.setNumLevels(fbLevels);
        fb.setWindowSize(fbWinSize);
        fb.setNumIterations(fbIterations);
        fb.setPolyN(fbPolyN);
        fb.setPolySigma(fbPolySigma);
        fb.setUseGaussian(fbUseGaussian);
        fb.setStepSize(fbStepSize);
        contourFinder.setMinArea(contourMinArea);
        fb.calcOpticalFlow(cam);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(ofColor::white);
    cam.draw(0,0,320,240);
    ofSetLineWidth(5);

    //calculate optical flow

    // store blurred flow in FBO
    blur_shader.begin(blurAmount, blurIterations);
        ofClear(0);
        ofSetColor(flowColor);
        ofSetLineWidth(flowLineWidth);
        fb.draw(0,0,320,240);
        ofSetLineWidth(1);
    blur_shader.end();

    // Find contours in blurred flow

    if(contoursActive){
        blur_shader.getFbo().readToPixels(grabbed);    
        contourFinder.findContours(grabbed);

        ofxCv::RectTracker& tracker = contourFinder.getTracker();
        if(contourShowLabels) {
            ofSetColor(255);
            contourFinder.draw();
            for(int i = 0; i < contourFinder.size(); i++) {
                ofPoint center = toOf(contourFinder.getCenter(i));
                ofPushMatrix();
                ofTranslate(center.x, center.y);
                int label = contourFinder.getLabel(i);
                string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
                ofDrawBitmapString(msg, 0, 0);
                ofVec2f velocity = toOf(contourFinder.getVelocity(i));
                ofScale(5, 5);
                ofDrawLine(0, 0, velocity.x, velocity.y);
                ofPopMatrix();
            }
        } else {
            for(int i = 0; i < contourFinder.size(); i++) {
                unsigned int label = contourFinder.getLabel(i);
                // only draw a line if this is not a new label
                if(tracker.existsPrevious(label)) {
                    // use the label to pick a random color
                    ofSeedRandom(label << 24);
                    ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
                    // get the tracked object (cv::Rect) at current and previous position
                    const cv::Rect& previous = tracker.getPrevious(label);
                    const cv::Rect& current = tracker.getCurrent(label);
                    // get the centers of the rectangles
                    ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
                    ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);                   

                    ofDrawLine(previousPosition, currentPosition);
                }
            }
        }
        // this chunk of code visualizes the creation and destruction of labels
        const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
        const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
        const vector<unsigned int>& newLabels = tracker.getNewLabels();
        const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
        ofSetColor(ofColor::blue);
        for(uint i = 0; i < currentLabels.size(); i++) {
            uint j = currentLabels[i];
            ofDrawLine(j, 0, j, 4);
        }
        ofSetColor(ofColor::pink);
        for(uint i = 0; i < previousLabels.size(); i++) {
            uint j = previousLabels[i];
            ofDrawLine(j, 4, j, 8);
        }
        ofSetColor(ofColor::yellow);
        for(uint i = 0; i < newLabels.size(); i++) {
            uint j = newLabels[i];
            ofDrawLine(j, 8, j, 12);
        }
        ofSetColor(ofColor::white);
        for(uint i = 0; i < deadLabels.size(); i++) {
            uint j = deadLabels[i];
            ofDrawLine(j, 12, j, 16);
        }        
    }
    
    ofDrawRectangle(10,255,50,5);

    fbo_out.begin();
    ofFill();
    ofClear(0);
    ofSetColor(ofColor::red);
    ofSetLineWidth(1);
    for(auto & line : contourFinder.getBoundingRects()){
        /*ofPolyline newLine;
        newLine.scale((float) 50/240, (float) 64/320);
        newLine.draw();*/
        ofRectangle newRect;
        newRect.x = ofMap(line.x,0,320,0,50);
        newRect.y = ofMap(line.y,0,240,0,64);
        newRect.width = ofMap(line.width,0,320,0,50);
        newRect.height = ofMap(line.height, 0, 240, 0, 64);
        ofDrawRectangle(newRect);
    }
    fbo_out.end();
    ofDrawBitmapStringHighlight("fbo_out:", 10, 250);
    fbo_out.draw(10,255);

    gui.draw();
    ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()) + "fps", 10, 20);
    artnet_communicator.sendFrame(fbo_out); 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        bLearnBackground = true;
    }
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