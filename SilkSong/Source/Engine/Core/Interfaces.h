#pragma once
#include <unordered_set>



class ITimerHandler
{
	std::unordered_set<class Timer*> timers;

public:
	ITimerHandler() {}
	virtual ~ITimerHandler();

	void AddTimer(Timer* timer) { timers.insert(timer); }
};