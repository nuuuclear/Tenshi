#include "Tenshi/Platform/Platform.h"

#include "Backends/SDL3/SDLPlatform.h"

#include <memory>

namespace Tenshi {

std::unique_ptr<Platform> CreatePlatform() {
    return std::make_unique<SDLPlatform>();
}

} // namespace Tenshi