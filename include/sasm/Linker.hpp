#pragma once

#include <cstdint>
#include <deque>
#include <map>
#include <sasm/Parser.hpp>
#include <string>

class Linker {
private:
    Linker();
    std::map<std::string, uint64_t> labels;
    Parser parser;

public:
    Linker(Parser parser);
    Instruction next();
    std::deque<Instruction> linkAll();
};
