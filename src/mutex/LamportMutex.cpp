#include "mutex/LamportMutex.h"

template<typename... Params>
static void QueueErase(std::priority_queue<Params...>& queue, std::pair<int,int> element);
static bool isGreater(int entry1, int id1, int entry2, int id2);

LamportMutex::LamportMutex(int id, MsgHandler* msgHandler)
   : m_Clock(id), m_MsgHandler(msgHandler), m_Id(id)
{}   

LamportMutex::~LamportMutex()
{
    /* Notify all other porcesses that we have finished */
    LOG_WARN("Deleting lamport mutex.");
}

void LamportMutex::requestCS()
{
    LOG_WARN("LamportMutex request CS. ID: {}", m_Id);
    m_Clock.Tick();
    int ticks = m_Clock.GetValue(m_Id);
    m_RequestQ[m_Id] = ticks;
    m_MsgHandler->BroadcastMsg(Tag::REQUEST, ticks, CORE_LAYER);
    std::unique_lock<std::mutex> lck(mtx_Wait);
    cv_Wait.wait(lck, [&](){return this->okeyCS();});   // Fix
}


void LamportMutex::releaseCS()
{
    LOG_INFO("LamportMutex releasing mutex. ID: {}", m_Id);
    m_RequestQ.erase(m_Id);
    m_MsgHandler->BroadcastMsg(Tag::RELEASE, m_Clock.GetValue(m_Id), CORE_LAYER);
}




bool LamportMutex::okeyCS()
{   
    /* We look if we are the best candidate in the priority queue */
    LOG_WARN("Checking CS. ID: {}", m_Id);
    std::lock_guard<std::mutex> lck(mtx_RequestQ);
    if (m_RequestQ.find(m_Id) != m_RequestQ.end())
    {
        int myTicks = m_RequestQ[m_Id];
        for (auto otherId : m_MsgHandler->GetLayer(CORE_LAYER))
        {
            if (m_RequestQ.find(otherId) != m_RequestQ.end())
            {
                if (isGreater(myTicks, m_Id, m_RequestQ[otherId], otherId))
                    return false;
            }
            if (isGreater(myTicks, m_Id, m_Clock.GetValue(otherId), otherId))    
                return false;
        }
    }
    LOG_INFO("Entering mutex area. ID: {}", m_Id);
    return true;
}   

template<typename... Params>
static void QueueErase(std::priority_queue<Params...>& queue, std::pair<int,int> element)
{
    std::priority_queue<Params...> newQueue;
    while (!queue.empty())
    {
        std::pair<int,int> move = queue.top();
        queue.pop();
        if (element.second != move.second)
            newQueue.push(move);
    }
    queue = newQueue;
}

static bool isGreater(int entry1, int id1, int entry2, int id2)
{
    return entry1 > entry2 || (entry1 == entry2 && id1 > id2);
}

void LamportMutex::HandleMsg(int message, int src, Tag tag)
{
    LOG_TRACE("Message, tag: {}, ticks {}, from {}", (char)tag, message, src);
    m_Clock.RecieveAction(src,message);
    switch (tag)
    {
    /* Lamport mutex */
    case Tag::REQUEST:
        m_RequestQ[src] = message;        
        m_MsgHandler->SendMsg(src, Tag::ACK, m_Clock.GetValue(m_Id));
        break;
    case Tag::RELEASE:
        {
            std::lock_guard<std::mutex> lck(mtx_RequestQ);
            m_RequestQ.erase(src);
        }
        break;
    default:
        break;
    }
    cv_Wait.notify_all();
}   