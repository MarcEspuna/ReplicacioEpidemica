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
    LamportMutex mtx_Lamport;
};