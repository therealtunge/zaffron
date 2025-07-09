#include <utils/utils.h>

uint8_t leftRotate(uint8_t n) {
    return (n << 1) | (n >> 7);
}

uint8_t rightRotate(uint8_t n) {
    return (n >> 1) | (n << 7);
}

uint16_t _8to16(uint8_t a, uint8_t b) {
	return ((a << 8) | b);
}

cpuflags _8toflags(uint8_t a) {
	cpuflags out;
	out.zero = a & 128;
	out.sub = a & 64;
	out.halfcarry = a & 32;
	out.carry = a & 16;
	return out;
}

uint8_t flagsto8(cpuflags a) {
	uint8_t out = 0;
	if (a.zero) {
		out|=128;
	}
	if (a.sub) {
		out|=64;
	}
	if (a.halfcarry) {
		out|=32;
	}
	if (a.carry) {
		out|=16;
	}
	return out;
}
uint8_t getbits8(uint8_t k, uint8_t p, uint8_t number) {
	return ((((1 << k) - 1)  &  (number >> p)));
}
uint8_t getbits16(uint8_t k, uint8_t p, uint16_t number) {
	return ((((1 << k) - 1)  &  (number >> p)));
}