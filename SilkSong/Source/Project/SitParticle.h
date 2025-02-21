#pragma once
#include "Objects/Actor.h"


class SitParticle :public Actor
{
public:
	SitParticle();

	void Update(float deltaTime);

private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
	float length;
};