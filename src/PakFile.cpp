#include "PakFile.h"

#include <algorithm>
#include <cstring>

namespace Tenshi {

PakFile::PakFile(const std::vector<uint8_t>& data) 
    : data(data) 
{
}

size_t PakFile::read(void* buffer, size_t size) {
    if (!buffer || size == 0) 
        return 0;
    
    if (position >= data.size()) 
        return 0;
    
    size_t available = data.size() - position;
    size_t amount = std::min(size, available);

    std::memcpy(buffer, data.data() + position, amount);

    position += amount;

    return amount;
}

bool PakFile::seek(int64_t offset, int origin) {
    int64_t newPosition;

    switch (origin) {
        case SEEK_SET:
            newPosition = offset;
            break;

        case SEEK_CUR:
            newPosition 
            =   static_cast<int64_t>(position) 
            +   offset;
            break;

        case SEEK_END:
            newPosition 
            =   static_cast<int64_t>(data.size()) 
            +   offset;
            break;

        default:
            return false;
    }

    if (newPosition < 0 || newPosition > static_cast<int64_t>(data.size())) {
        return false;
    }

    position = static_cast<size_t>(newPosition);

    return true;
}

int64_t PakFile::tell() const {
    return static_cast<int64_t>(position);
}

int64_t PakFile::size() const {
    return static_cast<int64_t>(data.size());
}

} // namespace Tenshi