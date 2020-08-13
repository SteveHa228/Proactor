//
//  CompletionEventQueue.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#ifndef CompletionEventQueue_h
#define CompletionEventQueue_h

#include <deque>
#include <memory>
#include <mutex>
#include <utility>

#include "../AsyncOperation/AsynchronousOperation.hpp"

namespace Proactor {
namespace CompletionEventQueue {

template<typename T>
class CompletionEventQueue : private std::deque<AsyncOperation::AsynchronousOperation<T>*>{
private:
    std::mutex mutex;
    unsigned int pendingOperation;
public:
    CompletionEventQueue() : std::deque<AsyncOperation::AsynchronousOperation<T>*>(), pendingOperation(0)
    {}
    
    virtual ~CompletionEventQueue()
    {
        this->clear();
    }
    
    AsyncOperation::AsynchronousOperation<T>* pop()
    {
        // Lock the queue
        std::lock_guard<std::mutex> locker(mutex);
        
        //get the first element
        AsyncOperation::AsynchronousOperation<T>* p = this->front();
        
        //Remove the first element from the queue
        this->pop_front();
        
        // Return the retrieved first element
        return p;
    }
    
    void push(AsyncOperation::AsynchronousOperation<T>* operation)
    {
        // Lock the queue
        std::lock_guard<std::mutex> locker(mutex);
        // Insert the element into the queue
        this->push_back(operation);
        
        //Update the counter of pending operations
        if(pendingOperation == 0)
            throw std::exception();
        
        --pendingOperation;
    }
    
    const size_t size()
    {
        std::lock_guard<std::mutex> locker(mutex);
        return std::deque<AsyncOperation::AsynchronousOperation<T>*>::size();
    }
    
    void incrementPendingOperation()
    {
        std::lock_guard<std::mutex> locker(mutex);
        ++pendingOperation;
    }
    
    bool arePendingOperation()
    {
        std::lock_guard<std::mutex> locker(mutex);
        return (pendingOperation == 0);
    }
};

}
}

#endif /* CompletionEventQueue_h */
