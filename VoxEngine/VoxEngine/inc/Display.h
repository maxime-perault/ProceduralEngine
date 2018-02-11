#ifndef DISPLAY_HH_
# define DISPLAY_HH_

# include <iostream>
# include <gl/glew.h>
# include <GL/glu.h>
# include <SDL2/SDL.h>


class	Display
{
public:
	SDL_Window		*_win;
	SDL_GLContext	_rend;

	std::size_t		_win_y;
	std::size_t		_win_x;

	SDL_DisplayMode	_mode;

	Display(std::size_t win_x, std::size_t win_y);
	~Display();

	void	clear(void);
	void	update(void);
};

#endif // !DISPLAY_HH_ 

