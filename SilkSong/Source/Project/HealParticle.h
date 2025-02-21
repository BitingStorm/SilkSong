#pragma once
#include "Objects/Actor.h"


class HealParticle :public Actor
{
public:
	HealParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};