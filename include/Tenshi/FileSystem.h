#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Tenshi {

class File {
public:
    virtual ~File() = default;

    virtual size_t read(void* buffer, size_t size) = 0;
    virtual bool seek(int64_t offset, int origin) = 0;
    virtual int64_t tell() const = 0;
    virtual int64_t size() const = 0;

    bool readAll(std::vector<uint8_t>& output);
};

class FileSystem {
public:
    // A virtual filesystem.
    FileSystem();
    ~FileSystem();

    // Set the root of the filesystem (most commonly the executable directory)
    void setRoot(const std::string& path);

    // Mount a directory using it's path
    bool mountDirectory(
        const std::string& virtualPath,
        const std::string& directory
    );

    // Mount a Pak file using it's physical path
    bool mountPak(
        const std::string& virtualPath,
        const std::string& pakPath
    );

    // Check if a file or directory exists within the filesystem
    bool exists(const std::string& path) const;

    // Open a file
    std::unique_ptr<File> open(const std::string& path) const;

    // Read all data from a file using it's path
    std::vector<uint8_t> readAll(const std::string& path) const;

    // Resolve a path from the filesystem
    std::string resolve(const std::string& path) const;

private:
    struct Mount;

    std::string root;

    std::vector<std::unique_ptr<Mount>> mounts;

    std::string normalizePath(const std::string& path) const;
    bool isWithinRoot(const std::string& path) const;
};

} // namespace Tenshi