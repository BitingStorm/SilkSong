#pragma once
#include "Objects/Actor.h"


class SilkParticle :public Actor
{
public:
	SilkParticle();
	void Init(FVector2D normal, bool dead = false);
private:
	class ParticleSystem* silk;
	Timer DestroyTimerHandle;
};