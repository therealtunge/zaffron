#ifndef _2BPP_H
#define _2BPP_H
#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#define SIZE 1
#define SIZEPERTILE SIZE * 8

typedef struct rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb;
typedef struct tile {
	uint16_t colors[8];
} tile;
typedef struct tilemap {
	uint8_t tileno[1024];
} tilemap;
void drawtilemap(tile *tiles, tilemap map, SDL_Renderer *renderer, int x, int y);
tilemap readtilemap(uint8_t *ram, bool no);
void readtile(uint8_t *ram, uint16_t *out);
tile _16totile(uint16_t *in);
uint16_t packbits(uint8_t a, uint8_t b);
void decode_row(uint16_t row, rgb *out);
void draw_row(uint16_t row, SDL_Renderer *renderer, int x, int y);
void draw_tile(tile tile, SDL_Renderer *renderer, int x, int y);
void readtiles(uint8_t *ram, tile *out);
#endif