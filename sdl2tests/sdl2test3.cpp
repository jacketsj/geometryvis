#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <tuple>
#include <vector>

int test() { return 1; }

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::vector<std::tuple<int, int>> circles = {{30, 30}, {100, 30}};

void draw_things(SDL_Renderer* renderer) {
	int w = SCREEN_WIDTH, h = SCREEN_HEIGHT;
	// Fill rect
	SDL_Rect fill_rect = {w / 4, h / 4, w / 2, h / 2};
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(renderer, &fill_rect);
	// Outline of rect
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &fill_rect);
	// horizontal line
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, 0, h / 2, w, h / 2);
	// vertical dotted line
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	for (int y = 0; y < h; y += 4)
		SDL_RenderDrawPoint(renderer, w / 2, y);
	// diagonal spaced line
	SDL_SetRenderDrawColor(renderer, 0x1f, 0xFF, 0x1F, 0xFF);
	int numsegs = 100;
	for (int i = 0; i <= numsegs; ++i) {
		// divide into numsegs sections
		// between: (0,0) and (w,h)
		// current convexity param: i/numsegs to (i+1)/numsegs
		int loc_x = w * (numsegs - i) / numsegs;
		int loc_y = h * (numsegs - i) / numsegs;
		int loc_x_next = w * (numsegs - i + 1) / numsegs;
		int loc_y_next = h * (numsegs - i + 1) / numsegs;
		if (i % 2) {
			SDL_RenderDrawLine(renderer, loc_x, loc_y, loc_x_next, loc_y_next);
		}
	}
	// int a = aalineRGBA(renderer, 12, 12, 40, 40, 0x33, 0x33, 0x33, 0x33);
	// a = 3;
	// aalineColor(renderer, 3 * w / 4, h / 4, w / 4, 3 * h / 4,
	// 0xFF1F55FF);
	thickLineRGBA(renderer, 3 * w / 4, h / 4, w / 4, 3 * h / 4, 30, 0xFF, 0x1F,
								0x55, 0xFF);
	// draw some circles
	for (auto [x, y] : circles) {
		filledCircleRGBA(renderer, x, y, 16, 0xB1, 0x33, 0x87, 0xFF);
		aacircleRGBA(renderer, x, y, 16, 0xB1, 0x33, 0x87, 0xFF);
	}
	// finalize
	SDL_RenderPresent(renderer);
}

void run_event_loop(SDL_Window* window, SDL_Renderer* renderer) {
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			draw_things(renderer);
		}
	}
}

int main() {

	// window to render to
	SDL_Window* window = NULL;

	SDL_Renderer* renderer = NULL;

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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				std::cout << "error initializing SDL renderer: " << SDL_GetError()
									<< std::endl;
			} else {
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// start the event loop
				run_event_loop(window, renderer);

				SDL_DestroyRenderer(renderer);
				renderer = NULL;
			}
			SDL_DestroyWindow(window);
			window = NULL;
		}
	}

	SDL_Quit();
}
