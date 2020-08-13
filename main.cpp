//
//  main.cpp
//  ProActorPattern
//
//  Created by HDTung on 8/12/20.
//  Copyright © 2020 HDTung. All rights reserved.
//


#include <iostream>
#include <memory>

#include "AsyncOperation/AsynchronousOperation.hpp"
#include "AsyncOperation/SumAsynchronousOperation.hpp"
#include "AsyncOperationProcessor/AsynchronousOperationProcessor.hpp"
#include "InitiatorCompletion/InitiatorCompletion.hpp"
#include "Logger/Logger.hpp"
#include "Proactor/Proactor.hpp"

using namespace Proactor::AsyncOperation;

int main(int argc, const char * argv[]) {
    // Create operations
    SumAsynchronousOperation<int> op1 = SumAsynchronousOperation<int>(10, 11);
    SumAsynchronousOperation<int> op2 = SumAsynchronousOperation<int>(20, 21);
    SumAsynchronousOperation<int> op3 = SumAsynchronousOperation<int>(30, 31);
    SumAsynchronousOperation<int> op4 = SumAsynchronousOperation<int>(40, 41);
    SumAsynchronousOperation<int> op5 = SumAsynchronousOperation<int>(50, 51);
    SumAsynchronousOperation<int> op6 = SumAsynchronousOperation<int>(60, 61);

    // Execute operations
    Proactor::Logger::Logger::log("Starting operations execution...");

    Proactor::InitiatorCompletion::InitiatorCompletion<int> initiator;
    initiator.processOperation(&op1);
    initiator.processOperation(&op2);
    initiator.processOperation(&op3);
    initiator.processOperation(&op4);
    initiator.processOperation(&op5);
    initiator.processOperation(&op6);

    Proactor::Logger::Logger::log("Done.");

    return 0;
}
