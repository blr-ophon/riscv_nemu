#ifndef DECODE_H

#include <stdint.h>
#include <string.h>


typedef enum{
    INST_TYPE_R,
    INST_TYPE_I,
    INST_TYPE_S,
    INST_TYPE_B,
    INST_TYPE_U,
    INST_TYPE_J,
}E_InstructionType;


typedef enum{
    INST_TYPE_CR,
    INST_TYPE_CI,
    INST_TYPE_CSS,
    INST_TYPE_CIW,
    INST_TYPE_CL,
    INST_TYPE_CS,
    INST_TYPE_CB,
    INST_TYPE_CJ,
}E_InstructionTypeComp;


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


int ParseInstruction(uint32_t instr);
int ParseInstruction_Comp(uint16_t instr);

#endif
