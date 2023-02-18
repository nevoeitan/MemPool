#pragma once
#include <mutex>
#include <unordered_map>
#include <queue>
#include <memory>
#include <cstdint>

#include "FixedHeap.hpp"

class MemPool : public std::enable_shared_from_this<MemPool>
{
    FixedHeap _heap;

    using DataBlock = std::weak_ptr<uint8_t*>;
    std::queue<DataBlock> _alloc_queue;
    std::queue<DataBlock> _free_queue;

    using SmartDataBlock = std::shared_ptr<uint8_t*>;
    std::unordered_map<uint8_t*, SmartDataBlock> _map;

    std::mutex _mutex;

public:
    static std::shared_ptr<MemPool> create(uint64_t capacity, uint64_t packet_size)
    {
        return std::shared_ptr<MemPool>(new MemPool(capacity, packet_size));
    }

    std::shared_ptr<uint8_t*> getAddress()
    {
        ensureAddressAvailable();
        auto address = popAvailableAddress();

        return createSmartAddress(address);
    }

    void freeAddress(uint8_t* address)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        DataBlock data_block = _map.at(address);
        _free_queue.emplace(std::move(data_block));
    }

private:
    MemPool(uint64_t capacity,
            uint64_t packet_size)
            :
            _heap(capacity, packet_size)
    {
        buildIndexMapping();
    }

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

    void ensureAddressAvailable()
    {
        while(_alloc_queue.empty())
        {
            std::lock_guard<std::mutex> lock(_mutex);

            _alloc_queue.swap(_free_queue);
        }
    }

    uint8_t* popAvailableAddress()
    {
        auto data_block = _alloc_queue.front();
        _alloc_queue.pop();
        auto smart_data_block = data_block.lock();

        return *smart_data_block;
    }

    std::shared_ptr<uint8_t*> createSmartAddress(uint8_t* address)
    {
        auto shared_this = shared_from_this();

        return {new uint8_t*(address), [shared_this](uint8_t** address)
        {
            shared_this->freeAddress(*address);
        }};
    }
};
