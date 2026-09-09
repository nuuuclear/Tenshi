#pragma once

#include "Tenshi/FileSystem.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Tenshi {

class PakArchive {
public:
    PakArchive();

    bool load(const std::string& path);
    bool contains(const std::string& path) const;
    std::unique_ptr<File> open(const std::string& path) const;

    uint64_t fileCount() const;
    void printFiles() const;

private:
    enum class Compression : uint32_t {
        None = 0,
        LZ4 = 1
    };

    struct Entry {
        uint64_t offset;
        uint64_t compressedSize;
        uint64_t uncompressedSize;

        Compression compression;

        uint32_t checksum;
    };

    std::string archivePath;

    std::unordered_map<std::string, Entry> entries;

    std::string normalizePath(const std::string& path) const;

    bool readHeader(std::ifstream& file);
    bool readEntryData(const Entry& entry, std::vector<uint8_t>& output) const;
};

} // namespace Tenshi