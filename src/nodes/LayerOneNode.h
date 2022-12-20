#pragma once

#include "nodes/Node.h"

class LayerOneNode : public Node {
public:
    LayerOneNode(const std::string& name, int id);
    ~LayerOneNode();

    virtual void HandleMsg(int message, int src, Tag tag) override;

    void Shutdown();
private:
    void Run() override;

    std::mutex m_WaitMutex;
    std::condition_variable m_WaitCondition;
};  
