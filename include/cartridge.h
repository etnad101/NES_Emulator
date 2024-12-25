#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define PRG_ROM_SIZE_ADDR 4
#define CHR_ROM_SIZE_ADDR 5

#define PRG_ROM_CHUNK_SIZE 0x4000
#define CHR_ROM_CHUNK_SIZE 0x2000


enum class Mapper {

};

enum class CartridgeType {
    INES,
    NES2,
};

class Cartridge {
private:
    int m_prgROMSize;
    int m_chrROMSize;

    bool m_nametableArangement;
    bool m_persistentMemory;
    bool m_hasTrainer;
    bool m_altNametableLayout;

    Mapper m_mapper;
    CartridgeType m_cartridgeType;

    uint8_t* m_prgROM;
    uint8_t* m_chrROM;
public:
    Cartridge(std::string rom_path);
    ~Cartridge();

    uint8_t getPrgROM(int index);
    uint8_t getChrROM(int index);
};