#pragma once

#if defined (__APPLE__)

#include <sys/time.h>

namespace Y {
    
    // millisecond
    inline double getTimeInDouble(void) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (double)(tv.tv_sec % 1000) * 1000.0 + (double)tv.tv_usec / 1000.0;
    }
    
    // microsecond
    inline unsigned long int getTimeInUnsignedLongInt(void) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (((unsigned long int)tv.tv_sec % 1000) * 1000000) + (unsigned long int)tv.tv_usec;
    }
    
}

#else
#if defined _WIN64 | defined _WIN32

#include <sys/types.h>
#include <sys/timeb.h>

namespace Y {
    
    // millisecond
    inline double getTimeInDouble(void) {
        _timeb tv;
        _ftime64_s(&tv);
        return (double)(tv.time) + (double)(tv.millitm) * 0.001;
    }
    
    // microsecond
    inline unsigned long getTimeInUnsignedLongInt(void) {
        _timeb tv;
        _ftime64_s(&tv);
        return (unsigned long int)(tv.time) * 1000000 + (unsigned long int)(tv.millitm) * 1000;
    }
    
}

#endif
#endif
