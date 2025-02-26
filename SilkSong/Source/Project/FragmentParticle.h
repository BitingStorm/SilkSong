#pragma once
#include "Objects/Actor.h"


class FragmentParticle :public Actor
{
public:
	FragmentParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};