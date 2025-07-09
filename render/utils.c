#include <render/utils.h>
void render_rgb_array(rgb *in, SDL_Renderer *renderer) {
	SDL_FRect fillRect = {0, 0, 4, 4};
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
//			printf("%d\n", in[(i*8)+j].r);
			SDL_SetRenderDrawColor(renderer, in[(i*8)+j].r, in[(i*8)+j].g, in[(i*8)+j].b, SDL_ALPHA_OPAQUE);
			fillRect.x= i*4;
			fillRect.y = j*4;
			SDL_RenderFillRect(renderer, &fillRect);
//			SDL_RenderPoint(renderer, i + 20, j + 30);
		}
	}
}