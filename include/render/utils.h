#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H
#include <render/2bpp.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
bool is_tile_in_line(int y, int line);
#endif