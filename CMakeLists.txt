cmake_minimum_required(VERSION 3.17)
project(nes_emulator) 

set(CMAKE_CXX_STANDARD 23)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

# add sdl2 for windows in else case here
if (MACOSX)
    find_package(SDL2 REQUIRED)
endif()

include_directories(include)
include_directories(${SDL2_INCLUDE_DIRS})

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

add_library(
    emulator
    include/emulator.h
    src/emulator.cpp
)

add_library(
    ppu
    include/ppu.h
    src/ppu.cpp
)

add_library(
    window
    include/window.h
    src/window.cpp
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
    emulator
    ppu
    window
    ${SDL2_LIBRARIES}
)


