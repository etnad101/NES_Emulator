#include "emulator.h"
#include "bus.h"
#include "cpu.h"

Emulator::Emulator() : m_running(false), m_bus(), m_cpu(&m_bus) {}

void Emulator::loadCartridge(Cartridge* cartridge) {
    m_bus.loadCartridge(cartridge);
}

void Emulator::run() {
    m_cpu.reset();
    m_running = true;
    int i = 0;
    while (m_running) {
        m_cpu.tick();
        if (i > 8992) {
            return;
        }
        i++;
    }
}