#pragma once
#include "Effect.h"


class PuffManager :public Actor
{
public:
	PuffManager();
protected:
	Timer SpawnTimerHandle;
	Timer DestroyTimerHandle;
	float spawnTimer = 0.f;
};