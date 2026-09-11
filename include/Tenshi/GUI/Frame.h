#pragma once

#include "Tenshi/Geometry/Vector.h"
#include "Tenshi/Geometry/Rect.h"
#include "Tenshi/GUI/Object.h"

#include <SDL3/SDL.h>

namespace Tenshi {
namespace GUI {

class Frame : public Object {
public:
    Frame(std::string id, Rect_f bounds);
    
    void draw(SDL_Renderer *renderer);
};

} // namespace GUI
} // namespace Tenshi