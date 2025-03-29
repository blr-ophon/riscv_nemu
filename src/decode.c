#include "decode.h" 

int ParseInstruction(uint32_t instruction){
    int rv = 0;
    uint8_t opcode = instruction & 0x70;

    ParsedInstruction p_inst;
    memset(&p_inst, 0, sizeof(ParseInstruction));
    
    switch(opcode){
        case 0x33:  //ALUOP
            p_inst.type = INST_TYPE_R;
            p_inst.rd = (instruction >> 7) & 0x1f;
            p_inst.func3 = (instruction >> 12) & 0x07;
            p_inst.rs1 = (instruction >> 15) & 0x1f;
            p_inst.rs2 = (instruction >> 20) & 0x1f;
            p_inst.func7 = (instruction >> 25) & 0x7f;
            break;
        case 0x13:  //ALUOP_I
        case 0x03:  //LOAD
        case 0x67:  //JALR
        case 0x73:  //ENVIRONMENT
            p_inst.type = INST_TYPE_I;
            p_inst.rd = (instruction >> 7) & 0x1f;
            p_inst.func3 = (instruction >> 12) & 0x07;
            p_inst.rs1 = (instruction >> 15) & 0x1f;
            p_inst.imm = (instruction >> 20) & 0xfff;
            if(p_inst.imm & (1 << 11)){
                p_inst.imm |= 0xfffff000;
            }
            break;
        case 0x23:  //STORE
            {
            p_inst.type = INST_TYPE_S;
            p_inst.func3 = (instruction >> 12) & 0x07;
            p_inst.rs1 = (instruction >> 15) & 0x1f;
            p_inst.rs2 = (instruction >> 20) & 0x1f;
            uint32_t imm_1 = (instruction >> 7) & 0x1f;
            uint32_t imm_2 = (instruction >> 25) & 0x7f;
            p_inst.imm = (imm_2 << 5) | imm_1;
            if(p_inst.imm & (1 << 11)){
                p_inst.imm |= 0xfffff000;
            }
            break;
            }
        case 0x63:  //BRANCH
            {
            p_inst.type = INST_TYPE_B;
            p_inst.func3 = (instruction >> 12) & 0x07;
            p_inst.rs1 = (instruction >> 15) & 0x1f;
            p_inst.rs2 = (instruction >> 20) & 0x1f;
            uint32_t imm_1 = (instruction >> 8) & 0x0f;
            uint32_t imm_2 = (instruction >> 25) & 0x3f;
            uint32_t imm_3 = (instruction >> 7) & 0x01;
            uint32_t imm_4 = (instruction >> 31) & 0x01;
            p_inst.imm = (imm_1 << 1) | (imm_2 << 5) | (imm_3 << 11) | (imm_4 << 12);
            if(p_inst.imm & (1 << 12)){
                p_inst.imm |= 0xffffe000;
            }
            break;
            }
        case 0x6f:  //JAL
            p_inst.type = INST_TYPE_J;
            p_inst.rd = (instruction >> 7) & 0x1f;
            uint32_t imm_1 = (instruction >> 21) & 0x3FF;
            uint32_t imm_2 = (instruction >> 20) & 0x1;
            uint32_t imm_3 = (instruction >> 12) & 0xFF;
            uint32_t imm_4 = (instruction >> 31) & 0x1;
            p_inst.imm = (imm_4 << 20) | (imm_3 << 12) | (imm_2 << 11) | (imm_1 << 1);
            if(p_inst.imm & (1 << 20)){
                p_inst.imm |= 0xffe00000;
            }
            break;
        case 0x37:  //LUI
        case 0x17:  //AUIPC
            p_inst.type = INST_TYPE_U;
            p_inst.rd = (instruction >> 7) & 0x1f;
            p_inst.imm = (instruction >> 12);
            break;
        default:
            rv = -1;
    }

    return rv;
}


int ParseInstruction_Comp(uint16_t instruction){
    int rv = 0;
    uint8_t opcode = instruction & 0x70;

    ParsedInstruction p_inst;
    memset(&p_inst, 0, sizeof(ParseInstruction));

    switch(opcode){
        case 0x2:   // LWSP, SWSP
            break;
        case 0x0:   // LW, SW
            break;
        case 0x1:   // J
        default:
            rv = -1;
    }

    return rv;
}
