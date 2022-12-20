#include "transactions/FileWatcher.h"

FileWatcher::FileWatcher(std::function<void(const std::string&, const std::string&)> lamda)
    : m_Handler(lamda), m_Start(std::chrono::system_clock::now())
{
}

void FileWatcher::handleFileAction(efsw::WatchID watchid, const std::string &dir,
                                   const std::string &filename, efsw::Action action,
                                   std::string oldFilename)
{
    m_End = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_End-m_Start);
    // Debounce to prevent multiple consecutive callbacks
    if (duration.count() > 500)
    {
        switch ( action ) {
            case efsw::Actions::Modified:
                m_Handler(filename, dir);
                break;
            default:
                break;
        }
    }
    m_Start = std::chrono::system_clock::now();
}

