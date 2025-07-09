#include <cpu/cbprefix.h>

void run0xcb(uint8_t *ram, cpustate *state) {
	switch (ram[state->pc + 1]) {
		case (0x7C): {
			printf("yeah\n");
			state->flags.zero = state->h & 0b10000000;
			break;
		}
	}
}