#pragma once

#include "Tenshi/Event/Event.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace Tenshi {

class Platform {
public:
    using EventCallback = std::function<void(const Event&)>;

    virtual ~Platform() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

    virtual double getTime() const = 0;

    virtual void delay(uint32_t milliseconds) = 0;
    virtual void pollEvents(const EventCallback& callback) = 0;
};

std::unique_ptr<Platform> CreatePlatform();

} // namespace Tenshi