#pragma once
#include "Objects/Level.h"


class BossLevel :public Level
{
	class Player* player;
	class SoulMaster* master;
public:
	BossLevel();

	virtual void Update(float deltaTime);

	Timer ShakeTimerHandle;
	int32 shakeTimer = 0;
};

