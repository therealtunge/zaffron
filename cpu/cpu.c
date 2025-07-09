#include <cpu/cpu.h>
#include <cpu/cbprefix.h>
#include <emu/emu.h>
#include <utils/utils.h>
/*
regobj
0000 a
0001 b
0010 c
0011 d
0100 e
0101 h
0110 l
0111 addr of hl
*/
void doinstr0x80to0xBF(cpustate *state, uint8_t instr, uint8_t b, uint8_t *ram) {
	// source
	uint8_t source = 0;
	switch (b) {
		case (0x00): {
			source = state->a;
			break;
		}
		case (0x01): {
			source = state->b;
			break;
		}
		case (0x02): {
			source = state->c;
			break;
		}
		case (0x03): {
			source = state->d;
			break;
		}
		case (0x04): {
			source = state->e;
			break;
		}
		case (0x05): {
			source = state->h;
			break;
		}
		case (0x06): {
			source = state->l;
			break;
		}
		case (0x07): {
			source = ram[_8to16(state->h, state->l)];
			break;
		}
	}
	switch (instr) {
		case ADD: {
			state->flags.carry = (((int)state->a + source) >= 256);
			state->a+=source;
			state->flags.zero = (state->a == 0);
			return;
		}
		case ADC: {
			state->a+=source + state->flags.carry;
			state->flags.carry = (((int)state->a + source + state->flags.carry) >= 256);
			state->flags.zero = (state->a == 0);
			return;
		}
		case SUB: {
			state->a-=source;
			state->flags.carry = (((int)state->a - source) >= 256);
			state->flags.zero = (state->a == 0);
			state->flags.sub = true;
			return;
		}
		case SBC: {
			state->a-=source;
			state->a-=state->flags.carry;
			state->flags.carry = ((((int)state->a - source) - state->flags.carry) >= 256);
			state->flags.zero = (state->a == 0);
			state->flags.sub = true;
			return;
		}
		case AND: { 
			state->a&=source;
			state->flags.zero = (state->a == 0);
			return;
		}
		case XOR: {
			state->a^=source;
			state->flags.zero = (state->a == 0);
			return;
		}
		case OR: {
			state->a|=source;
			state->flags.zero = (state->a == 0);
			return;
		}
		case CP: {
			state->flags.zero = state->a == source;
			return;
		}
	}
}
void ld0x40to0x7F(cpustate *state, uint8_t info, uint8_t *ram) {
	// source
	uint8_t source;
	switch (info & 0x0F) {
		case (0x00): {
			source = state->a;
			break;
		}
		case (0x01): {
			source = state->b;
			break;
		}
		case (0x02): {
			source = state->c;
			break;
		}
		case (0x03): {
			source = state->d;
			break;
		}
		case (0x04): {
			source = state->e;
			break;
		}
		case (0x05): {
			source = state->h;
			break;
		}
		case (0x06): {
			source = state->l;
			break;
		}
		case (0x07): {
			source = ram[_8to16(state->h, state->l)];
			break;
		}
	}
	// dest
	switch ((info & 0xF0) >> 4) {
		case (0x00): {
			state->a = source;
			return;
		}
		case (0x01): {
			state->b = source;
			return;
		}
		case (0x02): {
			state->c = source;
			return;
		}
		case (0x03): {
			state->d = source;
			return;
		}
		case (0x04): {
			state->e = source;
			return;
		}
		case (0x05): {
			state->h = source;
			return;
		}
		case (0x06): {
			state->l = source;
			return;
		}
		case (0x07): {
			ram[_8to16(state->h, state->l)] = source;
			return;
		}
	}
}
// handle gigantic block of LD's (this is gonna be tedious)
void handleInstruction0x4Xto0x7X(cpustate *state, uint8_t *ram) {
	uint8_t info = 0;
	uint8_t dest = 0;
	uint8_t src = 0;
	// handle destination of ld
	switch (ram[state->pc] & 0xF0) {
		case (0x40): {
			// b
			if (ram[state->pc] <= 0x47) {
				dest = 1;
			} else {
				// c
				dest = 2;
			}
			break;
		}
		case (0x50): {
			// d
			if (ram[state->pc] <= 0x57) {
				dest = 3;
			} else {
				// e
				dest = 4;
			}
			break;
		}
		case (0x60): {
			// h
			if (ram[state->pc] <= 0x67) {
				dest = 5;
			} else {
				// l
				dest = 6;
			}
			break;
		}
		case (0x70): {
			// to hl
			if (ram[state->pc] <= 0x77) {
				dest = 7;
			} else {
				// a
				dest = 0;
			}
			break;
		}
	}
	// handle source of ld
	switch (ram[state->pc] & 0x0F) {
		// b
		case (0x00): {
			src = 1;
			break;
		}
		// c
		case (0x01): {
			src = 2;
			break;
		}
		// d
		case (0x02): {
			src = 3;
			break;
		}
		// e
		case (0x03): {
			src = 4;
			break;
		}
		// h
		case (0x04): {
			src = 5;
			break;
		}
		// l
		case (0x05): {
			src = 6;
			break;
		}
		// hl
		case (0x06): {
			src = 7;
			break;
		}
		// a
		case (0x07): {
			src = 0;
			break;
		}
		// b
		case (0x08): {
			src = 1;
			break;
		}
		// c
		case (0x09): {
			src = 2;
			break;
		}
		// d
		case (0x0A): {
			src = 3;
			break;
		}
		// e
		case (0x0B): {
			src = 4;
			break;
		}
		// h
		case (0x0C): {
			src = 5;
			break;
		}
		// l
		case (0x0D): {
			src = 6;
			break;
		}
		// hl
		case (0x0E): {
			src = 7;
			break;
		}
		// a
		case (0x0F): {
			src = 0;
			break;
		}
	}
	info = src | (dest << 4);
	ld0x40to0x7F(state, info, ram);
	state->pc++;
}
// handle alu instructions
void handleInstruction0x8Xto0xBX(cpustate *state, uint8_t *ram) {
	bool alt = !((ram[state->pc] & 0x0F) <= 0x07);
	uint8_t instr = 0;
	uint8_t b = 0;
	// handle instruction type
	switch (ram[state->pc] & 0xF0) {
		case (0x80): {
			if (!alt) {
				instr = ADD;
				break;
			} else {
				instr = ADC;
				break;
			}
		}
		case (0x90): {
			if (!alt) {
				instr = SUB;
				break;
			} else {
				instr = SBC;
				break;
			}
		}
		case (0xA0): {
			if (!alt) {
				instr = AND;
				break;
			} else {
				instr = XOR;
				break;
			}
		}
		case (0xB0): {
			if (!alt) {
				instr = OR;
				break;
			} else {
				instr = CP;
				break;
			}
		}
	}
	// handle B
	switch (ram[state->pc] & 0x0F) {
		// b
		case (0x00): {
			b = 1;
			break;
		}
		// c
		case (0x01): {
			b = 2;
			break;
		}
		// d
		case (0x02): {
			b = 3;
			break;
		}
		// e
		case (0x03): {
			b = 4;
			break;
		}
		// h
		case (0x04): {
			b = 5;
			break;
		}
		// l
		case (0x05): {
			b = 6;
			break;
		}
		// hl
		case (0x06): {
			b = 7;
			break;
		}
		// a
		case (0x07): {
			b = 0;
			break;
		}
		// b
		case (0x08): {
			b = 1;
			break;
		}
		// c
		case (0x09): {
			b = 2;
			break;
		}
		// d
		case (0x0A): {
			b = 3;
			break;
		}
		// e
		case (0x0B): {
			b = 4;
			break;
		}
		// h
		case (0x0C): {
			b = 5;
			break;
		}
		// l
		case (0x0D): {
			b = 6;
			break;
		}
		// hl
		case (0x0E): {
			b = 7;
			break;
		}
		// a
		case (0x0F): {
			b = 0;
			break;
		}
	}
	doinstr0x80to0xBF(state, instr, b, ram);
	state->pc++;
}
void run_instruction(cpustate *state, uint8_t *ram) {
	uint16_t pc = state->pc;
	if ((((ram[pc] & 0xF0) >= 0x40)
	&& (ram[pc] & 0xF0) <= 0x70)
	&& ram[pc] != 0x76) {
		handleInstruction0x4Xto0x7X(state, ram);
		return;
	}
	if (((
	ram[pc] & 0xF0) >= 0x80)
	&& (ram[pc] & 0xF0)	<= 0xB0) {
		handleInstruction0x8Xto0xBX(state, ram);
		return;
	}
	switch (ram[pc]) {
		case (0x00): {
			pc++;
			break;
		}
		case (0x01): {
			state->b = ram[pc+1];
			state->c = ram[pc+2];
			pc+=3;
			break;
		}
		case (0x02): {
			ram[_8to16(state->b, state->c)] = state->a;
			pc++;
			break;
		}
		case (0x03): {
			uint16_t bc = _8to16(state->b, state->c);
			bc++;
			state->b = (bc & 0xFF00) >> 8;
			state->c = bc & 0x00FF;
			pc++;
			break;
		}
		case (0x04): {
			state->b++;
			pc++;
			break;
		}
		case (0x05): {
			state->b--;
			state->flags.zero = state->b == 0;
			pc++;
			break;
		}
		case (0x06): {
			state->b = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x07): {
			state->a = leftRotate(state->a);
			pc++;
			break;
		}
		case (0x08): {
			ram[_8to16(ram[pc+1], ram[pc+2])] = state->sp & 0x00FF;
			pc+=3;
			break;
		}
		case(0x09): {
			uint16_t bc = _8to16(state->b, state->c);
			uint16_t hl = _8to16(state->h, state->l);
			hl+=bc;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x0A): {
			state->a = ram[_8to16(state->b, state->c)];
			pc++;
			break;
		}
		case (0x0B): {
			uint16_t bc = _8to16(state->b, state->c);
			bc--;
			state->b = (bc & 0xFF00) >> 8;
			state->c = bc & 0x00FF;
			pc++;
			break;
		}
		case (0x0C): {
			state->c++;
			pc++;
			break;
		}
		case (0x0D): {
			state->c--;
			pc++;
			break;
		}
		case (0x0E): {
			state->c = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x0F): {
			state->c = rightRotate(state->c);
			pc++;
			break;
		}
		case (0x10): {
			for (;;) {}
			pc++;
			break;
		}
		case (0x11): {
			state->d = ram[pc+1];
			state->e = ram[pc+2];
			pc+=3;
			break;
		}
		case (0x12): {
			ram[_8to16(state->d, state->e)] = state->a;
			pc++;
			break;
		}
		case (0x13): {
			uint16_t de = _8to16(state->d, state->e);
			de++;
			state->d = (de & 0xFF00) >> 8;
			state->e = de & 0x00FF;
			pc++;
			break;
		}
		case (0x14): {
			state->d++;
			pc++;
			break;
		}
		case (0x15): {
			state->d--;
			pc++;
			break;
		}
		case (0x16): {
			state->d = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x17): {
			uint8_t a = rightRotate(state->a);
			a&=(state->flags.carry);
			pc++;
			break;
		}
		case (0x18): {
			pc+=(int8_t)ram[pc+1];
			return;
		}
		case(0x19): {
			uint16_t de = _8to16(state->d, state->e);
			uint16_t hl = _8to16(state->h, state->l);
			hl+=de;
			state->h = hl & 0xFF00 >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x1A): {
			state->a = ram[_8to16(state->d, state->e)];
			pc++;
			break;
		}
		case (0x1B): {
			uint16_t de = _8to16(state->d, state->e);
			de--;
			state->d = (de & 0xFF00) >> 8;
			state->e = de & 0x00FF;
			pc++;
			break;
		}
		case (0x1C): {
			state->e++;
			pc++;
			break;
		}
		case (0x1D): {
			state->e--;
			pc++;
			break;
		}
		case (0x1E): {
			state->e = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x1F): {
			uint8_t a = leftRotate(state->a);
			a&=(state->flags.carry);
			pc++;
			break;
		}
		case (0x20): {
			if (!state->flags.zero) {
				pc+=(int8_t)ram[pc+1];
				pc+=2;
			} else {
				pc+=2;
			}
			break;
		}
		case (0x21): {
			state->h = ram[pc+2];
			state->l = ram[pc+1];
			pc+=3;
			break;
		}
		case (0x22): {
			uint16_t hl = _8to16(state->h, state->l);
			ram[hl] = state->a;
			hl++;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x23): {
			uint16_t hl = _8to16(state->h, state->l);
			hl++;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x24): {
			state->h++;
			pc++;
			break;
		}
		case (0x25): {
			state->h--;
			pc++;
			break;
		}
		case (0x26): {
			state->h = ram[pc+1];
			pc+=2;
			break;
		}
// daa is stupid
// reference:
/* note: assumes a is a uint8_t and wraps from 0xff to 0
if (!n_flag) {  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
  if (c_flag || a > 0x99) { a += 0x60; c_flag = 1; }
  if (h_flag || (a & 0x0f) > 0x09) { a += 0x6; }
} else {  // after a subtraction, only adjust if (half-)carry occurred
  if (c_flag) { a -= 0x60; }
  if (h_flag) { a -= 0x6; }
}
// these flags are always updated
z_flag = (a == 0); // the usual z flag
h_flag = 0; // h flag is always cleared
*/
		case (0x27): {
			if (!state->flags.sub) {
				if (state->flags.carry || state->a > 0x99) { state->a += 0x60; state->flags.carry = true; }
				if (state->flags.halfcarry || (state->a & 0x0f) > 0x09) { state->a+=0x6; }
			} else {
				if (state->flags.carry) { state->a-=0x60; }
				if (state->flags.halfcarry) {state->a-=0x06; }
			}
			state->flags.zero = (state->a == 0);
			state->flags.halfcarry = false;
			pc++;
			break;
		}
		case (0x28): {
			if (state->flags.zero) {
				pc+=(int8_t)ram[pc+1];
				pc+=2;
			} else {
				pc+=2;
			}
			break;
		}
		case(0x29): {
			uint16_t hl = _8to16(state->h, state->l);
			hl*=2;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x2A): {
			uint16_t hl = _8to16(state->h, state->l);
			hl++;
			state->a = ram[hl];
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x2B): {
			uint16_t hl = _8to16(state->h, state->l);
			hl--;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x2C): {
			state->l++;
			pc++;
			break;
		}
		case (0x2D): {
			state->l--;
			pc++;
			break;
		}
		case (0x2E): {
			state->l = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x2F): {
			state->a = !state->a;
			pc++;
			break;
		}
		case (0x30): {
			if (!state->flags.carry) {
				pc+=ram[pc+1];
			} else {
				pc+=2;
			}
			break;
		}
		case (0x31): {
			state->sp = _8to16(ram[pc+1], ram[pc+2]);
			pc+=3;
			break;
		}
		case (0x32): {
			uint16_t hl = _8to16(state->h, state->l);
			ram[hl] = state->a;
			hl--;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x33): {
			state->sp++;
			pc++;
			break;
		}
		case (0x34): {
			ram[_8to16(state->h, state->l)]++;
			pc++;
			break;
		}
		case (0x35): {
			ram[_8to16(state->h, state->l)]--;
			pc++;
			break;
		}
		case (0x36): {
			ram[_8to16(state->h, state->l)] = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x37): {
			state->flags.carry = true;
			pc++;
			break;
		}
		case (0x38): {
			if (state->flags.carry) {
				pc+=ram[pc+1];
			} else {
				pc+=2;
			}
			break;
		}
		case(0x39): {
			uint16_t hl = _8to16(state->h, state->l);
			hl+=state->sp;
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x3A): {
			uint16_t hl = _8to16(state->h, state->l);
			hl--;
			state->a = ram[hl];
			state->h = (hl & 0xFF00) >> 8;
			state->l = hl & 0x00FF;
			pc++;
			break;
		}
		case (0x3B): {
			state->sp--;
			pc++;
			break;
		}
		case (0x3C): {
			state->a++;
			pc++;
			break;
		}
		case (0x3D): {
			state->a--;
			state->flags.zero = state->a == 0;
			pc++;
			break;
		}
		case (0x3E): {
			state->a = ram[pc+1];
			pc+=2;
			break;
		}
		case (0x3F): {
			state->flags.carry = !state->flags.carry;
			pc++;
			break;
		}
// block 0x40 to 0x7F (excluding 0x76)
		case (0x76): {
			break;
		}
// block 0x80 to 0xB0
		case (0xC1): {
			state->b = ram[state->sp-2];
			state->c = ram[state->sp-1];
			state->sp -= 2;
			pc++;
			break;
		}
		case (0xC2): {
			if (!state->flags.zero) {
				pc = _8to16(ram[pc+1], ram[pc+2]);
				break;
			} else {
				pc += 3;
				break;
			}
		}
		case (0xC3): {
			pc = _8to16(ram[pc+1], ram[pc+2]);
			break;
		}
		case (0xC5): {
			ram[state->sp] = state->b;
			ram[state->sp+1] = state->c;
			state->sp += 2;
			pc++;
			break;
		}
		case (0xC9): {
			pc = _8to16(ram[state->sp-2], ram[state->sp-1]) + 1;
			ram[state->sp] = 0;
			ram[state->sp+1] = 0;
			state->sp -= 2;
			break;
		}
		case (0xCA): {
			if (state->flags.zero) {
				pc = _8to16(ram[pc+1], ram[pc+2]);
				break;
			} else {
				pc += 3;
				break;
			}
		}
		// oh no
		case (0xCB): {
			run0xcb(ram, state);
			pc += 2;
			break;
		}
		case (0xCD): {
			ram[state->sp] = (pc & 0xFF00) >> 8;
			ram[state->sp+1] = pc & 0x00FF;
			pc = _8to16(ram[pc+2], ram[pc+1]);
			state->sp += 2;
			break;
		}
		case (0xE0): {
			ram[0xFF00 + ram[pc+1]] = state->a;
			pc+=2;
			break;
		}
		case (0xE2): {
			ram[0xFF00 + state->c] = state->a;
			pc++;
			break;
		}
		case (0xE9): {
			pc = _8to16(state->h, state->l);
			break;
		}
		case (0xEA): {
			ram[_8to16(ram[pc+1], ram[pc+2])] = state->a;
			pc+=3;
			break;
		}
		case (0xEE): {
			state->a^=ram[pc+1];
			state->flags.zero = state->a == 0;
			pc+=2;
			break;
		}
		case (0xEF): {
			pc = ram[0x28] >> 8;
			break;
		}
		case (0xF0): {
			state->a = 0xFF00 + ram[pc+1];
			pc+=2;
			break;
		}
		case (0xF1): {
			state->a = ram[state->sp];
			state->sp++;
			state->flags = _8toflags(ram[state->sp]);
			state->sp++;
			pc++;
			break;
		}
		case (0xF2): {
			state->a = 0xFF00 + state->c;
			pc++;
			break;
		}
		case (0xF3): {
			// TODO: DI
			pc++;
			break;
		}
		case (0xF5): {
			state->sp--;
			ram[state->sp] = state->a;
			state->sp--;
			ram[state->sp] = flagsto8(state->flags);
			pc++;
			break;
		}
		case (0xF6): {
			state->a|=ram[pc+1];
			state->flags.zero = state->a == 0;
			pc+=2;
			break;
		}
		case (0xF7): {
			pc = ram[0x30] >> 8;
			break;
		}
		case (0xF8): {
			uint16_t sp = state->sp + (int8_t)ram[pc+1];
			state->h = (sp & 0xFF00) >> 8;
			state->l = sp & 0x00FF;
			pc+=2;
			break;
		}
		case (0xF9): {
			state->sp = _8to16(state->h, state->l);
			pc++;
			break;
		}
		case (0xFA): {
			state->a = ram[_8to16(ram[pc+1], ram[pc+2])];
			pc+=3;
			break;
		}
		case (0xFB): {
			// TODO: EI
			pc++;
			break;
		}
		case (0xFE): {
			state->flags.zero = state->a == ram[pc+1];
			pc+=2;
			break;
		}
		case (0xFF): {
			pc = ram[0x38] >> 8;
			break;
		}
		
		default: {
			printf("unknown instruction %d at %d\n", ram[pc], pc);
			break;
		}
	}
	state->pc = pc;
	return;
}

void reset_cpuflags(cpuflags *flags) {
	flags->carry = false;
	flags->halfcarry = false;
	flags->sub = false;
	flags->zero = false;
}

void reset_cpustate(cpustate *state) {
	state->a = 0;
	state->b = 0;
	state->c = 0;
	state->d = 0;
	state->h = 0;
	state->l = 0;
	state->pc = 0;
	state->sp = 0xB000;
	reset_cpuflags(&state->flags);
}