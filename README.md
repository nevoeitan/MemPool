# MemPool

This project shows how a memory pool can be implemented in order to offer scalable solution for high bandwidth nerwork applications.
The idea is to have 1 receive thread with N worker threads.

## Usage

To use the code include MemPool.hpp

## Example
Here is an exmaple from main.cpp:
```
auto pool = MemPool::create(capacity, packet_size);

{
    auto block_address_ptr = pool->getAddress();
    // Do receive on address and required calculations
}//Address released back to pool automatically
```
