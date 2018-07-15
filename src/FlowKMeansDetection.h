#pragma once

#include "ofMain.h"

#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "Profiler.h"

using namespace ofxCv;
using namespace cv;
using namespace glm;
class FigureDetection {
    public:

        const double MHI_DURATION = 5;
        const double MAX_TIME_DELTA = 0.5;
        const double MIN_TIME_DELTA = 0.05;

        int width = 320;
        int height = 240;
        ofVideoGrabber cam;

        vector<Mat> buf;
        int last = 0;

        Mat mhi, orient, mask, segmask, zplane;
        vector<Rect> regions;

        Mat motion;

        ofxPanel gui;
        ofParameter<int> diff_threshold;
        uint32_t time, last_time;

        void setup(){
            cam.setup(width,height);
            buf.resize(2);
            gui.add(diff_threshold.set("diff_threshold", 30, 0, 100));
        }

        void update(){
            cam.update();

            if(cam.isFrameNew()){
                double timestamp = (double)clock() / CLOCKS_PER_SEC; // get current time in seconds
                Size size = Size(cam.getWidth(), cam.getHeight()); 
                int i, idx1 = last;
                Rect comp_rect;
                double count;
                double angle;
                Point center;
                double magnitude;
                Scalar color;

                // allocate images at the beginning or
                // reallocate them if the frame size is changed
                if (mhi.size() != size)
                {
                    mhi = Mat::zeros(size, CV_32F);
                    zplane = Mat::zeros(size, CV_8U);

                    buf[0] = Mat::zeros(size, CV_8U);
                    buf[1] = Mat::zeros(size, CV_8U);
                }
                Mat frame = ofxCv::toCv(cam);
                cvtColor(frame, buf[last], COLOR_BGR2GRAY); // convert frame to grayscale

                int idx2 = (last + 1) % 2; // index of (last - (N-1))th frame
                last = idx2;

                Mat silh = buf[idx2];
                absdiff(buf[idx1], buf[idx2], silh); // get difference between frames

                cv::threshold(silh, silh, diff_threshold, 1, THRESH_BINARY); // and threshold it
                updateMotionHistory(silh, mhi, timestamp, MHI_DURATION);
                // convert MHI to blue 8u image
                mhi.convertTo(mask, CV_8U, 255. / MHI_DURATION, (MHI_DURATION - timestamp)*255. / MHI_DURATION);

                Mat planes[] = { mask, zplane, zplane };
                merge(planes, 3, dst);

                // calculate motion gradient orientation and valid orientation mask
                cv::calcMotionGradient(mhi, mask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3);

                // segment motion: get sequence of motion components
                // segmask is marked motion components map. It is not used further
                regions.clear();
                cv::segmentMotion(mhi, segmask, regions, timestamp, MAX_TIME_DELTA);
            }
        }

        void draw(int x, int y){
            drawMat(mhi,0,0);
            drawMat(segmask,320,0);
            drawMat(orient, 0, 240);
            drawMat(zplane, 320,240);
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