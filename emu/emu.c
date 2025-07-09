#include <cpu/cpu.h>
#include <emu/emu.h>
#include <stdio.h>
emustate emustart(uint8_t ram[]) {
	emustate state;
	state.ram = ram;
	cpustart(&state);
	return state;
}
int cpustart(emustate *state) {
	reset_cpustate(&state->state);
	
	return 0;
}
