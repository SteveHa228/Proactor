//
//  Observer.hpp
//  ProActorPattern
//
//  Created by HDTung on 8/13/20.
//  Copyright © 2020 HDTung. All rights reserved.
//

#ifndef Observer_h
#define Observer_h

namespace Proactor {
namespace Observer {
template<typename T>
class Observer{
public:
    virtual void notify(T* operation) = 0;
    virtual ~Observer(){}
};

}// namespace observeer
}// namespace proactor


#endif /* Observer_h */
