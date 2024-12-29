#include "ppu.h"

void Ppu::tick() {

    dot++;
    if (dot > 340) {
        dot = 0;
        scanline++;
    }
    if (scanline > 261)
        scanline = 0;
}

Ppu::Ppu() : dot(0), scanline(261) {}