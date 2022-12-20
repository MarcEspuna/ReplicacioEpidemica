#pragma once 
#include "Log.h"
#include "Commons.h"
#include <efsw.hpp>

using FileHandlerFunction = std::function<void(const std::string&, const std::string&)>;

#define BIND_FILE_HANDLER(handler) std::bind(&handler, this, std::placeholders::_1, std::placeholders::_2)

class FileWatcher : public efsw::FileWatchListener {
public:
    FileWatcher() = delete;
    FileWatcher(FileHandlerFunction lamda);

    virtual void handleFileAction(efsw::WatchID watchid, const std::string& dir,
                           const std::string& filename, efsw::Action action,
                           std::string oldFilename) override;

private:
    std::chrono::system_clock::time_point m_Start;
    std::chrono::system_clock::time_point m_End;

    FileHandlerFunction m_Handler;
};