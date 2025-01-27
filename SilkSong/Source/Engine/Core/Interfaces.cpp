#include "Interfaces.h"
#include "World.h"


ITimerHandler::~ITimerHandler()
{
	for (auto& timer : timers)
	{
		mainWorld.GameTimers.erase(timer);
	}
}
