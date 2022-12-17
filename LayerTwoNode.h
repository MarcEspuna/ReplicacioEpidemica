#pragma once
#include "Commons.h"
#include "Node.h"


class LayerTwoNode : public Node {
public:
    LayerTwoNode(int id);
    ~LayerTwoNode();
    
private:
    void Run() override;

};

