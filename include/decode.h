#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>


typedef enum{
    INSTR_TYPE_R,
    INSTR_TYPE_I,
    INSTR_TYPE_S,
    INSTR_TYPE_B,
    INSTR_TYPE_U,
    INSTR_TYPE_J,
}E_InstructionType;


/*
typedef enum{
    INSTR_TYPE_CR,
    INSTR_TYPE_CI,
    INSTR_TYPE_CSS,
    INSTR_TYPE_CIW,
    INSTR_TYPE_CL,
    INSTR_TYPE_CS,
    INSTR_TYPE_CB,
    INSTR_TYPE_CJ,
}E_InstructionTypeComp;
*/


typedef struct{ 
    E_InstructionType type;
    uint8_t opcode;
    uint8_t rd;
    uint8_t funct3;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t funct7;
    uint32_t imm;
}ParsedInstruction;


void DecodeTypeR(uint32_t instr, ParsedInstruction *p_instr);
void DecodeTypeI(uint32_t instr, ParsedInstruction *p_instr);
void DecodeTypeS(uint32_t instr, ParsedInstruction *p_instr);
void DecodeTypeB(uint32_t instr, ParsedInstruction *p_instr);
void DecodeTypeU(uint32_t instr, ParsedInstruction *p_instr);
void DecodeTypeJ(uint32_t instr, ParsedInstruction *p_instr);

#endif
