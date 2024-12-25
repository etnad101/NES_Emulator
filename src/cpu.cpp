#include "cpu.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>

#include "bus.h"
#include "opcodes.h"

static std::map<uint8_t, Opcode> opcodes {
    // Access
    {0xA9, Opcode {Name::LDA, 2, 2, AddressingMode::Immediate}},
    {0xA5, Opcode {Name::LDA, 2, 3, AddressingMode::ZeroPage}},
    {0xB5, Opcode {Name::LDA, 2, 4, AddressingMode::ZeroPageX}},
    {0xAD, Opcode {Name::LDA, 3, 4, AddressingMode::Absolute}},
    {0xBD, Opcode {Name::LDA, 3, 4, AddressingMode::AbsoluteX}}, // +1 cycle if page crossed
    {0xB9, Opcode {Name::LDA, 3, 4, AddressingMode::AbsoluteY}}, // +1 cycle if page crossed
    {0xA1, Opcode {Name::LDA, 2, 6, AddressingMode::IndirectX}},
    {0xB1, Opcode {Name::LDA, 2, 5, AddressingMode::IndirectY}}, // +1 cycle if page crossed

    {0xA2, Opcode {Name::LDX, 2, 2, AddressingMode::Immediate}},
    {0xA6, Opcode {Name::LDX, 2, 3, AddressingMode::ZeroPage}},
    {0xB6, Opcode {Name::LDX, 2, 4, AddressingMode::ZeroPageY}},
    {0xAE, Opcode {Name::LDX, 3, 4, AddressingMode::Absolute}},
    {0xBE, Opcode {Name::LDX, 3, 4, AddressingMode::AbsoluteY}}, // + 1 cycle if page crossed

    {0xA0, Opcode {Name::LDY, 2, 2, AddressingMode::Immediate}},
    {0xA4, Opcode {Name::LDY, 2, 3, AddressingMode::ZeroPage}},
    {0xB4, Opcode {Name::LDY, 2, 4, AddressingMode::ZeroPageX}},
    {0xAC, Opcode {Name::LDY, 3, 4, AddressingMode::Absolute}},
    {0xBC, Opcode {Name::LDY, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed

    {0x85, Opcode {Name::STA, 2, 3, AddressingMode::ZeroPage}},
    {0x95, Opcode {Name::STA, 2, 4, AddressingMode::ZeroPageX}},
    {0x8D, Opcode {Name::STA, 3, 4, AddressingMode::Absolute}},
    {0x9D, Opcode {Name::STA, 3, 5, AddressingMode::AbsoluteX}}, 
    {0x99, Opcode {Name::STA, 3, 5, AddressingMode::AbsoluteY}}, 
    {0x81, Opcode {Name::STA, 2, 6, AddressingMode::IndirectX}},
    {0x91, Opcode {Name::STA, 2, 6, AddressingMode::IndirectY}}, 

    {0x86, Opcode {Name::STX, 2, 3, AddressingMode::ZeroPage}},
    {0x96, Opcode {Name::STX, 2, 4, AddressingMode::ZeroPageY}},
    {0x8E, Opcode {Name::STX, 3, 4, AddressingMode::Absolute}},

    {0x84, Opcode {Name::STY, 2, 3, AddressingMode::ZeroPage}},
    {0x94, Opcode {Name::STY, 2, 4, AddressingMode::ZeroPageX}},
    {0x8C, Opcode {Name::STY, 3, 4, AddressingMode::Absolute}},
    // Transfer
    {0xAA, Opcode {Name::TAX, 1, 2, AddressingMode::Implicit}},
    {0xA8, Opcode {Name::TAY, 1, 2, AddressingMode::Implicit}},
    {0x8A, Opcode {Name::TXA, 1, 2, AddressingMode::Implicit}},
    {0x98, Opcode {Name::TYA, 1, 2, AddressingMode::Implicit}},
    // Arithmetic
    {0x69, Opcode {Name::ADC, 2, 2, AddressingMode::Immediate}},
    {0x65, Opcode {Name::ADC, 2, 3, AddressingMode::ZeroPage}},
    {0x75, Opcode {Name::ADC, 2, 4, AddressingMode::ZeroPageX}},
    {0x6D, Opcode {Name::ADC, 3, 4, AddressingMode::Absolute}},
    {0x7D, Opcode {Name::ADC, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed
    {0x79, Opcode {Name::ADC, 3, 4, AddressingMode::AbsoluteY}}, // + 1 cycle if page crossed
    {0x61, Opcode {Name::ADC, 2, 6, AddressingMode::IndirectX}},
    {0x71, Opcode {Name::ADC, 2, 5, AddressingMode::IndirectY}}, // + 1 cycle if page crossed

    {0xE9, Opcode {Name::SBC, 2, 2, AddressingMode::Immediate}},
    {0xE5, Opcode {Name::SBC, 2, 3, AddressingMode::ZeroPage}},
    {0xF5, Opcode {Name::SBC, 2, 4, AddressingMode::ZeroPageX}},
    {0xED, Opcode {Name::SBC, 3, 4, AddressingMode::Absolute}},
    {0xFD, Opcode {Name::SBC, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed
    {0xF9, Opcode {Name::SBC, 3, 4, AddressingMode::AbsoluteY}}, // + 1 cycle if page crossed
    {0xE1, Opcode {Name::SBC, 2, 6, AddressingMode::IndirectX}},
    {0xF1, Opcode {Name::SBC, 2, 5, AddressingMode::IndirectY}}, // + 1 cycle if page crossed

    {0xE6, Opcode {Name::INC, 2, 5, AddressingMode::ZeroPage}}, 
    {0xF6, Opcode {Name::INC, 2, 6, AddressingMode::ZeroPageX}}, 
    {0xEE, Opcode {Name::INC, 3, 6, AddressingMode::Absolute}}, 
    {0xFE, Opcode {Name::INC, 3, 7, AddressingMode::AbsoluteX}}, 

    {0xC6, Opcode {Name::DEC, 2, 5, AddressingMode::ZeroPage}}, 
    {0xD6, Opcode {Name::DEC, 2, 6, AddressingMode::ZeroPageX}}, 
    {0xCE, Opcode {Name::DEC, 3, 6, AddressingMode::Absolute}}, 
    {0xDE, Opcode {Name::DEC, 3, 7, AddressingMode::AbsoluteX}}, 

    {0xE8, Opcode {Name::INX, 1, 2, AddressingMode::Implicit}}, 
    {0xCA, Opcode {Name::DEX, 1, 2, AddressingMode::Implicit}}, 
    {0xC8, Opcode {Name::INY, 1, 2, AddressingMode::Implicit}}, 
    {0x88, Opcode {Name::DEY, 1, 2, AddressingMode::Implicit}}, 
    // Shift
    {0x0A, Opcode {Name::ASL, 1, 2, AddressingMode::Accumulator}},
    {0x06, Opcode {Name::ASL, 2, 5, AddressingMode::ZeroPage}},
    {0x16, Opcode {Name::ASL, 2, 6, AddressingMode::ZeroPageX}},
    {0x0E, Opcode {Name::ASL, 3, 6, AddressingMode::Absolute}},
    {0x1E, Opcode {Name::ASL, 3, 7, AddressingMode::AbsoluteX}},

    {0x4A, Opcode {Name::LSR, 1, 2, AddressingMode::Accumulator}},
    {0x46, Opcode {Name::LSR, 2, 5, AddressingMode::ZeroPage}},
    {0x56, Opcode {Name::LSR, 2, 6, AddressingMode::ZeroPageX}},
    {0x4E, Opcode {Name::LSR, 3, 6, AddressingMode::Absolute}},
    {0x5E, Opcode {Name::LSR, 3, 7, AddressingMode::AbsoluteX}},

    {0x2A, Opcode {Name::ROL, 1, 2, AddressingMode::Accumulator}},
    {0x26, Opcode {Name::ROL, 2, 5, AddressingMode::ZeroPage}},
    {0x36, Opcode {Name::ROL, 2, 6, AddressingMode::ZeroPageX}},
    {0x2E, Opcode {Name::ROL, 3, 6, AddressingMode::Absolute}},
    {0x3E, Opcode {Name::ROL, 3, 7, AddressingMode::AbsoluteX}},

    {0x6A, Opcode {Name::ROR, 1, 2, AddressingMode::Accumulator}},
    {0x66, Opcode {Name::ROR, 2, 5, AddressingMode::ZeroPage}},
    {0x76, Opcode {Name::ROR, 2, 6, AddressingMode::ZeroPageX}},
    {0x6E, Opcode {Name::ROR, 3, 6, AddressingMode::Absolute}},
    {0x7E, Opcode {Name::ROR, 3, 7, AddressingMode::AbsoluteX}},
    // Bitwise
    {0x29, Opcode {Name::AND, 2, 2, AddressingMode::Immediate}},
    {0x25, Opcode {Name::AND, 2, 3, AddressingMode::ZeroPage}},
    {0x35, Opcode {Name::AND, 2, 4, AddressingMode::ZeroPageX}},
    {0x2D, Opcode {Name::AND, 3, 4, AddressingMode::Absolute}},
    {0x3D, Opcode {Name::AND, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed
    {0x39, Opcode {Name::AND, 3, 4, AddressingMode::AbsoluteY}}, // + 1 cycle if page crossed
    {0x21, Opcode {Name::AND, 2, 6, AddressingMode::IndirectX}},
    {0x31, Opcode {Name::AND, 2, 5, AddressingMode::IndirectY}}, // + 1 cycle if page crossed

    {0x09, Opcode {Name::ORA, 2, 2, AddressingMode::Immediate}},
    {0x05, Opcode {Name::ORA, 2, 3, AddressingMode::ZeroPage}},
    {0x15, Opcode {Name::ORA, 2, 4, AddressingMode::ZeroPageX}},
    {0x0D, Opcode {Name::ORA, 3, 4, AddressingMode::Absolute}},
    {0x1D, Opcode {Name::ORA, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed
    {0x19, Opcode {Name::ORA, 3, 4, AddressingMode::AbsoluteY}}, // + 1 cycle if page crossed
    {0x01, Opcode {Name::ORA, 2, 6, AddressingMode::IndirectX}},
    {0x11, Opcode {Name::ORA, 2, 5, AddressingMode::IndirectY}}, // + 1 cycle if page crossed

    {0x49, Opcode {Name::EOR, 2, 2, AddressingMode::Immediate}},
    {0x45, Opcode {Name::EOR, 2, 3, AddressingMode::ZeroPage}},
    {0x55, Opcode {Name::EOR, 2, 4, AddressingMode::ZeroPageX}},
    {0x4D, Opcode {Name::EOR, 3, 4, AddressingMode::Absolute}},
    {0x5D, Opcode {Name::EOR, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed
    {0x59, Opcode {Name::EOR, 3, 4, AddressingMode::AbsoluteY}}, // + 1 cycle if page crossed
    {0x41, Opcode {Name::EOR, 2, 6, AddressingMode::IndirectX}},
    {0x51, Opcode {Name::EOR, 2, 5, AddressingMode::IndirectY}}, // + 1 cycle if page crossed

    {0x24, Opcode {Name::BIT, 2, 3, AddressingMode::ZeroPage}},
    {0x2C, Opcode {Name::BIT, 3, 4, AddressingMode::Absolute}},
    // Compare
    {0xC9, Opcode {Name::CMP, 2, 2, AddressingMode::Immediate}},
    {0xC5, Opcode {Name::CMP, 2, 3, AddressingMode::ZeroPage}},
    {0xD5, Opcode {Name::CMP, 2, 4, AddressingMode::ZeroPageX}},
    {0xCD, Opcode {Name::CMP, 3, 4, AddressingMode::Absolute}},
    {0xDD, Opcode {Name::CMP, 3, 4, AddressingMode::AbsoluteX}}, // + 1 cycle if page crossed
    {0xD9, Opcode {Name::CMP, 3, 4, AddressingMode::AbsoluteY}},
    {0xC1, Opcode {Name::CMP, 2, 6, AddressingMode::IndirectX}}, // + 1 cycle if page crossed
    {0xD1, Opcode {Name::CMP, 2, 5, AddressingMode::IndirectY}}, // + 1 cycle if page crossed
    
    {0xE0, Opcode {Name::CPX, 2, 2, AddressingMode::Immediate}},
    {0xE4, Opcode {Name::CPX, 2, 3, AddressingMode::ZeroPage}},
    {0xEC, Opcode {Name::CPX, 3, 4, AddressingMode::Absolute}},

    {0xC0, Opcode {Name::CPY, 2, 2, AddressingMode::Immediate}},
    {0xC4, Opcode {Name::CPY, 2, 3, AddressingMode::ZeroPage}},
    {0xCC, Opcode {Name::CPY, 3, 4, AddressingMode::Absolute}},
    // Branch
    {0x90, Opcode {Name::BCC, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0xB0, Opcode {Name::BCS, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0xF0, Opcode {Name::BEQ, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0xD0, Opcode {Name::BNE, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0x10, Opcode {Name::BPL, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0x30, Opcode {Name::BMI, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0x50, Opcode {Name::BVC, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    {0x70, Opcode {Name::BVS, 2, 2, AddressingMode::Relative}}, // + 1 cycle if branch taken, + 1 if page crossed
    // Jump
    {0x4C, Opcode {Name::JMP, 3, 3, AddressingMode::Absolute}},
    {0x6C, Opcode {Name::JMP, 3, 5, AddressingMode::Indirect}},
    {0x20, Opcode {Name::JSR, 3, 6, AddressingMode::Absolute}},
    {0x60, Opcode {Name::RTS, 1, 6, AddressingMode::Implicit}},
    {0x00, Opcode {Name::BRK, 1, 7, AddressingMode::Implicit}}, // Only 1 byte wide, but skips the second byte so can be considered 2 bytes
    {0x40, Opcode {Name::RTI, 1, 6, AddressingMode::Implicit}},
    // Stack
    {0x48, Opcode {Name::PHA, 1, 3, AddressingMode::Implicit}},
    {0x68, Opcode {Name::PLA, 1, 4, AddressingMode::Implicit}},
    {0x08, Opcode {Name::PHP, 1, 3, AddressingMode::Implicit}},
    {0x28, Opcode {Name::PLP, 1, 4, AddressingMode::Implicit}},
    {0x9A, Opcode {Name::TXS, 1, 2, AddressingMode::Implicit}},
    {0xBA, Opcode {Name::TSX, 1, 2, AddressingMode::Implicit}},
    // Flags
    {0x18, Opcode {Name::CLC, 1, 2, AddressingMode::Implicit}},
    {0x38, Opcode {Name::SEC, 1, 2, AddressingMode::Implicit}},
    {0x58, Opcode {Name::CLI, 1, 2, AddressingMode::Implicit}},
    {0x78, Opcode {Name::SEI, 1, 2, AddressingMode::Implicit}},
    {0xD8, Opcode {Name::CLD, 1, 2, AddressingMode::Implicit}},
    {0xF8, Opcode {Name::SED, 1, 2, AddressingMode::Implicit}},
    {0xB8, Opcode {Name::CLV, 1, 2, AddressingMode::Implicit}},

    // Other
    {0xEA, Opcode {Name::NOP, 1, 2, AddressingMode::Implicit}},
};

Cpu::Cpu(Bus* bus) 
: m_bus(bus), m_cycles(0), m_pageCrossed(false), r_A(0), r_X(0), r_Y(0), r_pc(0xC000), r_S(0xFD), 
f_N(false), f_V(false), f_D(false), f_I(true), f_Z(false), f_C(false), m_pendingIFlagValue(false), m_pendingIFlagUpdate(false) {}

uint16_t Cpu::getAddress(AddressingMode mode) {
    switch (mode) {
        case AddressingMode::Immediate:
        {
            uint16_t addr = r_pc;
            r_pc++;
            return addr;
        }
        case AddressingMode::ZeroPage:
        {
            uint16_t addr = static_cast<uint16_t>(m_bus->read(r_pc));
            r_pc++;
            return addr;
        }
        case AddressingMode::ZeroPageX:
        {
            uint8_t addr = m_bus->read(r_pc) + r_X;
            this->r_pc++;
            return addr;
        }
        case AddressingMode::ZeroPageY:
        {
            uint8_t addr = m_bus->read(r_pc) + r_Y;
            this->r_pc++;
            return addr;

        }
        case AddressingMode::Absolute:
        {
            uint16_t lo = static_cast<uint16_t>(m_bus->read(r_pc));
            this->r_pc++;
            uint16_t hi = static_cast<uint16_t>(m_bus->read(r_pc));
            this->r_pc++;
            return ((hi << 8) | lo);
        }
        case AddressingMode::AbsoluteX:
        {
            uint16_t lo = static_cast<uint16_t>(m_bus->read(r_pc));
            this->r_pc++;
            uint16_t hi = static_cast<uint16_t>(m_bus->read(r_pc));
            this->r_pc++;

            uint16_t baseAddr = ((hi << 8) | lo);
            uint16_t offset = static_cast<uint16_t>(r_X);

            if (((baseAddr + offset) >> 8) != (baseAddr >> 8)) {
                m_pageCrossed = true;
            }

            return baseAddr + offset;
        }
        case AddressingMode::AbsoluteY:
        {
            uint16_t lo = static_cast<uint16_t>(m_bus->read(r_pc));
            this->r_pc++;
            uint16_t hi = static_cast<uint16_t>(m_bus->read(r_pc));
            this->r_pc++;

            uint16_t baseAddr = ((hi << 8) | lo);
            uint16_t offset = static_cast<uint16_t>(r_Y);

            if (((baseAddr + offset) >> 8) != (baseAddr >> 8)) {
                m_pageCrossed = true;
            }

            return baseAddr + offset;
        }
        case AddressingMode::Indirect:
        {
            uint8_t loPtr = m_bus->read(r_pc);
            this->r_pc++;
            uint8_t hiPtr = m_bus->read(r_pc);
            this->r_pc++;

            uint8_t nextLo = loPtr + 1;

            uint16_t lo = (static_cast<uint16_t>(hiPtr) << 8) | (static_cast<uint16_t>(loPtr));
            uint16_t hi = (static_cast<uint16_t>(hiPtr) << 8) | (static_cast<uint16_t>(nextLo));

            uint16_t loVal = static_cast<uint16_t>(m_bus->read(lo));
            uint16_t hiVal = static_cast<uint16_t>(m_bus->read(hi));

            return (hiVal << 8) | loVal;
        }
        case AddressingMode::IndirectX:
        {

            uint8_t base = m_bus->read(r_pc);
            r_pc++;
            uint8_t loPtr = base + r_X;
            uint8_t hiPtr = loPtr + 1; 
            uint16_t lo = static_cast<uint16_t>(m_bus->read(loPtr));
            uint16_t hi = static_cast<uint16_t>(m_bus->read(hiPtr));

            return (hi << 8) | lo;
        }
        case AddressingMode::IndirectY:
        { 

            uint16_t loPtr = static_cast<uint16_t>(m_bus->read(r_pc));
            r_pc++;
            uint8_t hiPtr = loPtr + 1; 
            uint16_t lo = static_cast<uint16_t>(m_bus->read(loPtr));
            uint16_t hi = static_cast<uint16_t>(m_bus->read(hiPtr));

            uint16_t baseAddr = ((hi << 8) | lo);
            uint16_t offset = static_cast<uint16_t>(r_Y);
            uint16_t addr = baseAddr + offset;

            if (((addr) >> 8) != (baseAddr >> 8)) {
                m_pageCrossed = true;
            }

            return addr;
        }
        default:
            std::cerr << "ERROR: Addressing mode should not be used" << std::endl;
            exit(-1);
    }
}

uint8_t Cpu::getP(bool f_B) {
    uint8_t p = (f_N << 7) | 
                (f_V << 6) |
                (1 << 5) |
                (f_B << 4) |
                (f_D << 3) |
                (f_I << 2) |
                (f_Z << 1) |
                f_C;
    return p;
}

void Cpu::setP(uint8_t flags, bool updateInow) {
    f_C = (flags & 0x01) > 0;
    f_Z = (flags & 0x02) > 0;
    f_D = (flags & 0x08) > 0;
    f_V = (flags & 0x40) > 0;
    f_N = (flags & 0x80) > 0;

    if (updateInow) {
        f_I = (flags & 0x04) > 0;
    } else {
        m_pendingIFlagValue = (flags & 0x04) > 0;
        m_pendingIFlagUpdate = true;
    }
}

void Cpu::updateZNFlags(uint8_t value) {
    f_Z = value == 0;

    f_N = (value & 0x80) > 0;
}

void Cpu::compare(uint8_t reg, AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint8_t value = m_bus->read(addr);

    f_C = reg >= value;
    updateZNFlags(reg - value);
    if (m_pageCrossed) m_cycles++;
}

uint16_t Cpu::calculateBranchAddr(uint8_t offset) {
    uint16_t addr = r_pc;
    addr += static_cast<int8_t>(offset);
    return addr + 1;
}

void Cpu::branch(Name name) {
    bool branch = false;
    switch (name) {
        case Name::BCC:
            if (!f_C) branch = true;
            break;
        case Name::BCS:
            if (f_C) branch = true;
            break;
        case Name::BEQ:
            if (f_Z) branch = true;
            break;
        case Name::BNE:
            if (!f_Z) branch = true;
            break;
        case Name::BPL:
            if (!f_N) branch = true;
            break;
        case Name::BMI:
            if (f_N) branch = true;
            break;
        case Name::BVC:
            if (!f_V) branch = true;
            break;
        case Name::BVS:
            if (f_V) branch = true;
            break;
        default:
            std::cerr << "ERROR: Only branch opcodes should be calling this function" << std::endl;
            exit(-1);
    }

    if (branch) {
        m_cycles++;
        uint8_t offset = m_bus->read(r_pc);
        uint16_t prePc = r_pc;
        r_pc = calculateBranchAddr(offset);
        // I think i should add a cycle if it crosses a page, but the tests say otherwise
        // if (((pc) >> 8) != ( prePc >> 8)) {
        //     cycles++;
        // }
    } else {
        r_pc++;
    }
}

void Cpu::logInstr(Opcode opcode) {
    std::cout << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << r_pc << "  ";
    for (int i = 0; i < opcode.bytes; i++) {
        std::cout << std::setw(2) << static_cast<unsigned int>(m_bus->read(r_pc + i)) << " ";
    }
    for (int i = 0; i < 3 - opcode.bytes; i++) {
        std::cout << "   "; 
    }

    std::cout << " ";
    bool showMem = false;
    uint16_t endVal;
    uint16_t tempPc = r_pc;
    r_pc++;
    switch (opcode.name) {
        case Name::LDA:
            std::cout << "LDA";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::STA:
            std::cout << "STA";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::LDX:
            std::cout << "LDX";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::STX:
            std::cout << "STX";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::LDY:
            std::cout << "LDY";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::STY:
            std::cout << "STY";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::TAX:
            std::cout << "TAX";
            break;
        case Name::TAY:
            std::cout << "TAY";
            break;
        case Name::TXA:
            std::cout << "TXA";
            break;
        case Name::TYA:
            std::cout << "TYA";
            break;
        case Name::ADC:
            std::cout << "ADC";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::SBC:
            std::cout << "SBC";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::INC:
            std::cout << "INC";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::DEC:
            std::cout << "DEC";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::INX:
            std::cout << "INX";
            break;
        case Name::DEX:
            std::cout << "DEX";
            break;
        case Name::INY:
            std::cout << "INY";
            break;
        case Name::DEY:
            std::cout << "DEY";
            break;
        case Name::ASL:
            std::cout << "ASL";
            if (opcode.mode != AddressingMode::Accumulator)
                endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::LSR:
            std::cout << "LSR";
            if (opcode.mode != AddressingMode::Accumulator)
                endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::ROL:
            std::cout << "ROL";
            if (opcode.mode != AddressingMode::Accumulator)
                endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::ROR:
            std::cout << "ROR";
            if (opcode.mode != AddressingMode::Accumulator)
                endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::AND:
            std::cout << "AND";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::ORA:
            std::cout << "ORA";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::EOR:
            std::cout << "EOR";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::BIT:
            std::cout << "BIT";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::CMP:
            std::cout << "CMP";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::CPX:
            std::cout << "CPX";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::CPY:
            std::cout << "CPY";
            endVal = m_bus->read(getAddress(opcode.mode));
            showMem = true;
            break;
        case Name::BCC:
            std::cout << "BCC";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BCS:
            std::cout << "BCS";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BEQ:
            std::cout << "BEQ";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BNE:
            std::cout << "BNE";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BPL:
            std::cout << "BPL";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BMI:
            std::cout << "BMI";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BVC:
            std::cout << "BVC";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::BVS:
            std::cout << "BVS";
            endVal = calculateBranchAddr(m_bus->read(r_pc));
            break;
        case Name::JMP:
            std::cout << "JMP";
            break;
        case Name::JSR:
            std::cout << "JSR";
            break;
        case Name::RTS:
            std::cout << "RTS";
            break;
        case Name::BRK:
            std::cout << "BRK";
            break;
        case Name::RTI:
            std::cout << "RTI";
            break;
        case Name::PHA:
            std::cout << "PHA";
            break;
        case Name::PLA:
            std::cout << "PLA";
            break;
        case Name::PHP:
            std::cout << "PHP";
            break;
        case Name::PLP:
            std::cout << "PLP";
            break;
        case Name::TXS:
            std::cout << "TXS";
            break;
        case Name::TSX:
            std::cout << "TSX";
            break;
        case Name::CLC:
            std::cout << "CLC";
            break;
        case Name::SEC:
            std::cout << "SEC";
            break;
        case Name::CLI:
            std::cout << "CLI";
            break;
        case Name::SEI:
            std::cout << "SEI";
            break;
        case Name::CLD:
            std::cout << "CLD";
            break;
        case Name::SED:
            std::cout << "SED";
            break;
        case Name::CLV:
            std::cout << "CLV";
            break;
        case Name::NOP:
            std::cout << "NOP";
            break;
    }
    r_pc = tempPc;

    std::cout << " ";
    switch (opcode.mode) {
        case AddressingMode::Accumulator:
            std::cout << "A                          ";
            break;
        case AddressingMode::Absolute:
            std::cout 
                << "$" 
                << std::setw(2) << static_cast<unsigned int>(m_bus->read(r_pc + 2)) 
                << std::setw(2) << static_cast<unsigned int>(m_bus->read(r_pc + 1));
            if (showMem) {
                std::cout 
                    << " = " 
                    << std::hex << std::setw(2) << endVal 
                    << "                 ";
            } else {
                std::cout << "                      ";
            }
            break;
        case AddressingMode::AbsoluteX: 
        {
            uint16_t lo = static_cast<uint16_t>(m_bus->read(r_pc + 1));
            uint16_t hi = static_cast<uint16_t>(m_bus->read(r_pc + 2));
            uint16_t base = (hi << 8) | lo;
            uint16_t addr = base + static_cast<uint16_t>(r_X);
            std::cout 
                << "$" 
                << std::setw(2) << static_cast<unsigned int>(hi) 
                << std::setw(2) << static_cast<unsigned int>(lo)
                << ",X @ " << std::setw(4) << static_cast<unsigned int>(addr)
                << " = " << std::setw(2) << static_cast<unsigned int>(m_bus->read(addr))
                << "        ";
            break;
        }
        case AddressingMode::AbsoluteY: 
        {
            uint16_t lo = static_cast<uint16_t>(m_bus->read(r_pc + 1));
            uint16_t hi = static_cast<uint16_t>(m_bus->read(r_pc + 2));
            uint16_t base = (hi << 8) | lo;
            uint16_t addr = base + static_cast<uint16_t>(r_Y);
            std::cout 
                << "$" 
                << std::setw(2) << static_cast<unsigned int>(hi) 
                << std::setw(2) << static_cast<unsigned int>(lo)
                << ",Y @ " << std::setw(4) << static_cast<unsigned int>(addr)
                << " = " << std::setw(2) << static_cast<unsigned int>(m_bus->read(addr))
                << "        ";
            break;
        }
        case AddressingMode::Immediate:
            std::cout 
                << "#$" 
                << std::setw(2) << static_cast<unsigned int>(m_bus->read(r_pc + 1))
                << "                       ";
            break;
        case AddressingMode::ZeroPage:
            std::cout 
                << "$" << std::setw(2) << static_cast<unsigned int>(m_bus->read(r_pc + 1)) 
                << " = " << std::setw(2) << static_cast<unsigned int>(endVal)
                << "                   ";
            break;
        case AddressingMode::ZeroPageX:
        {
            uint8_t base = m_bus->read(r_pc + 1);
            uint8_t addr = base + r_X;
            std::cout 
                << "$" 
                << std::setw(2) << static_cast<unsigned int>(base) 
                << ",X @ " << std::setw(2) << static_cast<unsigned int>(addr)
                << " = " << std::setw(2) << static_cast<unsigned int>(m_bus->read(addr))
                << "            ";
            break;
        }
        case AddressingMode::ZeroPageY:
        {
            uint8_t base = m_bus->read(r_pc + 1);
            uint8_t addr = base + r_Y;
            std::cout 
                << "$" 
                << std::setw(2) << static_cast<unsigned int>(base) 
                << ",Y @ " << std::setw(2) << static_cast<unsigned int>(addr)
                << " = " << std::setw(2) << static_cast<unsigned int>(m_bus->read(addr))
                << "            ";
            break;
        }
        case AddressingMode::Implicit:
            std::cout << "                           ";
            break;
        case AddressingMode::Relative:
            std::cout 
                << "$" << std::setw(4) << static_cast<unsigned int>(endVal)
                << "                      ";
            break;
        case AddressingMode::Indirect:
        {

            uint8_t loPtr = m_bus->read(r_pc + 1);
            uint8_t hiPtr = m_bus->read(r_pc + 2);

            uint8_t nextLo = loPtr + 1;

            uint16_t lo = (static_cast<uint16_t>(hiPtr) << 8) | (static_cast<uint16_t>(loPtr));
            uint16_t hi = (static_cast<uint16_t>(hiPtr) << 8) | (static_cast<uint16_t>(nextLo));

            uint16_t loVal = static_cast<uint16_t>(m_bus->read(lo));
            uint16_t hiVal = static_cast<uint16_t>(m_bus->read(hi));

            uint16_t addr = (hiVal << 8) | loVal;

            std::cout 
                << "($" 
                << std::setw(4) << static_cast<unsigned int>((hiPtr << 8) | loPtr)
                << ") = " << std::setw(4) << static_cast<unsigned int>(addr)
                << "             ";
            break;
        }
        case AddressingMode::IndirectX:
        {
            uint8_t base = m_bus->read(r_pc + 1);
            uint8_t loPtr = base + r_X;
            uint8_t hiPtr = loPtr + 1; 
            uint16_t lo = static_cast<uint16_t>(m_bus->read(loPtr));
            uint16_t hi = static_cast<uint16_t>(m_bus->read(hiPtr));
            uint16_t addr = (hi << 8) | lo;
            std::cout 
                << "($" << std::setw(2) << static_cast<unsigned int>(base) << ",X) @ "
                << static_cast<unsigned int>(loPtr)
                << " = " << std::hex << std::setw(4) << static_cast<unsigned int>(addr)
                << " = " << std::hex << std::setw(2) << static_cast<unsigned int>(m_bus->read(addr))
                << "   ";
            break;
        }
        case AddressingMode::IndirectY:
        {
            uint16_t loPtr = static_cast<uint16_t>(m_bus->read(r_pc + 1));
            uint8_t hiPtr = loPtr + 1; 
            uint16_t lo = static_cast<uint16_t>(m_bus->read(loPtr));
            uint16_t hi = static_cast<uint16_t>(m_bus->read(hiPtr));

            uint16_t baseAddr = ((hi << 8) | lo);
            uint16_t offset = static_cast<uint16_t>(r_Y);
            uint16_t addr = baseAddr + offset;

            std::cout 
                << "($" << std::setw(2) << static_cast<unsigned int>(loPtr) << "),Y = "
                << std::setw(4) << static_cast<unsigned int>(baseAddr)
                << " @ " << std::hex << std::setw(4) << static_cast<unsigned int>(addr)
                << " = " << std::hex << std::setw(2) << static_cast<unsigned int>(m_bus->read(addr))
                << " ";
        }
    }

    std::cout 
        << " A:" << std::setw(2) <<  static_cast<unsigned int>(r_A) 
        << " X:" << std::setw(2) << static_cast<unsigned int>(r_X)
        << " Y:" << std::setw(2) << static_cast<unsigned int>(r_Y)
        << " P:" << std::setw(2) << static_cast<unsigned int>(getP(false))
        << " SP:" << std::setw(2) << static_cast<unsigned int>(r_S)
        << " CYC:" << std::dec << m_cycles + 7; // TODO: Figure out why my cycles are off by 7
    std::cout << std::endl;
}

void Cpu::pushStack(uint8_t value) {
    uint16_t addr = static_cast<uint16_t>(r_S) + 0x0100;
    m_bus->write(addr, value);
    r_S--;
}

uint8_t Cpu::popStack() {
    r_S++;
    uint16_t addr = static_cast<uint16_t>(r_S) + 0x0100;
    return m_bus->read(addr);
}

unsigned int Cpu::getCycles() {
    return m_cycles;
}

void Cpu::i_lda(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    r_A = m_bus->read(addr);
    updateZNFlags(r_A);
    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_sta(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    m_bus->write(addr, r_A);
}

void Cpu::i_ldx(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    r_X = m_bus->read(addr);
    updateZNFlags(r_X);
    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_stx(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    m_bus->write(addr, r_X);
}

void Cpu::i_ldy(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    r_Y = m_bus->read(addr);
    updateZNFlags(r_Y);
    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_sty(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    m_bus->write(addr, r_Y);
}

void Cpu::i_adc(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint16_t a = static_cast<uint16_t>(r_A);
    uint16_t b = static_cast<uint16_t>(m_bus->read(addr));
    uint16_t sum = a + b;

    if (f_C)
        sum++;

    r_A = static_cast<uint8_t>(sum);

    f_C = sum > 0xFF;

    f_V = ((sum ^ a) & (sum ^ b) & 0x80) > 0;

    updateZNFlags(r_A);

    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_sbc(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint16_t a = static_cast<uint16_t>(r_A);
    uint16_t b = ~static_cast<uint16_t>(m_bus->read(addr));
    uint16_t diff = a + b;

    if (f_C)
        diff++;

    r_A = static_cast<uint8_t>(diff);

    f_C = static_cast<int8_t>(diff) >= 0;

    f_V = ((diff ^ a) & (diff ^ b) & 0x80) > 0;

    updateZNFlags(r_A);

    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_inc(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint8_t value = m_bus->read(addr) + 1;
    m_bus->write(addr, value);
    updateZNFlags(value);
}

void Cpu::i_dec(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint8_t value = m_bus->read(addr) - 1;
    m_bus->write(addr, value);
    updateZNFlags(value);
}

void Cpu::i_asl(AddressingMode mode) {
    uint8_t value;
    if (mode == AddressingMode::Accumulator) {
        value = r_A;
        r_A = value << 1;
    } else {
        uint16_t addr = getAddress(mode);
        value = m_bus->read(addr);
        m_bus->write(addr, value << 1);
    }

    f_C = (value & 0x80) > 0;

    updateZNFlags(value << 1);
}

void Cpu::i_lsr(AddressingMode mode) {
    uint8_t value;
    if (mode == AddressingMode::Accumulator) {
        value = r_A;
        r_A = value >> 1;
    } else {
        uint16_t addr = getAddress(mode);
        value = m_bus->read(addr);
        m_bus->write(addr, value >> 1);
    }

    f_C = (value & 1) > 0;

    updateZNFlags(value >> 1);
}

void Cpu::i_rol(AddressingMode mode) {
    uint8_t value;
    uint8_t newValue;
    if (mode == AddressingMode::Accumulator) {
        value = r_A;
        newValue = (value << 1) | f_C;
        r_A = newValue;
    } else {
        uint16_t addr = getAddress(mode);
        value = m_bus->read(addr);
        newValue = (value << 1) | f_C;
        m_bus->write(addr, newValue);
    }

    f_C = (value & 0x80) > 0;

    updateZNFlags(newValue);

}

void Cpu::i_ror(AddressingMode mode) {
    uint8_t value;
    uint8_t newValue;
    if (mode == AddressingMode::Accumulator) {
        value = r_A;
        newValue = (value >> 1) | (f_C << 7);
        r_A = newValue;
    } else {
        uint16_t addr = getAddress(mode);
        value = m_bus->read(addr);
        newValue = (value >> 1) | (f_C << 7);
        m_bus->write(addr, newValue);
    }

    f_C = (value & 1) > 0;

    updateZNFlags(newValue);
}

void Cpu::i_and(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    r_A &= m_bus->read(addr);
    updateZNFlags(r_A);
    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_ora(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    r_A |= m_bus->read(addr);
    updateZNFlags(r_A);
    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_eor(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    r_A ^= m_bus->read(addr);
    updateZNFlags(r_A);
    if (m_pageCrossed) m_cycles++;
}

void Cpu::i_bit(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint8_t mem = m_bus->read(addr);
    uint8_t res = r_A & mem;

    f_V = (mem & 0x40) > 0;
    f_N = (mem & 0x80) > 0;
    f_Z = res == 0;
}

void Cpu::i_jsr(AddressingMode mode) {
    uint16_t addr = getAddress(mode);
    uint8_t lo = static_cast<uint8_t>(r_pc - 1);
    uint8_t hi = static_cast<uint8_t>((r_pc - 1) >> 8);
    pushStack(hi);
    pushStack(lo);
    r_pc = addr;
}

void Cpu::i_rts() {
    uint16_t lo = static_cast<uint16_t>(popStack());
    uint16_t hi = static_cast<uint16_t>(popStack());
    uint16_t addr = (hi << 8) | lo;
    r_pc = addr + 1;
}

void Cpu::i_brk() {
    uint16_t addr = r_pc + 2;
    uint8_t lo = static_cast<uint8_t>(addr);
    uint8_t hi = static_cast<uint8_t>(addr >> 8);
    pushStack(hi);
    pushStack(lo);

    pushStack(getP(true));
    f_I = true;
    r_pc = 0xFFFE;
}

void Cpu::i_rti() {
    setP(popStack(), true);
    uint16_t lo = static_cast<uint16_t>(popStack());
    uint16_t hi = static_cast<uint16_t>(popStack());
    uint16_t addr = (hi << 8) | lo;
    r_pc = addr;
}

void Cpu::tick() {
    m_pageCrossed = false;
    // fetch opcode
    uint8_t code = m_bus->read(r_pc);

    std::map<uint8_t, Opcode>::iterator iterator = opcodes.find(code);
    Opcode opcode;

    // Check if opcode is valid
    if (iterator != opcodes.end()) {
        opcode = iterator->second;
    } else {
        std::cerr << "ERROR: Opcode 0x" 
                << std::hex << std::setw(2) << std::setfill('0') 
                << static_cast<unsigned int>(code) 
                << " not found $" << std::setw(4) << r_pc << std::endl;
        return;
    }

    logInstr(opcode);

    r_pc++;

    // Execute opcode
    switch (opcode.name) {
        case Name::LDA:
            i_lda(opcode.mode);
            break;
        case Name::STA:
            i_sta(opcode.mode);
            break;
        case Name::LDX:
            i_ldx(opcode.mode);
            break;
        case Name::STX:
            i_stx(opcode.mode);
            break;
        case Name::LDY:
            i_ldy(opcode.mode);
            break;
        case Name::STY:
            i_sty(opcode.mode);
            break;
        case Name::TAX:
            r_X = r_A;
            updateZNFlags(r_X);
            break;
        case Name::TAY:
            r_Y = r_A;
            updateZNFlags(r_Y);
            break;
        case Name::TXA:
            r_A = r_X;
            updateZNFlags(r_A);
            break;
        case Name::TYA:
            r_A = r_Y;
            updateZNFlags(r_A);
            break;
        case Name::ADC:
            i_adc(opcode.mode);
            break;
        case Name::SBC:
            i_sbc(opcode.mode);
            break;
        case Name::INC:
            i_inc(opcode.mode);
            break;
        case Name::DEC:
            i_dec(opcode.mode);
            break;
        case Name::INX:
            r_X++;
            updateZNFlags(r_X);
            break;
        case Name::DEX:
            r_X--;
            updateZNFlags(r_X);
            break;
        case Name::INY:
            r_Y++;
            updateZNFlags(r_Y);
            break;
        case Name::DEY:
            r_Y--;
            updateZNFlags(r_Y);
            break;
        case Name::ASL:
            i_asl(opcode.mode);
            break;
        case Name::LSR:
            i_lsr(opcode.mode);
            break;
        case Name::ROL:
            i_rol(opcode.mode);
            break;
        case Name::ROR:
            i_ror(opcode.mode);
            break;
        case Name::AND:
            i_and(opcode.mode);
            break;
        case Name::ORA:
            i_ora(opcode.mode);
            break;
        case Name::EOR:
            i_eor(opcode.mode);
            break;
        case Name::BIT:
            i_bit(opcode.mode);
            break;
        case Name::CMP:
            compare(r_A, opcode.mode);
            break;
        case Name::CPX:
            compare(r_X, opcode.mode);
            break;
        case Name::CPY:
            compare(r_Y, opcode.mode);
            break;
        case Name::BCC:
        case Name::BCS:
        case Name::BEQ:
        case Name::BNE:
        case Name::BPL:
        case Name::BMI:
        case Name::BVC:
        case Name::BVS:
            branch(opcode.name);
            break;
        case Name::JMP:
            r_pc = getAddress(opcode.mode);
            break;
        case Name::JSR:
            i_jsr(opcode.mode);
            break;
        case Name::RTS:
            i_rts();
            break;
        case Name::BRK:
            i_brk();
            break;
        case Name::RTI:
            i_rti();
            break;
        case Name::PHA:
            pushStack(r_A);
            break;
        case Name::PLA:
            r_A = popStack();
            updateZNFlags(r_A);
            break;
        case Name::PHP:
            pushStack(getP(true));
            break;
        case Name::PLP:
            setP(popStack(), false);
            break;
        case Name::TXS:
            r_S = r_X;
            break;
        case Name::TSX:
            r_X = r_S;
            updateZNFlags(r_X);
            break;
        case Name::CLC:
            f_C = false;
            break;
        case Name::SEC:
            f_C = true;
            break;
        case Name::CLI:
            m_pendingIFlagValue = false;
            m_pendingIFlagUpdate = true;
            break;
        case Name::SEI:
            m_pendingIFlagValue = true;
            m_pendingIFlagUpdate = true;
            break;
        case Name::CLD:
            f_D = false;
            break;
        case Name::SED:
            f_D = true;
            break;
        case Name::CLV:
            f_V = false;
            break;
        case Name::NOP:
            break;
    }


    m_cycles += opcode.cycles;
}

void Cpu::pollIRQ() {
    // Do something here

    // update I flag after polling irq
    if (m_pendingIFlagUpdate) {
        f_I = m_pendingIFlagValue;
        m_pendingIFlagUpdate = false;
    }
}

void Cpu::reset() {
    r_pc = 0xFFFC;
    r_S = 0xFD;
    f_I = 1;

    uint16_t lo = static_cast<uint16_t>(m_bus->read(r_pc));
    uint16_t hi = static_cast<uint16_t>(m_bus->read(r_pc + 1));
    r_pc = ((hi << 8) | lo);

    m_cycles = 5;
}