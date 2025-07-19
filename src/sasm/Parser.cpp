#include <format>
#include <sasm/Parser.hpp>
#include <string>
#include <stdexcept>

#include "sasm/Lexer.hpp"

Parser::Parser(Lexer lexer)
    : lexer(lexer) {
	this->tokensTrace.push_back(this->lexer.nextToken());
	this->ttAndItPairs = std::deque<std::pair<TokenType, InstructionType>>{
	    {TokenType::Mov, InstructionType::Mov},
	    {TokenType::Lod, InstructionType::Lod},
	    {TokenType::Sav, InstructionType::Sav},
	    {TokenType::Ldi, InstructionType::Ldi},
	    {TokenType::Add, InstructionType::Add},
	    {TokenType::Min, InstructionType::Min},
	    {TokenType::Mul, InstructionType::Mul},
	    {TokenType::Div, InstructionType::Div},
	    {TokenType::Mod, InstructionType::Mod},
	    {TokenType::Gth, InstructionType::Lth},
	    {TokenType::Lth, InstructionType::Lth},
	    {TokenType::Geq, InstructionType::Geq},
	    {TokenType::Leq, InstructionType::Leq},
	    {TokenType::Equ, InstructionType::Equ},
	    {TokenType::Neq, InstructionType::Neq},
	    {TokenType::Land, InstructionType::Land},
	    {TokenType::Lor, InstructionType::Lor},
	    {TokenType::Not, InstructionType::Not},
	    {TokenType::Band, InstructionType::Band},
	    {TokenType::Bor, InstructionType::Bor},
	    {TokenType::Bnot, InstructionType::Bnot},
	    {TokenType::Xor, InstructionType::Xor},
	    {TokenType::Jmp, InstructionType::Jmp},
	    {TokenType::Jiz, InstructionType::Jiz},
	    {TokenType::Jnz, InstructionType::Jnz},
	    {TokenType::Word, InstructionType::Label}};
}
std::string toString(InstructionType instr) {
	switch (instr) {
	case InstructionType::Mov:
		return "MOV";
	case InstructionType::Lod:
		return "LOD";
	case InstructionType::Sav:
		return "SAV";
	case InstructionType::Ldi:
		return "LDI";
	case InstructionType::Add:
		return "ADD";
	case InstructionType::Min:
		return "MIN";
	case InstructionType::Mul:
		return "MUL";
	case InstructionType::Div:
		return "DIV";
	case InstructionType::Mod:
		return "MOD";
	case InstructionType::Gth:
		return "GTH";
	case InstructionType::Lth:
		return "LTH";
	case InstructionType::Geq:
		return "GEQ";
	case InstructionType::Leq:
		return "LEQ";
	case InstructionType::Equ:
		return "EQU";
	case InstructionType::Neq:
		return "NEQ";
	case InstructionType::Land:
		return "LAND";
	case InstructionType::Lor:
		return "LOR";
	case InstructionType::Not:
		return "NOT";
	case InstructionType::Band:
		return "BAND";
	case InstructionType::Bor:
		return "BOR";
	case InstructionType::Bnot:
		return "BNOT";
	case InstructionType::Xor:
		return "XOR";
	case InstructionType::Jmp:
		return "JMP";
	case InstructionType::Jiz:
		return "JIZ";
	case InstructionType::Jnz:
		return "JNZ";
	default:
		return "UNKNOWN";
	}
}

std::string Register::toString() {
	switch (this->type) {
	case RegisterType::A:
		return "A";
	case RegisterType::B:
		return "B";
	case RegisterType::C:
		return "C";
	case RegisterType::Ip:
		return "IP";
	case RegisterType::Sp:
		return "SP";
	case RegisterType::Sbp:
		return "SBP";
	case RegisterType::Flg:
		return "FLG";
	case RegisterType::AddReg:
		return "R" + std::to_string(this->addRegNumber);
	default:
		return "UNKNOWN";
	}
}

std::string Instruction::toString() {
	std::string result = ::toString(this->type);
	if (this->expandable) {
		if (this->type == InstructionType::Label) {
			result = std::format("{}:", this->strVal);
		} else if (this->type == InstructionType::Ldi) {
			result += std::format(" {} {};", this->left.toString(), this->strVal);
		}
	} else {
		switch (this->type) {
		case InstructionType::Mov:
			result += ' ' + this->left.toString() + ' ' + this->right.toString();
			break;
		case InstructionType::Ldi:
			result += ' ' + this->left.toString() + ' ' + std::to_string(this->arg);
		default:
			break;
		}

		result += ';';
	}
	return result;
}

bool Parser::atEof() {
	return this->lexer.atEof();
}

void Parser::reset() {
	this->lexer.reset();
}

