#pragma once

#include "nodes/Node.h"

class LayerOneNode : public Node {
public:
    LayerOneNode(const std::string& name, int id);
    ~LayerOneNode();

    virtual void HandleMsg(int message, int src, Tag tag) override;
private:
    void Run() override;

};  
