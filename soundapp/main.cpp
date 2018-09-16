#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cmath>
#include <vector>

SDL_Texture *NJR_LoadImage(const char *file, SDL_Renderer *renderer)
{
	SDL_Surface *sfc = IMG_Load(file);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sfc);
	SDL_FreeSurface(sfc);
	return tex;
}

struct vec2 {
	int x, y;
};
struct vec2f {
	float x, y;
};

int absless(int a, int b) {
	if (std::abs(a) < std::abs(b)) { return std::abs(a); }
	else { return std::abs(b); }
}

#define WINDOWSIZE 240
int main(int argc, char *argv[])
{
	
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	SDL_Window *win;
	SDL_Renderer *ren;
	
	// Init graphics
	SDL_CreateWindowAndRenderer(WINDOWSIZE, WINDOWSIZE, SDL_WINDOW_BORDERLESS, &win, &ren);
	SDL_Texture *tex_play = NJR_LoadImage("Content/texplay.png", ren);
	SDL_Texture *tex_lose = NJR_LoadImage("Content/texlose.png", ren);
		
	

	bool running = true;
	bool ghost_is_it = true;
	int defeatframes = 0;
	vec2f velocity; velocity.x = velocity.y = 0;
	while (running)
	{
		
		SDL_Event sdlevent;
		while (SDL_PollEvent(&sdlevent))
		{
			switch (sdlevent.type)
			{
			case SDL_QUIT:
				running = false; break;
			case SDL_KEYDOWN:
				switch (sdlevent.key.keysym.sym)
				{

				case SDLK_ESCAPE: running = false; break;
				}
				break;
			}
		}

		
		// my program
		vec2 mouse, window, gameplayingmouse;
		vec2f distance, distanceMoving;
		SDL_GetGlobalMouseState(&mouse.x, &mouse.y);
		SDL_GetMouseState(&gameplayingmouse.x, &gameplayingmouse.y);
		SDL_GetWindowPosition(win, &window.x, &window.y);

		// If Ghost is it and tags player, player is it!
		//scratch that. game ends if player is tagged
		if (ghost_is_it && gameplayingmouse.x > 0 && gameplayingmouse.x < WINDOWSIZE && gameplayingmouse.y > 0 && gameplayingmouse.y < WINDOWSIZE) {
			ghost_is_it = false;

			/*window.x -= velocity.x;
			window.y -= velocity.y;
			velocity.x = -velocity.x;
			velocity.y = -velocity.y;*/
		}

		/*if (!ghost_is_it && gameplayingmouse.x > 0 && gameplayingmouse.x < WINDOWSIZE && gameplayingmouse.y > 0 && gameplayingmouse.y < WINDOWSIZE) {
			ghost_is_it = true;
			int resetspeed = 20;
			velocity.x = 0;
			velocity.y = 0;
			window.x += velocity.x;
			window.y += velocity.y;
			
		}*/

		distance.x = mouse.x - ( window.x + WINDOWSIZE / 2);
		distance.y = mouse.y - (window.y + WINDOWSIZE / 2);

		float speed = 5;
		speed = speed / 10;
		double angle = std::atan2(distance.y, distance.x);

		if(ghost_is_it)
		{
			velocity.x += std::cos(angle);
			velocity.y += std::sin(angle);
		}
		/*if (!ghost_is_it)
		{
			velocity.x -= std::cos(angle);
			velocity.y -= std::sin(angle);
		}*/
		velocity.x *= .96;
		velocity.y *= .96;

		SDL_SetWindowPosition(win, window.x + velocity.x, window.y + velocity.y);
		SDL_RenderClear(ren);
		if(ghost_is_it) { SDL_RenderCopy(ren, tex_play, NULL, NULL);  }
		else { 
			SDL_RenderCopy(ren, tex_lose, NULL, NULL);
			if (defeatframes < 60) {
				defeatframes++;
			}
			else {
				velocity.x = 0;
				velocity.y = 0;
			}
		}
		SDL_RenderPresent(ren);

		SDL_Delay(16);
	}	
	SDL_DestroyTexture(tex_play);
	SDL_DestroyTexture(tex_lose);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
	return 0;
}