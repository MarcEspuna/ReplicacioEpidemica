#pragma once

#include "nodes/Node.h"
#include "LamportMutex.h"

class CoreNode : public Node {
public:
    CoreNode(const std::string& name, int id);
    ~CoreNode();

    virtual void ExecuteTransaction(TransactionData transaction) override;

protected:
    virtual void Run() override;

private:
    LamportMutex mtx_Lamport;
};