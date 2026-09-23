#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>

namespace Tenshi {

class Input {
public:

    void beginFrame();
    void poll(const SDL_Event &e);

    SDL_Event* GetEvent();

    bool check(SDL_Scancode key) const;
    bool pressed(SDL_Scancode key) const;
    bool released(SDL_Scancode key) const;

    bool mouseCheck(int button) const;
    bool mousePressed(int button) const;
    bool mouseReleased(int button) const;

    int mouseX() const;
    int mouseY() const;
    int mouseWheel() const;

    void bind(const std::string& action, SDL_Scancode key);

    bool action(const std::string& action) const;
    bool actionPressed(const std::string& action) const;
    bool actionReleased(const std::string& action) const;

private:
    SDL_Event event;

    bool keyDown[SDL_SCANCODE_COUNT] = {};
    bool keyPressed[SDL_SCANCODE_COUNT] = {};
    bool keyReleased[SDL_SCANCODE_COUNT] = {};

    bool mouseDown[8] = {};
    bool mousePressedState[8] = {};
    bool mouseReleasedState[8] = {};

    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_mouseWheel = 0;

    std::unordered_map<std::string, SDL_Scancode> bindings;

    // requests
    bool m_quitRequested = false;
};

} // namespace Tenshi