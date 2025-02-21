#pragma once
#include "Objects/Actor.h"


class RockParticle :public Actor
{
public:
	RockParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};