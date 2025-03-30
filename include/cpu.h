#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    uint32_t pc;
    uint32_t gpr[32];
    uint32_t *memory;
    uint32_t memOffset;
    uint32_t memSize;
}RVCore;

typedef enum{
    MEMFAULT,
}E_Exceptions;


void RVCore_Destroy(RVCore *core);
RVCore *RVCore_Create(uint32_t mem_size, uint32_t mem_offset, uint32_t pc_start);
int RVCore_Run(RVCore *core);

#endif
