#include "cartridge.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

Cartridge::Cartridge(std::string romPath) {

    std::ifstream file(romPath, std::ios::binary);

    if (!file) {
        std::cerr << "ERROR: couldnt open file: " << romPath << std::endl;
        exit(-1);
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});

    if (buffer[0] != 'N' || buffer[1] != 'E' || buffer[2] != 'S' || buffer[3] != 0x1A) {
        std::cerr << "ERROR: not valid INES or NES2.0 file" << std::endl;
        exit(-1);
    }

    prgROMSize = buffer[PRG_ROM_SIZE_ADDR] * PRG_ROM_CHUNK_SIZE;
    chrROMSize = buffer[CHR_ROM_SIZE_ADDR] * CHR_ROM_CHUNK_SIZE;

    std::cout << "prg rom size: 0x" << std::hex << prgROMSize << ", chr rom size: 0x" << chrROMSize << std::endl;

    prgROM = new uint8_t[prgROMSize];
    chrROM = new uint8_t[chrROMSize];

    for (int i = 0; i < prgROMSize; i++) {
        prgROM[i] = buffer[0x10 + i];
    }

    for (int i = 0; i < chrROMSize; i++) {
        chrROM[i] = buffer[i + 0x10 + prgROMSize];
    }

    file.close();
}

Cartridge::~Cartridge() {
    delete[] prgROM;
    delete[] chrROM;
}

uint8_t Cartridge::getPrgROM(int index) {
    if (prgROMSize == PRG_ROM_CHUNK_SIZE) {
        index &= 0x3FFF;
    }
    if (index < 0 || index >= prgROMSize) {
        std::cerr << "ERROR: Attempted to read outside of prgROM";
        exit(-1);
    }
    return prgROM[index];
}

uint8_t Cartridge::getChrROM(int index) {
    if (index < 0 || index >= chrROMSize) {
        std::cerr << "ERROR: Attempted to read outside of chrROM";
        exit(-1);
    }
    return chrROM[index];
}