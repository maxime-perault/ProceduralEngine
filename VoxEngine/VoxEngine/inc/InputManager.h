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
	enum e_keys
	{
		Z = 0,
		Q,
		S,
		D,
		LALT,
		SPACE,
		LCTRL,
		ESCAPE,
		TAB
	};
	InputManager();
	~InputManager();

	bool	getKeyDown(e_keys key);
	bool	getKeyUp(e_keys key);
	bool	getRawKey(e_keys key);
	bool	getEvent(SDL_EventType event);

	glm::vec2	getMouseMotion(SDL_Window * win, int win_x, int win_y);

	void	Update(void);

	SDL_Event	_event;
	Uint8*		_keystate;

	std::vector<bool>			_prevKeys;
	std::vector<bool>			_actualKeys;
	std::vector<SDL_Scancode>	_scancodes;
};

#endif // !INPUTMANAGER_H_

