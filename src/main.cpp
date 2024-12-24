#include <iostream>
#include <iomanip>
#include "bus.h"
#include "cpu.h"
#include "cartridge.h"

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
    Bus bus = Bus();
    Cpu cpu = Cpu(&bus);
    std::string romPath = "../roms/DonkeyKong.nes";
    Cartridge cartridge(romPath);
    bus.loadCartridge(&cartridge);
    cpu.reset();
    for (int i = 0; i < 15; i++) {
        cpu.tick();
    }
    //print_bus(bus, 0x8000, 0xFFFF);
    return 0;
}
