#pragma once
// Include oF
#include "ofMain.h"
// Addons
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
// Internal Classes
#include "ArtnetCommunicator.h"
#include "FlowFigureDetection.h"
// #include "ContourFigureDetection.h"
// #include "FlowKMeansDetection.h"

#define OLA_INSTALLED

class ofApp : public ofBaseApp{
	public:
		ArtnetCommunicator artnet_communicator;
		FlowFigureDetection fig_detection;
		// ContourFigureDetection fig_detection;
		// FigureDetection fig_detection;
		ofFbo fbo_out;
		
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
