#include "Input.h"

#include <cstring>

namespace Tenshi {

void Input::beginFrame() {
    memset(keyPressed, 0, sizeof(keyPressed));
    memset(keyReleased, 0, sizeof(keyReleased));

    memset(mousePressedState, 0, sizeof(mousePressedState));
    memset(mouseReleasedState, 0, sizeof(mouseReleasedState));

    m_mouseWheel = 0;
}

void Input::poll() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_EVENT_QUIT:
            m_quitRequested = true;
            break;

        case SDL_EVENT_KEY_DOWN: {
            SDL_Scancode key = event.key.scancode;

            if (!keyDown[key])
                keyPressed[key] = true;

            keyDown[key] = true;
            break;
        }

        case SDL_EVENT_KEY_UP: {
            SDL_Scancode key = event.key.scancode;

            keyDown[key] = false;
            keyReleased[key] = true;
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            int b = event.button.button;

            if (!mouseDown[b])
                mousePressedState[b] = true;

            mouseDown[b] = true;
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_UP: {
            int b = event.button.button;

            mouseDown[b] = false;
            mouseReleasedState[b] = true;
            break;
        }

        case SDL_EVENT_MOUSE_MOTION: {
            m_mouseX = event.motion.x;
            m_mouseY = event.motion.y;
            break;
        }

        case SDL_EVENT_MOUSE_WHEEL: {
            m_mouseWheel = event.wheel.y;
            break;
        }

        }
    }
}

SDL_Event* Input::GetEvent() {
    return &event;
}

// keybaord
bool Input::check(SDL_Scancode key) const {
    return keyDown[key];
}

bool Input::pressed(SDL_Scancode key) const {
    return keyPressed[key];
}

bool Input::released(SDL_Scancode key) const {
    return keyReleased[key];
}

// mouse
bool Input::mouseCheck(int button) const {
    return mouseDown[button];
}

bool Input::mousePressed(int button) const {
    return mousePressedState[button];
}

bool Input::mouseReleased(int button) const {
    return mouseReleasedState[button];
}

int Input::mouseX() const {
    return m_mouseX;
}

int Input::mouseY() const {
    return m_mouseY;
}

int Input::mouseWheel() const {
    return m_mouseWheel;
}

// bindings

void Input::bind(const std::string& action, SDL_Scancode key) {
    bindings[action] = key;
}

bool Input::action(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return check(it->second);
}

bool Input::actionPressed(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return pressed(it->second);
}

bool Input::actionReleased(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return released(it->second);
}

// requests

bool Input::quitRequested() const {
    return m_quitRequested;
}

} // namespace Tenshi