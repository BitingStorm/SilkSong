#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class RockParticle :public Actor
{
	DEFINE_SUPER(Actor)

public:
	RockParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};