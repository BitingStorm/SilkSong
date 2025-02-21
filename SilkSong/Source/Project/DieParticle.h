#pragma once
#include "Objects/Actor.h"


class DieParticle :public Actor
{
public:
	DieParticle();

private:
	class ParticleSystem* particle;
	class ParticleSystem* silk;
	Timer ChangeTimerHandle;
	Timer DestroyTimerHandle;
};