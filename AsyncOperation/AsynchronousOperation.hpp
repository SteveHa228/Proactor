//
//  AsynchronousOperation.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#ifndef AsynchronousOperation_h
#define AsynchronousOperation_h

#include "AsynchronousOperation.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>

#include "../Observer/Observer.hpp"
#include "../Logger/Logger.hpp"
#include "../Utils/Utils.hpp"

namespace Proactor {
namespace AsyncOperation {


template<typename T>
class AsynchronousOperation {

private :
    static unsigned long long operationId;
protected:
    const unsigned long long opId;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    bool executed;
    // Observer in order to that the given operation has finished its excution
    Observer::Observer<AsynchronousOperation<T>> *observer;
    T result;
    
    //constructor
    AsynchronousOperation() : opId(++operationId), startTime(), endTime(), executed(false),
    observer(nullptr), result()
    {}
    
    //execute virtual method
    virtual void executeOperation() = 0;
    
public:
    //set an observer to thee operation. The observer will be notified once the operation
    //has been finished.
    void setObserver(Observer::Observer<AsynchronousOperation<T>> * observer)
    {
        this->observer = observer;
    }
    
    //
    void execute()
    {
        //set the start time
        startTime = std::chrono::system_clock::now();
        Logger::Logger::log("\tStarting operation ",opId, std::this_thread::get_id(), startTime);
        
        //Use the template pattern
        executeOperation();
        
        //set the finish time
        endTime = std::chrono::system_clock::now();
        Logger::Logger::log("\tFinished operation ", opId, std::this_thread::get_id(), endTime);
        
        //Notify the observer, if defined
        if(observer != nullptr)
            observer->notify(this);
    }
    
    const unsigned int getID() const{
        return opId;
    }
    
    virtual T getResult() const = 0;
    
    virtual ~AsynchronousOperation() {}
};

template<typename T>
unsigned long long AsynchronousOperation<T>::operationId = 0;

}
}

#endif /* AsynchronousOperation_h */
