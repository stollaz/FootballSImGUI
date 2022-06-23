#include "geometry.h"

void renderPixel(int x, int y, int H, int W, uint32_t& buffer, Geometry g){
	buffer[x+y*H] = 0xFFFFFFFF;
}