#pragma once
#include "Objects/Actor.h"
#include "Tools/Timer.h"


class SilkParticle :public Actor
{
	DEFINE_SUPER(Actor)

public:
	SilkParticle();
	void Init(FVector2D normal, bool dead = false);
private:
	class ParticleSystem* silk;
	Timer DestroyTimerHandle;
};
