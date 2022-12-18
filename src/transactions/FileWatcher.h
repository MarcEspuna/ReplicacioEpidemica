#pragma once 
#include "Log.h"
#include "Commons.h"
#include <efsw.hpp>


class FileWatcher : public efsw::FileWatchListener {
public:
    FileWatcher() = delete;
    FileWatcher(std::function<void(const std::string&, const std::string&)> lamda);

    virtual void handleFileAction(efsw::WatchID watchid, const std::string& dir,
                           const std::string& filename, efsw::Action action,
                           std::string oldFilename) override;

private:
    std::chrono::system_clock::time_point m_Start;
    std::chrono::system_clock::time_point m_End;

    std::function<void(const std::string&, const std::string&)> m_Handler;

};