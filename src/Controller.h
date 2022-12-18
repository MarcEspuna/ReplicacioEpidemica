#pragma once
#include "Commons.h"
#include "Log.h"
#include "nodes/CoreNode.h"
#include "nodes/LayerOneNode.h"
#include "nodes/LayerTwoNode.h"
#include "FileWatcher.h"
#include "TransactionReader.h"

class Controller {
public:
    Controller();
    ~Controller();

    void Run();

private:
    void FileModifiedCallback(const std::string& filename, const std::string& dir);

private:
    /* Nodes */
    CoreNode A1;
    CoreNode A2;
    CoreNode A3;

    /* File system */
    efsw::FileWatcher m_FileWatcher;
    FileWatcher m_FileListener;
    efsw::WatchID m_WatchId;
};