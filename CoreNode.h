#pragma once

#include "Node.h"
#include "LamportMutex.h"

class CoreNode : public Node {
public:
    CoreNode(const std::string& name, int id);
    ~CoreNode();
protected:
    virtual void Run() override;

private:
    LamportMutex mtx_Lamport;
};