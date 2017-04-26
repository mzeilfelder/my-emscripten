#include <stdio.h>
#include <SDL/SDL.h>
#include <assert.h>
#include <emscripten.h>
#include <emscripten/html5.h>

void printResult(EMSCRIPTEN_RESULT result)
{
	switch ( result )
	{
		case EMSCRIPTEN_RESULT_SUCCESS:
			printf("SUCCESS.\n");
		break;

		case EMSCRIPTEN_RESULT_DEFERRED:
			printf("DEFERRED.\n");
		break;

		case EMSCRIPTEN_RESULT_NOT_SUPPORTED:
			printf("NOT_SUPPORTED.\n");
		break;

		case EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED:
			printf("NOT_DEFERRED.\n");
		break;

		case EMSCRIPTEN_RESULT_INVALID_TARGET:
			printf("INVALID_TARGET.\n");
		break;

		case EMSCRIPTEN_RESULT_UNKNOWN_TARGET:
			printf("UNKNOWN_TARGET.\n");
		break;

		case EMSCRIPTEN_RESULT_INVALID_PARAM:
			printf("INVALID_PARAM.\n");
		break;

		case EMSCRIPTEN_RESULT_FAILED:
			printf("FAILED.\n");
		break;
		
		case EMSCRIPTEN_RESULT_NO_DATA:
			printf("NO_DATA.\n");
		break;
	}
}

EM_BOOL PointerLockChangeCallback(int eventType, const EmscriptenPointerlockChangeEvent* event, void* userData)
{
	if (event->isActive)
		printf("PointerLockChangeCallback says it's active now.\n");
	else
		printf("PointerLockChangeCallback says it's NOT active .\n");
		
	return EM_TRUE;
}

EM_BOOL FocusChangeCallback(int eventType, const EmscriptenFocusEvent* event, void* userData)
{
	switch ( eventType )
	{
		case EMSCRIPTEN_EVENT_BLUR:
			printf("blur");
		break;
		case EMSCRIPTEN_EVENT_FOCUS:
			printf("focus");
		break;
		case EMSCRIPTEN_EVENT_FOCUSIN:
			printf("focusin");
		break;
		case EMSCRIPTEN_EVENT_FOCUSOUT:
			printf("focusout");
		break;
	}
	
	printf(" - nodeName: %s id: %s\n", event->nodeName, event->id);
	
	 return EM_TRUE;
}

EM_BOOL MouseUpDownCallback(int eventType, const EmscriptenMouseEvent * event, void* userData)
{
	return EM_FALSE;
}

EM_BOOL MouseEnterCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
	printf("mouse enter\n");
	return EM_FALSE;
}

EM_BOOL MouseLeaveCallback(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
	printf("mouse leave\n");
	return EM_FALSE;
}

void one()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch(event.type) 
		{
			case SDL_MOUSEMOTION: 
			{
				/*
				SDL_MouseMotionEvent *m = (SDL_MouseMotionEvent*)&event;
				int x, y;
				SDL_GetMouseState(&x, &y);
				printf("motion: %d,%d  %d,%d\n", m->x, m->y, m->xrel, m->yrel);
				*/

				break;
			}
			case SDL_MOUSEBUTTONDOWN: 
			{
				EmscriptenPointerlockChangeEvent pointerlockStatus;
				if ( emscripten_get_pointerlock_status(&pointerlockStatus) == EMSCRIPTEN_RESULT_SUCCESS )
				{
					if ( !pointerlockStatus.isActive )
					{
						EMSCRIPTEN_RESULT tryLock = emscripten_request_pointerlock(0, true);
						printResult(tryLock);
					}
				}
				
				
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
	
	
	EmscriptenVisibilityChangeEvent emVisibility;
	if ( emscripten_get_visibility_status(&emVisibility) == EMSCRIPTEN_RESULT_SUCCESS)
	{
		static bool oldHidden = emVisibility.hidden;
		if ( oldHidden != emVisibility.hidden )
		{
			oldHidden = emVisibility.hidden;
			if ( emVisibility.hidden )
				printf("now hidden\n");
			else
				printf("now visible\n");
		}
	}
}

int main() 
{
	SDL_Init(SDL_INIT_VIDEO);
	emscripten_set_canvas_size( 600, 450);
	SDL_Surface *screen = SDL_SetVideoMode(600, 450, 32, SDL_HWSURFACE);

	SDL_Rect rect = { 0, 0, 600, 450 };
	SDL_FillRect(screen, &rect, 0x2244ffff);
	
	// just trying if we can get it working by catching the callbacks
//	emscripten_set_pointerlockchange_callback(0, 0, false, PointerLockChangeCallback);
	
	emscripten_set_mousedown_callback(0, 0, true, MouseUpDownCallback);
    emscripten_set_mouseup_callback(0, 0, true, MouseUpDownCallback);
	emscripten_set_focusout_callback(0, 0, false, FocusChangeCallback);
	emscripten_set_focus_callback(0, 0, false, FocusChangeCallback);
	emscripten_set_mouseenter_callback("#canvas", 0, false, MouseEnterCallback);
	emscripten_set_mouseleave_callback("#canvas", 0, false, MouseLeaveCallback);
	
	emscripten_set_main_loop(one, 0, 0);

	return 0;
}
