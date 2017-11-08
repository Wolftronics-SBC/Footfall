//
//  InOutXMLLogger.cpp
//  FFDemo2
//
//  Created by Amit Jain on 07/11/17.
//
//

#include "InOutXMLLogger.hpp"

void InOutXMLLogger::setup(Configuration config) {
    _storeFolder = config.otherConfig.csvOutputPath;
    logLength_Mins = config.recordingConfig.videoLength_Mins;
    
    
    if(!boost::filesystem::exists(_storeFolder)) {
        boost::filesystem::create_directory(_storeFolder);
    }
    
    //Reset logHeader
    logHeader.cameraId = config.otherConfig.cameraID;
    logHeader.storeId = config.otherConfig.storeID;
    logHeader.appVer = config.otherConfig.appVersion;
    logHeader.totalCustomerIn = 0;
    logHeader.totalCustomerOut = 0;
    logHeader.totalStaffIn = 0;
    logHeader.totalStaffOut = 0;
    
    personId = 0;
    
    time(&logHeader.logStartTime);
    logFName = genFileNameForTime(logHeader.logStartTime);
    genOpFileObject(logFName, opFile);
    cout<<"Log file for In/Out created: "<<logFName<<endl;
}

void InOutXMLLogger::update() {
    time_t now;
    time(&now);
    if(now - logHeader.logStartTime > (logLength_Mins * 60)) {
        logHeader.logEndTime = now;
        opFile.close();
        
        //Create final file
        size_t pos;
        string val;
        int markerLength = 4;
        string finalFPath = currentFilePath;
        pos = finalFPath.find(".tmp");
        val = ".log";
        finalFPath = finalFPath.replace(pos, markerLength, val);
        
        //Generate content for final file
        string xmlHeaderTemplate = "<Coordinates><CameraID>#1#</CameraID><StoreID>#2#</StoreID><AppVer>#3#</AppVer><CaptureStartDateTime>#4#</CaptureStartDateTime><CaptureEndDateTime>#5#</CaptureEndDateTime><TotalCustomerInCount-Line1>#6#</TotalCustomerInCount-Line1><TotalCustomerOutCount-Line1>#7#</TotalCustomerOutCount-Line1><TotalStaffInCount-Line1>#8#</TotalStaffInCount-Line1><TotalStaffOutCount-Line1>#9#</TotalStaffOutCount-Line1>";
        string xmlEndTag = "</Coordinates>";
        string xmlHeader = xmlHeaderTemplate;
        markerLength = 3;
        
        pos = xmlHeader.find("#1#");
        val = to_string(logHeader.cameraId);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#2#");
        val = to_string(logHeader.storeId);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#3#");
        val = logHeader.appVer;
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#4#");
        val = stringForTime(logHeader.logStartTime);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#5#");
        val = stringForTime(logHeader.logEndTime);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#6#");
        val = to_string(logHeader.totalCustomerIn);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#7#");
        val = to_string(logHeader.totalCustomerOut);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#8#");
        val = to_string(logHeader.totalStaffIn);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        pos = xmlHeader.find("#9#");
        val = to_string(logHeader.totalStaffOut);
        xmlHeader = xmlHeader.replace(pos, markerLength, val);
        
        ofstream fFile(finalFPath);
        fFile<<xmlHeader;
        
        for(int i=0; i<inOutXMLRecordStrs.size(); i++) {
            fFile<<inOutXMLRecordStrs[i];
        }
        
        fFile<<xmlEndTag;
        fFile.close();
        cout<<"Log file finalized for In/Out: "<<finalFPath<<endl;
        
        //Remove tmp file
        bool deleted = boost::filesystem::remove(currentFilePath);
        if(deleted) {
            cout<<"Temporary file successfully deleted, "<<currentFilePath<<endl;
        }else {
            cout<<"Failed to delete temporary file, "<<currentFilePath<<endl;
        }
        
        //Reset LogHeader
        logHeader.totalCustomerIn = 0;
        logHeader.totalCustomerOut = 0;
        logHeader.totalStaffIn = 0;
        logHeader.totalStaffOut = 0;
        
        personId = 0;
        
        //Generate new file
        time(&now);
        logHeader.logStartTime = now;
        logFName = genFileNameForTime(logHeader.logStartTime);
        genOpFileObject(logFName,opFile);
        cout<<"Log file for In/Out created: "<<logFName<<endl;
    }
}

