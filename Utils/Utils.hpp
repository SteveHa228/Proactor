//
//  Utils.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//
#include <chrono>
#include <ctime>
#include <iomanip>
#include "sstream"
#include "string"

#ifndef Utils_h
#define Utils_h

namespace Proactor {
namespace Utils {

class Utils{
  
public:
    template<typename T>
    static std::string tostr(T t){
        std::stringstream os;
        os << t;
        return os.str();
    };
    
    static std::string dateToString(const std::chrono::system_clock::time_point& time)
    {
        const std::time_t convTime = std::chrono::system_clock::to_time_t(time);
        std::tm timetm = *std::localtime(&convTime);
        std::stringstream oss;
        oss << std::put_time(&timetm, "%c %Z");
        return oss.str();
    }
    
    static std::string dateToString(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end)
    {
        std::stringstream oss;
        oss << dateToString(start);
        oss << "- elapsed: " << diffTimeToString(start, end);
        return oss.str();
    }
    
    static int diffTimeToString(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end)
    {
        std::chrono::milliseconds milliStart = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch());
        std::chrono::milliseconds milliEnd = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch());
        return milliEnd.count() - milliStart.count();
    }
};

}
}


#endif /* Utils_h */
