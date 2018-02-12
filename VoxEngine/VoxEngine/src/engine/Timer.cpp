
#include "Timer.h"


Timer::Timer()
{
	QueryPerformanceCounter(&_lastUpdateTime);

	LARGE_INTEGER li_freq;
	QueryPerformanceFrequency(&li_freq);
	_freq = li_freq.QuadPart;
	_freq /= 1000;
}

Timer::~Timer()
{

}

void	Timer::start(void)
{
	QueryPerformanceCounter(&_lastUpdateTime);
}

float	Timer::getElapsedSeconds(bool restart = false)
{
	LARGE_INTEGER timeNow;
	QueryPerformanceCounter(&timeNow);
	LONGLONG elapsedLong = timeNow.QuadPart - _lastUpdateTime.QuadPart;

	float elapsed = (float)((float)elapsedLong / (float)_freq);
	elapsed /= 1000.0f;

	if (restart)
		_lastUpdateTime = timeNow;

	return elapsed;
}

unsigned long	Timer::getElapsedMs(bool restart = false)
{
	LARGE_INTEGER timeNow;
	QueryPerformanceCounter(&timeNow);
	LONGLONG elapsedLong = timeNow.QuadPart - _lastUpdateTime.QuadPart;

	unsigned long elapsed = (unsigned long)((float)elapsedLong / (float)_freq);
	return elapsed;
}