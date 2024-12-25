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

    m_prgROMSize = buffer[PRG_ROM_SIZE_ADDR] * PRG_ROM_CHUNK_SIZE;
    m_chrROMSize = buffer[CHR_ROM_SIZE_ADDR] * CHR_ROM_CHUNK_SIZE;

    std::cout << "prg rom size: 0x" << std::hex << m_prgROMSize << ", chr rom size: 0x" << m_chrROMSize << std::endl;

    m_prgROM = new uint8_t[m_prgROMSize];
    m_chrROM = new uint8_t[m_chrROMSize];

    for (int i = 0; i < m_prgROMSize; i++) {
        m_prgROM[i] = buffer[0x10 + i];
    }

    for (int i = 0; i < m_chrROMSize; i++) {
        m_chrROM[i] = buffer[i + 0x10 + m_prgROMSize];
    }

    file.close();
}

Cartridge::~Cartridge() {
    delete[] m_prgROM;
    delete[] m_chrROM;
}

uint8_t Cartridge::getPrgROM(int index) {
    if (m_prgROMSize == PRG_ROM_CHUNK_SIZE) {
        index &= 0x3FFF;
    }
    if (index < 0 || index >= m_prgROMSize) {
        std::cerr << "ERROR: Attempted to read outside of prgROM";
        exit(-1);
    }
    return m_prgROM[index];
}

uint8_t Cartridge::getChrROM(int index) {
    if (index < 0 || index >= m_chrROMSize) {
        std::cerr << "ERROR: Attempted to read outside of chrROM";
        exit(-1);
    }
    return m_chrROM[index];
}