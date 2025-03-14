#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class QuakeParticle :public Actor
{
	DEFINE_SUPER(Actor)

public:
	QuakeParticle();
private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};