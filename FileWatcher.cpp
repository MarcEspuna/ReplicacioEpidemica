#include "FileWatcher.h"

FileWatcher::FileWatcher(std::function<void(const std::string&, const std::string&)> lamda)
    : m_Handler(lamda)
{
}

void FileWatcher::handleFileAction(efsw::WatchID watchid, const std::string &dir,
                                   const std::string &filename, efsw::Action action,
                                   std::string oldFilename)
{
    switch ( action ) {
        case efsw::Actions::Modified:
            m_Handler(filename, dir);
            break;
        default:
            break;
    }
}

