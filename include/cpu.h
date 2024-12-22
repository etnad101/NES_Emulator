#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <map>

#include "bus.h"
#include "opcodes.h"

class Cpu {
private:
    uint8_t a, x, y, s, p;
    uint16_t pc;
    uint8_t getData(AddressingMode mode, Bus* bus);
public: 
    Cpu();
    void tick(Bus* bus);
};