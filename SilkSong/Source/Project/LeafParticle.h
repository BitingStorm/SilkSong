#pragma once
#include "Objects/Actor.h"


class LeafParticle :public Actor
{
public:
	LeafParticle();

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};
