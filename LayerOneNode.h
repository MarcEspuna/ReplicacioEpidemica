#pragma once

#include "Node.h"

class LayerOneNode : public Node {
public:
    LayerOneNode(int id);
    ~LayerOneNode();

private:
    void Run() override;

};  
