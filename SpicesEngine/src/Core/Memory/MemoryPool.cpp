#include "Pchheader.h"
#include "MemoryPool.h"

namespace Spices {

    bool MemoryPool::CreatePool(size_t size, MemorySizeUnit unit)
    {
        size_t s = size * (size_t)unit;
        size_t s1 = s / 8;
        start = malloc(s + s1);

        return true;
    }

}