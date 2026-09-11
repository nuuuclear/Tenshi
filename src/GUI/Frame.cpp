#include "Tenshi/GUI/Frame.h"

namespace Tenshi {
namespace GUI {

Frame::Frame(std::string id, Rect_f bounds)
:   Object(
    std::move(id), bounds
)
{}

void Frame::draw(SDL_Renderer* renderer) {
    if (!active) return;

    // draw self...
    SDL_RenderRect(renderer, bounds);

    Object::draw(renderer);
}

} // namespace GUI
} // namespace Tenshi