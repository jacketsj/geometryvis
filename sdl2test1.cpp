#include <SDL2/SDL.h>
#include <iostream>

int test() { return 1; }

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void run_event_loop(SDL_Window* window, SDL_Surface* screen_surface) {
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
	}
}

int main() {

	// window to render to
	SDL_Window* window = NULL;
	// surface contained by window
	SDL_Surface* screen_surface = NULL;

	// initialize video only
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "error initializing SDL: " << SDL_GetError() << std::endl;
	} else {
		window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED,
															SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
															SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cout << "error initializing SDL window: " << SDL_GetError()
								<< std::endl;
		} else {
			screen_surface = SDL_GetWindowSurface(window);
			SDL_FillRect(screen_surface, NULL,
									 SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0x1D));
			SDL_UpdateWindowSurface(window);

			// start the event loop
			run_event_loop(window, screen_surface);

			SDL_DestroyWindow(window);
		}
	}

	SDL_Quit();
}
