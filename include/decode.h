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


typedef struct{ 
    E_InstructionType type;
    uint8_t rd;
    uint8_t func3;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t func7;
    uint32_t imm;
}ParsedInstruction;


#endif