Token Parser::current() {
	if (this->tokensTrace.size() == 0) {
		this->tokensTrace.push_back(this->lexer.nextToken());
	}
	return this->tokensTrace[0];
}

void Parser::advance() {
	if (this->tokensTrace.size() <= 1) {
		this->tokensTrace.push_back(this->lexer.nextToken());
	}
	this->tokensTrace.pop_front();
}

Token Parser::peek(uint64_t offset) {
	while (offset >= this->tokensTrace.size() - 1) {
		this->tokensTrace.push_back(this->lexer.nextToken());
	}
	return this->tokensTrace[offset];
}

std::deque<Instruction> Parser::parseAll() {
	std::deque<Instruction> instructions;
	while (!this->atEof()) {
		instructions.push_back(this->next());
	}
	return instructions;
}

Instruction Parser::next() {
	Token t = this->current();
	Instruction i;
	if (t.type == TokenType::EndOfFile) {
		return Instruction();
	}
	i.type = this->convertTtToIt(t.type);
	switch (t.type) {
	case TokenType::Mov: {
		this->advance();
		t = this->current();
		Register left = Parser::parseReg(t);
		this->advance();
		t = this->current();
		Register right = Parser::parseReg(t);
		this->advance();
		t = this->current();
		if (!match(TokenType::Semicolon, t)) {
			throw std::runtime_error("Expected ';' token after instruction");
		}
		i.type = InstructionType::Mov;
		i.left = left;
		i.right = right;
	} break;
	case TokenType::Ldi: {
		this->advance();
		t = this->current();
		Register dest = Parser::parseReg(t);
		this->advance();
		t = this->current();
		if (t.type == TokenType::Num) {
			i.expandable = false;
			i.arg = t.numVal;
		} else if (t.type == TokenType::Word) {
			i.expandable = true;
			i.strVal = t.wordVal;
		} else {
			throw std::runtime_error("unexpected argument in LDI instruction");
		}
		i.type = InstructionType::Ldi;
		i.left = dest;
		this->advance();
		t = this->current();
		if (!match(TokenType::Semicolon, t)) {
			throw std::runtime_error("Expected ';' token after instruction");
		}
	} break;
	case TokenType::Lod:
	case TokenType::Sav:

	case TokenType::Add:
	case TokenType::Min:
	case TokenType::Mul:
	case TokenType::Div:
	case TokenType::Mod:

	case TokenType::Gth:
	case TokenType::Lth:
	case TokenType::Geq:
	case TokenType::Leq:

	case TokenType::Equ:
	case TokenType::Neq:

	case TokenType::Land:
	case TokenType::Lor:
	case TokenType::Not:

	case TokenType::Band:
	case TokenType::Bor:
	case TokenType::Bnot:
	case TokenType::Xor:

	case TokenType::Jmp:
	case TokenType::Jiz:
	case TokenType::Jnz: {
		this->advance();
		t = this->current();
		if (!match(TokenType::Semicolon, t)) {
			throw std::runtime_error("Expected ';' token after instruction");
		}
	} break;
	case TokenType::Word: {
		std::println("WORD: {}", t.wordVal);
		i.strVal = t.wordVal;
		i.expandable = true;
		this->advance();
		if (!match(TokenType::Colon, t)) {
			throw std::runtime_error("Expected ':' token after label");
		}
	} break;
	default:
		throw std::runtime_error("Unexpected token");
	}
	return i;
}

bool Parser::match(TokenType type, Token &t) {
	t = this->current();
	if (t.type == type) {
		this->advance();
		return true;
	}
	return false;
}

Register Parser::parseReg(Token t) {
	Register reg;
	switch (t.type) {
	case TokenType::SecReg:
		reg.type = RegisterType::AddReg;
		reg.addRegNumber = t.numVal;
		break;
	case TokenType::A:
		reg.type = RegisterType::A;
		break;
	case TokenType::B:
		reg.type = RegisterType::B;
		break;
	case TokenType::C:
		reg.type = RegisterType::C;
		break;
	case TokenType::Ip:
		reg.type = RegisterType::Ip;
		break;
	case TokenType::Sp:
		reg.type = RegisterType::Sp;
		break;
	case TokenType::Sbp:
		reg.type = RegisterType::Sbp;
		break;
	case TokenType::Flg:
		reg.type = RegisterType::Flg;
		break;
    default:
        throw std::runtime_error("unknown reg");
	}
	return reg;
}
InstructionType Parser::convertTtToIt(TokenType tt) {
	auto it = this->ttAndItPairs.begin();
	while (it != this->ttAndItPairs.end()) {
		if (it->first == tt) {
			return it->second;
		}
		it++;
	}
	throw std::runtime_error("unknown token type");
}
