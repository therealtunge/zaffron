#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <cpu/cpu_types.h>
typedef struct cpuflags {
	bool carry;
	bool halfcarry; //implement later (this is stupid)
	bool sub;
	bool zero;
} cpuflags;

typedef struct cpustate {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t pc;
	uint16_t sp;
	cpuflags flags;
} cpustate;
void handleInstruction0x4Xto0x7X(cpustate *state, uint8_t *ram);
void handleInstruction0x8Xto0xBX(cpustate *state, uint8_t *ram);
void reset_cpuflags(cpuflags *flags);
void reset_cpustate(cpustate *state);
void run_instruction(cpustate *state, uint8_t ram[]);
#endif