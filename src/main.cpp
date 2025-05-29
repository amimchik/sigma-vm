#include <iostream>
#include <sasm/Lexer.hpp>
#include <sasm/Parser.hpp>
#include <sasm/CodeGenerator.hpp>
#include <sigma-vm/VirtualMachine.hpp>

#include <iomanip>

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
