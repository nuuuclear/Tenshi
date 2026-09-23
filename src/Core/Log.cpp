#include "Tenshi/Core/Log.h"

#include <iostream>
#include <mutex>

namespace Tenshi {

namespace {

LogOutputFunction outputFunction = nullptr;
void* outputUserdata = nullptr;

std::mutex logMutex;

const char* GetPriorityName(LogPriority priority) {
    switch (priority) {
        case LogPriority::Verbose: return "Verbose";
        case LogPriority::Debug:   return "Debug";
        case LogPriority::Info:    return "Info";
        case LogPriority::Warning: return "Warning";
        case LogPriority::Error:   return "Error";
    }

    return "Unknown";
}

const char* GetCategoryName(LogCategory category) {
    switch (category) {
        case LogCategory::Application: return "Application";
        case LogCategory::System:      return "System";
        case LogCategory::Renderer:    return "Renderer";
        case LogCategory::Audio:       return "Audio";
        case LogCategory::Input:       return "Input";
        case LogCategory::Resource:    return "Resource";
    }

    return "Unknown";
}

void DefaultLogOutput(LogPriority priority, LogCategory category, std::string_view message) {
    std::lock_guard lock(logMutex);

    std::ostream& stream
	= 	priority == LogPriority::Error 
	|| 	priority == LogPriority::Warning
        ? std::cerr
        : std::cout;

    stream
        << "<"
        << GetPriorityName(priority)
        << "> <"
        << GetCategoryName(category)
        << "> "
        << message
        << '\n';
}

} // namespace

void SetLogOutputFunction(LogOutputFunction function, void* userdata) {
    std::lock_guard lock(logMutex);

    outputFunction = function;
    outputUserdata = userdata;
}

void LogMessage(LogPriority priority, LogCategory category, std::string_view message) {
    if (outputFunction) {
        outputFunction(priority, category, message, outputUserdata);

        return;
    }
    
    DefaultLogOutput(priority, category, message);
}

} // namespace tenshi