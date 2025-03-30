#include "decode.h" 

int ParseInstruction(uint32_t instr){
    int rv = 0;
    ParsedInstruction p_inst;
    memset(&p_inst, 0, sizeof(ParseInstruction));

    p_inst.opcode = instr & 0x7f;

    
    switch(p_inst.opcode){
        case 0x33:  //ALUOP
            p_inst.type = INST_TYPE_R;
            p_inst.rd = (instr >> 7) & 0x1f;
            p_inst.funct3 = (instr >> 12) & 0x07;
            p_inst.rs1 = (instr >> 15) & 0x1f;
            p_inst.rs2 = (instr >> 20) & 0x1f;
            p_inst.funct7 = (instr >> 25) & 0x7f;
            break;
        case 0x13:  //ALUOP_I
        case 0x03:  //LOAD
        case 0x67:  //JALR
        case 0x73:  //ENVIRONMENT
            p_inst.type = INST_TYPE_I;
            p_inst.rd = (instr >> 7) & 0x1f;
            p_inst.funct3 = (instr >> 12) & 0x07;
            p_inst.rs1 = (instr >> 15) & 0x1f;
            p_inst.imm = (instr >> 20) & 0xfff;
            if(p_inst.imm & (1 << 11)){
                p_inst.imm |= 0xfffff000;
            }
            break;
        case 0x23:  //STORE
            {
            p_inst.type = INST_TYPE_S;
            p_inst.funct3 = (instr >> 12) & 0x07;
            p_inst.rs1 = (instr >> 15) & 0x1f;
            p_inst.rs2 = (instr >> 20) & 0x1f;
            uint32_t imm_1 = (instr >> 7) & 0x1f;
            uint32_t imm_2 = (instr >> 25) & 0x7f;
            p_inst.imm = (imm_2 << 5) | imm_1;
            if(p_inst.imm & (1 << 11)){
                p_inst.imm |= 0xfffff000;
            }
            break;
            }
        case 0x63:  //BRANCH
            {
            p_inst.type = INST_TYPE_B;
            p_inst.funct3 = (instr >> 12) & 0x07;
            p_inst.rs1 = (instr >> 15) & 0x1f;
            p_inst.rs2 = (instr >> 20) & 0x1f;
            uint32_t imm_1 = (instr >> 8) & 0x0f;
            uint32_t imm_2 = (instr >> 25) & 0x3f;
            uint32_t imm_3 = (instr >> 7) & 0x01;
            uint32_t imm_4 = (instr >> 31) & 0x01;
            p_inst.imm = (imm_1 << 1) | (imm_2 << 5) | (imm_3 << 11) | (imm_4 << 12);
            if(p_inst.imm & (1 << 12)){
                p_inst.imm |= 0xffffe000;
            }
            break;
            }
        case 0x6f:  //JAL
            p_inst.type = INST_TYPE_J;
            p_inst.rd = (instr >> 7) & 0x1f;
            uint32_t imm_1 = (instr >> 21) & 0x3FF;
            uint32_t imm_2 = (instr >> 20) & 0x1;
            uint32_t imm_3 = (instr >> 12) & 0xFF;
            uint32_t imm_4 = (instr >> 31) & 0x1;
            p_inst.imm = (imm_4 << 20) | (imm_3 << 12) | (imm_2 << 11) | (imm_1 << 1);
            if(p_inst.imm & (1 << 20)){
                p_inst.imm |= 0xffe00000;
            }
            break;
        case 0x37:  //LUI
        case 0x17:  //AUIPC
            p_inst.type = INST_TYPE_U;
            p_inst.rd = (instr >> 7) & 0x1f;
            p_inst.imm = (instr >> 12);
            break;
        default:
            rv = -1;
    }

    return rv;
}


