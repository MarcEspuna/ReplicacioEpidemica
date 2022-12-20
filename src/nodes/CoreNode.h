#pragma once

#include "nodes/Node.h"
#include "mutex/LamportMutex.h"

class CoreNode : public Node {
public:
    CoreNode(const std::string& name, int id);
    ~CoreNode();

    virtual void ExecuteTransaction(TransactionData transaction) override;
    virtual void HandleMsg(int message, int src, Tag tag) override;
protected:
    virtual void Run() override;

private:
    LamportMutex mtx_Lamport;                           // Lamport mutex for the core layer transactions

    int m_WriteCount;                                   // Number of writes that have been executed

    int m_FinishedNodes;                                // Number of nodes that have finished their transaction
    std::mutex mtx_Transaction;                         // Mutex for the transaction cv
    std::condition_variable cv_Transaction;             // Condition variable to wait for the transaction
};