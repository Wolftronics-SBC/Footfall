//
//  FrameRateCounter.cpp
//  FFDemo2
//
//  Created by Amit Jain on 09/11/17.
//
//

#include "FrameRateCounter.hpp"

using namespace std;

//! Setup
void FrameRateCounter::setup() {
    intervalLength_Mins = 0.5;
    
    count = 0;
    time(&t_start);
}

void FrameRateCounter::update() {
    
    count++;
    
    time_t now;
    time(&now);
    time_t diff = now - t_start;
    if(diff > intervalLength_Mins*60) {
        double fps = count / (double) diff;
        cout<<"Current FPS: "<<fps<<endl;
        
        //Reset counter
        count = 0;
        t_start = now;
    }
}

//Close
void FrameRateCounter::close() {
    time_t now;
    time(&now);
    time_t diff = now - t_start;
    
    double fps = count / (double) diff;
    cout<<"Current FPS: "<<fps<<endl;
}
