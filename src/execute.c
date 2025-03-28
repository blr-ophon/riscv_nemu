#include "execute.h"


int ExecuteR_Aluop(RVCore *core, ParsedInstruction *p_inst){
    int rv = 0;
    uint32_t x0_dummy;
    uint32_t *rd = p_inst->rd == 0? &x0_dummy : &core->gpr[p_inst->rd];
    uint32_t rs1 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs1];
    uint32_t rs2 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs2];

    switch(p_inst->func3){ 
        case 0x0:   // ADD/SUB
            if(p_inst->func7 == 0x00){ 
                *rd = rs1 + rs2;
            }else if(p_inst->func7 == 0x20){
                *rd = rs1 - rs2;
            }
            else{
                rv = -1;
            }
            break;
        case 0x4:   // XOR
            rv = p_inst->func7 == 0x00? 0 : 1;
            *rd = rs1 ^ rs2;
            break;
        case 0x6:   // OR
            rv = p_inst->func7 == 0x00? 0 : 1;
            *rd = rs1 | rs2;
            break;
        case 0x7:   // AND
            rv = p_inst->func7 == 0x00? 0 : 1;
            *rd = rs1 & rs2;
            break;
        case 0x1:   // SLL
            rv = p_inst->func7 == 0x00? 0 : 1;
            *rd = rs1 << rs2;
            break;
        case 0x5:   // SRL / SRA
            if(p_inst->func7 == 0x00){
                *rd = rs1 >> rs2;
            }else if(p_inst->func7 == 0x20){
                uint32_t sign_mask = 0;
                if((*rd) & (1 << 31)){
                    sign_mask = 0xffffffff << (32 - rs2);
                }
                *rd = (rs1 >> rs2) | sign_mask;
            }else{
                rv = -1;
            }
            break;
        case 0x2:   // SLT
            rv = p_inst->func7 == 0x00? 0 : 1;
            *rd = ((int32_t) rs1 < (int32_t) rs2)? 1 : 0;
            break;
        case 0x3:   // SLTU
            rv = p_inst->func7 == 0x00? 0 : 1;
            *rd = (rs1 < rs2)? 1 : 0;
            break;
        default:
            rv = -1;
            break;

    }

    return rv;
}


int ExecuteI_Aluopi(RVCore *core, ParsedInstruction *p_inst){
    int rv = 0;
    uint32_t x0_dummy;
    uint32_t *rd = p_inst->rd == 0? &x0_dummy : &core->gpr[p_inst->rd];
    uint32_t rs1 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs1];
    uint32_t imm = p_inst->imm == 0? 0 : p_inst->imm;

    switch(p_inst->func3){ 
        case 0x0:   // ADDI
            *rd = rs1 + imm;
            break;
        case 0x4:   // XORI
            *rd = rs1 ^ imm;
            break;
        case 0x6:   // ORI
            *rd = rs1 | imm;
            break;
        case 0x7:   // ANDI
            *rd = rs1 & imm;
            break;
        case 0x1:   // SLLI
            if(((imm >> 5) & 0x7f) == 0){
                *rd = rs1 << imm;
            }else{
                rv = -1;
            }
            break;
        case 0x5:   // SRL / SRA
            if(p_inst->func7 == 0x00){
                *rd = rs1 >> imm;
            }else if(p_inst->func7 == 0x20){
                imm &= 0x1f;
                uint32_t sign_mask = 0;
                if((*rd) & (1 << 31)){
                    sign_mask = 0xffffffff << (32 - imm);
                }
                *rd = (rs1 >> imm) | sign_mask;
            }else{
                rv = -1;
            }
            break;
        case 0x2:   // SLTI
            *rd = ((int32_t) rs1 < (int32_t) imm)? 1 : 0;
            break;
        case 0x3:   // SLTIU
            *rd = (rs1 < imm)? 1 : 0;
            break;
        default:
            rv = -1;
            break;

    }

    return rv;
}

int ExecuteI_Load(RVCore *core, ParsedInstruction *p_inst){
    int rv = 0;
    uint32_t x0_dummy;
    uint32_t *rd = p_inst->rd == 0? &x0_dummy : &core->gpr[p_inst->rd];
    uint32_t rs1 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs1];
    uint32_t imm = p_inst->imm == 0? 0 : p_inst->imm;

    switch(p_inst->func3){
        case 0x0:   // LB
            *rd = (core->memory[rs1 + imm]) & 0xff;
            if(*rd & (1 << 7)){
                *rd |= 0xffffff;
            }
            break;
        case 0x1:   // LH
            *rd = (core->memory[rs1 + imm]) & 0xffff;
            if(*rd & (1 << 15)){
                *rd |= 0xffff;
            }
            break;
        case 0x2:   // LW
            *rd = core->memory[rs1 + imm];
            break;
        case 0x4:   // LBU
            *rd = (core->memory[rs1 + imm]) & 0xff;
            break;
        case 0x5:   // LHU
            *rd = (core->memory[rs1 + imm]) & 0xffff;
            break;
    }

    return rv;
}


int Execute_Store(RVCore *core, ParsedInstruction *p_inst){
    int rv = 0;
    uint32_t rs1 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs1];
    uint32_t rs2 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs2];
    uint32_t imm = p_inst->imm == 0? 0 : p_inst->imm;

    switch(p_inst->func3){
        case 0x0:   // SB
            core->memory[rs1+imm] = rs2 & 0xff;
            break;
        case 0x1:   // SH
            core->memory[rs1+imm] = rs2 & 0xffff;
            break;
        case 0x2:   // SW
            core->memory[rs1+imm] = rs2;
            break;
    }

    return rv;
}

int Execute_Branch(RVCore *core, ParsedInstruction *p_inst){
    int rv = 0;
    uint32_t rs1 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs1];
    uint32_t rs2 = p_inst->rd == 0? 0 : core->gpr[p_inst->rs2];
    uint32_t imm = p_inst->imm == 0? 0 : p_inst->imm;

    switch(p_inst->func3){
        case 0x0:   // BEQ
            if(rs1 == rs2) core->pc += imm;
            break;
        case 0x1:   // BNE
            if(rs1 != rs2) core->pc += imm;
            break;
        case 0x4:   // BLT
            if((int32_t) rs1 < (int32_t) rs2) core->pc += imm;
            break;
        case 0x5:   // BGE
            if((int32_t) rs1 >= (int32_t) rs2) core->pc += imm;
            break;
        case 0x6:   // BLTU
            if(rs1 < rs2) core->pc += imm;
            break;
        case 0x7:   // BGEU
            if(rs1 >= rs2) core->pc += imm;
            break;
    }
    return rv;
}
