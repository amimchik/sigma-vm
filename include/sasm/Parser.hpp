#pragma once

#include <deque>
#include <sasm/Lexer.hpp>
#include <stdexcept>

enum class InstructionType {
    Mov,
    Lod,
    Sav,

    Ldi,

    Add,
    Min,
    Mul,
    Div,
    Mod,

    Gth,
    Lth,
    Geq,
    Leq,

    Equ,
    Neq,

    Land,
    Lor,
    Not,

    Band,
    Bor,
    Bnot,
    Xor,

    Jmp,
    Jiz,
    Jnz,
};

std::string toString(InstructionType instr);

enum class RegisterType {
    A,
    B,
    C,
    Ip,
    Sp,
    Sbp,
    Flg,
    AddReg,
};

struct Register {
    RegisterType type;
    uint64_t addRegNumber;
    std::string toString();
};

struct Instruction {
    bool expandable;
    InstructionType type;
    Register left;
    Register right;
    uint64_t arg;
    std::string toString();
};

class Parser {
private:
    Lexer lexer;
    std::deque<Token> tokensTrace;
    Token current();
    void advance();
    Token peek(uint64_t offset);
    Register parseReg(Token t);
    bool match(TokenType type, Token& t);
    std::deque<std::pair<TokenType, InstructionType>> ttAndItPairs;
    Parser();
    InstructionType convertTtToIt(TokenType tt);

public:
    Parser(Lexer lexer);
    Instruction next();
    std::deque<Instruction> parseAll();
    bool atEof();
    void reset();
};
