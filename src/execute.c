#include "execute.h"


int Execute32(RVCore *core, uint32_t instr, ParsedInstruction *p_instr){
    int rv = 0;

    uint8_t opcode = instr & 0x3;
    if(opcode == 0x3){      // 32 bit instructions
        p_instr->opcode = instr & 0x7f;
    }else{                  // Compressed instructions
        p_instr->opcode = opcode;
    }

    switch(p_instr->opcode){
        case 0x33:  //ALUOP
            DecodeTypeR(instr, p_instr);
            if(p_instr->funct7 == 0x1){
                InstrR_MUL(core, p_instr);
            }else{
                InstrR_Aluop(core, p_instr);
            }
            break;
        case 0x13:  //ALUOP_I
            DecodeTypeI(instr, p_instr);
            InstrI_Aluopi(core, p_instr);
            break;
        case 0x03:  //LOAD
            DecodeTypeI(instr, p_instr);
            InstrI_Load(core, p_instr);
            break;
        case 0x67:  //JALR
            DecodeTypeI(instr, p_instr);
            InstrJ_JALR(core, p_instr);
            break;
        case 0x73:  //ENVIRONMENT
            DecodeTypeI(instr, p_instr);
            // TODO
            break;
        case 0x23:  //STORE
            DecodeTypeS(instr, p_instr);
            InstrS_Store(core, p_instr);
            break;
        case 0x63:  //BRANCH
            DecodeTypeB(instr, p_instr);
            InstrB_Branch(core, p_instr);
            break;
        case 0x6f:  //JAL
            DecodeTypeJ(instr, p_instr);
            InstrJ_JAL(core, p_instr);
            break;
        case 0x37:  //LUI
            DecodeTypeU(instr, p_instr);
            InstrU_LUI(core, p_instr);
            break;
        case 0x17:  //AUIPC
            DecodeTypeU(instr, p_instr);
            InstrU_AUIPC(core, p_instr);
            break;
        default:
            rv = -1;
    }

    return rv;
}


