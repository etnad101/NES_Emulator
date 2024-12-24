#include "bus.h"

#include <cstdint>
#include <iostream>

Bus::Bus() {
    for (int i = 0; i < 0x0800; i++) {
        this->ram[i] = 0;
    }

    for (int i = 0; i < 8; i++) {
        this->ppuRegs[i] = 0;
    }

    for (int i = 0; i < 0x18; i++) {
        this->apuIORregs[i] = 0;
    }

    for (int i = 0; i < 0xBFE0; i++) {
        this->cartridge[i] = 0;
    } 
}

uint8_t Bus::read(uint16_t addr) {
    if ((addr & 0xE000) == 0) {
        return this->ram[addr & 0x07FF];
    }
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        return this->ppuRegs[addr & 7];
    }
    if (addr >= 0x4000 && addr <= 0x4017) {
        return this->apuIORregs[addr - 0x4000];
    }
    if (addr >= 0x4018 && addr <= 0x401F) {
        return 0x00; // APU and I/O functionality that is normally disabled, I think this should return 0
    }
    return this->cartridge[addr - 0x4020];
}

void Bus::write(uint16_t addr, uint8_t value) {
    if ((addr & 0xE000) == 0) {
        this->ram[addr & 0x07FF] = value;
        return;
    }
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        this->ppuRegs[addr & 7] = value;
        return;
    }
    if (addr >= 0x4000 && addr <= 0x4017) {
        this->apuIORregs[addr - 0x4000] = value;
        return;
    }
    if (addr >= 0x4020) {
        this->cartridge[addr - 0x4020] = value;
        return;
    }
    std::cout << "ERROR: Writing to Illegal address" << std::endl;
    exit(-1);
}