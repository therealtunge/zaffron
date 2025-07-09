#ifndef EMU_H
#define EMU_H
#include <stdint.h>
#include <cpu/cpu.h>
typedef struct emustate {
	cpustate state;
	uint8_t *ram;
} emustate;

emustate emustart(uint8_t ram[]);
int cpustart(emustate *state);
#endif