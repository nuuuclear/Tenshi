#pragma once

#include "Tenshi/Input/Key.h"

#include <cstdint>

namespace Tenshi {

enum class InputEventType : uint8_t {
    KeyDown,
    KeyUp,

    MouseButtonDown,
    MouseButtonUp,

    MouseMotion,
    MouseWheel
};

struct InputEvent {
    InputEventType type = InputEventType::KeyDown;

    Key key = Key::Unknown;
    MouseButton button = MouseButton::Left;

    int mouseX = 0;
    int mouseY = 0;

    int wheelX = 0;
    int wheelY = 0;
};

} // namespace Tenshi