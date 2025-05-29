#include <sigma-vm/RAM.hpp>
#include <cstdint>


RAM::RAM(uint64_t size) {
    this->content = std::vector<uint64_t>(size);
}


uint64_t RAM::getAt(uint64_t i) {
    if (i >= this->content.size()) {
        throw std::runtime_error("Memory address is out of range");
    }
    if (i < 0) {
        throw std::runtime_error("Memory address cannot be less than 0");
    }
    return this->content[(size_t)i];
}

void RAM::setAt(uint64_t i, uint64_t v) {
    if (i >= this->content.size()) {
        throw std::runtime_error("Memory address is out of range");
    }
    if (i < 0) {
        throw std::runtime_error("Memory address cannot be less than 0");
    }
    this->content[i] = v;
}
