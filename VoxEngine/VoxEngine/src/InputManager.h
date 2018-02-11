#ifndef INPUTMANAGER_H_
# define INPUTMANAGER_H_

#include <iostream>
#include <gl/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include "renderEngine.h"

class	InputManager
{
public:
	InputManager();
	~InputManager();

	bool	getKeyDown(SDL_Keycode keycode);
	bool	getKeyUp(SDL_Keycode keycode);
	bool	getEvent(SDL_EventType event);
	bool	getRawKey(SDL_Scancode scancode);

	glm::vec2	getMouseMotion(SDL_Window * win, int win_x, int win_y);

	void	Update(void);

	SDL_Event	_event;
	Uint8*		_keystate;
};

#endif // !INPUTMANAGER_H_

