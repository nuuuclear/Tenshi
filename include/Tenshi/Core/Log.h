#pragma once

#include <format>
#include <string_view>

namespace Tenshi {

enum class LogPriority {
    Verbose,
    Debug,
    Info,
    Warning,
    Error
};

enum class LogCategory {
    Application,
    System,
    Renderer,
    Audio,
    Input,
    Resource
};

using LogOutputFunction = void (*)(
    LogPriority priority, LogCategory category, std::string_view message, void* userdata
);

void SetLogOutputFunction(LogOutputFunction function, void* userdata = nullptr);
void LogMessage(LogPriority priority, LogCategory category, std::string_view message);

template<typename... Args>
void Log(LogPriority priority, LogCategory category, std::format_string<Args...> fmt, Args&&... args) {
    LogMessage(priority, category, std::format(fmt, std::forward<Args>(args)...));
}

template<typename... Args>
void LogVerbose(LogCategory category, std::format_string<Args...> fmt, Args&&... args) {
    Log(LogPriority::Verbose, category, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogDebug(LogCategory category, std::format_string<Args...> fmt, Args&&... args) {
    Log(LogPriority::Debug, category, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogInfo(LogCategory category, std::format_string<Args...> fmt, Args&&... args) {
    Log(LogPriority::Info, category, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogWarning(LogCategory category, std::format_string<Args...> fmt, Args&&... args) {
    Log(LogPriority::Warning, category, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogError(LogCategory category, std::format_string<Args...> fmt, Args&&... args) {
    Log(LogPriority::Error, category, fmt, std::forward<Args>(args)...);
}

} // namespace tenshi