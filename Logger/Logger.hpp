//
//  Logger.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//
//note : if use static mutex will not thread-safe with multi-thread and have optimal effect because just have one instance of mutex
//          for all logger instance.
#ifndef Logger_h
#define Logger_h

#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>

#include "../Utils/Utils.hpp"

namespace Proactor {
namespace Logger {

class Logger{
private:
    //static std::mutex mutex;
    //construction
    Logger(){};
public:
    static void log(const std::string &message, std::ostream& ostr = std::cout)
    {
        // Lock
        std::mutex mutex; //internal init cause static func 
        std::lock_guard<std::mutex> locker(mutex);
        // Put and flush
        ostr << message << std::endl <<std::flush;
    };
    
    static void log(const std::stringstream& message, std::ostream& ostr = std::cout)
    {
        log(message.str(), ostr);
    };
    
    static void log(const std::string& message,
                    const long long operationId,
                    const std::thread::id threadID,
                    const std::chrono::system_clock::time_point& time)
    {
        log(message + Proactor::Utils::Utils::tostr(operationId) +
            "\t[Thread: " + Proactor::Utils::Utils::tostr(threadID) + "](" +
            Proactor::Utils::Utils::dateToString(time) + ")");
    };
};

}
}

#endif /* Logger_h */
