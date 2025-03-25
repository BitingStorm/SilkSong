#pragma once
#include "Objects/Actor.h"


class GrimmSmoke :public Actor
{
public:
	GrimmSmoke();
private:
	class ParticleSystem* particle;
	Timer DestroyTimerHandle;
};
