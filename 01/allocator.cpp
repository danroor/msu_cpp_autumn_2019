#include "allocator.h"

namespace Alloc
{
    char *begin, *freemem;
    size_t maxsz;
};

void makeAllocator(size_t maxSize)
{
    Alloc::begin = (char *) malloc(maxSize);
    Alloc::freemem = Alloc::begin;
    Alloc::maxsz = maxSize;
}

char* alloc(size_t size)
{
    if (Alloc::freemem == nullptr || 
        Alloc::freemem + size > Alloc::begin + Alloc::maxsz)
            return nullptr;

    char *ptr = Alloc::freemem;
    Alloc::freemem += size;
    return ptr;
}

void reset()
{
    Alloc::freemem = Alloc::begin;
}
