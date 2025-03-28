#include "cpu.h"

static int Decode(RVCore *core, uint32_t instruction);

int main(void){
    RVCore *core = RVCore_Create(4*128, 0, 0);

    RVCore_Run(core);

    RVCore_Destroy(core);
}


RVCore *RVCore_Create(uint32_t mem_size, uint32_t mem_offset, uint32_t pc_start){
    RVCore *core = malloc(sizeof(RVCore));
    core->memory = calloc(mem_size/4, 4);
    core->memSize = mem_size;
    core->memOffset = mem_offset;
    core->pc = pc_start;

    return core;
}


int RVCore_Run(RVCore *core){
    if(core->pc > core->memSize){
        return -MEMFAULT;
    }
    uint32_t instruction = core->memory[core->pc];
}


void RVCore_Destroy(RVCore *core){
    free(core->memory);
    free(core);
}

static int Decode(RVCore *core, uint32_t instruction){
}
