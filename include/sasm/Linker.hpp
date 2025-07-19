#pragma once

#include <cstdint>
#include <deque>
#include <format>
#include <map>
#include <sasm/Parser.hpp>
#include <string>

class Linker {
private:
	Linker();
	std::map<std::string, uint64_t> labels;
	std::deque<Instruction> instructions;
	std::deque<Instruction> linkedForward;
	uint64_t resolveLabel(std::string label);
	uint64_t currentAddr;
	void addLabel(std::string label);
	Parser parser;
	bool linkingForward;

public:
	Linker(Parser parser);
	Instruction next();
	std::deque<Instruction> linkAll();
	bool atEof();
};
