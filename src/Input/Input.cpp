#include "Tenshi/Input/Input.h"

#include <algorithm>

namespace Tenshi {

void Input::beginFrame() {
    keyPressed.fill(false);
    keyReleased.fill(false);

    mousePressedState.fill(false);
    mouseReleasedState.fill(false);

    m_mouseWheelX = 0;
    m_mouseWheelY = 0;
}

void Input::process(const InputEvent& event) {
    switch (event.type) {
        case InputEventType::KeyDown: {
            const size_t index = static_cast<size_t>(event.key);

            if (index >= keyDown.size())
                return;

            if (!keyDown[index]) {
                keyPressed[index] = true;
            }

            keyDown[index] = true;
            break;
        }

        case InputEventType::KeyUp: {
            const size_t index = static_cast<size_t>(event.key);

            if (index >= keyDown.size())
                return;

            keyDown[index] = false;
            keyReleased[index] = true;
            break;
        }

        case InputEventType::MouseButtonDown: {
            const size_t index = static_cast<size_t>(event.button);

            if (index >= mouseDown.size())
                return;

            if (!mouseDown[index]) {
                mousePressedState[index] = true;
            }

            mouseDown[index] = true;
            break;
        }

        case InputEventType::MouseButtonUp: {
            const size_t index = static_cast<size_t>(event.button);

            if (index >= mouseDown.size())
                return;

            mouseDown[index] = false;
            mouseReleasedState[index] = true;
            break;
        }

        case InputEventType::MouseMotion:
            m_mouseX = event.mouseX;
            m_mouseY = event.mouseY;
            break;

        case InputEventType::MouseWheel:
            m_mouseWheelX += event.wheelX;
            m_mouseWheelY += event.wheelY;
            break;
    }
}

bool Input::check(Key key) const {
    const size_t index = static_cast<size_t>(key);

    if (index >= keyDown.size())
        return false;

    return keyDown[index];
}

bool Input::pressed(Key key) const {
    const size_t index = static_cast<size_t>(key);

    if (index >= keyPressed.size())
        return false;

    return keyPressed[index];
}

bool Input::released(Key key) const {
    const size_t index = static_cast<size_t>(key);

    if (index >= keyReleased.size())
        return false;

    return keyReleased[index];
}

bool Input::mouseCheck(MouseButton button) const {
    const size_t index = static_cast<size_t>(button);

    if (index >= mouseDown.size())
        return false;

    return mouseDown[index];
}

bool Input::mousePressed(MouseButton button) const {
    const size_t index = static_cast<size_t>(button);

    if (index >= mousePressedState.size())
        return false;

    return mousePressedState[index];
}

bool Input::mouseReleased(MouseButton button) const {
    const size_t index = static_cast<size_t>(button);

    if (index >= mouseReleasedState.size())
        return false;

    return mouseReleasedState[index];
}

int Input::mouseX() const {
    return m_mouseX;
}

int Input::mouseY() const {
    return m_mouseY;
}

int Input::mouseWheelX() const {
    return m_mouseWheelX;
}

int Input::mouseWheelY() const {
    return m_mouseWheelY;
}

void Input::bind(const std::string& action, Key key) {
    auto& keys = bindings[action];

    if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
        keys.push_back(key);
    }
}

void Input::unbind(const std::string& action, Key key) {
    auto it = bindings.find(action);

    if (it == bindings.end())
        return;

    auto& keys = it->second;

    keys.erase(
        std::remove(keys.begin(), keys.end(), key),
        keys.end()
    );

    if (keys.empty()) {
        bindings.erase(it);
    }
}

void Input::clearBindings(const std::string& action) {
    bindings.erase(action);
}

bool Input::action(const std::string& action) const {
    auto it = bindings.find(action);

    if (it == bindings.end())
        return false;

    for (Key key : it->second) {
        if (check(key))
            return true;
    }

    return false;
}

bool Input::actionPressed(const std::string& action) const {
    auto it = bindings.find(action);

    if (it == bindings.end())
        return false;

    for (Key key : it->second) {
        if (pressed(key))
            return true;
    }

    return false;
}

bool Input::actionReleased(const std::string& action) const {
    auto it = bindings.find(action);

    if (it == bindings.end())
        return false;

    for (Key key : it->second) {
        if (released(key))
            return true;
    }

    return false;
}


} // namespace Tenshi