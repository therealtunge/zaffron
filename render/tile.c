#include <render/2bpp.h>
#include <utils/utils.h>
#include <stdint.h>
#include <stdio.h>

rgb palette[] = {{255, 255, 255}, {170, 170, 170}, {85, 85, 85}, {0, 0, 0}};
void drawtilemap(tile *tiles, tilemap map, SDL_Renderer *renderer, int x, int y) {
	for (int i = 0; i < 1024; i++) {
		draw_tile(tiles[map.tileno[i]], renderer, ((i % 32) * SIZEPERTILE) + x, ((i / 32) * SIZEPERTILE) + y);
	}
	return;
}
tilemap readtilemap(uint8_t *ram, bool no) {
	tilemap ret;
	if (no) {
		memcpy(ret.tileno, ram+0x9C00, 1024);
	} else {
		memcpy(ret.tileno, ram+0x9800, 1024);
	}
	return ret;
}
void readtile(uint8_t *ram, uint16_t *out) {
	for (int i = 0; i < 16; i++) {
		out[i] = _8to16(
		ram[(i*2) + 0x8000],
		ram[(i*2) + 0x8001]);
//		printf("A %d\n", out[i]);
	}
	return;
}
tile _16totile(uint16_t *in) {
	tile ret;
	memcpy(ret.colors, in, 16);
	return ret;
}
uint16_t packbits(uint8_t a, uint8_t b) {
	uint16_t temp = 0;
	for (int i = 0; i < 16; i+=2) {
		temp = temp | getbits8(1, i/2, b) << i;
		temp = temp | (getbits8(1, i/2, a) << (i + 1));
	}
	return temp;
}
void decode_row(uint16_t row, rgb *out) {
	uint8_t a = (row & 0xFF00) >> 8;
	uint8_t b = row & 0x00FF;
//	printf("B %d\n", packbits(a, b));
	for (int i = 0; i < 8; i++) {
		out[i] = palette[getbits16(2, i*2, packbits(a, b))];
	}
	return;
}
void draw_row(uint16_t row, SDL_Renderer *renderer, int x, int y) {
	rgb colors[8];
	decode_row(row, colors);
	SDL_FRect fillRect = {0, y, SIZE, SIZE};
	for (int i = 0; i < 8; i++) {
		fillRect.x = ((SIZE*80)-(i*SIZE)) + x;
		SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(renderer, &fillRect);
	}
	return;
}
void draw_tile(tile tile, SDL_Renderer *renderer, int x, int y) {
	for (int i = 0; i < 8; i++) {
		draw_row(tile.colors[i], renderer, x, (i*SIZE) + y);
	}
	return;
}
void readtiles(uint8_t *ram, tile *out) {
	for (int i = 0; i < 1024; i++) {
		readtile(ram + (i * 16), out[i].colors);
	}
}