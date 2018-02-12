#ifndef TIMER_HH_
# define TIMER_HH_

# include <Windows.h>

class	Timer
{
public:
	Timer();
	~Timer();

	LARGE_INTEGER	_lastUpdateTime;
	LONGLONG		_freq;

	void			start(void);
	float			getElapsedSeconds(bool restart);
	unsigned long	getElapsedMs(bool restart);
};

#endif // !TIMER_HH_ 