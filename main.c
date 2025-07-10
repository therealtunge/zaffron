#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <emu/emu.h>
#include <cpu/cpu.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <render/2bpp.h>
#include <render/utils.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>

emustate state;
const int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
struct timeval start0, start, stop, startrender, stoprender;
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
SDL_Event event;

#define WINDOW_WIDTH 1280 // 640
#define WINDOW_HEIGHT 650 // 480
#define FPS 60
//uint8_t rom[] = { 0x3E, 0x10, 0x3E, 0x20, 0xC3, 0x00, 0x00 };
//uint8_t rom[90] = {0x06, 0x12, 0x0E, 0x12, 0x3E, 0x7D, 0x02, 0xC3, 0x12, 0x12, 0x10};
int32_t deltatime;
tilemap bgmap;
tilemap windowmap;
tile draw[256];
uint16_t colors[8] = {0x3C7E, 0x4242, 0x4242, 0x4242, 0x7E5E, 0x7E0A, 0x7C56, 0x387C};
uint8_t rom[0xFFFF] = {0x06, 0x80, 0x0E, 0x00, 0x3E, 0xFF, 0x02, 0x3E, 0x00, 0x0E, 0x01, 0x02, 0x3E, 0x7E, 0x0E, 0x02, 0x02, 0x3E, 0xFF, 0x0E, 0x03, 0x02};
uint8_t rom[0xFFFF];
uint8_t ram[0xFFFF] = {0};
uint8_t ramwrite[0xFFFF] = {0};
void renderfunc(uint8_t *ram);
void initram(uint8_t *rom, uint8_t *ram) {
	memset(ram, 0, 0xFFFF);
	int n = 0x8000;
	FILE *fptr = fopen("rom.txt", "w");
	for( int i = 0; i<n; i++ ) {
		ram[i] = rom[i];
	}
	fwrite(ram, 1, sizeof(ram), fptr);
	fclose(fptr);
	return;
}
void renderfunc(uint8_t *ram) {
	uint8_t lcdc = 0;
	uint8_t wx = 0;
	uint8_t wy = 0;
	uint8_t scx = 0;
	uint8_t scy = 0;
	for (;;) {
		lcdc = ram[0xFF40];
		wx = ram[0xFF4A];
		wy = ram[0xFF4B];
		scx = ram[0xFF42];
		scy = ram[0xFF43];
		gettimeofday(&startrender, NULL);
		readtile(ram + 16, colors);
		bgmap = readtilemap(ram, lcdc & 0b00001000);
		windowmap = readtilemap(ram, lcdc & 0b01000000);
		draw[1] = _16totile(colors);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		drawtilemap(draw, bgmap, renderer, scx * SIZE, scy * SIZE);
		drawtilemap(draw, windowmap, renderer, wx * SIZE, wy * SIZE);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		SDL_RenderDebugTextFormat(renderer, (float) ((WINDOW_WIDTH - (charsize * 46)) / 2), 400, "a: %d, b: %d, c: %d, pc: %d", state.state.a, state.state.b, state.state.c, state.state.pc);

		gettimeofday(&stoprender, NULL);
		deltatime = stoprender.tv_sec - startrender.tv_sec;
		SDL_RenderDebugTextFormat(renderer, 0, 100, "FPS: %d", deltatime);
		if (deltatime / 1000 > (1000 / FPS)) {} else {
    		SDL_Delay((1000 / FPS) - deltatime / 1000);
		}
		SDL_RenderPresent(renderer);
	}
	printf("uh oh\n");
}
int main(int argc, char *argv[]) {
	FILE *fptr = fopen("b", "r");
	long size;
	fseek(fptr, 0, SEEK_END); 
	size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET); 
	fread(ram, 1, size, fptr);
	fread(ramwrite, 1, size, fptr);
	SDL_SetAppMetadata("Zaffron", "1.0", "com.tunge.gbemu.zaffron");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("Zaffron", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_SetRenderVSync(renderer, 1);
	state = emustart(ram);
	gettimeofday(&start0, NULL);
	pthread_t renderer;
	int status = pthread_create(&renderer, NULL, renderfunc, ram);
	if (status) {
		printf("oh no!\n");
		exit(1);
	}
	uint8_t prev = 205;
	for (;;) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT: {
					exit(0);
				}
			}
		}
		gettimeofday(&start, NULL);
		run_instruction(&state.state, ram);
		gettimeofday(&stop, NULL);
	}
}
