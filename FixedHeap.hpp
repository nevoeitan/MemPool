#pragma once
#include <cstdint>

#include <sys/mman.h>

class FixedHeap
{
    uint8_t* _heap;
    uint64_t _capacity;
    uint64_t _block_size;
    uint64_t _allocated_size;

    static constexpr auto HEAP_MUTEX_TRAITS = PROT_READ | PROT_WRITE;
    static constexpr auto HEAP_PRIVATE_TRAITS = MAP_PRIVATE | MAP_ANONYMOUS;

public:
    FixedHeap(uint64_t capacity,
              uint64_t block_size)
              :
              _capacity(capacity),
              _block_size(block_size),
              _allocated_size(_capacity * _block_size * sizeof(std::byte))
    {
        _heap = static_cast<uint8_t*>(mmap(
                nullptr,
                _allocated_size,
                HEAP_MUTEX_TRAITS,
                HEAP_PRIVATE_TRAITS,
                0,
                0
        ));

        if(_heap == MAP_FAILED)
        {
            throw std::runtime_error("Allocating map failed");
        }
    }

    ~FixedHeap()
    {
        munmap(_heap, _allocated_size);
    }

    uint8_t* calculateBlockAddress(uint64_t block_index) const
    {
        auto block_offset = block_index * _block_size * sizeof(std::byte);

        return _heap + block_offset;
    }

    uint64_t getCapacity() const
    {
        return _capacity;
    }
};