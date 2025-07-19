#include <cstdint>
#include <sasm/Lexer.hpp>

#include "sigma-vm/VirtualMachine.hpp"

std::string toString(TokenType type) {
	switch (type) {
	case TokenType::Mov:
		return "Mov";
	case TokenType::Lod:
		return "Lod";
	case TokenType::Sav:
		return "Sav";
	case TokenType::Ldi:
		return "Ldi";
	case TokenType::Add:
		return "Add";
	case TokenType::Min:
		return "Min";
	case TokenType::Mul:
		return "Mul";
	case TokenType::Div:
		return "Div";
	case TokenType::Mod:
		return "Mod";
	case TokenType::Gth:
		return "Gth";
	case TokenType::Lth:
		return "Lth";
	case TokenType::Geq:
		return "Geq";
	case TokenType::Leq:
		return "Leq";
	case TokenType::Equ:
		return "Equ";
	case TokenType::Neq:
		return "Neq";
	case TokenType::Land:
		return "Land";
	case TokenType::Lor:
		return "Lor";
	case TokenType::Not:
		return "Not";
	case TokenType::Band:
		return "Band";
	case TokenType::Bor:
		return "Bor";
	case TokenType::Bnot:
		return "Bnot";
	case TokenType::Xor:
		return "Xor";
	case TokenType::Jmp:
		return "Jmp";
	case TokenType::Jiz:
		return "Jiz";
	case TokenType::Jnz:
		return "Jnz";
	case TokenType::Comma:
		return "Comma";
	case TokenType::Semicolon:
		return "Semicolon";
	case TokenType::Colon:
		return "Colon";
	case TokenType::A:
		return "A";
	case TokenType::B:
		return "B";
	case TokenType::C:
		return "C";
	case TokenType::Ip:
		return "Ip";
	case TokenType::Sp:
		return "Sp";
	case TokenType::Sbp:
		return "Sbp";
	case TokenType::Flg:
		return "Flg";
	case TokenType::SecReg:
		return "SecReg";
	case TokenType::Word:
		return "Word";
	case TokenType::Num:
		return "Num";
	case TokenType::EndOfFile:
		return "EndOfFile";
	default:
		return "Unknown";
	}
}

Token::Token() {
}

Token::Token(TokenType type)
    : type(type) {
}

Token Token::makeAddReg(size_t n) {
	Token token(TokenType::SecReg);
	token.numVal = n;
	return token;
}

Token Token::makeNum(uint64_t numVal) {
	Token token(TokenType::Num);
	token.numVal = numVal;
	return token;
}

Token Token::makeWord(std::string wordVal) {
	Token token(TokenType::Word);
	token.wordVal = wordVal;
	return token;
}

Token Token::T_EOF() {
	return Token(TokenType::EndOfFile);
}

std::string Token::toString() {
	if (this->type == TokenType::Word) {
		return "word:" + this->wordVal;
	}
	if (this->type == TokenType::Num) {
		return "num:" + std::to_string(this->numVal);
	}
	if (this->type == TokenType::SecReg) {
		return "reg" + std::to_string(this->numVal);
	}
	return ::toString(this->type);
}

char Lexer::current() {
	return this->pos < this->input.size() ? this->input[this->pos] : 0;
}

char Lexer::next() {
	if (this->pos < this->input.size()) {
		this->pos++;
	}
	return this->current();
}

Token Lexer::tokenizeWord() {
	std::string buffer;

	char c;
	while (std::isalnum(c = std::tolower(this->current())) || c == '_') {
		buffer += c;
		this->next();
	}

	if (this->kwrds.find(buffer) != this->kwrds.end()) {
		return this->kwrds.at(buffer);
	}
	return Token::makeWord(buffer);
}

Token Lexer::tokenizeNum() {
	uint64_t v = 0;

	bool hexMode = true;

	char c = this->current();

	hexMode = hexMode && (c == '0');

	if (!hexMode) {
		v += c - '0';
	}

	c = std::tolower(this->next());

	// std::cout << c << std::endl;
	// std::cout << (hexMode ? "true" : "false") << std::endl;

	hexMode = hexMode && (c == 'x');
	if (hexMode) {
		c = std::tolower(this->next());
	}

	while (std::isdigit(c) ||
	       (hexMode && (c >= 'a' && c <= 'f'))) {
		v *= hexMode ? 0x10 : 10;
		if (hexMode) {
			v += std::isdigit(c) ? c - '0' : c - 'a' + 10;
		} else {
			v += c - '0';
		}
		c = std::tolower(this->next());
	}

	return Token::makeNum(v);
}

Token Lexer::nextToken() {
	while (true) {
		char c = this->current();
		// std::cout << '\'' << c << '\'' << std::endl;
		if (std::isdigit(c)) {
			return this->tokenizeNum();
		} else if (std::isalpha(c) || c == '_') {
			return this->tokenizeWord();
		} else if (c == ';') {
			this->next();
			return Token(TokenType::Semicolon);
		} else if (c == ':') {
			this->next();
			return Token(TokenType::Colon);
		} else if (c == ',') {
			this->next();
			return Token(TokenType::Comma);
		} else if (c == '\0') {
			return Token::T_EOF();
		}
		this->next();
	}
}

std::deque<Token> Lexer::tokenizeAll() {
	std::deque<Token> tokens;
	Token temp;
	while ((temp = this->nextToken()).type != TokenType::EndOfFile) {
		tokens.push_back(temp);
	}
	tokens.push_back(temp);
	return tokens;
}

Lexer::Lexer(std::string input)
    : Lexer() {
	this->input = input;
}

Lexer::Lexer() {
	this->kwrds = std::map<std::string, Token>{
	    {"mov", Token(TokenType::Mov)},
	    {"lod", Token(TokenType::Lod)},
	    {"sav", Token(TokenType::Sav)},
	    {"ldi", Token(TokenType::Ldi)},
	    {"add", Token(TokenType::Add)},
	    {"min", Token(TokenType::Min)},
	    {"mul", Token(TokenType::Mul)},
	    {"div", Token(TokenType::Div)},
	    {"mod", Token(TokenType::Mod)},
	    {"gth", Token(TokenType::Gth)},
	    {"lth", Token(TokenType::Lth)},
	    {"geq", Token(TokenType::Geq)},
	    {"leq", Token(TokenType::Leq)},
	    {"equ", Token(TokenType::Equ)},
	    {"neq", Token(TokenType::Neq)},
	    {"land", Token(TokenType::Land)},
	    {"lor", Token(TokenType::Lor)},
	    {"not", Token(TokenType::Not)},
	    {"band", Token(TokenType::Band)},
	    {"bor", Token(TokenType::Bor)},
	    {"bnot", Token(TokenType::Bnot)},
	    {"xor", Token(TokenType::Xor)},
	    {"jmp", Token(TokenType::Jmp)},
	    {"jiz", Token(TokenType::Jiz)},
	    {"jnz", Token(TokenType::Jnz)},
	    {"a", Token(TokenType::A)},
	    {"b", Token(TokenType::B)},
	    {"c", Token(TokenType::C)},
	    {"sp", Token(TokenType::Sp)},
	    {"sbp", Token(TokenType::Sbp)},
	    {"ip", Token(TokenType::Ip)},
	    {"flg", Token(TokenType::Flg)},
	};
	for (size_t i = 0; i < ADD_REGS_COUNT; i++) {
		this->kwrds.insert({"r" + std::to_string(i), Token::makeAddReg(i)});
	}
	this->pos = 0;
}

bool Lexer::atEof() {
	return this->current() == 0;
}

void Lexer::reset() {
	this->pos = 0;
}
