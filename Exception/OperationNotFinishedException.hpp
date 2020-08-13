//
//  OperationNotFinishedException.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#include <exception>
#include <iostream>

#ifndef OperationNotFinishedException_h
#define OperationNotFinishedException_h

#include "../Logger/Logger.hpp"

namespace Proactor {
namespace Exception {

class OperationNotFinishedException : public std::exception{
private:
    std::string message;
public:
    OperationNotFinishedException() : std::exception(), message("")
    {};
    OperationNotFinishedException(std::string message) : std::exception(), message(message)
    {
        Logger::Logger::log(message);
    }
    
    const std::string getMessage() const {
        return this->message;
    }
};

}//
}


#endif /* OperationNotFinishedException_h */
