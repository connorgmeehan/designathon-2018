#pragma once

#include "ofMain.h"

#include "ofxCv.h"
#include "ofxOpenCv.h"

using namespace ofxCv;
using namespace cv;
using namespace glm;

const float dyingTime = 1;

class Figure : public ofxCv::RectFollower{
    public:
        ofColor color;
        ofVec2f cur, smooth;
        ofPolyline all;
        float startedDying;

        Figure() : startedDying(0) {

        }
        void setup(const cv::Rect& track){
            color.setHsb(ofRandom(0, 255), 255, 255);
            cur = toOf(track).getCenter();
            smooth = cur;
        }

        void update(const cv::Rect& track){
            cur = toOf(track).getCenter();
            smooth.interpolate(cur, .5);
            all.addVertex(smooth.x, smooth.y, 0);
        }

        void kill(){
            float curTime = ofGetElapsedTimef();
            if(startedDying == 0) {
                startedDying = curTime;
            } else if(curTime - startedDying > dyingTime) {
                dead = true;
            }
        }

        void draw(){
            ofDrawCircle(cur, 5);
        }

};

class ContourFigureDetection{
    public:
        int width = 320; int height = 240;
        ofVideoGrabber cam;
        cv::Mat cv_frame, cv_background, cv_diff, cv_diff_mini;
        ofxCv::ContourFinder contourFinder;
        ofImage frame, diff;
        ofImage thresholded, thresholded_mini;

        ofxCv::RunningBackground background;
        ofxCv::RectTrackerFollower<Figure> tracker;

        ofxPanel gui;
        ofParameter<bool> drawCamInput;
        ofParameter<bool> drawBg;
        ofParameter<bool> drawDiffImage;
        ofParameter<int> diffThreshold;
        ofParameter<int> diffGrowIter;
        ofParameter<bool> contoursActive, drawContours;
        ofParameter<int> contourThreshold;
        ofParameter<float> contourMinRadius, contourMaxRadius, contourOutlineWidth;

        ofParameter<bool> resetBackground;
        ofParameter<float> learningTime, thresholdValue;
        ofParameter<bool> trackContours;
        ofParameter<int> trackingMemory;

        void setup(){
            cam.setup(width,height);
            contourFinder.setFindHoles(false);
            RunningBackground background;
            cv_frame = Mat(320,240,CV_8UC1);
            cv_diff = Mat(320, 240, CV_8UC1);

            frame.allocate(width,height,OF_IMAGE_GRAYSCALE);
            diff.allocate(width,height,OF_IMAGE_GRAYSCALE);

            gui.setup();
            gui.add(drawCamInput.set("draw_cam_input", true));
            gui.add(drawBg.set("draw_bg", true));
            gui.add(resetBackground.set("Reset Background", false));
            gui.add(learningTime.set("Learning Time", 100, 0, 30));
            gui.add(thresholdValue.set("Threshold Value", 10, 0, 255));            gui.add(diffThreshold.set("diff_threshold", 80, 0, 255));
            gui.add(diffGrowIter.set("diff_grow_itter", 1, 0, 10));
            gui.add(contoursActive.set("contours_active", false));
            gui.add(drawContours.set("draw_contours", false));
            gui.add(contourThreshold.set("contour_threshold", 80, 1, 255));
            gui.add(contourMinRadius.set("contour_min_radius", 2.0f, 1.0f, 60.0f));
            gui.add(contourMaxRadius.set("contour_max_radius", 90.0f, 40.0f, 120.0f));
            gui.add(contourOutlineWidth.set("contour_outline_width", 2.0f, 0.5f, 5.0f));
            gui.add(trackContours.set("track_contours", true));
            gui.add(trackingMemory.set("track_contours", 15, 0, 100));

            // wait for half a frame before forgetting something
            tracker.setPersistence(trackingMemory);
            // an object can move up to 50 pixels per frame
            tracker.setMaximumDistance(50);
        }

        void update(){

            if(resetBackground) {
                background.reset();
                resetBackground = false;
            }

            cam.update();

            if(cam.isFrameNew()){

                cv_frame = ofxCv::toCv(cam);

                contourFinder.setMinAreaRadius(contourMinRadius);
                contourFinder.setMaxAreaRadius(contourMaxRadius);
                contourFinder.setThreshold(contourThreshold);
                tracker.setPersistence(trackingMemory);

                background.setLearningTime(learningTime);
                background.setThresholdValue(thresholdValue);
                background.update(cv_frame, cv_diff);

                // Downscale for contour detection
                cv::resize(cv_diff, cv_diff_mini, cv::Size(80,60), 0, 0, INTER_CUBIC);
                cv::Mat kernel(Size(3,3), CV_8UC1);
                cv::dilate(cv_diff_mini, cv_diff_mini, kernel, cv::Point(-1,-1), diffGrowIter);


                if(contoursActive){
                    contourFinder.findContours(cv_diff_mini);
                }
                if(trackContours){
                    tracker.track(contourFinder.getBoundingRects());
                }

            }
        }

        void draw(int x, int y){
            ofDrawBitmapStringHighlight("ContourFigureDetection: ", x, y-10);

            if(drawCamInput){
                drawMat(cv_frame, x,y);
                ofDrawBitmapStringHighlight("Camera Input:", x, y+10);
            }
            if(drawDiffImage){
                drawMat(cv_diff,x, y+250);
                drawMat(cv_diff_mini,x-320, y+250);
            }

            

               ofSetColor(255,0,0,175);
                ofSetLineWidth(contourOutlineWidth);
                ofPushMatrix();
                ofTranslate(x,y+250);
                ofScale(4,4);
            if(drawContours && contoursActive){
 
                for(int i = 0; i < contourFinder.size(); i++){
                    contourFinder.getPolyline(i).draw();
                }
            }
            if(trackContours){
                std::vector<Figure>& followers = tracker.getFollowers();
                for(int i = 0; i < followers.size(); i++) {
                    followers[i].draw();
                }
            }
            ofPopMatrix();
            gui.draw();
        }

        void drawMat(cv::Mat & mat, int x, int y){
            ofImage toDraw;
            toDraw.allocate(mat.cols, mat.rows, OF_IMAGE_GRAYSCALE);
            ofxCv::toOf(mat, toDraw);
            toDraw.update();
            toDraw.draw(x,y);
        }
        
};