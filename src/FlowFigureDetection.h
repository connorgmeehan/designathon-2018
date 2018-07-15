#pragma once

#include "ofMain.h"

#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "Profiler.h"

using namespace ofxCv;
using namespace cv;
using namespace glm;

class FlowFigureDetection {
    public:
        // int width = 320;
		// int height = 240;
        int width = 320;
        int height = 240;
        ofVideoGrabber cam;
        cv::Mat cv_frame, cv_frame_mini;
        ofxCv::FlowFarneback fb;
        ofxCv::ContourFinder contourFinder;

        ofxPanel gui;
        ofParameter<bool> drawCamInput, drawFlowImage, contoursActive, contourShowLabels, flowBlur;
        ofParameter<int> contourThreshold;
        ofParameter<int> diffGrowIter;
        ofParameter<float> contourMinRadius, contourMaxRadius, contourOutlineWidth, flowBlurDist;


        void setup(){
            Profiler fig_setup("fig_setup");
            cam.setup(width,height);
            contourFinder.setFindHoles(false);

            Mat cv_frame_mini(80, 60, CV_8UC1);
            Mat cv_frame(320, 240, CV_8UC1);
            
            gui.setup();
            gui.add(drawCamInput.set("draw_cam_input", true));
            gui.add(drawFlowImage.set("draw_flow_image", true));
            gui.add(flowBlur.set("do_blur", false));
            gui.add(flowBlurDist.set("blur_dist", 10, 1, 25));
            gui.add(diffGrowIter.set("diff_grow_iter", 1, 0, 10));
            gui.add(contoursActive.set("contours_active", false));
            gui.add(contourShowLabels.set("contour_show_labels", true));
            gui.add(contourThreshold.set("contour_threshold", 150, 1, 255));
            gui.add(contourMinRadius.set("contour_min_radius", 10.0f, 1.0f, 60.0f));
            gui.add(contourMaxRadius.set("contour_max_radius", 90.0f, 40.0f, 120.0f));
            gui.add(contourOutlineWidth.set("contour_outline_width", 2.0f, 0.5f, 30.0f));

        }

        void update(){
            cam.update();

            if(cam.isFrameNew()){
                
                contourFinder.setMinAreaRadius(contourMinRadius);
                contourFinder.setMaxAreaRadius(contourMaxRadius);
                contourFinder.setThreshold(contourThreshold);

                fb.calcOpticalFlow(cam);
                cv_frame = fb.getFlowAsAlpha();
                // if(flowBlur){
                //     GaussianBlur(cv_frame, cv_frame, flowBlurDist);
                // }
                // Downscale for contour detection
                if(contoursActive){
                    cv::resize(cv_frame, cv_frame_mini, cv::Size(80,60), 0, 0, INTER_CUBIC);
                    cv::Mat kernel(Size(3,3), CV_8UC1);
                    cv::dilate(cv_frame_mini, cv_frame_mini, kernel, cv::Point(-1,-1), diffGrowIter);
                    contourFinder.findContours(cv_frame_mini);
                }
            }
        }

        void draw(int x, int y){
            if(drawCamInput){
                ofDrawBitmapStringHighlight("Camera Input:", x, y-6);
                cam.draw(x,y);
            }
            if(drawFlowImage){
                ofDrawBitmapStringHighlight("Flow Input:", x, y-6+250);
                drawMat(cv_frame,x,y+250);
                drawMat(cv_frame_mini,x-320,y+250);
            }
          

            if(contoursActive){

                RectTracker& tracker = contourFinder.getTracker();
                
                ofSetColor(255,0,0,125);
                ofSetLineWidth(contourOutlineWidth);
                ofPushMatrix();
                ofTranslate(x,y+250);
                ofScale(4,4);
                for(int i = 0; i < contourFinder.size(); i++){
                    ofPoint center = toOf(contourFinder.getCenter(i));

                    int label = contourFinder.getLabel(i);
                    string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
                    ofDrawBitmapString(msg, center);

                    contourFinder.getPolyline(i).draw();                
                    //ofDrawRectangle(ofxCv::toOf(contourFinder.getBoundingRect(i)));
                    //ofDrawCircle(contourFinder.getCentroid(i).x,contourFinder.getCentroid(i).y, 0, contourFinder.getContourArea(i));
                }
                ofPopMatrix();
            }
            
            gui.draw();
        }

        unsigned int size() const {
            return contourFinder.size();
        }

        const std::vector<ofPolyline>& getPolylines(){
            return contourFinder.getPolylines();
        }

        const std::vector<glm::vec3> getContour(int index){
            std::vector<cv::Point> vp = contourFinder.getContour(index);
            std::vector<glm::vec3> retval(vp.size());

            for(int i = 0; i < retval.size(); i++){
                retval[i].x = vp[i].x;
                retval[i].y = vp[i].y;
            }
            return retval;
        }

        const std::vector<glm::vec2>& getCentroids(){
            std::vector<glm::vec2> retval(contourFinder.size());
            for(int i = 0; i < contourFinder.size(); i++){
                cv::Point p = contourFinder.getCentroid(i);
                retval[i].x = p.x;
                retval[i].y = p.y;
            }
            return retval;
        }
        
        const std::vector<glm::vec2>& getCenters(){
            std::vector<glm::vec2> retval(contourFinder.size());
            for(int i = 0; i < contourFinder.size(); i++){
                cv::Point p = contourFinder.getCenter(i);
                retval[i].x = p.x;
                retval[i].y = p.y;
            }
            return retval;
        } 
};

/*
    
            if(contoursActive){
                ofxCv::RectTracker& tracker = contourFinder.getTracker();
                if(contourShowLabels) {
                    ofSetColor(255);
                    contourFinder.draw();
                    for(int i = 0; i < contourFinder.size(); i++) {
                        ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
                        ofPushMatrix();
                        ofTranslate(x,y+250);
                        ofTranslate(center.x, center.y);
                        int label = contourFinder.getLabel(i);
                        string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
                        ofDrawBitmapString(msg, 0, 0);
                        ofVec2f velocity = ofxCv::toOf(contourFinder.getVelocity(i));
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
    */