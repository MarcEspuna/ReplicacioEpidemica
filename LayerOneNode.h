#pragma once

#include "Node.h"

class LayerOneNode : public Node {
public:
    LayerOneNode(int id, const std::vector<int>& current, const std::vector<int>& next);
    ~LayerOneNode();

private:
    void Run() override;

};  
