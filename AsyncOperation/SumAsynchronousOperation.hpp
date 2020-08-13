//
//  SumAsynchronousOperation.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//
#include <list>

#include "AsynchronousOperation.hpp"
#include "../Exception/OperationNotFinishedException.hpp"

#ifndef SumAsynchronousOperation_h
#define SumAsynchronousOperation_h

namespace Proactor {
namespace AsyncOperation {

template <typename T>
class SumAsynchronousOperation : public AsynchronousOperation<T> {
private:
    std::list<T> elements;
public:
    SumAsynchronousOperation(std::list<T> numbers)
    {
        elements = numbers;
        SumAsynchronousOperation<T>::executed = false;
    }
    
    SumAsynchronousOperation(T element) : SumAsynchronousOperation(std::list<T>({element})){};
    
    SumAsynchronousOperation(T element, T element1) : SumAsynchronousOperation(std::list<T>({element,element1})){};
    
    ~SumAsynchronousOperation()
    {
        elements.clear();
    }
    
    void executeOperation()
    {
        SumAsynchronousOperation<T>::result = 0;
        for(T element : elements)
            SumAsynchronousOperation<T>::result += element;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand()%10000));
        SumAsynchronousOperation<T>::executed = true;
    }
    
    T getResult() const {
        if(AsynchronousOperation<T>::executed)
            return AsynchronousOperation<T>::result;
        throw Proactor::Exception::OperationNotFinishedException();
    }
};

}
}

#endif /* SumAsynchronousOperation_h */
