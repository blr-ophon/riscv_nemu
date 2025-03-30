#ifndef EXECUTE_H
#define EXECUTE_H

#include "cpu.h"
#include "decode.h"
#include "instructions.h"

int Execute32(RVCore *core, uint32_t instr, ParsedInstruction *p_instr);
int Execute16(uint16_t instr, ParsedInstruction *p_instr);


#endif

