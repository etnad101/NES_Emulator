#pragma once

class Ppu {
private:
    int scanline;
    int dot;
public:
    Ppu();
    void tick();
};