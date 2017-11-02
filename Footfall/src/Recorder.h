// * Name: Recorder.h
// * Project: Footfall
// * Author: Amit Jain
// * Creation Date: 02/11/2017

#ifndef Recorder_h
#define Recorder_h

#include <stdio.h>
#include <sys/time.h>
#include "ofMain.h"
#include "ofxCv.h"

using namespace cv;

class Recorder
{
	public:
    
        //! Setup
		void setup();
	
		//! Update
        void write(Mat img);
    
        //! Shutdown
        void close();
	
	private:
    
        string genFileNameForTime(time_t timeVal);
        VideoWriter genVideoWriter(string fPName);
    
        //Video Writer Constructs
        time_t time_OVidFCreated;
        string vidFPName;
        VideoWriter vidWriter;
        unsigned int frmCount;
};

#endif /* CameraManager_h */
