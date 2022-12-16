#pragma once
#include "Commons.h"
#include "Node.h"


class LayerTwoNode : public Node {
public:
    LayerTwoNode(int id, const std::vector<int>& current, const std::vector<int>& next);
    ~LayerTwoNode();
    
private:
    void Run() override;

};

