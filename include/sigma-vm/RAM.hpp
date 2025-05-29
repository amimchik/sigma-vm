#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

class RAM {
private:
    std::vector<uint64_t> content;
public:
    RAM(uint64_t size);
    uint64_t getAt(uint64_t i);
    void setAt(uint64_t i, uint64_t v);
};
