#pragma once
#include "Commons.h"
#include "Log.h"
#include "nodes/CoreNode.h"
#include "nodes/LayerOneNode.h"
#include "nodes/LayerTwoNode.h"
#include "transactions/FileWatcher.h"
#include "transactions/TransactionReader.h"

class Controller {
public:
    Controller();
    ~Controller();

    void Run();

private:
    void FileModifiedCallback(const std::string& filename, const std::string& dir);

private:
    std::array<CoreNode, 3> m_CoreNodes;            // Core layer
    std::array<LayerOneNode, 2> m_LayerOneNodes;    // Layer 1
    std::array<LayerTwoNode, 2> m_LayerTwoNodes;    // Layer 2

    // File system
    efsw::FileWatcher m_FileWatcher;
    FileWatcher m_FileListener;
    efsw::WatchID m_WatchId;
};