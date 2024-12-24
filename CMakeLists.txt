cmake_minimum_required(VERSION 3.17)
project(nes_emulator) 

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_library(
    bus
    include/bus.h
    src/bus.cpp
)

add_library(
    cpu
    include/cpu.h
    src/cpu.cpp
)

add_library(
    opcodes
    include/opcodes.h
    src/opcodes.cpp
)

add_library(
    cartridge
    include/cartridge.h
    src/cartridge.cpp
)

add_executable(
    nes_emulator 
    src/main.cpp 
)

target_link_libraries(
    nes_emulator
    bus
    cpu
    opcodes
    cartridge
)


