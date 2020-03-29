#include <SDL2/SDL.h>
#include <iostream>

int test() { return 1; }

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main() {

	// window to render to
	SDL_Window* window = NULL;
	// surface contained by window
	// SDL_Surface* screenSurface = NULL;

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
		}
		char c;
		while (std::cin >> c) {
			// int a = 1;
		}
	}

	SDL_Quit();
}
