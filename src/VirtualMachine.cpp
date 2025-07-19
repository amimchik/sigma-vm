#include <climits>
#include <iomanip>
#include <sigma-vm/VirtualMachine.hpp>
#include <stdexcept>

// #define DEBUG_VM

#ifdef DEBUG_VM
#include <chrono>
#include <thread>
#endif

bool VirtualMachine::getRunning() {
	return this->regFlag & 0x1;
}
void VirtualMachine::setRunning(bool v) {
	if (v) {
		this->regFlag |= 0x1;
	} else {
		this->regFlag &= ~0x1;
	}
}
bool VirtualMachine::getBiosMode() {
	return this->regFlag & 0x10;
}
void VirtualMachine::setBiosMode(bool v) {
	if (v) {
		this->regFlag |= 0x10;
	} else {
		this->regFlag &= ~0x10;
	}
}

VirtualMachine::VirtualMachine(uint64_t ramSize)
    : regIp(0), ram(ramSize) {
}

uint64_t *VirtualMachine::locateRegister(uint8_t flag, uint16_t v) {
	if (flag != 0) {
		if (ADD_REGS_COUNT <= v) {
			return nullptr;
		}
		return &this->addRegs[v];
	}
	switch (v) {
	case REG_A:
		return &this->regA;
	case REG_B:
		return &this->regB;
	case REG_C:
		return &this->regC;
	case REG_IP:
		return &this->regIp;
	case REG_SP:
		return &this->regSp;
	case REG_SBP:
		return &this->regSbp;
	case REG_FLG:
		return &this->regFlag;
	}
	return nullptr;
}

void VirtualMachine::biosTick() {
	this->setBiosMode(false);
	this->setRunning(true);
	if (this->regA == 0x1001) {
		std::cout << (char)this->regB;
	}
}

void VirtualMachine::tick() {
	if (this->getBiosMode() || !this->getRunning()) {
		this->biosTick();
	}
	uint64_t info = ram.getAt(this->regIp++);
	uint16_t op = (info >> 0) & 0xFFFF;
	uint16_t flag = (info >> 16) & 0xFFFF;
	uint16_t larg = (info >> 32) & 0xFFFF;
	uint16_t rarg = (info >> 48) & 0xFFFF;

	uint64_t val = ram.getAt(this->regIp++);

	switch (op) {
	case CMD_MOV: {
		uint64_t *left = this->locateRegister(flag & 0xffff, larg);
		uint64_t *right = this->locateRegister(flag >> 8, rarg);
		if (!left) {
			throw std::runtime_error("left arg in mov command does not exist");
		}
		if (!right) {
			throw std::runtime_error("right arg in mov command does not exist");
		}
		*left = *right;
	} break;
	case CMD_LOD:
		this->regA = this->ram.getAt(this->regB);
		break;
	case CMD_SAV:
		this->ram.setAt(this->regB, this->regA);
		break;
	case CMD_LDI: {
		uint64_t *reg = this->locateRegister(flag & 0xffff, larg);
		if (!reg) {
			throw std::runtime_error("unable to locate register to load");
		}
		*reg = val;
	} break;
	case CMD_ADD:
		this->regC = this->regA + this->regB;
		break;
	case CMD_MIN:
		this->regC = this->regA - this->regB;
		break;
	case CMD_MUL:
		this->regC = this->regA * this->regB;
		break;
	case CMD_DIV:
		this->regC = this->regA / this->regB;
		break;
	case CMD_MOD:
		this->regC = this->regA % this->regB;
		break;
	case CMD_GTH:
		this->regC = this->regA > this->regB;
		break;
	case CMD_LTH:
		this->regC = this->regA < this->regB;
		break;
	case CMD_GEQ:
		this->regC = this->regA >= this->regB;
		break;
	case CMD_LEQ:
		this->regC = this->regA <= this->regB;
		break;
	case CMD_EQU:
		this->regC = this->regA == this->regB;
		break;
	case CMD_NEQ:
		this->regC = this->regA != this->regB;
		break;
	case CMD_LAND:
		this->regC = this->regA && this->regB;
		break;
	case CMD_LOR:
		this->regC = this->regA || this->regB;
		break;
	case CMD_NOT:
		this->regC = !this->regA;
		break;
	case CMD_BAND:
		this->regC = this->regA & this->regB;
		break;
	case CMD_BOR:
		this->regC = this->regA | this->regB;
		break;
	case CMD_BNOT:
		this->regC = ~this->regA;
		break;
	case CMD_XOR:
		this->regC = this->regA ^ this->regB;
		break;
	case CMD_JMP:
		this->regIp = this->regA;
		break;
	case CMD_JIZ:
		if (this->regB == 0) {
			this->regIp = this->regA;
		}
		break;
	case CMD_JNZ:
		if (this->regB != 0) {
			this->regIp = this->regA;
		}
		break;
	}
}

void VirtualMachine::launch() {
	this->regFlag = 1;
	while (this->regFlag != 0x00) {
#ifdef DEBUG_VM
		this->dumpVm();
		std::this_thread::sleep_for(std::chrono::seconds(1));
#endif // DEBUG_VM
		this->tick();
	}
}

#ifdef DEBUG_VM
void VirtualMachine::dumpVm() {
	std::ios_base::fmtflags f(std::cout.flags()); // сохранить текущие флаги форматирования

	std::cout << "=== VM Register Dump ===\n";
	std::cout << "regA   = 0x" << std::hex << std::setw(16) << std::setfill('0') << regA << "\n";
	std::cout << "regB   = 0x" << std::hex << std::setw(16) << std::setfill('0') << regB << "\n";
	std::cout << "regC   = 0x" << std::hex << std::setw(16) << std::setfill('0') << regC << "\n";
	std::cout << "regIP  = 0x" << std::hex << std::setw(16) << std::setfill('0') << regIp << "\n";
	std::cout << "regSP  = 0x" << std::hex << std::setw(16) << std::setfill('0') << regSp << "\n";
	std::cout << "regSBP = 0x" << std::hex << std::setw(16) << std::setfill('0') << regSbp << "\n";
	std::cout << "regF   = 0x" << std::hex << std::setw(16) << std::setfill('0') << regFlag << "\n";
#ifdef LOG_ADD_REGS
	std::cout << "--- Additional Registers ---\n";
	for (size_t i = 0; i < addRegs.size(); ++i) {
		std::cout << "addReg[" << i << "] = 0x"
		          << std::hex << std::setw(16) << std::setfill('0') << addRegs[i] << "\n";
	}
#endif // LOG_ADD_REGS

	std::cout.flags(f); // восстановить флаги форматирования
}
#endif // DEBUG_VM

VirtualMachine::VirtualMachine()
    : ram(0) {
}
