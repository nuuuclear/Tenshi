#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include <fkYAML/node.hpp>

namespace Tenshi {

class FileSystem;

class YamlException : public std::exception {
public:
    explicit YamlException(std::string message);
    const char* what() const noexcept override;

private:
    std::string message;
};

class YamlDocument {
public:
    using Node = fkyaml::node;

    YamlDocument() = default;
    explicit YamlDocument(Node root);

    Node& GetRoot();
    const Node& GetRoot() const;

    bool IsNull() const;
    bool IsMap() const;
    bool IsSequence() const;
    bool IsScalar() const;

private:
    Node root;
};

class YamlLoader {
public:
    explicit YamlLoader(FileSystem& files);
    YamlDocument Load(const std::string& path);
    YamlDocument Parse(const std::string& data);
private:
    FileSystem& files;

};

}