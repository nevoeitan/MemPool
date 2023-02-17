#include <iostream>
#include <vector>

#include "MemPool.hpp"

int main()
{
    constexpr auto capacity = 3;
    constexpr auto packet_size = 10;

    MemPool pool(capacity, packet_size);

    std::vector<uint8_t> buffer(packet_size, 'a');
    auto block_address = pool.getAddress();
    std::memcpy(block_address, buffer.data(), buffer.size());

    std::vector<uint8_t> buffer2(packet_size, '0');
    std::memcpy(buffer2.data(), block_address, packet_size);

    std::cout << buffer2.data() << std::endl;

    return 0;
}