int Execute16(uint16_t instr, ParsedInstruction *p_instr){
    int rv = 0;

    p_instr->opcode = instr & 0x03;
    p_instr->funct3 = (instr) >> 13;

    
    switch((p_instr->funct3 << 2) | p_instr->opcode){
        //CR
        case 0b10010:   // group
            p_instr->opcode = instr & 0xf;
            p_instr->rs2 = (instr >> 2) & 0x1f;
            p_instr->rs1 = (instr >> 7) & 0x1f;
            p_instr->rd = p_instr->rs1;
            break;


        //CI
        case 0b01010:   // lwsp
            p_instr->rs1 = (instr >> 7) & 0x1f;
            p_instr->rd = p_instr->rs1;
            p_instr->imm |= ((instr & 0x70) >> 2);       // imm[4:2]
            p_instr->imm |= ((instr & (1 << 12)) >> 7);  // imm[5]
            p_instr->imm |= ((instr & 0xa) << 4);        // imm[7:6]
            break;
        case 0b01001:   // li
            p_instr->rs1 = (instr >> 7) & 0x1f;
            p_instr->rd = p_instr->rs1;
            p_instr->imm |= ((instr & 0x7a) >> 2);       // imm[4:0]
            p_instr->imm |= ((instr & (1 << 12)) >> 7);  // imm[5]
            break;
        case 0b01101:   // addi16sp, lui
            p_instr->rs1 = (instr >> 7) & 0x1f;
            p_instr->rd = p_instr->rs1;
            if(p_instr->rs1 == 2){    // addi16sp
                p_instr->imm |= ((instr & (1 << 6) ) >> 2);       //imm[4]
                p_instr->imm |= ((instr & (1 << 2) ) << 3);       //imm[5]
                p_instr->imm |= ((instr & (1 << 5) ) << 1);       //imm[6]
                p_instr->imm |= ((instr & (0x3 << 3) ) << 4);     //imm[8:7]
                p_instr->imm |= ((instr & (1 << 12) ) >> 3);      //imm[9]
            }else if(p_instr->rs1 != 0){  // lui
                p_instr->imm |= ((instr & 0x7a) << 10);       // imm[16:12]
                p_instr->imm |= ((instr & (1 << 12)) << 5);   // imm[17]
            }
            break;
        case 0b00001:   // nop, addi
            p_instr->rs1 = (instr >> 7) & 0x1f;
            p_instr->rd = p_instr->rs1;
            if(p_instr->rd == 0){     // nop
            }else{      // addi
                p_instr->imm |= ((instr & 0x7a) >> 2);       // imm[4:0]
                p_instr->imm |= ((instr & (1 << 12)) >> 7);  // imm[5]
            }
            break;
        case 0b00010:   // slli
            p_instr->imm |= ((instr & 0x7a) >> 2);       // uimm[4:0]
            p_instr->imm |= ((instr & (1 << 12)) >> 7);  // uimm[5]
            p_instr->rs1 = (instr >> 7) & 0x1f;
            p_instr->rd = p_instr->rs1;
            break;


        //CSS
        case 0b11010:   // swsp
            p_instr->rs2 = (instr >> 2) & 0x1f;
            p_instr->imm |= ((instr & 0x1e00) >> 7);
            p_instr->imm |= ((instr & 0x0180) >> 1);
            break;


        //CIW
        case 0b00000:   // Illegal, addi4spn
            p_instr->rd = (instr >> 2) & 0x7;
            if(p_instr->rd == 0){     // Illegal
                // TODO
            }else{      // addi4spn
                p_instr->imm |= (instr & (1 << 6)) >> 4;  // uimm[2]
                p_instr->imm |= (instr & (1 << 5)) >> 2;  // uimm[3]
                p_instr->imm |= (instr & (0x3 << 11)) >> 7;   // uimm[5:4]
                p_instr->imm |= (instr & (0xf << 7)) >> 1;    // uimm[9:6]
            }
            break;


        //CL
        case 0b01000:   // lw
            p_instr->rd = (instr >> 2) & 0x7;
            p_instr->rs1 = (instr >> 7) & 0x7;
            p_instr->imm |= (instr & (1 << 6)) >> 4;      //uimm[2] 
            p_instr->imm |= (instr & 0x1a00) >> 7;        //uimm[5:3]
            p_instr->imm |= (instr & (1 << 5)) << 1;      //uimm[6] 
            break;


        //CS
        case 0b11000:   // sw
            p_instr->rs2 = (instr >> 2) & 0x7;
            p_instr->rs1 = (instr >> 7) & 0x7;
            p_instr->imm |= (instr & (1 << 6)) >> 4;      //uimm[2] 
            p_instr->imm |= (instr & 0x1a00) >> 7;        //uimm[5:3]
            p_instr->imm |= (instr & (1 << 5)) << 1;      //uimm[6] 
            break;


        //CB
        case 0b11001:   // beqz
        case 0b11101:   // bnez
            p_instr->rs1 = (instr >> 7) & 0x7;
            p_instr->imm |= (instr & 0x18) >> 2;          //imm[2:1]
            p_instr->imm |= (instr & 0x0a00) >> 7;        //imm[4:3]
            p_instr->imm |= (instr & (1 << 2)) << 3;      //imm[5] 
            p_instr->imm |= (instr & 0x60) << 1;          //imm[7:6]
            p_instr->imm |= (instr & (1 << 12)) >> 4;     //imm[8] 
            break;


        //CJ
        case 0b10101:   // j
        case 0b00101:   // jal, addiw
            p_instr->imm |= (instr & 0x31) >> 2;          // imm[3:1]
            p_instr->imm |= (instr & (1 << 11)) >> 7;     // imm[4]
            p_instr->imm |= (instr & (1 << 2)) << 3;      // imm[5]
            p_instr->imm |= (instr & (1 << 7)) >> 1;      // imm[6]
            p_instr->imm |= (instr & (1 << 6)) << 1;      // imm[7]
            p_instr->imm |= (instr & 0x600) >> 1;         // imm[9:8]
            p_instr->imm |= (instr & (1 << 8)) << 2;      // imm[10]
            p_instr->imm |= (instr & (1 << 12)) >> 1;     // imm[11]
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
