//
//  InOutXMLLogger.hpp
//  FFDemo2
//
//  Created by Amit Jain on 07/11/17.
//
//

#ifndef InOutXMLLogger_hpp
#define InOutXMLLogger_hpp

#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <stdio.h>
#include <sys/time.h>
#include <boost/filesystem.hpp>
#include "ofxCv.h"
#include "Configuration.h"

using namespace std;

struct InOutLogHeader {
    int cameraId;
    int storeId;
    string appVer;
    time_t logStartTime;
    time_t logEndTime;
    int totalCustomerIn;
    int totalCustomerOut;
    int totalStaffIn;
    int totalStaffOut;
};

struct InOutRecord {
    int personId;
    bool isStaff;
    time_t dateTime;
    bool isIn;
    int lineNumber;
};

class InOutXMLLogger
{
public:
    //! Setup the Log Manager
    void setup(Configuration config);
    
    void update();
    
    //! Add a new Record
    void addInOutRecord(int count);
    
    //! Close the Log Manager
    void close();
    
private:
    string _storeFolder;
    int logLength_Mins;
    
    //Log file constructs
    InOutLogHeader logHeader;
    unsigned int personId;
    string currentFilePath;
    string logFName;
    vector<string> inOutXMLRecordStrs;
    ofstream opFile;
    
    //Helper methods
    string genFileNameForTime(time_t timeVal);
    void genOpFileObject(string fPName, ofstream &fObj);
    string stringForTime(time_t timeVal);
};


#endif /* InOutXMLLogger_hpp */
