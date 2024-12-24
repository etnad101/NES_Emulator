#pragma once
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "cartridge.h"

class Bus {
private:
    uint8_t ram[0x0800];
    uint8_t ppuRegs[0x0008];
    uint8_t apuIORregs[0x0018];
    bool cartridgeInserted;
    Cartridge* cartridge;
public:
    Bus();
    void loadCartridge(Cartridge* cartridge);
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t value);
};
