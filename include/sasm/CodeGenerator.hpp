#pragma once

#include <array>
#include <cstdint>
#include <sasm/Parser.hpp>

class CodeGenerator {
private:
    Parser parser;
    CodeGenerator();
    struct RegBin {
        uint8_t flag;
        uint16_t arg;
    };
    RegBin convReg(Register r);

public:
    CodeGenerator(Parser parser);
    std::array<uint64_t, 2> genInstruction();
    std::vector<std::array<uint64_t, 2>> genAll();
};
