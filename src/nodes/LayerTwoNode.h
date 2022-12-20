#pragma once
#include "Commons.h"
#include "nodes/Node.h"


class LayerTwoNode : public Node {
public:
    LayerTwoNode(const std::string& name, int id);
    ~LayerTwoNode();
    
    virtual void HandleMsg(int message, int src, Tag tag) override;
private:
    void Run() override;

};

