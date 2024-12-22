#include "cpu.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <map>

#include "bus.h"
#include "opcodes.h"

std::map<uint8_t, Opcode> opcodes {
    {0xA9, Opcode {Name::LDA, 2, 2, AddressingMode::Immediate}},
    {0xA5, Opcode {Name::LDA, 2, 3, AddressingMode::ZeroPage}},
    {0xB5, Opcode {Name::LDA, 2, 4, AddressingMode::ZeroPageX}},
    {0xAD, Opcode {Name::LDA, 3, 4, AddressingMode::Absolute}},
    {0xBD, Opcode {Name::LDA, 3, 4, AddressingMode::AbsoluteX}}, // +1 cycle if page crossed
    {0xB9, Opcode {Name::LDA, 3, 4, AddressingMode::AbsoluteY}}, // +1 cycle if page crossed
    {0xA1, Opcode {Name::LDA, 2, 6, AddressingMode::IndirectX}},
    {0xB1, Opcode {Name::LDA, 2, 5, AddressingMode::IndirectY}}, // +1 cycle if page crossed
};

Cpu::Cpu() 
: a(0), x(0), y(0), pc(0xFFFC), s(0xFD), p(0b00100100) {

}

uint8_t Cpu::getData(AddressingMode mode, Bus* bus) {
    switch (mode) {
        case AddressingMode::Immediate:
            return bus->read(this->pc);
            this->pc++;
            break;
        default:
            std::cout << "ERROR: Addressing mode not implemented yet" << std::endl;
            exit(-1);
    }
}



void Cpu::tick(Bus* bus) {
    // fetch opcode
    uint8_t code = bus->read(this->pc);
    this->pc++;

    std::map<uint8_t, Opcode>::iterator iterator = opcodes.find(code);
    Opcode opcode;
    if (iterator != opcodes.end()) {
        opcode = iterator->second;
    } else {
        std::cout << "ERROR: Opcode 0x" 
                << std::hex << std::setw(2) << std::setfill('0') 
                << static_cast<unsigned int>(code) 
                << " not found" << std::endl;
        exit(-1);
    }

    switch (opcode.name) {
        case Name::LDA:

            break;
    }
    
}