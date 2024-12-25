#include "bus.h"

#include <cstdint>
#include <iostream>

Bus::Bus() : m_cartridgeInserted(false), m_cartridge(nullptr) {
    for (int i = 0; i < 0x0800; i++) {
        this->m_ram[i] = 0;
    }

    for (int i = 0; i < 8; i++) {
        this->m_ppuRegs[i] = 0;
    }

    for (int i = 0; i < 0x18; i++) {
        this->m_apuIORregs[i] = 0;
    }
}

void Bus::loadCartridge(Cartridge* cartridge) {
    if (cartridge) {
        this->m_cartridge = cartridge;
        this->m_cartridgeInserted = true;
    }
}

uint8_t Bus::read(uint16_t addr) {
    // if (addr >= 0xC000) {
    //     return m_cartridge->getPrgROM(addr - 0xC000); // Remove this after testing
    // } 
    if ((addr & 0xE000) == 0) {
        return this->m_ram[addr & 0x07FF];
    }
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        return this->m_ppuRegs[addr & 7];
    }
    if (addr >= 0x4000 && addr <= 0x4017) {
        return this->m_apuIORregs[addr - 0x4000];
    }
    if (addr >= 0x4018 && addr <= 0x401F) {
        return 0x00; // APU and I/O functionality that is normally disabled, I think this should return 0
    }
    if (addr >= 0x4020 && addr <= 0x5FFF) {
        // TODO: figure out what to do here
        return 0;
    }
    if (addr >= 0x6000 && addr <= 0x7FFF) {
        // TODO: Make this return cartridge ram
        return 0;
    }
    if (m_cartridgeInserted) {
        return m_cartridge->getPrgROM(addr - 0x8000);
    }
    std::cerr << "ERROR: No cartridge inserted" << std::endl;
    exit(-1);
}

void Bus::write(uint16_t addr, uint8_t value) {
    if ((addr & 0xE000) == 0) {
        this->m_ram[addr & 0x07FF] = value;
        return;
    }
    if (addr >= 0x2000 && addr <= 0x3FFF) {
        this->m_ppuRegs[addr & 7] = value;
        return;
    }
    if (addr >= 0x4000 && addr <= 0x4017) {
        this->m_apuIORregs[addr - 0x4000] = value;
        return;
    }
    std::cerr << "ERROR: Writing to something i haven't implemented yet" << std::endl;
    exit(-1);
}