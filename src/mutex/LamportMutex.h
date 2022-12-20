#pragma once
#include "Commons.h"
#include "nodes/MsgHandler.h"
#include "mutex/Lock.h"
#include "Log.h"
#include "mutex/DirectClock.h"


class LamportMutex : public Lock {
public:
    LamportMutex() = delete;
    LamportMutex(int id, MsgHandler* msgHandler);
    virtual ~LamportMutex();

    void requestCS() override;
    void releaseCS() override;

    void HandleMsg(int message, int src, Tag tag);          /* Used for current level processes */
private:
    bool okeyCS();
    
private:
    int m_Id;
    DirectClock m_Clock;
    std::unordered_map<int, int> m_RequestQ;

    MsgHandler* m_MsgHandler;
    std::mutex mtx_RequestQ;
};
