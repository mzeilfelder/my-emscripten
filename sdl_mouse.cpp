#include <stdio.h>
#include <SDL/SDL.h>
#include <assert.h>
#include <emscripten.h>

void one()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type) 
		{
			case SDL_MOUSEMOTION: 
			{
				SDL_MouseMotionEvent *m = (SDL_MouseMotionEvent*)&event;
				int x, y;
				SDL_GetMouseState(&x, &y);
				printf("motion: %d,%d  %d,%d\n", m->x, m->y, m->xrel, m->yrel);
				break;
			}
			case SDL_MOUSEBUTTONDOWN: 
			{
				SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
				printf("button down: %d,%d  %d,%d\n", m->button, m->state, m->x, m->y);
				break;
			}
			case SDL_MOUSEBUTTONUP: 
			{
				SDL_MouseButtonEvent *m = (SDL_MouseButtonEvent*)&event;
				printf("button up: %d,%d  %d,%d\n", m->button, m->state, m->x, m->y);
				break;
			}
		}
	}
}

int main() 
{
	SDL_Init(SDL_INIT_VIDEO);
	emscripten_set_canvas_size( 600, 450);
	SDL_Surface *screen = SDL_SetVideoMode(600, 450, 32, SDL_HWSURFACE);

//	SDL_Rect rect = { 0, 0, 600, 450 };
	//SDL_FillRect(screen, &rect, 0x2244ffff);

	emscripten_set_main_loop(one, 0, 0);

	return 0;
}
