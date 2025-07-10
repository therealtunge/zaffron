#include <render/utils.h>
bool is_tile_in_line(int y, int line) {
	return ((y + 8) >= line) & (y <= line);
}