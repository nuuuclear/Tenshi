#pragma once

#include "Tenshi/Utility/Types.h"
#include "Tenshi/GUI/Element.h"

#include <SDL3/SDL.h>

namespace Tenshi {
namespace GUI {

class Frame : public Element {
public:
    Frame(std::string id, Rect<float> bounds);
    
    void draw(SDL_Renderer *renderer);
};

} // namespace GUI
} // namespace Tenshi