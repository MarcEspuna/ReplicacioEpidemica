#pragma once
#include <iostream>
#include <memory>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

class Log {
public:
    /* Static impl */
    static void StartLogging();
    static void CreateLogger(const std::string& name);
    static std::shared_ptr<spdlog::logger> GetLogger();
    static void EndLogging();
private:
    static std::shared_ptr<spdlog::logger> m_Logger;        // Core logger
    Log() = default;
};

#define ACTIVE_LOGGING  
#ifdef ACTIVE_LOGGING
    #define LOG_TRACE(...)              SPDLOG_LOGGER_TRACE(Log::GetLogger(), __VA_ARGS__)
    #define LOG_INFO(...)               SPDLOG_LOGGER_INFO(Log::GetLogger(), __VA_ARGS__)
    #define LOG_WARN(...)               SPDLOG_LOGGER_WARN(Log::GetLogger(), __VA_ARGS__)
    #define LOG_ERROR(...)              SPDLOG_LOGGER_ERROR(Log::GetLogger(), __VA_ARGS__)
    #define LOG_CRITICAL(...)           SPDLOG_LOGGER_CRITICAL(Log::GetLogger(), __VA_ARGS__)
    #define LOG_ASSERT(check, ...)      { if (!(check)) { SPDLOG_LOGGER_CRITICAL(Log::GetLogger(), __VA_ARGS__); __debugbreak(); }}
#else
    #define LOG_TRACE(...)      
    #define LOG_INFO(...)       
    #define LOG_WARN(...)       
    #define LOG_ERROR(...)      
    #define LOG_CRITICAL(...)   
    #define LOG_ASSERT(check, ...)      assert(check);
#endif