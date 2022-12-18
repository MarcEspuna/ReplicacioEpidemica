#pragma once
#include "Commons.h"
#include "Node.h"


class LayerTwoNode : public Node {
public:
    LayerTwoNode(const std::string& name, int id);
    ~LayerTwoNode();
    
private:
    void Run() override;

};

