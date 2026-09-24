#pragma once

#include "Tenshi/Input/InputEvent.h"

#include <array>
#include <cstddef>
#include <unordered_map>
#include <string>
#include <vector>

namespace Tenshi {

class Input {
public:
    Input() = default;
    ~Input() = default;

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    void beginFrame();
    void process(const InputEvent& event);

    // keyboard
    bool check(Key key) const;
    bool pressed(Key key) const;
    bool released(Key key) const;

    // mouse
    bool mouseCheck(MouseButton button) const;
    bool mousePressed(MouseButton button) const;
    bool mouseReleased(MouseButton button) const;

    int mouseX() const;
    int mouseY() const;

    int mouseWheelX() const;
    int mouseWheelY() const;

    // actions
    void bind(const std::string& action, Key key);
    void unbind(const std::string& action, Key key);
    void clearBindings(const std::string& action);

    bool action(const std::string& action) const;
    bool actionPressed(const std::string& action) const;
    bool actionReleased(const std::string& action) const;

private:
    static constexpr size_t KeyCount = static_cast<size_t>(Key::Count);
    static constexpr size_t MouseButtonCount = static_cast<size_t>(MouseButton::Count);

    std::array<bool, KeyCount> keyDown{};
    std::array<bool, KeyCount> keyPressed{};
    std::array<bool, KeyCount> keyReleased{};

    std::array<bool, MouseButtonCount> mouseDown{};
    std::array<bool, MouseButtonCount> mousePressedState{};
    std::array<bool, MouseButtonCount> mouseReleasedState{};

    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_mouseWheelX = 0;
    int m_mouseWheelY = 0;

    std::unordered_map<std::string, std::vector<Key>> bindings;
};

} // namespace Tenshi