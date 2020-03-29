#include "iostream"
#include "SDL.h"

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void plot_pixel_ndc(SDL_Surface *surface, int x, int y, Uint32 pixel, SDL_Window *targetWindow);

const int baseWidth = 640;
const int baseHeight = 480;

int main(int argc, char *argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, baseWidth, baseHeight, SDL_WINDOW_SHOWN);
	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_Surface *screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);
	SDL_Event event;
	bool quit = false;
	while (quit == false)
	{
		if (SDL_WaitEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		// plot pixel at any location and any color.
		// putpixel(screen, x, y, SDL_MapRGB(screen->format, r, g, b));

		// first example: (red pixel at 100,100);
		putpixel(screen, 100, 100, SDL_MapRGB(screen->format, 255, 0, 0));
		// plot_pixel_ndc(screen, 200, 200, SDL_MapRGB(screen->format, 250, 0, 0), window); // wip test

		SDL_UpdateWindowSurface(window);
	}

	SDL_Quit();
	return 0;
}

void plot_pixel_ndc(SDL_Surface *surface, int x, int y, Uint32 pixel, SDL_Window *targetWindow) // work in progress
{
	int bpp = surface->format->BytesPerPixel;

	int width = SDL_GetWindowSurface(targetWindow)->w;
	int height = SDL_GetWindowSurface(targetWindow)->h;

	// make sure to include error check for x, y, which must be between -1 and 1, or else break;

	// convert ndc back to dc formula, derived from reversing dc to ndc formula

	Uint8 *p = (Uint8 *)surface->pixels + ((y*height)/baseHeight) * surface->pitch + ((x*width)/baseWidth) * bpp;

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


