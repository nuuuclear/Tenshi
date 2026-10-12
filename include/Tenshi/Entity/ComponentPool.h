#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

#include "Entity.h"

namespace Tenshi {
class IComponentPool {
public:
    virtual ~IComponentPool() = default;

    virtual bool remove(Entity entity) = 0;
};

template<typename T>
class ComponentPool final : public IComponentPool {
public:
    template<typename... Args>
    T& emplace(Entity entity, Args&&... args) {
        if (!entity.valid())
            throw std::invalid_argument("Invalid entity");

        if (has(entity))
            throw std::logic_error("Component already exists");

        if (entity.index >= sparse.size())
            sparse.resize(static_cast<std::size_t>(entity.index) + 1, 0);

        const std::size_t position = components.size();

        entities.push_back(entity);

        try {
            components.emplace_back(
                std::forward<Args>(args)...);
        } catch (...) {
            entities.pop_back();
            throw;
        }

        sparse[entity.index] = position + 1;
        return components.back();
    }

    bool has(Entity entity) const {
        if (!entity.valid() || entity.index >= sparse.size())
            return false;

        const std::size_t slot = sparse[entity.index];

        if (slot == 0)
            return false;

        const std::size_t position = slot - 1;

        return position < entities.size()
            && entities[position] == entity;
    }

    T* get(Entity entity) {
        if (!has(entity))
            return nullptr;

        return &components[sparse[entity.index] - 1];
    }

    const T* get(Entity entity) const {
        if (!has(entity))
            return nullptr;

        return &components[sparse[entity.index] - 1];
    }

    template<typename... Args>
    T& emplaceOrReplace(Entity entity, Args&&... args) {
        if (T* component = get(entity)) {
            *component = T(std::forward<Args>(args)...);
            return *component;
        }
        return emplace(entity, std::forward<Args>(args)...);
    }

    bool remove(Entity entity) override {
        if (!has(entity))
            return false;

        const std::size_t position = sparse[entity.index] - 1;
        const std::size_t last = components.size() - 1;

        if (position != last) {
            components[position] = std::move(components[last]);
            entities[position] = entities[last];

            sparse[entities[position].index] = position + 1;
        }

        sparse[entity.index] = 0;
        components.pop_back();
        entities.pop_back();
        return true;
    }

    const std::vector<Entity>& entityList() const {
        return entities;
    }

    std::size_t size() const noexcept {
        return components.size();
    }

private:
    std::vector<std::size_t> sparse;
    std::vector<Entity> entities;
    std::vector<T> components;
};
} // name