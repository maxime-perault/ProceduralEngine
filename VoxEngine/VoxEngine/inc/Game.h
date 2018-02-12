#ifndef GAME_HH_
# define GAME_HH_

# include "Display.h"
# include "InputManager.h"
# include "renderEngine.h"
# include "Camera.h"
# include "Timer.h"

class	Game
{
public:
	Game();
	~Game();

	Display			*_display;
	renderEngine	*_renderEngine;
	InputManager	*_inputManager;
	Camera			*_camera;
	Timer			*_timer;

	int		_win_x;
	int		_win_y;

	void	loop(void);
	void	moveMouseCamera(float elapsed);
	void	moveKeyboardCamera(float elapsed);
};

#endif // !GAME_HH_ 