//
//  AsynchronousOperationProcessor.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#ifndef AsynchronousOperationProcessor_h
#define AsynchronousOperationProcessor_h

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <deque>

#include "../AsyncOperation/AsynchronousOperation.hpp"
#include "../CompletionEventQueue/CompletionEventQueue.hpp"
#include "../Logger/Logger.hpp"
#include "../Observer/Observer.hpp"

namespace Proactor {
namespace AsyncOperationProcessor {

template<typename T>
class AsynchronousOperationProcessor : public Observer::Observer<AsyncOperation::AsynchronousOperation<T>>
{
private:
    size_t poolSize;
    std::mutex mutex;
    std::condition_variable cv;
    std::deque<AsyncOperation::AsynchronousOperation<T>*> pool;
    std::shared_ptr<CompletionEventQueue::CompletionEventQueue<T>> completionEventQueue;
public:
    static const size_t DEFAULT_QUEUE_SIZE = 3;
    //construction
    AsynchronousOperationProcessor(std::shared_ptr<CompletionEventQueue::CompletionEventQueue<T>>& completionEventQueue, const size_t size = DEFAULT_QUEUE_SIZE) : poolSize(size), pool(std::deque<AsyncOperation::AsynchronousOperation<T>*>()),
        completionEventQueue(completionEventQueue){};
    
    //destruction
    virtual ~AsynchronousOperationProcessor() {};
    
    // Add an operation to the execution queue
    void AddOperation(AsyncOperation::AsynchronousOperation<T>* operation)
    {
        std::unique_lock<std::mutex> locker(mutex);
        if(pool.size() == poolSize)
            cv.wait(locker, [&]{return pool.size() < poolSize;});
        
        operation->setObserver(this);
        completionEventQueue->incrementPendingOperation();
        
        //start the operation in a new thread
        std::thread t = std::thread(&AsyncOperation::AsynchronousOperation<T>::execute, operation);
        t.detach();
        
        pool.push_back(operation);
    }
    
    // Notify the class that an operation has been completed
    void notify(AsyncOperation::AsynchronousOperation<T>* operation)
    {
        std::unique_lock<std::mutex> locker(mutex);
        completionEventQueue->push(operation);
        bool isFull = false;
        typedef typename std::deque<AsyncOperation::AsynchronousOperation<T>*>::iterator iterType;
        iterType iter = std::find(pool.begin(), pool.end(), operation);
        if(iter != pool.end())
        {
            isFull = pool.size() == poolSize;
            pool.erase(iter);
        }
        if(isFull)
            cv.notify_one();
    }
};

}
}

#endif /* AsynchronousOperationProcessor_h */
