#pragma once

#include <cstdint>

namespace Tenshi {

enum class Key : uint16_t {
    Unknown = 0,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,

    Escape,
    Enter,
    Tab,
    Backspace,
    Space,

    Minus,
    Equals,
    LeftBracket,
    RightBracket,
    Backslash,
    Semicolon,
    Apostrophe,
    Grave,
    Comma,
    Period,
    Slash,

    CapsLock,

    LeftShift,
    RightShift,
    LeftCtrl,
    RightCtrl,
    LeftAlt,
    RightAlt,

    Left,
    Right,
    Up,
    Down,

    Insert,
    Delete,
    Home,
    End,
    PageUp,
    PageDown,

    PrintScreen,
    ScrollLock,
    Pause,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    Numpad0,
    Numpad1,
    Numpad2,
    Numpad3,
    Numpad4,
    Numpad5,
    Numpad6,
    Numpad7,
    Numpad8,
    Numpad9,

    NumpadDivide,
    NumpadMultiply,
    NumpadMinus,
    NumpadPlus,
    NumpadEnter,
    NumpadPeriod,

    Count
};

enum class MouseButton : uint8_t {
    Left = 0,
    Middle,
    Right,
    X1,
    X2,

    Count
};

} // namespace Tenshi