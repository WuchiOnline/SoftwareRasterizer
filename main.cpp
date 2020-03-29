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

		// Assignment one, DC example: red pixel at 100,100;
		// putpixel(screen, 100, 100, SDL_MapRGB(screen->format, 255, 0, 0));

		// Assignment Two: NDC --> DC example: Plot red pixel at center of screen using NDC inputs, regardless of window size.
		plot_pixel_ndc(screen, 0, 0, SDL_MapRGB(screen->format, 250, 0, 0), window);

		// Bottom Left Corner test.
		plot_pixel_ndc(screen, -1, 1, SDL_MapRGB(screen->format, 250, 0, 0), window);

		// Invalid input test.
		plot_pixel_ndc(screen, -1000, 1000, SDL_MapRGB(screen->format, 250, 0, 0), window);

		SDL_UpdateWindowSurface(window);
	}
	SDL_Quit();
	return 0;
}

void plot_pixel_ndc(SDL_Surface *surface, float x_ndc, float y_ndc, Uint32 pixel, SDL_Window *targetWindow) // work in progress
{
	// Handles X and Y inputs that are not in NDC space.
	if (abs(x_ndc) > 1 || abs(y_ndc) > 1)
	{
		std::cout << "plot_pixel_ndc function received invalid inputs (X or Y was not between -1 and 1).";
		return;
	}

	int bpp = surface->format->BytesPerPixel;

	int currentWindowWidth = SDL_GetWindowSurface(targetWindow)->w;
	int currentWindowHeight = SDL_GetWindowSurface(targetWindow)->h;

	float x_dc = ((x_ndc + 1) / 2)*(currentWindowWidth);
	float y_dc = ((y_ndc + 1) / 2)*(currentWindowHeight);

	int x_dc_int = round(x_dc);
	int y_dc_int = round(y_dc);

	// Currently throws error when X or Y is exactly -1.
	// Error does not occur when X or Y is .99, or -1. Not sure why! Would love some guidance on this.
	// In the meantime, this will fix this edge-case by decrementing the converted value.
	if (x_dc_int == currentWindowWidth) {
		x_dc_int--;
	}
	if (y_dc_int == currentWindowHeight) {
		y_dc_int--;
	}

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


