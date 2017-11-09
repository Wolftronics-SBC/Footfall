// * Name: ofApp.cpp
// * Project: Footfall
// * Author: David Haylock
// * Creation Date: 13/02/2017
// * Copyright: (c) 2017 by Watershed Arts Trust Ltd.


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	cout << "-------------------- Footfall --------------------" << endl;
	
	configManager.loadConfiguration("config.json");
    cout<<"Config version: "<<configManager.getConfiguration().otherConfig.configVersion<<endl;
	
	_logToCsv = configManager.getConfiguration().useCsvLogging;
	_logToServer = configManager.getConfiguration().useHttp;
    _recordingEnabled = configManager.getConfiguration().vidRecordingEnabled;
    
    cout<<"Recording Enabled: "<<_recordingEnabled<<endl;
	
	cameraManager.setup(configManager.getConfiguration().cameraConfig);
	trackingManager.setup(configManager.getConfiguration().trackingConfig);
	
	if (_logToServer) httpManager.setup(configManager.getConfiguration().httpConfig);
	if (_logToCsv) csvManager.setup("csvlogs");
    if (_recordingEnabled) recorder.setup(configManager.getConfiguration().recordingConfig);
    inOutLogger.setup(configManager.getConfiguration());
    frmRateCounter.setup();
	
	ofAddListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofAddListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::exit()
{
	if (_logToServer) httpManager.close();
	if (_logToCsv) csvManager.close();
    if (_recordingEnabled) recorder.close();
    inOutLogger.close();
    frmRateCounter.close();
    
	ofRemoveListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofRemoveListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::update()
{
	cameraManager.update();
	trackingManager.update(cameraManager.getImage());
    if (_recordingEnabled) recorder.write(cameraManager.videoMatrix);
    inOutLogger.update();
    frmRateCounter.update();
}
//--------------------------------------------------------------
void ofApp::draw()
{
	ofPushMatrix();
	ofScale(1,1);
	cameraManager.draw();
	trackingManager.draw();
	ofPopMatrix();
	
	stringstream ss;
	ss << "Footfall" << endl;
	ss << "People In: " << peopleIn;
	ss << " People Out: " << peopleOut;
	ss << " Tally: " << (peopleIn-peopleOut);
	ss << " FPS: " << ofGetFrameRate() << endl;
	ofDrawBitmapStringHighlight(ss.str(),7,ofGetHeight()-20);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}
//--------------------------------------------------------------
void ofApp::blobIn(int &val)
{
	peopleIn += val;
	cout << val << " Blob(s) Came In" << endl;
	
	if (_logToServer) httpManager.post(ofToString(val));
	if (_logToCsv) csvManager.addRecord(ofToString(val), ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
    inOutLogger.addInOutRecord(val);
}
//--------------------------------------------------------------
void ofApp::blobOut(int &val)
{
	peopleOut += abs(val);
	cout << val << " Blob(s) Went Out" << endl;
	
	if (_logToServer) httpManager.post(ofToString(val));
	if (_logToCsv) csvManager.addRecord(ofToString(val), ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
    inOutLogger.addInOutRecord(val);
}
//--------------------------------------------------------------
void ofApp::takePhotoForCalibration() {
    cout<<"Taking photo for calibration"<<endl;
    string imgDirPath = configManager.getConfiguration().otherConfig.baseImagePath;
    string imgPath = imgDirPath + "calibPhoto.jpg";
    
    //Get current frame
    Mat img = cameraManager.videoMatrix;
    
    //Resize Image according to recording size
    Recording_Configuration recdConfig;
    recdConfig = configManager.getConfiguration().recordingConfig;
    int frameW,frameH;
    frameW = recdConfig.frameWidth;
    frameH = recdConfig.frameHeight;
    Mat img_resized;
    resize(img,img_resized,cv::Size(frameW,frameH));
    img = img_resized;
    
    //Mark In/Out indicator
    int fontFace = FONT_HERSHEY_PLAIN;
    double fontScale = 2;
    int thickness = 2;
    int padding = 5;
    {
        string text = "In";
        int baseline = 0;
        cv::Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
        // center the text
        cv::Point textOrg((img.cols - textSize.width)/2, textSize.height + padding);
        putText(img, text, textOrg, fontFace, fontScale, Scalar(0,200,0), thickness, 8);
    }
    {
        string text = "Out";
        int baseline = 0;
        cv::Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
        // center the text
        cv::Point textOrg((img.cols - textSize.width)/2, img.rows - padding);
        putText(img, text, textOrg, fontFace, fontScale, Scalar(0,0,200), thickness, 8);
    }
    
    //Write Image
    imwrite(imgPath,img);
    
    cout<<"Photo for calibration written at Path: "<<imgPath<<endl;
}
