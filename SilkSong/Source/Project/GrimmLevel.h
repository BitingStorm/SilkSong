#pragma once
#include "Objects/Level.h"


class GrimmLevel :public Level
{
	class Player* player;
	class NightMare* nightmare;
public:
	GrimmLevel();

	virtual void Update(float deltaTime);

	Timer ShakeTimerHandle;
	int32 shakeTimer = 0;
};