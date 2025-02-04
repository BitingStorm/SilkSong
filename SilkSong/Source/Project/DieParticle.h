#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


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