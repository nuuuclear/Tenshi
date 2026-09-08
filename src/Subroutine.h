#pragma once

#include <SDL3/SDL.h>

namespace Tenshi {
class Game;

class Subroutine {
public:
    virtual ~Subroutine() = default;

    virtual bool init() { return true; }
    virtual void event(const SDL_Event& event) {}
    virtual void update(double deltaTime) {}
    virtual void draw() {}

protected:
    Game* game = nullptr;

    explicit Subroutine(Game* parentGame) : game(parentGame)
    {
    }
};

} // namespace Tenshi