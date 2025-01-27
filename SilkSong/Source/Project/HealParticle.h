#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class HealParticle :public Actor
{
	DEFINE_SUPER(Actor)

public:
	HealParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};