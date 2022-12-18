#pragma once
#include "Commons.h"
#include "mutex/MsgHandler.h"
#include "nodes/Node.h"

/// @brief Lock interface
class Lock : public MsgHandler {
public:
    Lock(int id, Node* sckManager) : MsgHandler(id, sckManager) {};
    virtual ~Lock() = default;

    virtual void requestCS() = 0;    
    virtual void releaseCS() = 0;
protected:
    /* Mutex only used to make thread wait */
    std::mutex mtx_Wait;
    std::condition_variable cv_Wait;   
};