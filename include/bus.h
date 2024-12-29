#pragma once
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "cartridge.h"

#define RAM_SIZE 0x800
#define VRAM_SIZE 0x800
#define PPU_REGS_SIZE 8
#define APU_IO_REGS_SIZE 0x18

class Bus {
private:
    uint8_t* m_ram;
    uint8_t* m_ppuRegs;
    uint8_t* m_apuIORregs;
    uint8_t* m_vram;
    bool m_cartridgeInserted;
    Cartridge* m_cartridge;
public:
    Bus();
    void loadCartridge(Cartridge* cartridge);
    uint8_t cpu_read(uint16_t addr);
    uint8_t ppu_read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);
};
