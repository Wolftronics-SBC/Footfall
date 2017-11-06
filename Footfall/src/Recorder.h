// * Name: Recorder.h
// * Project: Footfall
// * Author: Amit Jain
// * Creation Date: 02/11/2017

#ifndef Recorder_h
#define Recorder_h

#include <stdio.h>
#include <sys/time.h>
#include "Configuration.h"
#include "ofMain.h"
#include "ofxCv.h"

using namespace cv;

class Recorder
{
	public:
    
        //! Setup
		void setup(Recording_Configuration _config);
	
		//! Update
        void write(Mat img);
    
        //! Shutdown
        void close();
	
	private:
    
        string genFileNameForTime(time_t timeVal);
        VideoWriter genVideoWriter(string fPName);
    
        //Recording parameters
        int recordingFPS;
        int recdFrameWidth,recdFrameHeight;
        int recdVidLength_Mins;
        string recdDirPath;
    
        //Video Writer Constructs
        time_t time_OVidFCreated;
        string vidFPName;
        VideoWriter vidWriter;
        unsigned int frmCount;
};

#endif /* CameraManager_h */
