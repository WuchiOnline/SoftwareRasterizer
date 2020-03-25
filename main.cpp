#include "iostream"
#include "SDL.h"

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

int main(int argc, char *argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

	SDL_Surface *screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);

	// plot pixel at any location and any color.
	// putpixel(screen, x, y, SDL_MapRGB(screen->format, r, g, b));

	// first example: (red pixel at 100,100);
	putpixel(screen, 100, 100, SDL_MapRGB(screen->format, 255, 0, 0));

	SDL_UpdateWindowSurface(window);

	SDL_Delay(15000);

	return 0;
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}


