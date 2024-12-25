#include <iostream>
#include <iomanip>
#include "cartridge.h"
#include "emulator.h"

void print_bus(Bus &bus, int start, int end) {
    for (int addr = start; addr <= end; addr++) {
        if (addr % 32 == 0) {
            std::cout << std::endl;
            std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << " ";
        }

        std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned)bus.read(addr) << " ";
    }
    std::cout << std::endl;
}

int main() {
    Emulator emulator = Emulator();
    std::string romPath = "../roms/DonkeyKong.nes";
    Cartridge cartridge(romPath);
    emulator.loadCartridge(&cartridge);
    emulator.run();
    return 0;
}
