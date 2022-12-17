#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::m_Logger;

void Log::StartLogging()
{
    spdlog::set_pattern("%^[%T] [%s]: %v%$");
    spdlog::set_level(spdlog::level::trace);
}

void Log::CreateLogger(const std::string& name)
{
    m_Logger = spdlog::stdout_color_mt(name);
}

std::shared_ptr<spdlog::logger> Log::GetLogger()
{
    return m_Logger;
}

void Log::EndLogging()
{
    spdlog::shutdown();
}