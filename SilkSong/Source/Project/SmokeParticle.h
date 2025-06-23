#pragma once
#include "Objects/Actor.h"


class SmokeParticle :public Actor
{
public:
	SmokeParticle();

protected:
	Timer DestroyTimerHandle;
	class ParticleSystem* particle;

	virtual void Update(float deltaTime)override;
};
