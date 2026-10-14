#include "Tenshi/GUI/Frame.h"

namespace Tenshi {
namespace GUI {

Frame::Frame(std::string id, Rect<float> bounds)
:   Element(std::move(id), bounds)
{
}

void Frame::draw(SDL_Renderer* renderer) {
    if (!active) return;

    SDL_FRect sdlRect = static_cast<SDL_FRect>(bounds);
    SDL_RenderFillRect(renderer, &sdlRect);

    Element::draw(renderer);
}

} // namespace GUI
} // namespace Tenshi