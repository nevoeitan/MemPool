# MemPool

This project shows how a memory pool can be implemented in order to offer scalable solution for high bandwidth nerwork applications.

## Usage

To use the code include MemPool.hpp

## Example
Here is an exmaple from main.cpp:
```
MemPool pool(capacity, packet_size);

auto block_address = pool.getAddress();
// Do receive on address and required calculations
pool.freeAddress(block_address);
```
