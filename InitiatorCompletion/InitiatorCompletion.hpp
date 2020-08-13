//
//  InitiatorCompletion.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#ifndef InitiatorCompletion_h
#define InitiatorCompletion_h


#include <future>
#include <memory>
#include <thread>

#include "../AsyncOperationProcessor/AsynchronousOperationProcessor.hpp"
#include "../CompletionEventQueue/CompletionEventQueue.hpp"
#include "../Logger/Logger.hpp"
#include "../Proactor/Proactor.hpp"
#include "../Utils/Utils.hpp"

namespace Proactor {
namespace InitiatorCompletion {

template <typename T>
class InitiatorCompletion : public Observer::Observer<AsyncOperation::AsynchronousOperation<T> > {
    
private:
    std::shared_ptr<CompletionEventQueue::CompletionEventQueue<T>> completionEventQueue;
    std::shared_ptr<AsyncOperationProcessor::AsynchronousOperationProcessor<T>> asynchronousOperationProcessor;
    std::unique_ptr<Proactor::Proactor<T> > proactor;
    std::future<void> proactorThread;
public:
    InitiatorCompletion() : completionEventQueue(std::shared_ptr<CompletionEventQueue::CompletionEventQueue<T>>(new CompletionEventQueue::CompletionEventQueue<T>())),
        asynchronousOperationProcessor(std::make_shared<AsyncOperationProcessor::AsynchronousOperationProcessor<T>>(completionEventQueue)),
        proactor(std::unique_ptr<Proactor::Proactor<T>>(new Proactor::Proactor<T>(completionEventQueue, this)))
    {
        //start proactor
        proactorThread = std::async(std::launch::async, &Proactor::Proactor<T>::exec, proactor.get());
    };
    
    //destruction
    
    virtual ~InitiatorCompletion()
    {
        //verify that all operations have been processed
        while(completionEventQueue->size() != 0)
        {
            Logger::Logger::log("waiting for last element...: " + Utils::Utils::tostr(completionEventQueue->size()));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        proactor->canFinish(true);
        proactorThread.wait();
        Logger::Logger::log("Finished InitiatorCompletion.");
    }
    
    void processOperation(AsyncOperation::AsynchronousOperation<T> *operation)
    {
        Logger::Logger::log("Initiating operation "+ Utils::Utils::tostr(operation->getID()) + "... ");
        asynchronousOperationProcessor->AddOperation(operation);
    }
    
    void notify(AsyncOperation::AsynchronousOperation<T>* operation)
    {
        Logger::Logger::log("Notified in Initiator/Completion - id :" + Utils::Utils::tostr(operation->getID()) + " - Result operation: " +
                            Utils::Utils::tostr(operation->getResult()));
    }
};

}  //
}

#endif /* InitiatorCompletion_h */
