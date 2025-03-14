#pragma once
#include "Objects/Actor.h"



class SoulOrbManager :public Actor
{
public:
	SoulOrbManager();

	virtual void Update(float deltaTime)override;

protected:
	class SoulOrb* orbs[4];

	float reverseTimer = 0.f;

	Timer DestroyTimerHandle;
};