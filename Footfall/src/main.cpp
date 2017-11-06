#include "ofMain.h"
#include "ofApp.h"
#include <signal.h>

ofApp *appInstance;

//========================================================================
int main( ){
	ofSetupOpenGL(320*3,300,OF_WINDOW);			// <-------- setup the GL context
    
    signal(SIGUSR1, handler);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
    appInstance = new ofApp();
	ofRunApp(appInstance);

}

void handler(int signal) {
    appInstance->takePhotoForCalibration();
}
