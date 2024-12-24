#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "bus.h"
#include "cpu.h"

void print_bus(Bus &bus, int size) {
    for (int addr = 0; addr <= size; addr++) {
        if (addr % 32 == 0) {
            std::cout << std::endl;
            std::cout << std::hex << std::setw(4) << std::setfill('0') << addr << " ";
        }

        std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned)bus.read(addr) << " ";
    }
    std::cout << std::endl;
}

int main() {
    const std::string rom_path = "../roms/nestest.nes";
    std::ifstream file(rom_path, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR: couldnt open file: " << rom_path << std::endl;
        return 1;
    }

    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    
    file.close();

    Bus bus = Bus();
    Cpu cpu = Cpu(&bus);

    for (uint16_t i = 0; i < buffer.size(); i++) {
        bus.write(0xC000+ i, buffer[i + 0x10]);
    }

    for (int i = 0; i < 8991; i++) {
        cpu.tick();
    }
    print_bus(bus, 0xFFFF);
    return 0;
}
