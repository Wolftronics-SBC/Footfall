//
//  FrameRateCounter.hpp
//  FFDemo2
//
//  Created by Amit Jain on 09/11/17.
//
//

#ifndef FrameRateCounter_hpp
#define FrameRateCounter_hpp

#include <iostream>
#include <stdio.h>
#include <sys/time.h>

class FrameRateCounter
{
public:
    //! Setup
    void setup();
    
    void update();
    
    //Close
    void close();
private:
    unsigned long count;
    time_t t_start;
    float intervalLength_Mins;
};

#endif /* FrameRateCounter_hpp */
