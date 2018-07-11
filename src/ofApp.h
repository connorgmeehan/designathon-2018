#pragma once
// Include oF
#include "ofMain.h"
// Addons
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
// Internal Classes
#include "ArtnetCommunicator.h"
#include "libs/ofxBlurShader.h"
#include "ofTessellator.h"

class ofApp : public ofBaseApp{
	public:

		int width = 320;
		int height = 240;

		ArtnetCommunicator artnet_communicator;
		ofImage grabbed;

		ofVideoGrabber cam;

		bool bLearnBackground = true;
		ofxCv::ContourFinder contourFinder;
		
		ofxCv::FlowFarneback fb;
		ofxBlurShader blur_shader;
		ofFbo flow_fbo;

		ofFbo fbo_out;
	
		// GUI
		ofxPanel gui;
		// My Params
		ofParameter<ofColor> flowColor;
		ofParameter<float> contourThreshold, contourMinArea, flowLineScale;
		ofParameter<int> fbStepSize, blurIterations, blurAmount, flowLineWidth;
		ofParameter<bool> contoursActive, contourShowLabels, blurFlow, preThreshold;
		
		ofParameter<float> fbPyrScale, lkQualityLevel, fbPolySigma;
		ofParameter<int> fbLevels, lkWinSize, fbIterations, fbPolyN, fbWinSize, lkMaxLevel, lkMaxFeatures, lkMinDistance;
		ofParameter<bool> fbUseGaussian, usefb;
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
