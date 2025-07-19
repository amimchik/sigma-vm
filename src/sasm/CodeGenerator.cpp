#include <sasm/CodeGenerator.hpp>
#include <sasm/Parser.hpp>
#include <sigma-vm/VirtualMachine.hpp>
#include <stdexcept>

CodeGenerator::CodeGenerator(Linker linker)
    : linker(linker) {
}

CodeGenerator::RegBin CodeGenerator::convReg(Register r) {
	RegBin rBin;
	rBin.flag = r.type == RegisterType::AddReg ? 1 : 0;
	if (rBin.flag) {
		rBin.arg = r.addRegNumber;
		return rBin;
	}
	switch (r.type) {
	case RegisterType::A:
		rBin.arg = REG_A;
		break;
	case RegisterType::B:
		rBin.arg = REG_B;
		break;
	case RegisterType::C:
		rBin.arg = REG_C;
		break;
	case RegisterType::Ip:
		rBin.arg = REG_IP;
		break;
	case RegisterType::Sp:
		rBin.arg = REG_SP;
		break;
	case RegisterType::Sbp:
		rBin.arg = REG_SBP;
		break;
	case RegisterType::Flg:
		rBin.arg = REG_FLG;
		break;
	default:
		throw std::runtime_error("Unknown register");
	}
	return rBin;
}

std::array<uint64_t, 2> CodeGenerator::genInstruction() {
	Instruction instr = this->linker.next();
	uint16_t opCode, flag = 0, larg = 0, rarg = 0;
	uint64_t arg = 0;
	switch (instr.type) {
	case InstructionType::Mov: {
        opCode = CMD_MOV;
		RegBin left = this->convReg(instr.left);
		RegBin right = this->convReg(instr.right);
		flag |= ((uint16_t)left.flag & 0xffff) << 0;
		larg = left.arg;
		flag |= ((uint16_t)right.flag & 0xffff) << 8;
		rarg = right.arg;
	} break;
	case InstructionType::Ldi: {
		opCode = CMD_LDI;
		RegBin dest = this->convReg(instr.left);
		flag = dest.flag;
		larg = dest.arg;
		arg = instr.arg;
	} break;
	case InstructionType::Add:
		opCode = CMD_ADD;
		break;
	case InstructionType::Min:
		opCode = CMD_MIN;
		break;
	case InstructionType::Mul:
		opCode = CMD_MUL;
		break;
	case InstructionType::Div:
		opCode = CMD_DIV;
		break;
	case InstructionType::Mod:
		opCode = CMD_MOD;
		break;
	case InstructionType::Gth:
		opCode = CMD_GTH;
		break;
	case InstructionType::Lth:
		opCode = CMD_LTH;
		break;
	case InstructionType::Geq:
		opCode = CMD_GEQ;
		break;
	case InstructionType::Leq:
		opCode = CMD_LEQ;
		break;
	case InstructionType::Equ:
		opCode = CMD_EQU;
		break;
	case InstructionType::Neq:
		opCode = CMD_NEQ;
		break;
	case InstructionType::Land:
		opCode = CMD_LAND;
		break;
	case InstructionType::Lor:
		opCode = CMD_LOR;
		break;
	case InstructionType::Not:
		opCode = CMD_NOT;
		break;
	case InstructionType::Band:
		opCode = CMD_BAND;
		break;
	case InstructionType::Bor:
		opCode = CMD_BOR;
		break;
	case InstructionType::Bnot:
		opCode = CMD_BNOT;
		break;
	case InstructionType::Xor:
		opCode = CMD_XOR;
		break;
	case InstructionType::Jmp:
		opCode = CMD_JMP;
		break;
	case InstructionType::Jiz:
		opCode = CMD_JIZ;
		break;
	case InstructionType::Jnz:
		opCode = CMD_JNZ;
		break;
	case InstructionType::Lod:
		opCode = CMD_LOD;
		break;
	case InstructionType::Sav:
		opCode = CMD_SAV;
		break;
	default:
		throw std::runtime_error("unknown instruction");
	}
	uint64_t info = 0;
	info |= (uint64_t(opCode) & 0xFFFF) << 0;
	info |= (uint64_t(flag) & 0xFFFF) << 16;
	info |= (uint64_t(larg) & 0xFFFF) << 32;
	info |= (uint64_t(rarg) & 0xFFFF) << 48;
	return std::array<uint64_t, 2>{info, arg};
}

std::vector<std::array<uint64_t, 2>> CodeGenerator::genAll() {
	std::vector<std::array<uint64_t, 2>> code;
	while (!linker.atEof()) {
		code.push_back(this->genInstruction());
	}
	return code;
}
