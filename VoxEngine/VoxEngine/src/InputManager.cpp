
#include "InputManager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

bool	InputManager::getRawKey(SDL_Scancode scancode)
{
	if (_keystate[scancode])
		return true;
	return false;
}

bool	InputManager::getKeyDown(SDL_Keycode keycode)
{
	if (_event.type == SDL_KEYDOWN)
	{
		if (_event.key.keysym.sym == keycode)
			return true;
	}
	return false;
}

bool	InputManager::getKeyUp(SDL_Keycode keycode)
{
	if (_event.type == SDL_KEYUP)
	{
		if (_event.key.keysym.sym == keycode)
			return true;
	}
	return false;
}

bool	InputManager::getEvent(SDL_EventType event)
{
	if (_event.type == event)
		return true;
	return false;
}

void	InputManager::Update(void)
{
	SDL_PollEvent(&_event);
	_keystate = (Uint8*)SDL_GetKeyboardState(NULL);
}

glm::vec2	InputManager::getMouseMotion(SDL_Window * win, int win_x, int win_y)
{
	static int	x, y;
	
	if (_event.type == SDL_MOUSEMOTION)
	{
		SDL_GetGlobalMouseState(&x, &y);
		SDL_WarpMouseInWindow(win, win_x / 2, win_y / 2);
		return glm::vec2(x, y);
	}
	return glm::vec2(-1, -1);
}