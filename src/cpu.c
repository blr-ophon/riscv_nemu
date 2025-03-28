#include "cpu.h"

static int Decode(RVCore *cpu, uint32_t instruction);

int main(void){
    RVCore *cpu = RVCore_Create(4*128, 0, 0);

    RVCore_Run(cpu);

    RVCore_Destroy(cpu);
}


RVCore *RVCore_Create(uint32_t mem_size, uint32_t mem_offset, uint32_t pc_start){
    RVCore *cpu = malloc(sizeof(RVCore));
    cpu->memory = calloc(mem_size/4, 4);
    cpu->memSize = mem_size;
    cpu->memOffset = mem_offset;
    cpu->pc = pc_start;

    return cpu;
}


int RVCore_Run(RVCore *cpu){
    if(cpu->pc > cpu->memSize){
        return -MEMFAULT;
    }
    uint32_t instruction = cpu->memory[cpu->pc];
}


void RVCore_Destroy(RVCore *cpu){
    free(cpu->memory);
    free(cpu);
}

static int Decode(RVCore *cpu, uint32_t instruction){
}