void InOutXMLLogger::addInOutRecord(int count) {
    
    //Create In/Out records Objects for the count
    vector<InOutRecord> recds;
    int absCount  = abs(count);
    bool isIn = (count >= 0)? true:false;
    int lineNumber = 1;
    bool isStaff = false;
    for(int i=0; i<absCount; i++) {
        InOutRecord recdObj;
        recdObj.personId = ++personId;
        recdObj.isStaff = isStaff;
        time(&recdObj.dateTime);
        recdObj.isIn = isIn;
        recdObj.lineNumber = lineNumber;
        recds.push_back(recdObj);
    }
    
    //Update logHeader construcsts
    if(isIn && isStaff) logHeader.totalStaffIn++;
    else if(isIn && !isStaff) logHeader.totalCustomerIn++;
    else if(!isIn && isStaff) logHeader.totalStaffOut++;
    else if(!isIn && !isStaff) logHeader.totalCustomerOut++;
    
    //Create XML
    string xmlRecdTemplate = "<Person><Person_ID>#1#</Person_ID><IsStaff>#2#</IsStaff><InOutDateTime>#3#</InOutDateTime><InOrOut>#4#</InOrOut><LineNumber>#5#</LineNumber></Person>";
    vector<string> xmlRecds;
    for(int i=0; i<recds.size(); i++) {
        string xmlRecd = xmlRecdTemplate;
        
        int markerLength = 3;
        size_t pos;
        string val;
        
        pos = xmlRecd.find("#1#");
        val = to_string(recds[i].personId);
        xmlRecd = xmlRecd.replace(pos, markerLength, val);
        
        pos = xmlRecd.find("#2#");
        val = (recds[i].isStaff)?"YES":"NO";
        xmlRecd = xmlRecd.replace(pos, markerLength, val);
        
        pos = xmlRecd.find("#3#");
        val = stringForTime(recds[i].dateTime);
        xmlRecd = xmlRecd.replace(pos, markerLength, val);
        
        pos = xmlRecd.find("#4#");
        val = (recds[i].isIn)?"IN":"OUT";
        xmlRecd = xmlRecd.replace(pos, markerLength, val);
        
        pos = xmlRecd.find("#5#");
        val = to_string(recds[i].lineNumber);
        xmlRecd = xmlRecd.replace(pos, markerLength, val);
        
        xmlRecds.push_back(xmlRecd);
    }
    
    for(int i=0; i<xmlRecds.size(); i++) {
        opFile<<xmlRecds[i];
        inOutXMLRecordStrs.push_back(xmlRecds[i]);
    }
}

void InOutXMLLogger::close() {
    opFile.close();
}


//----------------Private Methods-------------------------

string InOutXMLLogger::genFileNameForTime(time_t timeVal) {
    struct tm * timeinfo;
    timeinfo = localtime(&timeVal);
    
    //Generate File Name
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", timeinfo);
    string fileName = string(buf);
    return fileName;
}

void InOutXMLLogger::genOpFileObject(string fPName, ofstream &fObj) {
    string fileName = _storeFolder + fPName + ".tmp";
    currentFilePath = fileName;
    fObj.open(fileName);
}

string InOutXMLLogger::stringForTime(time_t timeVal) {
    struct tm * timeinfo;
    timeinfo = localtime(&timeVal);
    
    //Generate File Name
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", timeinfo);
    string timeStr = string(buf);
    return timeStr;
}
