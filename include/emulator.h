#include "bus.h"
#include "cpu.h"

class Emulator {
private:
    bool m_running;
    Bus m_bus;
    Cpu m_cpu;
public:
    Emulator();
    void loadCartridge(Cartridge* cartridge);
    void run();
};
