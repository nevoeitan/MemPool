#include <iostream>
#include <vector>

#include "MemPool.hpp"

int main()
{
    constexpr auto capacity = 1;
    constexpr auto packet_size = 10;

    auto pool = MemPool::create(capacity, packet_size);

    std::vector<uint8_t> src_buffer(packet_size, 'a');
    std::vector<uint8_t> dest_buffer(packet_size, '0');

    auto addr_ptr = pool->getAddress();
    std::memcpy(*addr_ptr, src_buffer.data(), packet_size);
    std::memcpy(dest_buffer.data(), *addr_ptr, packet_size);

    std::cout << dest_buffer.data() << std::endl;

    return 0;
}
