#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace Tenshi {

template<typename T>
class AssetRegistry {
public:
    using Ptr = std::unique_ptr<T>;

    // Add an asset to the registry
    void Add(const std::string& key, Ptr asset) {
        if (!asset) {
            return;
        }

        assets[key] = std::move(asset);
    }

    // Get an asset from the registry
    //
    // Returns nullptr if the asset does not exist
    T* Get(const std::string& key) {
        auto it = assets.find(key);

        if (it == assets.end()) {
            return nullptr;
        }

        return it->second.get();
    }

    // Get an asset from the registry
    //
    // Returns nullptr if the asset does not exist
    const T* Get(const std::string& key) const {
        auto it = assets.find(key);

        if (it == assets.end()) {
            return nullptr;
        }

        return it->second.get();
    }

    // Get an asset or a fallback from the registry
    T* GetOr(const std::string& key, T* fallback) {
        T* asset = Get(key);

        if (asset) {
            return asset;
        }

        return fallback;
    }

    // Require an asset from the registry
    //
    // Throws an error if the asset does not exist!
    T& Require(const std::string& key) {
        T* asset = Get(key);

        if (!asset) {
            throw std::runtime_error(
                "Required asset not found: " + key
            );
        }

        return *asset;
    }

    // Check if an asset exists in the registry
    bool Exists(const std::string& key) const {
        return assets.find(key) != assets.end();
    }

    // Get size of the registry
    size_t Size() const {
        return assets.size();
    }

    // Remove an asset from the registry
    void Remove(const std::string& key) {
        assets.erase(key);
    }

    // Clear all assets from the registry
    void Clear() {
        assets.clear();
    }

private:
    std::unordered_map<std::string, Ptr> assets;
};

} // namespace Tenshi