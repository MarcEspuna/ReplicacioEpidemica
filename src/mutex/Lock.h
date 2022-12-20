#pragma once
#include "Commons.h"

/// @brief Lock interface
class Lock {
public:
    Lock() = default;
    virtual ~Lock() = default;

    virtual void requestCS() = 0;    
    virtual void releaseCS() = 0;
protected:
    /* Mutex only used to make thread wait */
    std::mutex mtx_Wait;
    std::condition_variable cv_Wait;   
};