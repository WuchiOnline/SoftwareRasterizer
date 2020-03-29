#include "iostream"
#include "SDL.h"

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void plot_pixel_ndc(SDL_Surface *surface, float x_ndc, float y_ndc, Uint32 pixel, SDL_Window *targetWindow);

int main(int argc, char *argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Software Rasterizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
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

		// base DC example: red pixel at 100,100;
		// putpixel(screen, 100, 100, SDL_MapRGB(screen->format, 255, 0, 0));
		plot_pixel_ndc(screen, 0, 0, SDL_MapRGB(screen->format, 250, 0, 0), window); // wip test

		SDL_UpdateWindowSurface(window);
	}

	SDL_Quit();
	return 0;
}

void plot_pixel_ndc(SDL_Surface *surface, float x_ndc, float y_ndc, Uint32 pixel, SDL_Window *targetWindow) // work in progress
{
	int bpp = surface->format->BytesPerPixel;

	int width = SDL_GetWindowSurface(targetWindow)->w;
	int height = SDL_GetWindowSurface(targetWindow)->h;

	float x_dc = ((x_ndc + 1) / 2)*(width);
	float y_dc = ((y_ndc + 1) / 2)*(height);

	int x_dc_int;
	int y_dc_int;

	// convert float to int
	if (x_dc >= 0)
		x_dc_int = (int)(x_dc + 0.5);
	else
		x_dc_int = (int)(x_dc - 0.5);

	if (y_dc >= 0)
		y_dc_int = (int)(y_dc + 0.5);
	else
		y_dc_int = (int)(y_dc - 0.5);


	Uint8 *p = (Uint8 *)surface->pixels + y_dc_int * surface->pitch + x_dc_int * bpp;

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


