#include <iostream>
#include "include\SDL.h"
#include "stdafx.h"
#include "clip.h"
//#include "clip.cpp"
#include <math.h>
#include <vector>

using namespace egc;

// define window dimensions
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

SDL_Window *window = NULL;
SDL_Renderer* renderer;
SDL_Surface *windowSurface = NULL;
SDL_Event currentEvent;

SDL_Rect rectangleCoordinates = { 100, 100, 200, 200 };
Uint32 rectangleColor;

bool quit = false;

int mouseX, mouseY;

bool initWindow()
{
	bool success = true;

	//Try to initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL initialization failed" << std::endl;
		success = false;
	}
	else {
		//Try to create the window
		// window = SDL_CreateWindow("SDL Hello World Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
		SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

		if (window == NULL)
		{
			std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Try to get the window surface
			windowSurface = SDL_GetWindowSurface(window);

			if (windowSurface == NULL)
			{
				std::cout << "Failed to get the window surface: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				//Initialize surface color
				SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 255, 255, 255));

				//Update the surface
				SDL_UpdateWindowSurface(window);
			}

		}
	}

	return success;
}

void destroyWindow()
{
	//Destroy window
	SDL_DestroyWindow(window);
	window = NULL;
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}


	//Quit SDL
	SDL_Quit();
}

int main(int argc, char *argv[]) {

	std::vector<vec3> clipWindow;
	clipWindow.resize(0);
	vec3 p1;
	vec3 p2;
	vec3 aux;

	clipWindow.push_back(egc::vec3(100.0, 100.0, 0.0));
	clipWindow.push_back(egc::vec3(10.0, 100.0, 0.0));
	clipWindow.push_back(egc::vec3(10.0, 10.0, 0.0));
	clipWindow.push_back(egc::vec3(100.0, 10.0, 0.0));

	p1.x = 0;
	p1.y = 50;
	p2.x = 120;
	p2.y = 50;

	if (!initWindow())
	{
		std::cout << "Failed to initialize" << std::endl;
		return -1;
	}

	if (!quit) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

		for (int i = 0; i < clipWindow.size() - 1; i++) {
			SDL_RenderDrawLine(renderer, clipWindow[i].x, clipWindow[i].y, clipWindow[i + 1].x, clipWindow[i + 1].y);
		}

		SDL_RenderDrawLine(renderer, clipWindow[clipWindow.size() - 1].x, clipWindow[clipWindow.size() - 1].y, clipWindow[0].x, clipWindow[0].y);

		SDL_SetRenderDrawColor(renderer, 255, 48, 206, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

		SDL_RenderPresent(renderer);

		//lineClip_CyrusBeck(clipWindow, p1, p2);

		SDL_Delay(2000);
	}
	
	SDL_RenderClear(renderer);

	if (lineClip_CohenSutherland(clipWindow, p1, p2) != -1) {

		while (!quit) {
			//Handle events on queue
			if (SDL_WaitEvent(&currentEvent) != 0)
			{
				//User requests quit
				if (currentEvent.type == SDL_QUIT)
				{
					quit = true;
				}

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);

				SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

				for (int i = 0; i < clipWindow.size() - 1; i++) {
					SDL_RenderDrawLine(renderer, clipWindow[i].x, clipWindow[i].y, clipWindow[i + 1].x, clipWindow[i + 1].y);
				}
				SDL_RenderDrawLine(renderer, clipWindow[clipWindow.size() - 1].x, clipWindow[clipWindow.size() - 1].y, clipWindow[0].x, clipWindow[0].y);

				SDL_SetRenderDrawColor(renderer, 255, 172, 48, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);

				SDL_RenderPresent(renderer);
			}
		}
	}

	std::cout << p1.x << " " << p1.y << "\n";
	std::cout << p2.x << " " << p2.y << "\n";

	destroyWindow();
	
	return 0;

}