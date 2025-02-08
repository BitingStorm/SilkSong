#include "Timer.h"
#include "Core/World.h"


TimerHandler::~TimerHandler()
{
	for (auto& obj : timers)
	{
		mainWorld.GameTimers.erase(obj);
	}
}



Timer::~Timer()
{
	mainWorld.GameTimers.erase(this);
}

void Timer::Bind(double delay, std::function<void()> function, bool repeat, double firstDelay)
{
	callback = function;
	this->delay = duration<double>(delay);
	lastTime = steady_clock::now();
	if (firstDelay >= 0)lastTime -= milliseconds(int32(1000 * (delay - firstDelay)));
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

void Timer::RegisterTimer()
{
	mainWorld.GameTimers.insert(this);
}
