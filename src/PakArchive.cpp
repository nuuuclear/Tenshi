#include "PakArchive.h"

#include "PakFile.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>

#include <lz4.h>

namespace { // file constuction

constexpr uint32_t PAK_MAGIC = 0x4B415054;
constexpr uint32_t PAK_VERSION = 1;

#pragma pack(push, 1)

struct PakHeader {
    uint32_t magic;
    uint32_t version;

    uint64_t fileCount;
    uint64_t tableOffset;
};

struct PakEntryHeader {
    uint32_t pathLength;

    uint64_t offset;
    uint64_t compressedSize;
    uint64_t uncompressedSize;

    uint32_t compression;
    uint32_t checksum;
};

#pragma pack(pop)

uint32_t crc32(const uint8_t* data, size_t size) {
    uint32_t crc = 0xFFFFFFFFu;

    for (size_t i = 0; i < size; ++i) {
        crc ^= data[i];

        for (int bit = 0; bit < 8; ++bit) {
            uint32_t mask = -(crc & 1u);
            crc = (crc >> 1) ^ (0xEDB88320u & mask);
        }
    }

    return ~crc;
}

} // namespace

namespace Tenshi {

PakArchive::PakArchive() {
}

std::string PakArchive::normalizePath(const std::string& path) const {
    std::string result = path;

    std::replace(
        result.begin(),
        result.end(),
        '\\',
        '/'
    );

    if (result.rfind("res://", 0) == 0) {
        result = result.substr(6);
    }

    while (!result.empty() && result.front() == '/') {
        result.erase(result.begin());
    }

    std::filesystem::path p = std::filesystem::path(result).lexically_normal();
    result = p.generic_string();

    while (result.rfind("../", 0) == 0) {
        result.erase(0, 3);
    }

    if (result == ".." 
    ||  result == "."
    ) {
        result.clear();
    }

    return result;
}

bool PakArchive::load(const std::string& path) {
    entries.clear();

    archivePath = path;

    std::ifstream file(path, std::ios::binary);

    if (!file) return false;

    PakHeader header{};

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!file) return false;
    
    if (header.magic != PAK_MAGIC) {
        return false;
    }

    if (header.version != PAK_VERSION) {
        return false;
    }

    file.seekg(
        static_cast<std::streamoff>(
            header.tableOffset
        ),
        std::ios::beg
    );

    if (!file) return false;

    for (uint64_t i = 0; i < header.fileCount; ++i) {
        PakEntryHeader entryHeader{};

        file.read(reinterpret_cast<char*>(&entryHeader), sizeof(entryHeader));

        if (!file) {
            entries.clear();
            return false;
        }

        if (entryHeader.pathLength == 0 
        ||  entryHeader.pathLength > 1024
        ) {
            entries.clear();
            return false;
        }

        std::string pathString(entryHeader.pathLength, '\0');

        file.read(
            pathString.data(),
            static_cast<std::streamsize>(entryHeader.pathLength)
        );

        if (!file) {
            entries.clear();
            return false;
        }

        std::string normalized = normalizePath(pathString);

        if (normalized.empty()) {
            entries.clear();
            return false;
        }

        Entry entry{};

        entry.offset = entryHeader.offset;
        entry.compressedSize = entryHeader.compressedSize;
        entry.uncompressedSize = entryHeader.uncompressedSize;

        entry.compression = static_cast<Compression>(entryHeader.compression);

        entry.checksum = entryHeader.checksum;

        entries[normalized] = entry;
    }

    return true;
}

bool PakArchive::contains(const std::string& path) const {
    std::string normalized = normalizePath(path);

    return entries.find(normalized) != entries.end();
}

std::unique_ptr<File> PakArchive::open(const std::string& path) const {
    std::string normalized = normalizePath(path);

    auto it = entries.find(normalized);
    if (it == entries.end()) 
        return nullptr;
    
    std::vector<uint8_t> data;

    if (!readEntryData(it->second, data)) 
        return nullptr;
    
    return std::make_unique<PakFile>(data);
}

bool PakArchive::readEntryData(const Entry& entry, std::vector<uint8_t>& output) const {
    std::ifstream file(archivePath, std::ios::binary);
    if (!file) return false;

    file.seekg(
        static_cast<std::streamoff>(entry.offset),
        std::ios::beg
    );

    if (!file) return false;
    
    if (entry.compressedSize 
    >   static_cast<uint64_t>(std::numeric_limits<size_t>::max())
    ) {
        return false;
    }

    std::vector<uint8_t> compressed(static_cast<size_t>(entry.compressedSize));

    if (!compressed.empty()) {
        file.read(
            reinterpret_cast<char*>(compressed.data()),
            static_cast<std::streamsize>(compressed.size())
        );

        if (!file) return false;
    }

    if (entry.compression == Compression::None) {
        output = std::move(compressed);
    } else if (entry.compression == Compression::LZ4) {
        if (entry.uncompressedSize 
        >   static_cast<uint64_t>(std::numeric_limits<int>::max())
        ) {
            return false;
        }

        if (entry.compressedSize 
        >   static_cast<uint64_t>(std::numeric_limits<int>::max())
        ) {
            return false;
        }

        output.resize(
            static_cast<size_t>(entry.uncompressedSize)
        );

        int result = LZ4_decompress_safe(
            reinterpret_cast<const char*>(compressed.data()),
            reinterpret_cast<char*>(output.data()),
            static_cast<int>(compressed.size()),
            static_cast<int>(output.size())
        );

        if (result < 0) return false;

        if (static_cast<size_t>(result) 
        !=  output.size()
        ) {
            return false;
        }
    } else {
        return false;
    }

    if (crc32(output.data(), output.size()) 
    !=  entry.checksum
    ) {
        output.clear();
        return false;
    }

    return true;
}

uint64_t PakArchive::fileCount() const {
    return entries.size();
}

} // namespace Tenshi