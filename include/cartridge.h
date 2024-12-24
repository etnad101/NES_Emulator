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
    int prgROMSize;
    int chrROMSize;

    bool nametableArangement;
    bool persistentMemory;
    bool hasTrainer;
    bool altNametableLayout;

    Mapper mapper;
    CartridgeType cartridgeType;

    uint8_t* prgROM;
    uint8_t* chrROM;
public:
    Cartridge(std::string rom_path);
    ~Cartridge();

    uint8_t getPrgROM(int index);
    uint8_t getChrROM(int index);
};