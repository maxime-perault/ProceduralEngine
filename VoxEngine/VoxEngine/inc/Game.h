#ifndef GAME_HH_
# define GAME_HH_

# include <thread> 
# include "Display.h"
# include "InputManager.h"
# include "renderEngine.h"
# include "Camera.h"
# include "Timer.h"
# include "World.h"

class	Game
{
public:
	Game();
	~Game();

	Display			*_display;
	renderEngine	*_renderEngine;
	InputManager	_inputManager;
	Camera			_camera;
	Timer			_timer;
	World			*_world;

	int		_win_x;
	int		_win_y;

	float	_elapsed;

	int		_fps;

	bool	_debug;

	void	loop(void);
	void	moveMouseCamera(float elapsed);
	void	moveKeyboardCamera(float elapsed);
	void	KeyboardAction(void);
	void	movePlayer(const glm::vec3 delta, float elapsed);
};

#endif // !GAME_HH_ 