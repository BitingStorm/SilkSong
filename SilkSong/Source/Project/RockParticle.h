#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class RockParticle :public Actor
{
public:
	RockParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};