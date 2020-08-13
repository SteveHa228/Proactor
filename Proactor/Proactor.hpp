//
//  Proactor.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#ifndef Proactor_h
#define Proactor_h

#include <condition_variable>
#include <memory>
#include <mutex>

#include "../CompletionEventQueue/CompletionEventQueue.hpp"
#include "../Logger/Logger.hpp"
#include "../Observer/Observer.hpp"
#include "../Utils/Utils.hpp"

namespace Proactor {
namespace Proactor {

static const unsigned short WAIT_TIME = 1000;

template <typename T>
class Proactor {
private:
    std::shared_ptr<CompletionEventQueue::CompletionEventQueue<T>> completionEventQueue;
    
    Observer::Observer<AsyncOperation::AsynchronousOperation<T>>* observer;
    bool finish;
public:
    
    Proactor(std::shared_ptr<CompletionEventQueue::CompletionEventQueue<T>> completionEventQueue, Observer::Observer<AsyncOperation::AsynchronousOperation<T>>* observer) :
    completionEventQueue(completionEventQueue),
    observer(observer),
    finish(false)
    {};
    
    virtual ~Proactor(){
        Logger::Logger::log("Finished Proactor.");
    }
    
    void canFinish(const bool finish)
    {
        this->finish = finish;
    }
    
    //
    void exec()
    {
        while (!finish || completionEventQueue->arePendingOperation()) {
            while ((completionEventQueue->size() == 0 && (!finish))) {
                std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
            }
            
            if(completionEventQueue->size() > 0)
            {
                Logger::Logger::log("Proactor remove element from queue (size " + Utils::Utils::tostr(completionEventQueue->size()) + ")...");
                
                AsyncOperation::AsynchronousOperation<T>* operation = completionEventQueue->pop();
                observer->notify(operation);
            }
        }
        Logger::Logger::log("Proactor execution finished.");
    }
};

}
}

#endif /* Proactor_h */
