#pragma once
#include <mutex>
#include <unordered_map>
#include <queue>
#include <memory>
#include <cstdint>

#include "FixedHeap.hpp"

class MemPool
{
    FixedHeap _heap;

    using DataBlock = std::weak_ptr<uint8_t*>;
    std::queue<DataBlock> _alloc_queue;
    std::queue<DataBlock> _free_queue;

    using SmartDataBlock = std::shared_ptr<uint8_t*>;
    std::unordered_map<uint8_t*, SmartDataBlock> _map;

    std::mutex _mutex;

public:
    MemPool(uint64_t capacity,
            uint64_t packet_size)
            :
            _heap(capacity, packet_size)
    {
        buildIndexMapping();
    }

    uint8_t* getAddress()
    {
        while(_alloc_queue.empty())
        {
            std::lock_guard<std::mutex> lock(_mutex);

            _alloc_queue = std::move(_free_queue);
        }

        auto data_block = _alloc_queue.front();
        _alloc_queue.pop();

        auto smart_data_block = data_block.lock();
        auto address = *smart_data_block;

        return address;
    }

    void freeAddress(uint8_t* address)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        DataBlock data_block = _map.at(address);
        _free_queue.emplace(std::move(data_block));
    }

private:
    void buildIndexMapping()
    {
        auto capacity = _heap.getCapacity();

        for(uint64_t index = 0; index < capacity; ++index)
        {
            auto block_address = _heap.calculateBlockAddress(index);
            auto smart_data_block = std::make_shared<uint8_t*>(block_address);

            _map[block_address] = smart_data_block;
            _alloc_queue.emplace(std::move(smart_data_block));
        }
    }
};
