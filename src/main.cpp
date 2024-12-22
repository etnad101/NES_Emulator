#include <iostream>
#include <iomanip>
#include "bus.h"
#include "cpu.h"

void print_bus(Bus &bus) {
    for (int addr = 0; addr <= 0xFFFF; addr++) {
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
    Cpu cpu = Cpu();
    cpu.tick(&bus);
    return 0;
}
