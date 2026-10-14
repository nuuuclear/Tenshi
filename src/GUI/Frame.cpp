#include "Tenshi/GUI/Frame.h"

namespace Tenshi {
namespace GUI {

Frame::Frame(std::string id, Rect<float> bounds)
:   Object(std::move(id), bounds)
{
}

void Frame::draw(SDL_Renderer* renderer) {
    if (!active) return;

    SDL_FRect sdlRect = static_cast<SDL_FRect>(bounds);
    SDL_RenderRect(renderer, &sdlRect);

    Object::draw(renderer);
}

} // namespace GUI
} // namespace Tenshi