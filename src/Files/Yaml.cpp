#include "Tenshi/Files/Yaml.h"
#include "Tenshi/Files/FileSystem.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace Tenshi {

YamlException::YamlException(std::string message)
    : message(std::move(message))
{
}

const char* YamlException::what() const noexcept {
    return message.c_str();
}

YamlDocument::YamlDocument(Node root)
    : root(std::move(root))
{
}

YamlDocument::Node& YamlDocument::GetRoot() {
    return root;
}

const YamlDocument::Node& YamlDocument::GetRoot() const {
    return root;
}

bool YamlDocument::IsNull() const {
    return root.is_null();
}

bool YamlDocument::IsMap() const {
    return root.is_mapping();
}

bool YamlDocument::IsSequence() const {
    return root.is_sequence();
}

bool YamlDocument::IsScalar() const {
    return root.is_scalar();
}

YamlLoader::YamlLoader(FileSystem& files)
    : files(files)
{
}

YamlDocument YamlLoader::Load(const std::string& path) {
    if (!files.exists(path)) {
        throw YamlException("YAML file does not exist: " + path);
    }

    std::vector<uint8_t> data = files.readAll(path);

    if (data.empty()) {
        throw YamlException("YAML file is empty: " + path);
    }

    try {
        std::string yaml(
            reinterpret_cast<const char*>(data.data()),
            data.size()
        );
        return YamlDocument(fkyaml::node::deserialize(yaml));
    } catch (const fkyaml::exception& e) {
        throw YamlException(
            "Failed to parse YAML file '" 
            +   path 
            +   "': " 
            +   e.what()
        );
    }
}

YamlDocument YamlLoader::Parse(const std::string& data) {
    if (data.empty()) {
        throw YamlException("Cannot parse empty YAML data");
    }

    try {
        return YamlDocument(fkyaml::node::deserialize(data));
    } catch (const fkyaml::exception& e) {
        throw YamlException(
            "Failed to parse YAML data: " 
            +   std::string(e.what())
        );
    }
}

} // namespace Tenshi