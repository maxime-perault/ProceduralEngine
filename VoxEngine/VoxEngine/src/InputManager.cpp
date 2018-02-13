
#include "InputManager.h"

InputManager::InputManager()
{
	_keystate = (Uint8*)SDL_GetKeyboardState(NULL);

	_scancodes.push_back(SDL_SCANCODE_W);
	_scancodes.push_back(SDL_SCANCODE_A);
	_scancodes.push_back(SDL_SCANCODE_S);
	_scancodes.push_back(SDL_SCANCODE_D);
	_scancodes.push_back(SDL_SCANCODE_LALT);
	_scancodes.push_back(SDL_SCANCODE_SPACE);
	_scancodes.push_back(SDL_SCANCODE_LCTRL);
	_scancodes.push_back(SDL_SCANCODE_ESCAPE);

	for (std::size_t i(0); i < _scancodes.size(); ++i)
	{
		_prevKeys.push_back(false);
		_actualKeys.push_back(false);
	}
}

InputManager::~InputManager()
{

}

bool	InputManager::getRawKey(e_keys key)
{
	if (_actualKeys[key])
		return true;
	return false;
}

bool	InputManager::getKeyDown(e_keys key)
{
	if (_actualKeys[key] && _prevKeys[key] == false)
		return true;
	return false;
}

bool	InputManager::getKeyUp(e_keys key)
{
	if (!_actualKeys[key] && _prevKeys[key] == true)
		return true;
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

	_prevKeys = _actualKeys;
	for (std::size_t i(0); i < _actualKeys.size(); ++i)
	{
		if (_keystate[_scancodes[i]])
			_actualKeys[i] = true;
		else
			_actualKeys[i] = false;
	}
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