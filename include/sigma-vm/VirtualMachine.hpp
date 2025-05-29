#pragma once

#include <cstdint>
#include <array>
#include <stdexcept>
#include <iostream>

#include <sigma-vm/RAM.hpp>

#define ADD_REGS_COUNT 10

enum Cmd {
    CMD_MOV = 0x0000, // mov between registers
    CMD_LOD = 0x0001, // load from mem at b to a
    CMD_SAV = 0x0002, // store to mem value of a at b

    CMD_LDI = 0x0010, // load from arg to any reg

    CMD_ADD = 0x1000,
    CMD_MIN = 0x1001,
    CMD_MUL = 0x1002,
    CMD_DIV = 0x1003,
    CMD_MOD = 0x1004,

    CMD_GTH = 0x1100,
    CMD_LTH = 0x1101,
    CMD_GEQ = 0x1102,
    CMD_LEQ = 0x1103,

    CMD_EQU = 0x1110,
    CMD_NEQ = 0x1111,

    CMD_LAND = 0x1200,
    CMD_LOR = 0x1201,
    CMD_NOT = 0x1202,

    CMD_BAND = 0x1300,
    CMD_BOR = 0x1301,
    CMD_BNOT = 0x1302,
    CMD_XOR = 0x1303,

    CMD_JMP = 0x2000,
    CMD_JIZ = 0x2001,
    CMD_JNZ = 0x2002,
};

enum Reg {
    REG_A = 0x0000,
    REG_B = 0x0001,
    REG_C = 0x0002,
    REG_IP = 0x0003,
    REG_SP = 0x0004,
    REG_SBP = 0x0005,
    REG_FLG = 0x0006,
};


class VirtualMachine {
private:
    void biosTick();    
    bool getRunning();
    void setRunning(bool v);
    bool getBiosMode();
    void setBiosMode(bool v);
    uint64_t* locateRegister(uint8_t flag, uint16_t v);
    void dumpVm();
    VirtualMachine();
public:
    VirtualMachine(uint64_t ramSize);
    uint64_t regA, regB, regC;
    std::array<uint64_t, ADD_REGS_COUNT> addRegs;
    uint64_t regIp, regSp, regSbp, regFlag;
    RAM ram;
    void launch();
    void tick();
};
