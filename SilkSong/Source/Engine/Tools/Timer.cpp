#include "Timer.h"



void Timer::Bind(double delay, std::function<void()> function, bool repeat, double firstDelay)
{
	callback = function;
	this->delay = duration<double>(delay);
	lastTime = steady_clock::now();
	if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (delay - firstDelay)));
	bPersistent = repeat;
	mainWorld.GameTimers.insert(this);
}

void Timer::Execute()
{
	if (bRunning && delay.count() > 0 && getDelay().count() >= delay.count())
	{
		callback();
		if (bPersistent)lastTime = steady_clock::now();
		else delay = duration<double>(0);
	}
}
