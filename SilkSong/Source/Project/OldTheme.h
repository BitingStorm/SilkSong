#pragma once
#include "Objects/Actor.h"


class ParticleSystem;

class OldTheme :public Actor
{
	DEFINE_SUPER(Actor)

    ParticleSystem* blackRadiantFar;
	ParticleSystem* blackRadiantClose;
	ParticleSystem* smoke;

public:
	OldTheme();
};