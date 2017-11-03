// * Name: Recorder.cpp
// * Project: Footfall
// * Author: Amit Jain
// * Creation Date: 02/11/2017

#include "Recorder.h"

#define baseDirPath "/home/pi/workspace/vidStore/"
#define maxVidLength_Mins 30
#define oVidFrameW 352
#define oVidFrameH 288
#define oVidFPS 15

//--------------------------------------------------------------
void Recorder::setup()
{
	cout << "Setting Up Recorder"<<endl;
	
    //Initialize videoWriter constructs
    time(&time_OVidFCreated);
    frmCount = 0;
    vidFPName = genFileNameForTime(time_OVidFCreated);
    vidWriter = genVideoWriter(vidFPName);
    cout<<"New video file created: "<<vidFPName<<endl;
}

//--------------------------------------------------------------
void Recorder::write(Mat img)
{
    if((img.rows * img.cols > 0) && vidWriter.isOpened()) {
        
        //Vid Length Check
        time_t now;
        time(&now);
        double secs_elapsed = difftime(now,time_OVidFCreated);
        if(secs_elapsed > (maxVidLength_Mins * 60)) {
            //Log Current Video file details
            cout<<"Closing Video file with name: "<<vidFPName<<endl;
            cout<<"Time Elapsed: "<<secs_elapsed<<" secs"<<endl;
            cout<<"Number of Frames Written: "<<frmCount<<endl;
            cout<<"Avg recording FPS: "<<frmCount/(double)secs_elapsed<<endl;
            
            //Close the current video file
            frmCount = 0;
            vidWriter.release();
            cout<<"Video File closed"<<endl<<endl;
            
            //Spawning new video writer for new file
            time(&time_OVidFCreated);
            vidFPName = genFileNameForTime(time_OVidFCreated);
            vidWriter = genVideoWriter(vidFPName);
            cout<<"New video file created: "<<vidFPName<<endl;
        }
        
        //Resize image
        Mat img_resized;
        resize(img,img_resized,Size(oVidFrameW,oVidFrameH));
        
        //Write to Video
        vidWriter.write(img_resized);
        frmCount++;
    }else {
        cout<<"Frame not written"<<endl;
        cout<<"Rows: "<<img.rows<<", Cols: "<<img.cols<<endl;
    }
}

//--------------------------------------------------------------
void Recorder::close()
{
    if(vidWriter.isOpened()) {
        vidWriter.release();
    }
}

//--------------------------------------------------------------
string Recorder::genFileNameForTime(time_t timeVal) {
    struct tm * timeinfo;
    timeinfo = localtime(&timeVal);
    
    //Generate File Name
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", timeinfo);
    string fileName = string(buf);
    return fileName;
}

//--------------------------------------------------------------
VideoWriter Recorder::genVideoWriter(string fPName) {
    string vidFName = baseDirPath + fPName + ".avi";
    VideoWriter vw(vidFName,CV_FOURCC('H','2','6','4'),oVidFPS,Size(oVidFrameW,oVidFrameH),true);
    return vw;
}
