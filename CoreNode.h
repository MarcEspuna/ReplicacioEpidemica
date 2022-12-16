#pragma once

#include "Node.h"

class CoreNode : public Node {
public:
    CoreNode(int id, const std::vector<int>& current, const std::vector<int>& next);
    ~CoreNode();
protected:
    virtual void Run() override;

private:

};