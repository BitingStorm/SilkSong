#pragma once
#include "Objects/Actor.h"


class SpatterParticle :public Actor
{
public:
	SpatterParticle();
	void Init(FVector2D start, FVector2D end);
private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};
