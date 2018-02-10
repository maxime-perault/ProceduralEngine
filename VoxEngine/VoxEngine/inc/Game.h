#ifndef GAME_HH_
# define GAME_HH_

#include "Display.h"

class	Game
{
public:
	Game();
	~Game();

	Display *_display;

	void	loop(void);
};

#endif // !GAME_HH_ 