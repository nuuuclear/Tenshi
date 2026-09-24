#pragma once

#include "Tenshi/Input/InputEvent.h"

#include <cstdint>
#include <string>

namespace Tenshi {

enum class EventType : uint8_t {
    None,

    Quit,
    WindowResize,
    WindowExposed,

    KeyDown,
    KeyUp,

    MouseButtonDown,
    MouseButtonUp,
    MouseMotion,
    MouseWheel,

    TextInput,
    FileDrop
};

struct WindowResizeEvent {
    int width = 0;
    int height = 0;
};

struct TextInputEvent {
    std::string text;
};

struct FileDropEvent {
    std::string path;
};

struct Event {
    EventType type = EventType::None;

    InputEvent input;
    WindowResizeEvent resize;
    TextInputEvent textInput;
    FileDropEvent fileDrop;
};

} // namespace Tenshi