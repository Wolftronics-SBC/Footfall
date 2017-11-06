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
	ofSetFrameRate(25);
	
	cout << "-------------------- Footfall --------------------" << endl;
	
	configManager.loadConfiguration("config.json");
	
	_logToCsv = configManager.getConfiguration().useCsvLogging;
	_logToServer = configManager.getConfiguration().useHttp;
    _recordingEnabled = configManager.getConfiguration().vidRecordingEnabled;
	
	cameraManager.setup(configManager.getConfiguration().cameraConfig);
	trackingManager.setup(configManager.getConfiguration().trackingConfig);
	
	if (_logToServer) httpManager.setup(configManager.getConfiguration().httpConfig);
	if (_logToCsv) csvManager.setup("csvlogs");
    if (_recordingEnabled) recorder.setup(configManager.getConfiguration().recordingConfig);
	
	ofAddListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofAddListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::exit()
{
	if (_logToServer) httpManager.close();
	if (_logToCsv) csvManager.close();
    if (_recordingEnabled) recorder.close();
    
	ofRemoveListener(trackingManager.blobIn, this, &ofApp::blobIn);
	ofRemoveListener(trackingManager.blobOut, this, &ofApp::blobOut);
}
//--------------------------------------------------------------
void ofApp::update()
{
	cameraManager.update();
	trackingManager.update(cameraManager.getImage());
    if (_recordingEnabled) recorder.write(cameraManager.videoMatrix);
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
}
//--------------------------------------------------------------
void ofApp::blobOut(int &val)
{
	peopleOut += abs(val);
	cout << val << " Blob(s) Went Out" << endl;
	
	if (_logToServer) httpManager.post(ofToString(val));
	if (_logToCsv) csvManager.addRecord(ofToString(val), ofGetTimestampString("%Y-%m-%d %H:%M:%S"));
}
//--------------------------------------------------------------
void ofApp::takePhotoForCalibration() {
    cout<<"Taking photo for calibration"<<endl;
    string imgDirPath = configManager.getConfiguration().otherConfig.baseImagePath;
    string imgPath = imgDirPath + "calibPhoto.jpg";
    
    Mat img = cameraManager.videoMatrix;
    imwrite(imgPath,img);
    
    cout<<"Photo for calibration written at Path: "<<imgPath<<endl;
}
