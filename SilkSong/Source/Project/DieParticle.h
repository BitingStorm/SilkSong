#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class DieParticle :public Actor
{
	DEFINE_SUPER(Actor)

public:
	DieParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};