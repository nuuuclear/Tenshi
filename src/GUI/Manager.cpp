#include "Tenshi/GUI/Manager.h"

#include "Tenshi/GUI/Object.h"
#include "Tenshi/Game.h"

namespace Tenshi {

UIManager::UIManager(Game* game) : Subroutine(game) {
}

UIManager::~UIManager() {
}

void UIManager::event(const SDL_Event& e) {
    for (auto& child : children) {
        child->event(e);
    }
}

void UIManager::update(double deltaTime) {
    for (auto& child : children) {
        child->update(deltaTime);
    }
}

void UIManager::draw() {
    for (auto& child : children) {
        child->draw(game->getRenderer());
    }
}

void UIManager::pushChild(std::unique_ptr<GUI::Object> child) {
    children.push_back(std::move(child));
}

} // namespace Tenshi