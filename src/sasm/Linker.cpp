#include <print>
#include <sasm/Linker.hpp>

Linker::Linker(Parser parser)
    : parser(parser), linkingForward(false) {
}

Instruction Linker::next() {
	std::println("[DEBUG]: LINKER::NEXT{}; size: {}", this->linkingForward ? " FRWRD" : "", this->linkedForward.size());
	this->currentAddr += 2;
	if (this->linkedForward.size() && !this->linkingForward) {
		Instruction v = this->linkedForward[0];
		this->linkedForward.pop_front();
		return v;
	}
	Instruction instr = parser.next();

	if (!instr.expandable)
		return instr;

	switch (instr.type) {
	case InstructionType::Ldi: {
		instr.expandable = false;
		instr.arg = this->resolveLabel(instr.strVal);
		return instr;
	} break;
	case InstructionType::Label: {
		this->currentAddr -= 2;
		this->addLabel(instr.strVal);
		return this->next();
	} break;
    default:
        break;
	}
	std::cout << "WARNING: instruction marked as expandable, but type is unknown" << std::endl;
	return instr;
}

void Linker::addLabel(std::string label) {
	this->labels.insert_or_assign(label, this->currentAddr);
}

uint64_t Linker::resolveLabel(std::string label) {
	std::println("[DEBUG]: LINKER::RESOLVE_LBL; size: {}", this->linkedForward.size());
	if (this->labels.find(label) != this->labels.end()) {
		return this->labels.at(label);
	}
	int addr = this->currentAddr;

	this->linkingForward = true;
	while (this->labels.find(label) == this->labels.end()) {
		if (this->atEof()) {
			throw std::runtime_error(std::format("label {} is unknown", label));
		}
		this->linkedForward.push_back(this->next());
	}
	this->linkingForward = false;

	this->currentAddr = addr;

	return this->resolveLabel(label);
}

std::deque<Instruction> Linker::linkAll() {
	std::deque<Instruction> instuctions;
	while (!this->atEof()) {
		instructions.push_back(this->next());
	}
	return instuctions;
}

bool Linker::atEof() {
	return parser.atEof() && (this->linkedForward.size() == 0);
}
