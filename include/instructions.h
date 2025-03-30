#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"
#include "decode.h"

/* 32bit instructions*/
int InstrR_Aluop(RVCore *core, ParsedInstruction *p_instr);
int InstrI_Aluopi(RVCore *core, ParsedInstruction *p_instr);
int InstrI_Load(RVCore *core, ParsedInstruction *p_instr);
int InstrS_Store(RVCore *core, ParsedInstruction *p_instr);
int InstrB_Branch(RVCore *core, ParsedInstruction *p_instr);
int InstrJ_JAL(RVCore *core, ParsedInstruction *p_instr);
int InstrJ_JALR(RVCore *core, ParsedInstruction *p_instr);
int InstrU_LUI(RVCore *core, ParsedInstruction *p_instr);
int InstrU_AUIPC(RVCore *core, ParsedInstruction *p_instr);
int InstrR_MUL(RVCore *core, ParsedInstruction *p_instr);

/* Compressed instructions */
int InstrCR(RVCore *core, ParsedInstruction *p_instr);

#endif
