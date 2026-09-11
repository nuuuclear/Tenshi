#pragma once

#include "Tenshi/Subroutine.h"
#include "Tenshi/GUI/Object.h"

#include <SDL3/SDL.h>
#include <vector>
#include <memory>

namespace Tenshi {

class Game;

// acts as a root for all ui objects
class UIManager : public Subroutine {
public:
    explicit UIManager(Game* game);
    ~UIManager();

    void event(const SDL_Event& event);
    void update(double deltaTime);
    void draw();

    void pushChild(std::unique_ptr<GUI::Object> child);
private:
    std::vector<std::unique_ptr<GUI::Object>> children;
};


} // namespace Tenshi