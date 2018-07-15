#include "ofMain.h"

class Profiler{
    public:
        string message;
        uint32_t time_start;
        Profiler(std::string _message){
            message = _message;
            time_start = ofGetElapsedTimeMicros();
        }
        ~Profiler(){
            ofLog() << "Profiler[ "<<message<<" ] total time: " << ofGetElapsedTimeMicros() - time_start; 
        }
};