#include "bus.h"
#include "cpu.h"
#include "ppu.h"
#include "window.h"
#include "SDL.h"

const int CLOCK_SPEED = 21441960;
const int CYCLES_PER_FRAME  = CLOCK_SPEED / 60;

class Emulator {
private:
    bool m_debug;
    bool m_running;
    Bus m_bus;
    Cpu m_cpu;
    Ppu m_ppu;
public:
    Emulator();
    void loadCartridge(Cartridge* cartridge);
    void run();
};
