#include "emulator.h"
#include "bus.h"
#include "cpu.h"

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

Emulator::Emulator() : m_running(false), m_bus(), m_cpu(&m_bus), m_debug(false) {}

void Emulator::loadCartridge(Cartridge* cartridge) {
    m_bus.loadCartridge(cartridge);
}

void Emulator::run() {
    m_cpu.reset();
    m_running = true;
    int cyclesThisFrame = 0;

    Window window = Window("NES emulator", SCREEN_WIDTH, SCREEN_HEIGHT);

    while (m_running) {
        int cyclesThisInstr = m_cpu.tick();
        cyclesThisFrame += cyclesThisInstr;

        // tick ppu 3 times per cpu cycle
        for (int i = 0; i < cyclesThisInstr * 3; i++) {
            m_ppu.tick();
        }

        if (cyclesThisFrame >= CYCLES_PER_FRAME) {
            cyclesThisFrame -= CYCLES_PER_FRAME;
            // TODO: Add delay to keep stable 60fps here
            SDL_Event e; 
            window.clear();
            window.drawPixel(1, 1, 0x00ff00);
            window.present();
            while( SDL_PollEvent( &e ) ) { 
                if( e.type == SDL_QUIT ) m_running = false; 
            } 
        }
    }

    window.cleanup();
}