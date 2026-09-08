#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

#include <SDL3/SDL.h>

#include "Log.h"

namespace Tenshi {

template<typename T>
class AssetRegistry {
public:
    using Ptr = std::shared_ptr<T>;

    void Add(const std::string& key, Ptr asset) {
        assets[key] = asset;
    }

    Ptr Get(const std::string& key) {
        auto it = assets.find(key);
        if (it == assets.end()) {
            LogError(
                LogCategory::Resource,
                "Missing asset: {}",
                key.c_str()
            );
            throw std::runtime_error("asset not found: " + key);
        }
        return it->second;
    }

    bool Exists(const std::string& key) const {
        return assets.find(key) != assets.end();
    }

private:
    std::unordered_map<std::string, Ptr> assets;
};

} // namespace Tenshi