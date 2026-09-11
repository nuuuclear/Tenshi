#pragma once

#include "Tenshi/Geometry/Vector.h"
#include "Tenshi/Geometry/Rect.h"

#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include <SDL3/SDL.h>

namespace Tenshi {
namespace GUI {

class Object {
public:
    Object(std::string id, Rect_f bounds)
    :   id(id)
    ,   bounds(bounds)
    {}

    virtual ~Object() = default;

    virtual bool event(const SDL_Event& event) {
        if (!active) return false;

        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if ((*it)->event(event)) {
                return true;
            }
        }

        return false;
    }

    virtual void update(float deltaTime) {
        if (!active) return;

        for (auto& child : children) {
            child->update(deltaTime);
        }
    }
    
    virtual void draw(SDL_Renderer* renderer) {
        if (!active) return;

        // draw self...
        
        for (auto& child : children) {
            child->draw(renderer);
        }
    }

    void addChild(std::unique_ptr<Object> child) {
        if (!child) return;
        child->parent = this;
        children.push_back(std::move(child));
    }

    bool removeChild(const std::string& id) {
        auto it = std::remove_if(children.begin(), children.end(),
            [&id](const std::unique_ptr<Object>& child) {
                return child->id == id;
            });
        
        if (it != children.end()) {
            children.erase(it, children.end());
            return true;
        }
        return false;
    }

    Vec2_f getGlobalPosition() const {
        if (parent) {
            Vec2_f parentGlobal = parent->getGlobalPosition();
            return { parentGlobal.x + bounds.x, parentGlobal.y + bounds.y };
        }
        return Vec2_f{bounds.x, bounds.y};
    }

    void setActive(bool active) { active = active; }

    bool isActive() const { return active; }
    const std::string& getId() const { return id; }

protected:
    std::string id;
    Object* parent = nullptr;
    std::vector<std::unique_ptr<Object>> children;

    Rect_f bounds;
    bool active = true;
};

} // namespace GUI
} // namespace Tenshi