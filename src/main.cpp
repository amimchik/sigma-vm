#include <iostream>
#include <sasm/Lexer.hpp>
#include <sasm/Parser.hpp>
#include <sasm/CodeGenerator.hpp>
#include <sigma-vm/VirtualMachine.hpp>

#include <iomanip>


void loadProgram(VirtualMachine& vm, std::initializer_list<uint16_t> data) {
    auto it = data.begin();
    size_t i = 0;
    while (it != data.end()) {
        if (std::distance(it, data.end()) < 8) {
            throw std::runtime_error("Incomplete instruction: expected 8 uint16_t");
        }

        uint64_t instr = 0;
        uint64_t val = 0;
        for (int j = 0; j < 4; ++j) {
            instr |= (uint64_t)(*it++) << (j * 16);
        }
        for (int j = 0; j < 4; ++j) {
            val |= (uint64_t)(*it++) << (j * 16);
        }

        vm.ram.setAt(i++, instr);
        vm.ram.setAt(i++, val);
    }
}

int main() {
    std::string input;
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        input += (char)c;
    }
    Lexer lexer(input);
    Parser parser(lexer);
    CodeGenerator codeGen(parser);

    auto code = codeGen.genAll();

    VirtualMachine vm(1024);

    for (size_t i = 0; i < code.size(); i++) {
        vm.ram.setAt(i * 2, code[i][0]);
        vm.ram.setAt(i * 2 + 1, code[i][1]);
    }

    for (size_t i = 0; i < code.size() * 2; i++) {
    std::cout << std::hex << std::setw(16) << std::setfill('0') << vm.ram.getAt(i) << "\n";
    }
    vm.launch();
}
