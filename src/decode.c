#include "decode.h" 


void DecodeTypeR(uint32_t instr, ParsedInstruction *p_instr){
    p_instr->type = INSTR_TYPE_R;
    p_instr->rd = (instr >> 7) & 0x1f;
    p_instr->funct3 = (instr >> 12) & 0x07;
    p_instr->rs1 = (instr >> 15) & 0x1f;
    p_instr->rs2 = (instr >> 20) & 0x1f;
    p_instr->funct7 = (instr >> 25) & 0x7f;
}


void DecodeTypeI(uint32_t instr, ParsedInstruction *p_instr){
    p_instr->type = INSTR_TYPE_I;
    p_instr->rd = (instr >> 7) & 0x1f;
    p_instr->funct3 = (instr >> 12) & 0x07;
    p_instr->rs1 = (instr >> 15) & 0x1f;
    p_instr->imm = (instr >> 20) & 0xfff;
    if(p_instr->imm & (1 << 11)){   // Sign extend
        p_instr->imm |= 0xfffff000;
    }
}


void DecodeTypeS(uint32_t instr, ParsedInstruction *p_instr){
    p_instr->type = INSTR_TYPE_S;
    p_instr->funct3 = (instr >> 12) & 0x07;
    p_instr->rs1 = (instr >> 15) & 0x1f;
    p_instr->rs2 = (instr >> 20) & 0x1f;
    uint32_t imm_1 = (instr >> 7) & 0x1f;
    uint32_t imm_2 = (instr >> 25) & 0x7f;
    p_instr->imm = (imm_2 << 5) | imm_1;
    if(p_instr->imm & (1 << 11)){   // Sign extend
        p_instr->imm |= 0xfffff000;
    }
}


void DecodeTypeB(uint32_t instr, ParsedInstruction *p_instr){
    p_instr->type = INSTR_TYPE_B;
    p_instr->funct3 = (instr >> 12) & 0x07;
    p_instr->rs1 = (instr >> 15) & 0x1f;
    p_instr->rs2 = (instr >> 20) & 0x1f;
    uint32_t imm_1 = (instr >> 8) & 0x0f;
    uint32_t imm_2 = (instr >> 25) & 0x3f;
    uint32_t imm_3 = (instr >> 7) & 0x01;
    uint32_t imm_4 = (instr >> 31) & 0x01;
    p_instr->imm = (imm_1 << 1) | (imm_2 << 5) | (imm_3 << 11) | (imm_4 << 12);
    if(p_instr->imm & (1 << 12)){   // Sign extend
        p_instr->imm |= 0xffffe000;
    }
}


void DecodeTypeU(uint32_t instr, ParsedInstruction *p_instr){
    p_instr->type = INSTR_TYPE_U;
    p_instr->rd = (instr >> 7) & 0x1f;
    p_instr->imm = (instr >> 12);
}


void DecodeTypeJ(uint32_t instr, ParsedInstruction *p_instr){
    p_instr->type = INSTR_TYPE_J;
    p_instr->rd = (instr >> 7) & 0x1f;
    uint32_t imm_1 = (instr >> 21) & 0x3FF;
    uint32_t imm_2 = (instr >> 20) & 0x1;
    uint32_t imm_3 = (instr >> 12) & 0xFF;
    uint32_t imm_4 = (instr >> 31) & 0x1;
    p_instr->imm = (imm_4 << 20) | (imm_3 << 12) | (imm_2 << 11) | (imm_1 << 1);
    if(p_instr->imm & (1 << 20)){   // Sign extend
        p_instr->imm |= 0xffe00000;
    }
}
