#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <cpu/cpu.h>
uint8_t leftRotate(uint8_t n);
uint8_t rightRotate(uint8_t n);
uint16_t _8to16(uint8_t a, uint8_t b);
cpuflags _8toflags(uint8_t a);
uint8_t flagsto8(cpuflags a);
uint8_t getbits8(uint8_t k, uint8_t p, uint8_t number);
uint8_t getbits16(uint8_t k, uint8_t p, uint16_t number);
#endif