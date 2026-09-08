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
    FileSystem();
    ~FileSystem();

    void setRoot(const std::string& path);

    bool mountDirectory(
        const std::string& virtualPath,
        const std::string& directory
    );

    bool mountPak(
        const std::string& virtualPath,
        const std::string& pakPath
    );

    bool exists(const std::string& path) const;

    std::unique_ptr<File> open(const std::string& path) const;
    std::vector<uint8_t> readAll(const std::string& path) const;
    std::string resolve(const std::string& path) const;

private:
    struct Mount;

    std::string root;

    std::vector<std::unique_ptr<Mount>> mounts;

    std::string normalizePath(const std::string& path) const;
    bool isWithinRoot(const std::string& path) const;
};

} // namespace Tenshi