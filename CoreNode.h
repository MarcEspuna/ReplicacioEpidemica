#pragma once

#include "Node.h"

class CoreNode : public Node {
public:
    CoreNode(int id);
    ~CoreNode();
protected:
    virtual void Run() override;

private:

};