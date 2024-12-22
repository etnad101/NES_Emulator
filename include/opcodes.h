#pragma once

enum class AddressingMode {
    Immediate,
    Implicit,
    Accumulator,
    ZeroPage,
    ZeroPageX,
    ZeroPageY,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    Relative,
    Indirect,
    IndirectX,
    IndirectY,
};

enum class Name {
    LDA,
};

struct Opcode {
    Name name;
    int bytes;
    int cycles;
    AddressingMode mode;
};

