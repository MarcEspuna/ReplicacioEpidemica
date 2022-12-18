#pragma once

#include "nodes/Node.h"

class LayerOneNode : public Node {
public:
    LayerOneNode(const std::string& name, int id);
    ~LayerOneNode();

private:
    void Run() override;

};  