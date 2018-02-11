#ifndef GAME_HH_
# define GAME_HH_

# include "Display.h"
# include "InputManager.h"
# include "renderEngine.h"
# include "Camera.h"

class	Game
{
public:
	Game();
	~Game();

	Display			*_display;
	renderEngine	*_renderEngine;
	InputManager	*_inputManager;
	Camera			*_camera;

	int		_win_x;
	int		_win_y;

	void	loop(void);
	void	moveCamera(float elapsed);
	double	getElapsedTime(void);
};

#endif // !GAME_HH_ 