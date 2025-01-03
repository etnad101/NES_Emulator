#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <map>

#include "bus.h"
#include "opcodes.h"

class Cpu {
private:
    bool m_debug;

    Bus* m_bus;
    unsigned int m_cycles;
    bool m_pageCrossed;

    // registers
    uint8_t r_A; 
    uint8_t r_X;
    uint8_t r_Y; 
    uint8_t r_S;
    uint16_t r_pc;

    // flags
    bool f_C;
    bool f_Z;
    bool f_I; 
    bool f_D; 
    bool f_V; 
    bool f_N;

    bool m_pendingIFlagValue;
    bool m_pendingIFlagUpdate;

    // helper functions
    uint16_t getAddress(AddressingMode mode);
    uint8_t getP(bool f_B);
    void setP(uint8_t flags, bool updateInow);
    void updateZNFlags(uint8_t value);
    void compare(uint8_t reg, AddressingMode mode);
    uint16_t calculateBranchAddr(uint8_t offset);
    void branch(Name name);
    void logInstr(Opcode opcode);
    void pushStack(uint8_t value);
    uint8_t popStack();

    // instructions
    void i_lda(AddressingMode mode);
    void i_sta(AddressingMode mode);
    void i_ldx(AddressingMode mode);
    void i_stx(AddressingMode mode);
    void i_ldy(AddressingMode mode);
    void i_sty(AddressingMode mode);
    void i_adc(AddressingMode mode);
    void i_sbc(AddressingMode mode);
    void i_inc(AddressingMode mode);
    void i_dec(AddressingMode mode);
    void i_asl(AddressingMode mode);
    void i_lsr(AddressingMode mode);
    void i_rol(AddressingMode mode);
    void i_ror(AddressingMode mode);
    void i_and(AddressingMode mode);
    void i_ora(AddressingMode mode);
    void i_eor(AddressingMode mode);
    void i_bit(AddressingMode mode);
    void i_jsr(AddressingMode mode);
    void i_rts();
    void i_brk();
    void i_rti();
public: 
    Cpu(Bus* bus);
    int tick();
    void pollIRQ();
    void reset();
    unsigned int getCycles();
};