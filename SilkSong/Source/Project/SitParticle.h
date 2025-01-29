#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class SitParticle :public Actor
{
	DEFINE_SUPER(Actor)

public:
	SitParticle();

	void Update(float deltaTime);

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
	float length;
};