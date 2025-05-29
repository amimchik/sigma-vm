#pragma once

#include <sigma-vm/VirtualMachine.hpp>

#include <cctype>
#include <cstdint>
#include <deque>
#include <iostream>
#include <map>
#include <string>

enum class TokenType {
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

    Comma,
    Semicolon,

    A,
    B,
    C,
    Ip,
    Sp,
    Sbp,
    Flg,

    SecReg,

    Word,
    Num,

    EndOfFile,
};

std::string toString(TokenType type); 

struct Token {
    TokenType type;
    uint64_t numVal;
    std::string wordVal;
    Token(TokenType type);
    Token();
    static Token makeNum(uint64_t numVal);
    static Token makeWord(std::string wordVal);
    static Token makeAddReg(size_t n);
    static Token T_EOF();
    std::string toString();
};

class Lexer {
private:
    Lexer();
    std::string input;
    size_t pos;
    char current();
    char next();
    Token tokenizeNum();
    Token tokenizeWord();
    std::map<std::string, Token> kwrds;

public:
    Lexer(std::string input);
    std::deque<Token> tokenizeAll();
    Token nextToken();
    bool atEof();
    void reset();
};