int ParseInstruction_Comp(uint16_t instr){
    int rv = 0;
    ParsedInstruction p_inst;
    memset(&p_inst, 0, sizeof(ParseInstruction));

    p_inst.opcode = instr & 0x03;
    p_inst.funct3 = (instr) >> 13;

    
    switch((p_inst.funct3 << 2) | p_inst.opcode){
        //CR
        case 0b10010:   // group
            p_inst.rs2 = (instr >> 2) & 0x1f;
            p_inst.rs1 = (instr >> 7) & 0x1f;
            p_inst.rd = p_inst.rs1;
            break;


        //CI
        case 0b01010:   // lwsp
            p_inst.rs1 = (instr >> 7) & 0x1f;
            p_inst.rd = p_inst.rs1;
            p_inst.imm |= ((instr & 0x70) >> 2);       // imm[4:2]
            p_inst.imm |= ((instr & (1 << 12)) >> 7);  // imm[5]
            p_inst.imm |= ((instr & 0xa) << 4);        // imm[7:6]
            break;
        case 0b01001:   // li
            p_inst.rs1 = (instr >> 7) & 0x1f;
            p_inst.rd = p_inst.rs1;
            p_inst.imm |= ((instr & 0x7a) >> 2);       // imm[4:0]
            p_inst.imm |= ((instr & (1 << 12)) >> 7);  // imm[5]
            break;
        case 0b01101:   // addi16sp, lui
            p_inst.rs1 = (instr >> 7) & 0x1f;
            p_inst.rd = p_inst.rs1;
            if(p_inst.rs1 == 2){    // addi16sp
                p_inst.imm |= ((instr & (1 << 6) ) >> 2);       //imm[4]
                p_inst.imm |= ((instr & (1 << 2) ) << 3);       //imm[5]
                p_inst.imm |= ((instr & (1 << 5) ) << 1);       //imm[6]
                p_inst.imm |= ((instr & (0x3 << 3) ) << 4);     //imm[8:7]
                p_inst.imm |= ((instr & (1 << 12) ) >> 3);      //imm[9]
            }else if(p_inst.rs1 != 0){  // lui
                p_inst.imm |= ((instr & 0x7a) << 10);       // imm[16:12]
                p_inst.imm |= ((instr & (1 << 12)) << 5);   // imm[17]
            }
            break;
        case 0b00001:   // nop, addi
            p_inst.rs1 = (instr >> 7) & 0x1f;
            p_inst.rd = p_inst.rs1;
            if(p_inst.rd == 0){     // nop
            }else{      // addi
                p_inst.imm |= ((instr & 0x7a) >> 2);       // imm[4:0]
                p_inst.imm |= ((instr & (1 << 12)) >> 7);  // imm[5]
            }
            break;
        case 0b00010:   // slli
            p_inst.imm |= ((instr & 0x7a) >> 2);       // uimm[4:0]
            p_inst.imm |= ((instr & (1 << 12)) >> 7);  // uimm[5]
            p_inst.rs1 = (instr >> 7) & 0x1f;
            p_inst.rd = p_inst.rs1;
            break;


        //CSS
        case 0b11010:   // swsp
            p_inst.rs2 = (instr >> 2) & 0x1f;
            p_inst.imm |= ((instr & 0x1e00) >> 7);
            p_inst.imm |= ((instr & 0x0180) >> 1);
            break;


        //CIW
        case 0b00000:   // Illegal, addi4spn
            p_inst.rd = (instr >> 2) & 0x7;
            if(p_inst.rd == 0){     // Illegal
                // TODO
            }else{      // addi4spn
                p_inst.imm |= (instr & (1 << 6)) >> 4;  // uimm[2]
                p_inst.imm |= (instr & (1 << 5)) >> 2;  // uimm[3]
                p_inst.imm |= (instr & (0x3 << 11)) >> 7;   // uimm[5:4]
                p_inst.imm |= (instr & (0xf << 7)) >> 1;    // uimm[9:6]
            }
            break;


        //CL
        case 0b01000:   // lw
            p_inst.rd = (instr >> 2) & 0x7;
            p_inst.rs1 = (instr >> 7) & 0x7;
            p_inst.imm |= (instr & (1 << 6)) >> 4;      //uimm[2] 
            p_inst.imm |= (instr & 0x1a00) >> 7;        //uimm[5:3]
            p_inst.imm |= (instr & (1 << 5)) << 1;      //uimm[6] 
            break;


        //CS
        case 0b11000:   // sw
            p_inst.rs2 = (instr >> 2) & 0x7;
            p_inst.rs1 = (instr >> 7) & 0x7;
            p_inst.imm |= (instr & (1 << 6)) >> 4;      //uimm[2] 
            p_inst.imm |= (instr & 0x1a00) >> 7;        //uimm[5:3]
            p_inst.imm |= (instr & (1 << 5)) << 1;      //uimm[6] 
            break;


        //CB
        case 0b11001:   // beqz
        case 0b11101:   // bnez
            p_inst.rs1 = (instr >> 7) & 0x7;
            p_inst.imm |= (instr & 0x18) >> 2;          //imm[2:1]
            p_inst.imm |= (instr & 0x0a00) >> 7;        //imm[4:3]
            p_inst.imm |= (instr & (1 << 2)) << 3;      //imm[5] 
            p_inst.imm |= (instr & 0x60) << 1;          //imm[7:6]
            p_inst.imm |= (instr & (1 << 12)) >> 4;     //imm[8] 
            break;


        //CJ
        case 0b10101:   // j
        case 0b00101:   // jal, addiw
            p_inst.imm |= (instr & 0x31) >> 2;          // imm[3:1]
            p_inst.imm |= (instr & (1 << 11)) >> 7;     // imm[4]
            p_inst.imm |= (instr & (1 << 2)) << 3;      // imm[5]
            p_inst.imm |= (instr & (1 << 7)) >> 1;      // imm[6]
            p_inst.imm |= (instr & (1 << 6)) << 1;      // imm[7]
            p_inst.imm |= (instr & 0x600) >> 1;         // imm[9:8]
            p_inst.imm |= (instr & (1 << 8)) << 2;      // imm[10]
            p_inst.imm |= (instr & (1 << 12)) >> 1;     // imm[11]
            break;


        case 0b00100:   // fld, lq
            break;
        case 0b01100:   // flw, ld
            break;
        case 0b10100:   // fsd, sq
            break;
        case 0b11100:   // fsw, sd
            break;

        case 0b10001:   // ALUOP
            // Special case
            break;

        case 0b00110:   // fldsp, lqsp
            break;
        case 0b01110:   // flwsp, ldsp
            break;
        case 0b10110:   // fsdsp, sqsp
            break;
        case 0b11110:   // fswsp, sdsp
            break;

        default:
            rv = -1;
    }

    return rv;
}


