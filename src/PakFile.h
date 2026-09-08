#pragma once

#include "FileSystem.h"

#include <cstdint>
#include <vector>

namespace Tenshi {

class PakFile : public File {
public:
    PakFile(const std::vector<uint8_t>& data);

    size_t read(void* buffer, size_t size) override;

    bool seek(int64_t offset,int origin) override;

    int64_t tell() const override;
    int64_t size() const override;

private:
    std::vector<uint8_t> data;
    size_t position = 0;

    // std::string* fs_path;
    
};

} // namespace Tenshi