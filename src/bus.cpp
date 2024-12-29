#include "bus.h"

#include <cstdint>
#include <iostream>

Bus::Bus() : m_cartridgeInserted(false), m_cartridge(nullptr) {
    m_ram = new uint8_t[RAM_SIZE];
    m_vram = new uint8_t[VRAM_SIZE];
    m_ppuRegs = new uint8_t[PPU_REGS_SIZE];
    m_apuIORregs = new uint8_t[APU_IO_REGS_SIZE];

    memset(m_ram, 0, RAM_SIZE);
    memset(m_vram, 0, VRAM_SIZE);
    memset(m_ppuRegs, 0, PPU_REGS_SIZE);
    memset(m_apuIORregs, 0, APU_IO_REGS_SIZE);

}

void Bus::loadCartridge(Cartridge* cartridge) {
    if (cartridge) {
        this->m_cartridge = cartridge;
        this->m_cartridgeInserted = true;
    }
}

uint8_t Bus::cpu_read(uint16_t addr) {
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
        return 0; // APU and I/O functionality that is normally disabled, I think this should return 0
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

uint8_t Bus::ppu_read(uint16_t addr) {
    if (addr <= 0x1FFF) {
        return m_cartridge->getChrROM(addr);
    }
    if (addr <= 0x2FFF) {
        return m_vram[addr - 0x2FFF];
    }
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
