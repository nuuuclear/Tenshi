#include "Tenshi/FileSystem.h"

#include "PakArchive.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <limits>

namespace fs = std::filesystem;

namespace Tenshi {

class DirectoryFile : public File {
public:
    explicit DirectoryFile(const fs::path& path)
        : file(path, std::ios::binary) 
    {
    }

    bool isOpen() const {
        return file.is_open();
    }

    // TODO: fix potential file size missmatch issue
    size_t read(void* buffer, size_t size) override {
        if (!file || !buffer || size == 0) {
            return 0;
        }

        file.read(
            static_cast<char*>(buffer),
            static_cast<std::streamsize>(size)
        );

        return static_cast<size_t>(file.gcount());
    }

    bool seek(int64_t offset, int origin) override {
        if (!file) {
            return false;
        }

        std::ios_base::seekdir direction;

        switch (origin) {
            case SEEK_SET:
                direction = std::ios::beg;
                break;

            case SEEK_CUR:
                direction = std::ios::cur;
                break;

            case SEEK_END:
                direction = std::ios::end;
                break;

            default:
                return false;
        }

        file.clear();
        file.seekg(static_cast<std::streamoff>(offset), direction);

        return !file.fail();
    }

    int64_t tell() const override {
        if (!file) {
            return -1;
        }

        auto position = file.tellg();

        if (position == std::streampos(-1)) {
            return -1;
        }

        return static_cast<int64_t>(position);
    }

    int64_t size() const override {
        if (!file) {
            return -1;
        }

        auto current = file.tellg();

        if (current == std::streampos(-1)) {
            return -1;
        }

        file.seekg(0, std::ios::end);

        auto end = file.tellg();

        file.seekg(current);

        if (end == std::streampos(-1)) {
            return -1;
        }

        return static_cast<int64_t>(end);
    }

private:
    mutable std::ifstream file;
};

struct FileSystem::Mount {
    enum class Type {
        Directory,
        Pak
    };

    Type type;

    std::string virtualPath;
    std::string directory;

    std::shared_ptr<PakArchive> pak;
};

FileSystem::FileSystem() {
}

FileSystem::~FileSystem() = default;

void FileSystem::setRoot(const std::string& path) {
    root = fs::absolute(path).lexically_normal().string();
}

bool FileSystem::mountDirectory(
    const std::string& virtualPath,
    const std::string& directory
) {
    fs::path physicalPath = directory;

    if (!physicalPath.is_absolute() && !root.empty()) {
        physicalPath = fs::path(root) / physicalPath;
    }

    physicalPath = fs::absolute(physicalPath).lexically_normal();

    if (!fs::exists(physicalPath) ||
        !fs::is_directory(physicalPath)) {
        return false;
    }

    auto mount = std::make_unique<Mount>();

    mount->type = Mount::Type::Directory;
    mount->virtualPath = normalizePath(virtualPath);
    mount->directory = physicalPath.string();

    mounts.insert(
        mounts.begin(),
        std::move(mount)
    );

    return true;
}

bool FileSystem::mountPak(
    const std::string& virtualPath,
    const std::string& pakPath
) {
    fs::path physicalPath = pakPath;

    if (!physicalPath.is_absolute() && !root.empty()) {
        physicalPath = fs::path(root) / physicalPath;
    }

    physicalPath = fs::absolute(physicalPath).lexically_normal();

    auto pak = std::make_shared<PakArchive>();

    if (!pak->load(physicalPath.string())) {
        return false;
    }

    auto mount = std::make_unique<Mount>();

    mount->type = Mount::Type::Pak;
    mount->virtualPath = normalizePath(virtualPath);
    mount->pak = pak;

    mounts.insert(
        mounts.begin(),
        std::move(mount)
    );

    return true;
}

std::string FileSystem::normalizePath(const std::string& path) const {
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

    fs::path normalized = fs::path(result).lexically_normal();
    result = normalized.generic_string();

    if (result == ".") {
        result.clear();
    }

    while (result.rfind("../", 0) == 0) {
        result.erase(0, 3);
    }

    if (result == "..") {
        result.clear();
    }

    return result;
}

bool FileSystem::isWithinRoot(const std::string& path) const {
    fs::path rootPath = fs::absolute(root).lexically_normal();

    fs::path targetPath = fs::absolute(path).lexically_normal();

    auto rootIt = rootPath.begin();
    auto targetIt = targetPath.begin();

    for (; rootIt != rootPath.end(); ++rootIt, ++targetIt) {
        if (targetIt == targetPath.end()) {
            return false;
        }

        if (*rootIt != *targetIt) {
            return false;
        }
    }

    return true;
}

bool FileSystem::exists(const std::string& path) const {
    return open(path) != nullptr;
}

std::unique_ptr<File> FileSystem::open(const std::string& path) const {
    std::string normalized = normalizePath(path);

    for (const auto& mount : mounts) {
        std::string relative = normalized;

        if (!mount->virtualPath.empty()) {
            if (relative == mount->virtualPath) {
                relative.clear();
            } else {
                std::string prefix =
                    mount->virtualPath + "/";

                if (relative.rfind(prefix, 0) != 0) {
                    continue;
                }

                relative =
                    relative.substr(prefix.size());
            }
        }

        if (mount->type == Mount::Type::Pak) {
            if (mount->pak->contains(relative)) {
                return mount->pak->open(relative);
            }
        } else {
            fs::path physical = fs::path(mount->directory) / relative;

            physical = fs::absolute(physical).lexically_normal();

            if (!isWithinRoot(physical.string())) {
                continue;
            }

            if (!fs::exists(physical) ||
                !fs::is_regular_file(physical)) {
                continue;
            }

            auto file = std::make_unique<DirectoryFile>(physical);

            if (file->isOpen()) {
                return file;
            }
        }
    }

    return nullptr;
}

std::vector<uint8_t> FileSystem::readAll(const std::string& path) const {
    auto file = open(path);
    if (!file) return {};
    
    int64_t fileSize = file->size();
    if (fileSize <= 0) return {};

    std::vector<uint8_t> data(static_cast<size_t>(fileSize));
    if (!file->readAll(data)) return {};

    return data;
}

std::string FileSystem::resolve(const std::string& path) const {
    std::string normalized = normalizePath(path);

    for (const auto& mount : mounts) {
        std::string relative = normalized;

        if (!mount->virtualPath.empty()) {
            std::string prefix = mount->virtualPath + "/";

            if (relative.rfind(prefix, 0) != 0) {
                continue;
            }

            relative = relative.substr(prefix.size());
        }

        if (mount->type == Mount::Type::Directory) {
            fs::path physical = fs::path(mount->directory) / relative;

            physical = fs::absolute(physical).lexically_normal();

            if (fs::exists(physical)) {
                return physical.string();
            }
        }
    }

    return {};
}

bool File::readAll(std::vector<uint8_t>& output) {
    if (size() < 0) return false;
    
    int64_t fileSize = size();

    output.resize(static_cast<size_t>(fileSize));

    if (fileSize == 0) return true;
    
    if (read(output.data(), output.size()) != output.size()) {
        output.clear();
        return false;
    }

    return true;
}

} // namespace Tenshi