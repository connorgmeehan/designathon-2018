#include "ofMain.h"

#ifdef INCLUDE_OLA_CLIENT_OLACLIENT_H_
#include <ola/DmxBuffer.h>
#include <ola/io/SelectServer.h>
#include <ola/Logging.h>
#include <ola/client/ClientWrapper.h>
#include <ola/Callback.h>

class ArtnetCommunicator {
    bool debug = true;
    ofFbo * debugFbo;
    std::string mAddress;
    int mPort = -1;
    int array_height = 16;
    int array_width = 50;
    std::vector<ola::DmxBuffer> dmxbuffers;
    ola::client::OlaClientWrapper wrapper;
     public:

        ArtnetCommunicator(){

            ola::InitLogging(ola::OLA_LOG_WARN, ola::OLA_LOG_STDERR);

            dmxbuffers.resize(array_height);

            for(auto & dmxbuffer : dmxbuffers){
                dmxbuffer.Blackout();
            }

             if (!wrapper.Setup()) {
                std::cerr << "Setup failed" << endl;
                ofExit(1);
            }


        }

        void sendFrame(ofFbo & fbo){
            ofPixels frame;
            fbo.readToPixels(frame);

            int fbo_step = 4;

            ola::client::SendDMXArgs dummy_args = ola::client::SendDMXArgs();
            for(int i = 0; i < dmxbuffers.size(); i++){
                dmxbuffers[i].Set( &frame.getPixels()[150*i*fbo_step], (unsigned int) 150 );
                wrapper.GetClient()->SendDMX(i, dmxbuffers.at(i), dummy_args);
            }
        }



};
#endif

#ifndef INCLUDE_OLA_CLIENT_OLACLIENT_H_

// STUB
class ArtnetCommunicator{

    ArtnetCommunicator(){
        ofLog() << " ArtnetCommunicator stub intitialised.  Wont be able to ";
    }

    void sendFrame(ofFbo & fbo){

    }

}
#endif